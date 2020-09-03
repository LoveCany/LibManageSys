#include <string>

using std::string;

enum userType {user = 1,
	admin = 2,
	undefined = -1
};

string generateMD5(const string& src);//生成MD5值的方法

class User
{
	userType type;
	string username;//用户名，登录时使用，不可更改
	string realName;//用户姓名，终端显示用
	string passwordMD5;//用户密码计算MD5值后存储用作鉴权时比对，不直接明文储存
public:
	User();
	User(string username,string passwordMD5);
	User(userType type, string username, string realName, string password);
	bool verifyPassword(string pw);//鉴权时校验输入密码是否与注册时一致
	bool modifyName(string newName);//修改真实姓名
	bool modifyPassword(string pw);
	userType getType();
};

