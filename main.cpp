#include <QApplication>
#include "VProductList.h"
#include "widget.h"

#include <iostream>
int main(int argc, char *argv[])
{
    std::cout<<"a"<<std::endl;
    QApplication a(argc, argv);
    VProductList t;
    t.show();



    //widget widget;
    //widget.show();
    return a.exec();
}
