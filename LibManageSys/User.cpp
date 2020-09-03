#include "User.h"
#include <openssl/md5.h>
#include <json/json.h>
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
			if (root["passwordMD5"].asString().compare(passwordMD5)!=0)
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
			}
		}
	}
}

bool User::verifyPassword(string pw)
{
	if (generateMD5(pw).compare(passwordMD5)==0)
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

userType User::getType()
{
	return type;
}
