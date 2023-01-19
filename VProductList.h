//
// Created by rkdgh on 2023-01-03.
//
#include <QLabel>
#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QDebug>
#include <QLineEdit>
#include <QSettings>
#include <QSettings>
#include <QGridLayout>

#ifndef LOSTARK_PE_VPRODUCTLIST_H
#define LOSTARK_PE_VPRODUCTLIST_H



class VProductList : public QWidget {
    Q_OBJECT

public:
    VProductList(QWidget *parent = nullptr);
    std::vector<std::pair<std::string,std::string>> ItemCMP;
    std::vector<std::pair<std::string,std::string>> ItemBC;
    std::vector<std::pair<std::string,std::string>> ItemRC;

    //UI
    void MakeCategoryListWidget();




    float getItemPrice(std::string ItemName);
    float getBenefitProduct(std::string ItemName);
    int DC_product_cost(float value);
    int see(int value);



public slots:
    void SetSelectedCategory();
    void SetViewTest();


private:
    QListWidget *category_listWidget;
    QListWidget *listWidget2;
    QPushButton *reloadButton;
    QLabel *nameLabel;
    QLineEdit *editDC;

    QString test;

    //setting
    void loadSetting();
    void saveSetting();



protected:
    QStringList post90000;
    QStringList productA;
};


#endif //LOSTARK_PE_VPRODUCTLIST_H
