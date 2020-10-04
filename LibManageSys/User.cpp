#include "User.h"
#include <openssl/md5.h>
#include <json/json.h>
#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

string generateMD5(const std::string& src)
{
	MD5_CTX ctx;

	std::string md5Str;
	unsigned char md[16] = { 0 };
	char tmp[33] = { 0 };

	MD5_Init(&ctx);
	MD5_Update(&ctx, src.c_str(), src.size());
	MD5_Final(md, &ctx);

	for (int i = 0; i < 16; i++)
	{
		memset(tmp, 0x00, sizeof(tmp));
		sprintf_s(tmp, "%02X", md[i]);
		md5Str += tmp;
	}

	return md5Str;
}

User::User()
{
	type = undefined;
	username = "";
	realName = "";
	passwordMD5 = "";
}

User::User(userType type, std::string username, std::string realName, std::string password)
{
	this->type = type;
	this->username = username;
	this->realName = realName;
	this->passwordMD5 = generateMD5(password);
	Json::Value root;
	root["username"] = Json::Value(username);
	root["realName"] = Json::Value(realName);
	root["passwordMD5"] = Json::Value(passwordMD5);
	root["type"] = Json::Value((int)type);
	root["borrowingInfo"].resize(0);
	root["returnedInfo"].resize(0);
	Json::StreamWriterBuilder wBuilder;
	wBuilder.setDefaults(&root);
	root["emitUTF8"] = true;

	std::ofstream os;
	std::string filename = "info\\users\\" + username + ".json";
	os.open(filename, std::ios::out | std::ios::app);
	if (!os.is_open())
	{
		std::cout << "ERROR: cannot create file \"" + filename +"\"." << std::endl;
	}
	os << root;
	os.close();
}

User::User(string username,string passwordMD5)
{
	string filename = "info\\users\\"+ username + ".json";
	std::ifstream in(filename);
	type = undefined;
	if (!in.is_open())
	{//用户不存在
		username = "";
		realName = "";
		passwordMD5 = "";
	}
	else
	{
		Json::Value root;
		Json::CharReaderBuilder rbuilder;
		rbuilder["collectComments"] = false;
		string errs;
		
		if (Json::parseFromStream(rbuilder, in, &root, &errs))
		{
			if (!verifyPassword(root["passwordMD5"].asString()))
			{//密码错误
				username = "";
				realName = "";
				passwordMD5 = "";
			}
			else
			{//密码正确
				int typeNum = root["type"].asInt();
				switch (typeNum)
				{
				case 1:
					type = user;
					break;
				case 2:
					type = admin;
					break;
				default:
					type = undefined;
					break;
				}
				username = root["username"].asString();
				realName = root["realName"].asString();
				passwordMD5 = root["passwordMD5"].asString();
				for (int i = 0; i < root["borrowingInfo"].size(); i++)
				{
					string bookID = root["borrowingInfo"][i]["bookID"].asString();
					int borrowStatus = root["borrowingInfo"][i]["borrowStatus"].asInt();
					string borrowedBy = root["borrowingInfo"][i]["borrowedBy"].asString();
					time_t borrowedTime = root["borrowingInfo"][i]["borrowedTime"].asInt();
					time_t dueTime = root["borrowingInfo"][i]["dueTime"].asInt();
					bool isReturned = root["borrowingInfo"][i]["isReturned"].asBool();
					time_t returnedTime = root["borrowingInfo"][i]["returnedTime"].asBool();
					bool isRenewed = root["borrowingInfo"][i]["isRenewed"].asBool();
					bool isLost = root["borrowingInfo"][i]["isLost"].asBool();
					int fine = root["borrowingInfo"][i]["fine"].asBool();
					borrowInfo thisInfo(bookID, borrowStatus, borrowedBy, borrowedTime, dueTime,
						isReturned, returnedTime, isRenewed, isLost, fine);
					borrowingInfo.push_back(thisInfo);
				}
				for (int i = 0; i < root["returnedInfo"].size(); i++)
				{
					string bookID = root["returnedInfo"][i]["bookID"].asString();
					int borrowStatus = root["returnedInfo"][i]["borrowStatus"].asInt();
					string borrowedBy = root["returnedInfo"][i]["borrowedBy"].asString();
					time_t borrowedTime = root["returnedInfo"][i]["borrowedTime"].asInt();
					time_t dueTime = root["returnedInfo"][i]["dueTime"].asInt();
					bool isReturned = root["returnedInfo"][i]["isReturned"].asBool();
					time_t returnedTime = root["returnedInfo"][i]["returnedTime"].asBool();
					bool isRenewed = root["returnedInfo"][i]["isRenewed"].asBool();
					bool isLost = root["returnedInfo"][i]["isLost"].asBool();
					int fine = root["returnedInfo"][i]["fine"].asBool();
					borrowInfo thisInfo(bookID, borrowStatus, borrowedBy, borrowedTime, dueTime,
						isReturned, returnedTime, isRenewed, isLost, fine);
					returnedInfo.push_back(thisInfo);
				}
			}
		}
	}
	in.close();
}

bool User::verifyPassword(string pw)
{
	if (generateMD5(pw).compare(passwordMD5))
	{
		return true;
	}
	else {
		return false;
	}
}

bool User::modifyName(string newName)
{
	realName = newName;
	return true;
}

bool User::modifyPassword(string pw)
{
	if (verifyPassword(pw)==0)
	{
		return false;//新密码不能与旧密码一致
	}
	else
	{
		passwordMD5 = generateMD5(pw);
		return true;
	}
}

