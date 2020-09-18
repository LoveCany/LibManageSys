#include <string>
#include <list>
#include "borrowInfo.h"
#include "book.h"

using std::string;
using std::list;

enum userType {
	user = 1,//普通用户，可以借书/还书/申请续期/缴纳滞纳金/荐购/查看个人统计
	admin = 2,//管理员，可以进行采购/申请报废/修改单次借书时长/修改滞纳金金额/查看全体统计
	undefined = -1
};

string generateMD5(const string& src);//生成MD5值的方法

class User
{
	userType type;
	string username;//用户名，登录时使用，不可更改
	string realName;//用户姓名，终端显示用
	string passwordMD5;//用户密码计算MD5值后存储用作鉴权时比对，不直接明文储存
	list<int> borrowing;//记录当前借书信息对应ID
	list<int> returned;//记录已还信息对应ID
public:
	User();
	User(string username,string passwordMD5);
	User(userType type, string username, string realName, string password);
	bool verifyPassword(string pw);//鉴权时校验输入密码是否与注册时一致
	bool modifyName(string newName);//修改真实姓名
	bool modifyPassword(string pw);
	list<int> borrowingInfo();
	borrowInfo newBorrow();
	list<int> returnedInfo();
	borrowInfo newReturn();
	userType getType();
};

