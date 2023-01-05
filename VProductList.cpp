//
// Created by rkdgh on 2023-01-03.
//

#include <QListWidget>
#include <QGridLayout>
#include <QString>
#include "VProductList.h"
#include <QMessageBox>

#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

using namespace std;

VProductList::VProductList(QWidget *parent)
{
    addButton = new QPushButton(tr("&Add"));
    connect(addButton,&QPushButton::clicked,this,&VProductList::SetSelectedCategory);
    nameLabel = new QLabel(tr("Name"));
    nameLabel->hide();

    //추후 db에서 조회하여 반복문처리
    nameA.append("A1");
    nameA.append("A2");
    nameA.append("A3");

    nameB.append("B1");
    nameB.append("B2");
    nameB.append("B3");

    nameC.append("C1");
    nameC.append("C2");
    nameC.append("C3");
    //-->





    listWidget1 = new QListWidget(this);
    new QListWidgetItem(tr("name"),listWidget1);
    new QListWidgetItem(tr("name2"),listWidget1);
    new QListWidgetItem(tr("name3"),listWidget1);

    listWidget2 = new QListWidget(this);

    connect(listWidget1,&QListWidget::itemClicked,
            this,&VProductList::SetSelectedCategory);

    connect(listWidget2,&QListWidget::itemClicked,
            this,&VProductList::SetViewTest);


    auto *QHbox = new QHBoxLayout(this);
    QHbox->addWidget(listWidget1);
    QHbox->addWidget(listWidget2);
    QHbox->addWidget(addButton);
    QHbox->addWidget(nameLabel);

    setLayout(QHbox);
    setWindowTitle("tt");
}

void VProductList::SetSelectedCategory()
{
    int selectedRow = listWidget1->currentRow();
    listWidget2->clear();

    switch (selectedRow)
    {
        case 0:
            for(int i=0;i<nameA.length();i++)
            {
                new QListWidgetItem(nameA[i],listWidget2);
            }
            break;
        case 1:
            for(int i=0;i<nameB.length();i++)
            {
                new QListWidgetItem(nameB[i],listWidget2);
            }
            break;
        case 2:
            for(int i=0;i<nameC.length();i++)
            {
                new QListWidgetItem(nameC[i],listWidget2);
            }
            break;
    }
}

void VProductList::SetViewTest()
{
    nameLabel->clear();
    nameLabel->show();

    boost::asio::io_context ioc;
    boost::asio::ip::tcp::resolver resolver(ioc);
    boost::beast::tcp_stream stream(ioc);

    try
    {
        auto const host = "jsonplaceholder.typicode.com";
        auto const port = "80";
        auto const target = "/posts/1";
        bool isVer1_0 = false;
        int version = isVer1_0 ? 10 : 11;

        auto const results = resolver.resolve(host, port);
        stream.connect(results);


        string urlHost = host;
        urlHost += ":";
        urlHost += port;

        boost::beast::http::request<boost::beast::http::string_body> req{ boost::beast::http::verb::get, target, version };
        req.set(boost::beast::http::field::host, urlHost);
        req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        boost::beast::http::write(stream, req);

        boost::beast::flat_buffer buffer;

        boost::beast::http::response<boost::beast::http::dynamic_body> res;

        boost::beast::http::read(stream, buffer, res);

        string json = boost::beast::buffers_to_string(res.body().data());

        QString test = QString::fromStdString(json);

        nameLabel->setText(test);

        boost::beast::error_code ec;
        stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

        if (ec && ec != boost::beast::errc::not_connected)
        {
            clog << "error: " << ec.message() << endl;
            return;
        }
    }
    catch (std::exception const& ex) {
        //nameLabel->setText(ex.what());
        return;
    }



    int selectedRow2 = listWidget2->currentRow();
    //






}
