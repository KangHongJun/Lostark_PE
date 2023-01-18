//
// Created by rkdgh on 2023-01-11.
//

typedef unsigned char Byte;
typedef Byte cs_byte;

#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <fstream>
#include "POSTAPI.h"
#include <curl/curl.h>
using namespace std;
//물품관련 개선
//현 POSTAPI를 다른 함수로 빼고,
//url, csv name, categoty code, header,,,

POSTAPI::POSTAPI()
{
    CURL *curl;
    CURLcode res;
    std::string url = "https://developer-lostark.game.onstove.com/markets/items";

    bool bEndPage = false;

    ofstream fout;
    fout.open("post90000.csv");
    int page = 1;

    while(!bEndPage)
    {
        string strpage = to_string(page);
        std::string category = "{\"CategoryCode\":90000,"
                               "\"PageNo\":" + strpage + std::string("}");
        struct curl_slist *headerlist = nullptr;
        headerlist = curl_slist_append(headerlist, "Accept: application/json");
        headerlist = curl_slist_append(headerlist, "Content-Type: application/json");
        headerlist = curl_slist_append(headerlist, "charset=UTF-8");
        headerlist = curl_slist_append(headerlist, "Authorization: Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsIng1dCI6IktYMk40TkRDSTJ5NTA5NWpjTWk5TllqY2lyZyIsImtpZCI6IktYMk40TkRDSTJ5NTA5NWpjTWk5TllqY2lyZyJ9.eyJpc3MiOiJodHRwczovL2x1ZHkuZ2FtZS5vbnN0b3ZlLmNvbSIsImF1ZCI6Imh0dHBzOi8vbHVkeS5nYW1lLm9uc3RvdmUuY29tL3Jlc291cmNlcyIsImNsaWVudF9pZCI6IjEwMDAwMDAwMDAwNjIzNDkifQ.QFz3lboYqWyvFwjiJXqjqxC3LvnEALpEaEItA_ORvPNtVViOCPIrjaOnkzs22tQpeKOgKxJ-K_OVh54ZGIo4QBLOX2ZNWV6KsevVz7gxXvuysesHzKVdP-_kSjn7T6X-X4CU7bdKXkYReM2AiROeT8HqENPd5L6ZGkGUcfiP319S9d57As0wyJWvhg5UuFStMPeitytFkE6IDm9o4I6Hu1_aiu3TO2YmwmnEZttk3J6HAFIO3eDqy2SgNNAGLZoYscbplhF1meQQMj3NQlkQsCdHfwgpLSJjpYy06LY4e70-UsGQWx_ckWbErSEfVh1f2PLzIdyelbXTzHoi_CIH3g");

        curl_global_init(CURL_GLOBAL_ALL);

        curl = curl_easy_init();

        if(curl) {
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
            //curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

            res = curl_easy_perform(curl);//위치 중요

            string APIdata_json = chunk.memory;

            int startN = 2;
            int endN;
            std::string APIdata_split;


            //데이터 csv로 저장 - 이부분은 따로 함수화하여 현재 함수 page를 넘기면서 실행
            while (startN < APIdata_json.size()) {

                startN = APIdata_json.find("{", startN) + 1;
                endN = APIdata_json.find("}", startN) - startN;

                APIdata_split = APIdata_json.substr(startN, endN);

                if(startN==0)
                {
                    bEndPage = true;
                    break;
                }


                //컬럼명 조작하기 귀찮으니 데이터만
//                if (bFirst) {
//                    auto m = mappify2(APIdata_split);
//
//                    for (auto const &p: m) {
//
//                        fout << regex_replace(p.first, regex("\""), "") + ",";
//                    }
//                    fout << "\n";
//                    bFirst = false;
//                }

                auto m = mappify2(APIdata_split);

                for (auto const &p: m) {
                    fout << regex_replace(p.second, regex("\""), "") + ",";
                }
                fout << "\n";

                if (startN + endN + 10 > APIdata_json.size())
                    break;

                startN += 10;
            }
        }

        page++;
    }
    fout<<"@,@,@,@,@,@,@,@,@,";
    fout.close();
}

//copy
size_t POSTAPI::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
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

//copy
std::map<std::string, std::string> POSTAPI::mappify2(std::string const& s)
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

//copy
vector<string> POSTAPI::csv_read_row(istream &file, char delimiter)
{
    stringstream ss;
    bool inquotes = false;
    vector<string> row;

    while (file.good())
    {
        char c = file.get();
        if (!inquotes && reinterpret_cast<const char *>(c) == "")
        {
            inquotes = true;
        }
        else
        if (inquotes && reinterpret_cast<const char *>(c) == "")
        {
            if (file.peek())
            {
                ss << (char) file.get();
            }
            else
            {
                inquotes = false;
            }
        } else if (!inquotes && c == delimiter)
        {
            row.push_back(ss.str());
            ss.str("");
        } else if (!inquotes && (c == '\r' || c == '\n'))
        {
            if (file.peek() == '\n') { file.get(); }
            row.push_back(ss.str());
            return row;
        }
        else
        {
            ss << c;
        }
    }
}