borrowInfo User::newBorrow(book book)
{
	borrowInfo newInfo = borrowInfo(book.bookID, username);
	borrowingInfo.push_back(newInfo);
	char borrowAt[64];
	strftime(borrowAt, sizeof(borrowAt), "%Y-%m-%d %H:%M:%S", localtime(&newInfo.borrowedTime));
	char returnDue[64];
	strftime(returnDue, sizeof(returnDue), "%Y-%m-%d %H:%M:%S", localtime(&(newInfo.returnedTime)));
	std::cout << "\"bookID\": " << newInfo.bookID <<",\n" << "借阅人: " << newInfo.borrowedBy << ",\n" \
		<< "借阅时间: " << (string)borrowAt << ",\n" << "应归还时间: " << (string)returnDue << std::endl;
	book.bookStatus = borrowed;
	book.borrowedTime++;
	book.updateBookInfo();
	updateInfo();
	return newInfo;
	std::cout << "请记得按期归还，逾期未还将收取罚金；同时在借阅期间请保持书籍干净整洁，为后人借阅提供方便~\n";
	std::cout << "按任意键返回主菜单……\n";
	std::cin.get();
}

borrowInfo User::newReturn(borrowInfo borrow)
{
	std::cout << "当前书籍是否完好归还，如是请输入Y，丢失或损坏请输入其他任意键：";
	string order;
	std::cin >> order;
	if (order == "Y")
	{
		borrow.isLost = false;
	}
	else 
	{
		borrow.isLost = true;
	}
	fine fine = borrow.calculateFine();
	if (fine.total > 0)
	{
		std::cout << std::setiosflags(std::ios::fixed);
		std::cout << "逾期罚金：" << std::setprecision(2) << (float)fine.delayFine / 100 << "元\n";
		if (borrow.isLost)
		{
			std::cout<<"丢失/损坏罚金："<< std::setprecision(2) << (float)fine.lostFine / 100 << "元\n";
		}
		std::cout << "当前所需支付罚金共" << std::setprecision(2) << (float)fine.total / 100 << "元，完成支付后按任意键返回主菜单……\n";
		std::cin.get();
	}
	borrow.returnedTime = time(NULL);
	returnedInfo.push_back(borrow);
	for (auto ip = borrowingInfo.begin(); ip != borrowingInfo.end(); ip++)
	{
		if (ip->bookID == borrow.bookID) 
		{
			borrowingInfo.erase(ip);
		}
	}
	book book = searchBookByID(borrow.bookID);
	book.bookStatus = available;
	book.updateBookInfo();
	updateInfo();
	return borrow;
}

userType User::getType()
{
	return type;
}

void User::updateInfo()
{
	Json::Value root;
	root["username"] = Json::Value(username);
	root["realName"] = Json::Value(realName);
	root["passwordMD5"] = Json::Value(passwordMD5);
	root["type"] = Json::Value((int)type);
	for (int i = 0; i < borrowingInfo.size(); i++)
	{
		root["borrowingInfo"]["bookID"] = borrowingInfo[i].bookID;
		root["borrowingInfo"]["borrowStatus"] = borrowingInfo[i].borrowStatus;
		root["borrowingInfo"]["borrowedBy"] = borrowingInfo[i].borrowedBy;
		root["borrowingInfo"]["borrowedTime"] = borrowingInfo[i].borrowedTime;
		root["borrowingInfo"]["dueTime"] = borrowingInfo[i].dueTime;
		root["borrowingInfo"]["isRenewed"] = borrowingInfo[i].isRenewed;
		root["borrowingInfo"]["isReturned"] = borrowingInfo[i].isReturned;
		root["borrowingInfo"]["returnedTime"] = borrowingInfo[i].returnedTime;
		root["borrowingInfo"]["isLost"] = borrowingInfo[i].isLost;
		root["borrowingInfo"]["fine"] = borrowingInfo[i].fine;
	}
	borrowInfo** returnArr = new borrowInfo * [returnedInfo.size()];
	for (int i = 0; i < returnedInfo.size(); i++)
	{
		root["returnedInfo"]["bookID"] = returnedInfo[i].bookID;
		root["returnedInfo"]["borrowStatus"] = returnedInfo[i].borrowStatus;
		root["returnedInfo"]["borrowedBy"] = returnedInfo[i].borrowedBy;
		root["returnedInfo"]["borrowedTime"] = returnedInfo[i].borrowedTime;
		root["returnedInfo"]["dueTime"] = returnedInfo[i].dueTime;
		root["returnedInfo"]["isRenewed"] = returnedInfo[i].isRenewed;
		root["returnedInfo"]["isReturned"] = returnedInfo[i].isReturned;
		root["returnedInfo"]["returnedTime"] = returnedInfo[i].returnedTime;
		root["returnedInfo"]["isLost"] = returnedInfo[i].isLost;
		root["returnedInfo"]["fine"] = returnedInfo[i].fine;
	}
	Json::StreamWriterBuilder wBuilder;
	wBuilder.setDefaults(&root);
	root["emitUTF8"] = true;
	

	std::ofstream os;
	std::string filename = "info\\users\\" + username + ".json";
	os.open(filename, std::ios::out | std::ios::app);
	if (!os.is_open())
	{
		std::cout << "ERROR: cannot create file \"" + filename + "\"." << std::endl;
	}
	os << root;
	os.close();
}
