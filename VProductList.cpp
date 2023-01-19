//
// Created by rkdgh on 2023-01-03.
//
#include <QListWidget>
#include <QGridLayout>
#include <QString>
#include <QDir>
#include <QGuiApplication>

#include <iostream>
#include <regex>
#include <fstream>

#include "CURL/POSTAPI.h"
#include "VProductList.h"
using namespace std;

VProductList::VProductList(QWidget *parent)
{
    //post api
    //POSTAPI();

    MakeCategoryListWidget();

    category_listWidget = new QListWidget(this);
    new QListWidgetItem(tr("post90000"),category_listWidget);
    new QListWidgetItem(tr("ProductA"),category_listWidget);

    listWidget2 = new QListWidget(this);

    connect(category_listWidget,&QListWidget::itemClicked,
            this,&VProductList::SetSelectedCategory);

    connect(listWidget2,&QListWidget::itemClicked,
            this,&VProductList::SetViewTest);


    //QHbox를 먼저 선언해야함
    auto *QHbox = new QHBoxLayout(this);
    auto *QVbox = new QVBoxLayout(this);
    QVbox->addWidget(nameLabel);
    QVbox->addWidget(editDC);
    QVbox->addWidget(category_listWidget);


    QHbox->addLayout(QVbox);
    QHbox->addWidget(listWidget2);

    setLayout(QHbox);
    setWindowTitle("LPE");

    loadSetting();
}

void VProductList::MakeCategoryListWidget()
{
    //정보
    nameLabel = new QLabel();
    editDC = new QLineEdit;

    //post90000 / list by read csv - 다른 코드도 처리하는 방식 고민
    //set list, save vector
    ifstream file("post90000.csv");
    while(file.good())
    {
        vector<string> row = POSTAPI::csv_read_row(file,',');

        if(row[0]=="@")
            break;

        if(!row[0].find('#'))
        {
            continue;
        }
        else
        {
            for(auto v: row)
            {
                //name (BC) price 형태로 변경하기
                QString listname = QString::fromStdString(row[POSTAPI::eInfoItem::Name]);
                post90000.append(listname);

                //CMP - 이름, 현재 가격 // BC - 이름, 번들 갯수
                //두개로 나눈 이유는..기억이 안난다 나중에
                ItemCMP.emplace_back(row[POSTAPI::eInfoItem::Name],row[POSTAPI::eInfoItem::CurrentMinPrice]);
                ItemBC.emplace_back(row[POSTAPI::eInfoItem::Name],row[POSTAPI::eInfoItem::BundleCount]);
                break;
            }
        }
    }

    //대놓고 반복되는 부분이다.
    //
    ifstream file2("recipe.csv");
    while(file2.good())
    {
        vector<string> row = POSTAPI::csv_read_row(file2,',');

        if(row[0]=="@")
            break;

        if(!row[0].find('#'))
        {
            continue;
        }
        else
        {
            for(auto v: row)
            {
                //name (BC) price 형태로 변경하기
                QString listname = QString::fromStdString(row[0]);
                productA.append(listname);
                break;
            }
        }
    }
}

//category item apeend to listwidget
void VProductList::SetSelectedCategory()
{
    listWidget2->clear();
    int selectedRow = category_listWidget->currentRow();

    switch (selectedRow)
    {
        case 0:
            for(int i=0;i<post90000.length();i++)
            {
                new QListWidgetItem(post90000[i],listWidget2);
            }
            break;
        case 1:
            for(int i=0;i<productA.length();i++)
            {
                new QListWidgetItem(productA[i],listWidget2);
            }
            break;
    }
}

void VProductList::SetViewTest()
{
    nameLabel->clear();
    nameLabel->show();

    string curCate = category_listWidget->currentItem()->text().toStdString();
    //curCate기준 enum을 사용하여 category를 나눌수도 있고, 나중엔 분리될 list들이기 때문에 connect 함수를 나눠도 된다.
    //당장은 if문

    if(curCate=="post90000")
    {
        //int -> string/Qstring 함수화
        std::ostringstream ss;
        ss.precision(2);
        ss<<getItemPrice(listWidget2->currentItem()->text().toStdString());


        string testla = ss.str();
        QString testqst = QString::fromStdString(testla);

        cout<<testla;

        nameLabel->setText(testqst);
    }
    else if(curCate=="ProductA")
    {

        string select = listWidget2->currentItem()->text().toStdString();
        QString test = QString::fromStdString(to_string(getBenefitProduct(select)));

        nameLabel->setText(test);

    }
}

//나중에 빼기
//Label name -> Item Name(BundleCount) - Price

//make getpricefun -> get ItemPrice - currentinrpice/byndlecoun
float VProductList::getItemPrice(std::string ItemName)
{
    float CMP;
    float BC;

    vector<pair<string,string>> ::iterator iter;
    for(iter=ItemCMP.begin();iter!=ItemCMP.end();iter++)
    {
        if(iter->first==ItemName)
            CMP = stoi(iter->second);
    }

    for(iter=ItemBC.begin();iter!=ItemBC.end();iter++)
    {
        if(iter->first==ItemName)
            BC = stoi(iter->second);
    }

    if(BC==1)
    {
        return CMP;
    }
    else
    {
        float InPrice = CMP/BC;
        return InPrice;
    }
}

//benefit of product -> get load resipe and Material price
//샘플로 제작 키트
float VProductList::getBenefitProduct(string ItemName)
{
    float benefit_product;
    //find ItemName price * 개수, see
    bool findP = false;

    //read recipe
    ifstream file("recipe.csv");
    while (!findP)
    {
        vector<string> row = POSTAPI::csv_read_row(file, ',');

        if(row[0]==ItemName)
        {
            findP = true;


            benefit_product = see(getItemPrice(row[0])*stoi(row[1]));
            for(int i=2;i<row.size()-1;i+=2)
            {
                if(row[i]=="조합비")
                {
                   benefit_product-= DC_product_cost(stoi(row[i+1]));
                }
                else
                {
                    benefit_product -= getItemPrice(row[i])*stoi(row[i+1]);
                }

            }
            break;
        }
    }

    return benefit_product;
}

int VProductList::see(int value)
{
    if (value == 1)
        return value;
    else
    {
        value -= ceil(value * 0.05);
        return int(value);
    }

}

//값 체크해보기
int VProductList::DC_product_cost(float cost)
{
    float DC =  editDC->text().toFloat();
    //실행하는 함수마다 체크하는 것 보다는 savesetting으로 인수를 보내서 하는게 좋은거 같다..
    //그러려면 따로 만들어서 오버라이딩

    QSettings settings("setting.ini",QSettings::IniFormat);
    settings.setValue("VALUE/DC_product_cost",DC);
    cost = cost - (cost * (DC / 100));

    return int(cost);
}

//setting load & save
void VProductList::loadSetting()
{
    QSettings settings("setting.ini",QSettings::IniFormat);
    settings.beginGroup("VALUE");
    QString DCPC = settings.value("DC_product_cost","").toString();
    settings.endGroup();

    if(editDC)
    {
        editDC->setText(DCPC);
    }

}

void VProductList::saveSetting()
{

}


//data... db->csv
//UI...drawing...
//reload btn