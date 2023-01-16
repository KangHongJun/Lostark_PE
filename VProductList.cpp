//https://doc.qt.io/qt-6/classes.html
// Created by rkdgh on 2023-01-03.
//

typedef unsigned char Byte;
typedef Byte cs_byte;

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
    POSTAPI();


    ifstream file("post90000.csv");
    while(file.good())
    {
        vector<string> row = POSTAPI::csv_read_row(file,',');

        if(!row[0].find('#'))
        {
            continue;
        }
        else
        {
            for(auto v: row)
            {
                QString listname = QString::fromStdString(row[POSTAPI::eInfoItem::Name]);
                nameA.append(listname);
                break;
            }
        }
    }


    addButton = new QPushButton(tr("&Add"));
    connect(addButton,&QPushButton::clicked,this,&VProductList::SetSelectedCategory);
    nameLabel = new QLabel(tr("Name"));
    nameLabel->hide();


    nameB.append("B1");
    nameB.append("B2");
    nameB.append("B3");

    nameC.append("C1");
    nameC.append("C2");
    nameC.append("C3");

    listWidget1 = new QListWidget(this);
    new QListWidgetItem(tr("post90001"),listWidget1);
    new QListWidgetItem(tr("name2"),listWidget1);
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
            for(int i=0;i<nameA.length();i++)
            {
                new QListWidgetItem(nameA[i],listWidget2);
            }
            break;
        case 1:
            for(int i=0;i<nameB.length();i++)
            {
                new QListWidgetItem(nameB[i],listWidget2);
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


    nameLabel->setText(listWidget2->currentItem()->text());

    //POSTAPI에서 선언해봤으나 에러.. 나중에 수정하자
    std::vector<std::pair<std::string,std::string>> ItemV;
    ifstream file("post90000.csv");

    while(true)
    {
        vector<string> read_row = POSTAPI::csv_read_row(file,',');

        if(read_row[0]=="@")
            break;

        if(!read_row[0].find('#'))
        {
            continue;
        }
        else
        {
            ItemV.emplace_back(read_row[POSTAPI::eInfoItem::Name],read_row[POSTAPI::eInfoItem::CurrentMinPrice]);
        }
    }

    QString qqq = QString::fromStdString(ItemV[5].first);
    nameLabel->setText(qqq);
}
