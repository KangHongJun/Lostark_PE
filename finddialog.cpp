//
// Created by rkdgh on 2023-01-04.
//

#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include "finddialog.h"

finddialog::finddialog(QWidget *parent) : QDialog(parent)
{
    QLabel *findLabel = new QLabel(tr("enter the name"));
    lineEdit = new QLineEdit;

    findButton = new QPushButton((tr("&Find")));
    findText = "";

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(findLabel);
    layout->addWidget(lineEdit);
    layout->addWidget(findButton);

    setLayout(layout);
    setWindowTitle(tr("qwe"));

    connect(findButton, &QPushButton::clicked,this, &finddialog::findClicked);
    connect(findButton, &QPushButton::clicked,this, &finddialog::accept);

}

void finddialog::findClicked()
{
    QString text = lineEdit->text();

    if(text.isEmpty())
    {
        QMessageBox::information(this,tr("Empty"),tr("enter a name"));
        return;
    }
    else
    {
        findText = text;
        lineEdit->clear();
        hide();
    }
}

QString finddialog::getFindText()
{
    return findText;
}