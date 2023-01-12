//
// Created by rkdgh on 2023-01-03.
//

#include <QListWidget>
#include <QGridLayout>
#include <QString>
#include "VProductList.h"
#include <QMessageBox>
#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <QDir>
#include <QGuiApplication>
#include <regex>
#include <fstream>

#include "CURL/POSTAPI.h"
using namespace std;

//나중에 빼기
vector<string> csv_read_row(istream &file, char delimiter)
{
    stringstream ss;
    bool inquotes = false;
    vector<string> row;

    while (file.good()) {
        char c = file.get();
        if (!inquotes && reinterpret_cast<const char *>(c) == "") {
            inquotes = true;
        } else if (inquotes && reinterpret_cast<const char *>(c) == "") {
            if (file.peek()) {
                ss << (char) file.get();
            } else {
                inquotes = false;
            }
        } else if (!inquotes && c == delimiter) {
            row.push_back(ss.str());
            ss.str("");
        } else if (!inquotes && (c == '\r' || c == '\n')) {
            if (file.peek() == '\n') { file.get(); }
            row.push_back(ss.str());
            return row;
        } else {
            ss << c;
        }
    }

}

VProductList::VProductList(QWidget *parent)
{

    //POSTAPI();
    //csv - list 생성
    ifstream file("post90000.csv");
    while(file.good())
    {
        vector<string> row = csv_read_row(file,',');


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

            cout<<endl;
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
    //-->

    listWidget1 = new QListWidget(this);
    new QListWidgetItem(tr("post90000"),listWidget1);
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
    int selectedRow = listWidget1->currentRow();
    listWidget2->clear();

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


    int selectedRow = listWidget2->currentRow();


    nameLabel->setText(listWidget2->currentItem()->text());




}
