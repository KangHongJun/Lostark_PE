//
// Created by rkdgh on 2023-01-03.
//

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QDebug>

#ifndef LOSTARK_PE_VPRODUCTLIST_H
#define LOSTARK_PE_VPRODUCTLIST_H



class VProductList : public QWidget {
    Q_OBJECT

public:
    VProductList(QWidget *parent = nullptr);

public slots:
    void SetSelectedCategory();

private:
    QListWidget *listWidget1;
    QListWidget *listWidget2;
    QPushButton *addButton;

protected:
    QStringList nameA;
    QStringList nameB;
    QStringList nameC;
};


#endif //LOSTARK_PE_VPRODUCTLIST_H
