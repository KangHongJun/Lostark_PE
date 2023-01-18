#include <QApplication>
#include "VProductList.h"
#include <iostream>

using namespace std;

//아이콘 메뉴로
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    VProductList t;
    t.show();
    return a.exec();
}
