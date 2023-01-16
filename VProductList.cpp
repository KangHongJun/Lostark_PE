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
                ItemV.emplace_back(row[POSTAPI::eInfoItem::Name],row[POSTAPI::eInfoItem::CurrentMinPrice]);
                break;
            }
        }
    }

    //productA product info - recipe
    productA.append("최상급");
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
    QHbox->addWidget(addButton);
    QHbox->addWidget(nameLabel);

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

//    string curCate = listWidget1->currentItem()->text().toStdString();
//    //curCate기준 enum을 사용하여 category를 나눌수도 있고, 나중엔 분리될 list들이기 때문에 connect를 나눠도 된다.
//    //카테고리 기준이므로 listWidget1값
//    //당장은 if문
//
//    if(curCate=="post90000")
//    {
//        nameLabel->setText(listWidget1->currentItem()->text());
//    }
//    else if(curCate=="제작A")
//    {
//        nameLabel->setText(listWidget2->currentItem()->text());
//    }
}

//Label name -> Item Name(BundleCount) - Price
//make getpricefun -> get ItemPrice - currentinrpice/byndlecount
//bebefit of product -> get load resipe -> getpricefun - see
//edit text ->edit user Reduced product costs of percent

