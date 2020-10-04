#pragma once
#ifndef BORROWINFO_H
#define BORROWINFO_H

#include "book.h"

#include <ctime>
#include <string>

using std::string;

enum borrowStatus {
    borrowing=1,
    returned=2,
    lost=3
};

struct fine
{
    int delayFine;
    int lostFine;
    int total;
};

class borrowInfo
{
public:
    string bookID;
    int borrowStatus;
    string borrowedBy;
    time_t borrowedTime;
    time_t dueTime;
    bool isReturned;
    time_t returnedTime;
    bool isRenewed;
    bool isLost;//若借阅书籍丢失，按（原价*管理员设置倍数）赔偿
    int fine;//罚款精确到分，只在完成归还操作后加入文件中

    borrowInfo(string bookID, string username);
    borrowInfo(string bookID, int borrowStatus, string borrowedBy, time_t borrowedTime,
        time_t dueTime, bool isReturned, time_t returnedTime, bool isRenewed,
        bool isLost, int fine);
    /*计算单本罚金（包含延期未还和丢失两种情形），单位为人民币分*/
    struct fine calculateFine();
};


#endif // !BORROWINFO_H