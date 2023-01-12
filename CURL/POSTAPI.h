//
// Created by rkdgh on 2023-01-11.
//

#ifndef LOSTARK_PE_POSTAPI_H
#define LOSTARK_PE_POSTAPI_H


struct MemoryStruct {
    char *memory;
    size_t size;
};

class POSTAPI {

public:
    POSTAPI();
    enum eInfoItem{BundleCount=0, CurrentMinPrice=1, Grade=2, Icon=3, Id=4, Name=5, RecentPrice=6, TradeRemainCount=7};
    static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
    std::map<std::string, std::string> mappify2(std::string const& s);


};


#endif //LOSTARK_PE_POSTAPI_H
