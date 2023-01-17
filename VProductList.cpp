//https://doc.qt.io/qt-6/classes.html
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


    //정보
    nameLabel = new QLabel(tr("Name"));
    testLabel = new QLabel(tr("Test"));

    //QListWidget 구성

    //post90000 list by read csv
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
                QString listname = QString::fromStdString(row[POSTAPI::eInfoItem::Name]);
                post90000.append(listname);
                ItemCMP.emplace_back(row[POSTAPI::eInfoItem::Name],row[POSTAPI::eInfoItem::CurrentMinPrice]);
                ItemBC.emplace_back(row[POSTAPI::eInfoItem::Name],row[POSTAPI::eInfoItem::BundleCount]);
                break;
            }
        }
    }

    //productA product info - recipe
    productA.append("최상급");
    productA.append("최상급1");
    productA.append("최상급2");
    nameC.append("C1");
    nameC.append("C2");
    nameC.append("C3");

    listWidget1 = new QListWidget(this);
    new QListWidgetItem(tr("post90000"),listWidget1);
    new QListWidgetItem(tr("제작A"),listWidget1);
    new QListWidgetItem(tr("name3"),listWidget1);

    listWidget2 = new QListWidget(this);

    connect(listWidget1,&QListWidget::itemClicked,
            this,&VProductList::SetSelectedCategory);

    connect(listWidget2,&QListWidget::itemClicked,
            this,&VProductList::SetViewTest);




    auto *QHbox = new QHBoxLayout(this);
    QHbox->addWidget(listWidget1);
    QHbox->addWidget(listWidget2);
    QHbox->addWidget(nameLabel);
    QHbox->addWidget(testLabel);

    setLayout(QHbox);
    setWindowTitle("tt");
}

void VProductList::SetSelectedCategory()
{
    listWidget2->clear();
    int selectedRow = listWidget1->currentRow();

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
        case 2:
            for(int i=0;i<nameC.length();i++)
            {
                new QListWidgetItem(nameC[i],listWidget2);
            }
            break;
    }
}

void VProductList::SetViewTest()
{
    nameLabel->clear();
    nameLabel->show();

    string curCate = listWidget1->currentItem()->text().toStdString();
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
    else if(curCate=="제작A")
    {

        QString test = QString::fromStdString(to_string(getBenefitProduct("달인용 제작 키트")));
        //nameLabel->setText(listWidget2->currentItem()->text());

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
    QString testlabeltext = "";
    bool findP = false;

    //read resipe
    ifstream file("recipe.csv");
    while (!findP)
    {
        vector<string> row = POSTAPI::csv_read_row(file, ',');

        testLabel->setText("step1");
        //이름 * 개수 ..., 조합비까지

        if(row[0]==ItemName)
        {
            findP = true;
            testLabel->setText(QString::fromStdString(row[0])+QString::fromStdString(ItemName));
            //testLabel->setText(QString::fromStdString(to_string(row.size())));

            benefit_product = getItemPrice(row[0])*stoi(row[1]);

            for(int i=2;i<row.size();i+=2)
            {
                if(row[i]=="조합비")
                    benefit_product-= stoi(row[i+1]);
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



//edit text ->edit user Reduced product costs of percent

//새로고침