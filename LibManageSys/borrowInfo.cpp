#pragma once
#include "borrowInfo.h"
#include "book.h"
#include "User.h"

borrowInfo::borrowInfo(string bookID, string username)
{
    this->bookID = bookID;
    this->borrowStatus = borrowing;
    this->borrowedBy = username;
    this->borrowedTime = time(NULL);
    this->dueTime = this->borrowedTime + 86400 * 30;
    this->isReturned = false;
    this->returnedTime = 0;
    this->isRenewed = false;
    this->isLost = true;
    this->fine = 0;
}

borrowInfo::borrowInfo(string bookID, int borrowStatus, string borrowedBy, time_t borrowedTime, 
    time_t dueTime, bool isReturned, time_t returnedTime, bool isRenewed, bool isLost, int fine)
{
    this->bookID = bookID;
    this->borrowStatus = borrowStatus;
    this->borrowedBy = borrowedBy;
    this->borrowedTime = borrowedTime;
    this->dueTime = dueTime;
    this->isRenewed = isRenewed;
    this->isReturned = isReturned;
    this->returnedTime = returnedTime;
    this->isLost = isLost;
    this->fine = fine;
}

fine borrowInfo::calculateFine()
{
    int delayFine = 0;
    int lostFine = 0;
    if (isLost)
    {
        lostFine = config.lostFineRatio * searchBookByID(bookID).price;
    }
    /*dueTime实际合算时应精确到天，超过1天后才应开始收取罚金*/
    if (dueTime / 86400 < time(NULL) / 86400)
    {
        delayFine = 0;
    }
    else
    {
        int delayDays = (time(NULL) - dueTime) / 86400;
        delayFine = config.delayFineRatio * delayDays;
    }
    this->fine = delayFine + lostFine;
    return {delayFine, lostFine, (delayFine + lostFine)};
}
