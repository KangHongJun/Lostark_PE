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

#include <QDir>
#include <QGuiApplication>


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


#include <curl/curl.h>
struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{

    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = static_cast<char *>(realloc(mem->memory, mem->size + realsize + 1));

    if(!ptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

//***
std::map<std::string, std::string> mappify1(std::string const& s)
{
    std::map<std::string, std::string> m;

    std::string key, val;
    std::istringstream iss(s);

    while(std::getline(std::getline(iss, key, ':') >> std::ws, val))
        m[key] = val;

    return m;
}

std::map<std::string, std::string> mappify2(std::string const& s)
{
    std::map<std::string, std::string> m;

    std::string::size_type key_pos = 0;
    std::string::size_type key_end;
    std::string::size_type val_pos;
    std::string::size_type val_end;

    while((key_end = s.find(':', key_pos)) != std::string::npos)
    {
        if((val_pos = s.find_first_not_of(": ", key_end)) == std::string::npos)
            break;

        val_end = s.find(',', val_pos);
        m.emplace(s.substr(key_pos, key_end - key_pos), s.substr(val_pos, val_end - val_pos));

        key_pos = val_end;
        if(key_pos != std::string::npos)
            ++key_pos;
    }

    return m;
}

static string curl_test()
{
    CURL *curl;
    CURLcode res;

    std::string url = "https://developer-lostark.game.onstove.com/markets/items";
    std::string category = "{\"CategoryCode\":90000,"
                           "\"PageNo\":1,"
                           "\"ItemGrade\":\"고급\"}";

    struct curl_slist *headerlist = nullptr;
    headerlist = curl_slist_append(headerlist, "Accept: application/json");
    headerlist = curl_slist_append(headerlist, "Content-Type: application/json");
    headerlist = curl_slist_append(headerlist, "charset=UTF-8");
    headerlist = curl_slist_append(headerlist, "Authorization: Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsIng1dCI6IktYMk40TkRDSTJ5NTA5NWpjTWk5TllqY2lyZyIsImtpZCI6IktYMk40TkRDSTJ5NTA5NWpjTWk5TllqY2lyZyJ9.eyJpc3MiOiJodHRwczovL2x1ZHkuZ2FtZS5vbnN0b3ZlLmNvbSIsImF1ZCI6Imh0dHBzOi8vbHVkeS5nYW1lLm9uc3RvdmUuY29tL3Jlc291cmNlcyIsImNsaWVudF9pZCI6IjEwMDAwMDAwMDAwNjIzNDkifQ.QFz3lboYqWyvFwjiJXqjqxC3LvnEALpEaEItA_ORvPNtVViOCPIrjaOnkzs22tQpeKOgKxJ-K_OVh54ZGIo4QBLOX2ZNWV6KsevVz7gxXvuysesHzKVdP-_kSjn7T6X-X4CU7bdKXkYReM2AiROeT8HqENPd5L6ZGkGUcfiP319S9d57As0wyJWvhg5UuFStMPeitytFkE6IDm9o4I6Hu1_aiu3TO2YmwmnEZttk3J6HAFIO3eDqy2SgNNAGLZoYscbplhF1meQQMj3NQlkQsCdHfwgpLSJjpYy06LY4e70-UsGQWx_ckWbErSEfVh1f2PLzIdyelbXTzHoi_CIH3g");

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();

    if(curl)
    {
        struct MemoryStruct chunk;
        chunk.memory = (char *) malloc(1);
        chunk.size = 0;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);


        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, category.c_str());


        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
        res = curl_easy_perform(curl);
        //need clean

        int startN = 2;
        int endN;

        //char to string
        string APIdata_json = chunk.memory;
        std::string APIdata_split;

        std::string test;



        while(startN<APIdata_json.size())
        {
            startN = APIdata_json.find("{",startN)+1;
            endN = APIdata_json.find("}",startN)-startN-1;

            cout<<startN<<"||"<<endN<<"end"<<"//"<<APIdata_json.size();

            APIdata_split = APIdata_json.substr(startN,endN);

            auto m = mappify2(APIdata_split);
            for(auto const& p: m)
            {
                std::cout << p.first << " - " << p.second << '\n';
                test += p.first + p.second;
            }

            if(startN+endN+10>APIdata_json.size())
                break;

            startN +=10;
        }

        return test;
    }
    return 0;
}

void VProductList::SetViewTest()
{
    nameLabel->clear();
    nameLabel->show();


    int selectedRow2 = listWidget2->currentRow();

    QString curlvalue = QString::fromStdString(curl_test());
    nameLabel->setText(curlvalue);

}
