#include "book.h"
#include <ctime>
#include <string>

using std::string;

enum borrowStatus {
    borrowing=1,
    returned=2,
    lost=3
};

class borrowInfo :
    public book
{
    borrowStatus status;
    string borrowedBy;
    tm borrowedTime;
    tm dueTime;
    bool isReturned;
    tm returnedTime;
    bool isRenewed;
    bool isLost;//若借阅书籍丢失，按（原价*管理员设置倍数）赔偿
    int delayFine;//罚款精确到分，只在完成归还操作时计算并加入文件中
};

