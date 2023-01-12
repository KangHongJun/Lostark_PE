//
// Created by rkdgh on 2022-12-28.
//example - Qwidget

#include <QLabel>
#include <QGridLayout>
#include <QMessageBox>
#include <QFileDialog>
#include "widget.h"

widget::widget(QWidget *parent)
{
    QLabel *nameLabel = new QLabel(tr("Name"));
    nameLine = new QLineEdit;
    nameLine->setReadOnly(true);

    QLabel *addressLabel = new QLabel(tr("Address"));
    addressText = new QTextEdit;
    addressText->setReadOnly(true);

    //button
    addButton = new QPushButton(tr("&Add"));
   // addButton->show();
    editButton = new QPushButton(tr("&Edit"));
    editButton->setEnabled(false);
    removeButton = new QPushButton(tr("&Remove"));
    removeButton->setEnabled(true);
    findButton = new QPushButton(tr("&Find"));
    findButton->setEnabled(false);
    submitButton = new QPushButton(tr("&Submit"));
    submitButton->hide();
    cancelButton = new QPushButton(tr("&Cancel"));
    cancelButton->hide();

    nextButton = new QPushButton(tr("&Next"));
    nextButton->setEnabled(false);
    previousButton = new QPushButton(tr("&Previous"));
    previousButton->setEnabled(false);

    saveButton = new QPushButton(tr("&Save"));
    saveButton->setToolTip(tr("Save contacts to a file"));
    saveButton->setEnabled(false);
    loadButton = new QPushButton(tr("&Load"));
    loadButton->setToolTip(tr("Load contacts from a file"));
    loadButton->setEnabled(false);


    dialog = new finddialog(this);

    connect(addButton,&QPushButton::clicked,
            this,&widget::addContact);
    connect(submitButton,&QPushButton::clicked,
            this,&widget::submitContact);
    connect(cancelButton,&QPushButton::clicked,
            this,&widget::cancel);
    connect(nextButton,&QPushButton::clicked,
            this,&widget::next);
    connect(previousButton,&QPushButton::clicked,
            this,&widget::previous);
    connect(findButton,&QPushButton::clicked,
            this,&widget::findContact);

    connect(editButton,&QPushButton::clicked,
            this,&widget::editContact);
    connect(removeButton,&QPushButton::clicked,
            this,&widget::removeContact);

    connect(saveButton,&QPushButton::clicked,
            this,&widget::saveFile);
    connect(loadButton,&QPushButton::clicked,
            this,&widget::saveFile);

    QVBoxLayout *buttonLayout1 = new QVBoxLayout;
    buttonLayout1->addWidget(addButton,Qt::AlignTop);
    buttonLayout1->addWidget(submitButton);
    buttonLayout1->addWidget(cancelButton);
    buttonLayout1->addWidget(editButton);
    buttonLayout1->addWidget(removeButton);
    buttonLayout1->addWidget(findButton);
    buttonLayout1->addWidget(saveButton);
    buttonLayout1->addWidget(loadButton);
    buttonLayout1->addStretch();//버튼 사이의 공간 조정



    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(nameLabel,0,0);
    mainLayout->addWidget(nameLine,0,1);
    mainLayout->addWidget(addressLabel,1,0,Qt::AlignTop);
    mainLayout->addWidget(addressText,1,1);
    mainLayout->addLayout(buttonLayout1,1,2);

    QHBoxLayout *buttonLayout2 = new QHBoxLayout;
    buttonLayout2->addWidget(previousButton);
    buttonLayout2->addWidget(nextButton);
    mainLayout->addLayout(buttonLayout2,2,1);

    setLayout(mainLayout);
    setWindowTitle("test");
}

void widget::addContact()
{
    oldName = nameLine->text();
    oldAddress = addressText->toPlainText();

    nameLine->clear();
    addressText->clear();

    updateInterface(AddingMode);
}

void widget::editContact()
{
    oldName = nameLine->text();
    oldAddress = addressText->toPlainText();

    updateInterface(EditingMode);
}

