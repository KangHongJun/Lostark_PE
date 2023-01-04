#include <QApplication>
#include "VProductList.h"
#include "widget.h"

#include <boost/filesystem.hpp>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //VProductList t;
    //t.show();



    widget wid;
    wid.show();

    return a.exec();
}
