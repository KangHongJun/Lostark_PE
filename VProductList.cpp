//
// Created by rkdgh on 2023-01-03.
//
#include <QListWidget>
#include <QGridLayout>
#include <QString>
#include "VProductList.h"
#include <QMessageBox>


#include <iostream>
using namespace std;

VProductList::VProductList(QWidget *parent)
{
    addButton = new QPushButton(tr("&Add"));
    connect(addButton,&QPushButton::clicked,this,&VProductList::SetSelectedCategory);

    //추후 db에서 조회하여 반복문처리
    nameA.append("A1");
    nameA.append("A2");
    nameA.append("A3");

    nameB.append("B1");
    nameB.append("B2");
    nameB.append("B3");

    nameC.append("C1");
    nameC.append("C2");
    nameC.append("C3");
    //-->





    listWidget1 = new QListWidget(this);
    new QListWidgetItem(tr("name"),listWidget1);
    new QListWidgetItem(tr("name2"),listWidget1);
    new QListWidgetItem(tr("name3"),listWidget1);

    listWidget2 = new QListWidget(this);
    //new QListWidgetItem(tr("A"),listWidget2);
    //new QListWidgetItem(tr("B"),listWidget2);
    //new QListWidgetItem(tr("C"),listWidget2);

    connect(listWidget1,&QListWidget::itemClicked,
            this,&VProductList::SetSelectedCategory);


    auto *QHbox = new QHBoxLayout(this);
    QHbox->addWidget(listWidget1);
    QHbox->addWidget(listWidget2);
    QHbox->addWidget(addButton);

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
                QString a = nameA[i];
                new QListWidgetItem(nameA[i],listWidget2);
            }
            break;
        case 1:
            for(int i=0;i<nameA.length();i++)
            {
                QString a = nameB[i];
                new QListWidgetItem(nameB[i],listWidget2);
            }
            break;
        case 2:
            for(int i=0;i<nameA.length();i++)
            {
                QString a = nameC[i];
                new QListWidgetItem(nameC[i],listWidget2);
            }
            break;
    }

}
