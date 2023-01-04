#ifndef LOSTARK_PE_WIDGET_H
#define LOSTARK_PE_WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include "finddialog.h"

class widget : public QWidget{
    Q_OBJECT

public:
    widget(QWidget* parent = nullptr);
    enum Mode {NavigationMode, AddingMode, EditingMode};

public slots:
    void addContact();
    void submitContact();
    void cancel();
    void next();
    void previous();
    void editContact();
    void removeContact();
    void findContact();
    void saveFile();
    void FromloadFile();

private:
    void updateInterface(Mode mode);
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *removeButton;
    QPushButton *findButton;
    QPushButton *loadButton;
    QPushButton *saveButton;

    QPushButton *submitButton;
    QPushButton *cancelButton;
    QLineEdit *nameLine;
    QTextEdit *addressText;

    QPushButton *nextButton;
    QPushButton *previousButton;


    QMap<QString,QString>contacts;
    QString oldName;
    QString oldAddress;
    Mode currentMode;

    finddialog *dialog;

};


#endif //LOSTARK_PE_WIDGET_H