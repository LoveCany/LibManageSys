#pragma once
#ifndef USER_H

#define USER_H
#include "config.h"
#include "borrowInfo.h"
#include <string>
#include <vector>

#include "book.h"




using std::string;
using std::vector;

enum userType {
	user = 1,//普通用户，可以借书/还书/申请续期/缴纳滞纳金/查看个人统计
	admin = 2,//管理员，可以进行采购/申请报废/修改单次借书时长/修改滞纳金金额/查看全体统计
	undefined = -1
};

string generateMD5(const string& src);//生成MD5值的方法

class User
{
	userType type;
	string realName;//用户姓名，终端显示用
	string passwordMD5;//用户密码计算MD5值后存储用作鉴权时比对，不直接明文储存
public:
	string username;//用户名，登录时使用，不可更改
	User();
	User(string username,string passwordMD5);
	User(userType type, string username, string realName, string password);
	bool verifyPassword(string pw);//鉴权时校验输入密码是否与注册时一致
	bool modifyName(string newName);//修改真实姓名
	bool modifyPassword(string pw);
	vector<borrowInfo> borrowingInfo;
	vector<borrowInfo> returnedInfo;
	/*借书与还书都应当逐本进行*/
	borrowInfo newBorrow(book book);
	borrowInfo newReturn(borrowInfo borrow);
	userType getType();
	/*完成借/还书后同时同步信息到文件*/
	void updateInfo();
};

#endif // !USER_H