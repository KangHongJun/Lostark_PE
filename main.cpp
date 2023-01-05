#include <QApplication>
#include "VProductList.h"
#include "widget.h"
#include <iostream>

#include <curl/curl.h>

using namespace std;

int main(int argc, char *argv[])
{



    QApplication a(argc, argv);
    VProductList t;
    t.show();


    //widget wid;
    //wid.show();

    return a.exec();
}
