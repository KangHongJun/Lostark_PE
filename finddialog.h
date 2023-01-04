//
// Created by rkdgh on 2023-01-04.
//

#ifndef LOSTARK_PE_FINDDIALOG_H
#define LOSTARK_PE_FINDDIALOG_H
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>


class finddialog : public QDialog{
    Q_OBJECT

public:
    finddialog (QWidget *parent = nullptr);
    QString getFindText();

public slots:
    void findClicked();

private:
    QPushButton *findButton;
    QLineEdit *lineEdit;
    QString findText;

};


#endif //LOSTARK_PE_FINDDIALOG_H