void widget::submitContact()
{
    QString name = nameLine->text();
    QString address = addressText->toPlainText();

    if(name.isEmpty() || address.isEmpty())
    {
        QMessageBox::information(this,tr("empty Field"),
                                 tr("enter a name and address"));
        return;
    }

    if(currentMode == AddingMode)
    {
        if(!contacts.contains(name))
        {
            contacts.insert(name,address);
            QMessageBox::information(this,tr("Add Successful"),
                                     tr("\"%1\" has been added").arg(name));
        }
        else
        {
            QMessageBox::information(this,tr("Add UnSuccessful"),
                                     tr("\"%1\" has been added").arg(name));
        }
    }
    else if(currentMode == EditingMode)
    {
        if (oldName != name)
        {
            if(!contacts.contains(name))
            {
                contacts.insert(name,address);
                QMessageBox::information(this,tr("Edit Successful"),
                                         tr("\"%1\" has been added").arg(name));
                contacts.remove(oldName);
                contacts.insert(name,address);
            }
            else
            {
                QMessageBox::information(this,tr("Edit UnSuccessful"),
                                         tr("\"%1\" has been added").arg(name));

            }
        }
        else if(oldAddress != address)
        {
            QMessageBox::information(this,tr("Edit Successful"),
                                     tr("\"%1\" has been adit").arg(name));
            contacts[name] = address;
        }
    }
    updateInterface(NavigationMode);
}

void widget::cancel()
{
    nameLine->setText(oldName);
    addressText->setText(oldAddress);

    updateInterface(NavigationMode);
}

void widget::next()
{
    QString name = nameLine->text();
    QMap<QString,QString>::iterator i = contacts.find(name);

    if(i!=contacts.end())
        i++;
    if(i==contacts.end())
        i = contacts.begin();

    nameLine->setText(i.key());
    addressText->setText(i.value());
}

void widget::previous()
{
    QString name = nameLine->text();
    QMap<QString,QString>::iterator i = contacts.find(name);

    if(i==contacts.end())
    {
        nameLine->clear();
        addressText->clear();
        return;
    }

    if (i==contacts.begin())
        i = contacts.end();
    i--;

    nameLine->setText(i.key());
    addressText->setText(i.value());
}

void widget::removeContact()
{
    QString name = nameLine->text();
    QString address = addressText->toPlainText();

    if(contacts.contains(name))
    {
        int button = QMessageBox::question(this,
                                           tr("Confirm Remove"),
                                           tr("remove \"%1\" ?").arg(name),
                                           QMessageBox::Yes|QMessageBox::No);

        if(button == QMessageBox::Yes)
        {
            previous();
            contacts.remove(name);

            QMessageBox::information(this, tr("Remove Successful"),
                                     tr("\"%1\" has been removed").arg(name));
        }
    }
    updateInterface(NavigationMode);
}

void widget::updateInterface(Mode mode)
{
    currentMode = mode;

    switch(currentMode)
    {
        case AddingMode:
        case EditingMode:

            nameLine->setReadOnly(false);
            nameLine->setFocus(Qt::OtherFocusReason);
            addressText->setReadOnly(false);

            addButton->setEnabled(false);
            editButton->setEnabled(false);
            removeButton->setEnabled(false);

            nextButton->setEnabled(false);
            previousButton->setEnabled(false);

            submitButton->show();
            cancelButton->show();
            break;
        case NavigationMode:
            if(contacts.isEmpty())
            {
                nameLine->clear();
                addressText->clear();
            }

            nameLine->setReadOnly(true);
            addressText->setReadOnly(true);
            addButton->setEnabled(true);

            int number = contacts.size();
            editButton->setEnabled(number >= 1);
            removeButton->setEnabled(number >= 1);
            findButton->setEnabled(number>1);
            nextButton->setEnabled(number > 1);
            previousButton->setEnabled(number >1 );
            saveButton->setEnabled(number >= 1);

            submitButton->hide();
            cancelButton->hide();
            break;
    }
}

void widget::findContact()
{
    dialog->show();

    if(dialog->exec() == QDialog::Accepted)
    {
        QString contactName = dialog->getFindText();

        if (contacts.contains(contactName))
        {
            nameLine->setText(contactName);
            addressText->setText(contacts.value(contactName));
        }
        else
        {
            QMessageBox::information(this, tr("Contact Not Found"),
                                     tr("Sorry, \"%1\" is not in your address book.").arg(contactName));
            return;
        }
    }
    updateInterface(NavigationMode);
}

void widget::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Address"),"",
                                                    tr("Address (*.abk);;All (*)"));
    if (fileName.isEmpty())
        return;
    else
    {

    }
}
void widget::FromloadFile()
{

}