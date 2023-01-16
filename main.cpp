#include <QApplication>
#include "VProductList.h"
#include <iostream>

using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VProductList t;
    t.show();




    return a.exec();

    return 0;
}
