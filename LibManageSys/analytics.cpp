#include "analytics.h"
#include <iostream>
#include <iomanip>

using std::cout;
using std::cin;
using std::cerr;

void analytics(User currentUser)
{
	if (currentUser.getType() == user)
	{
		userAnalytics(currentUser);
	}
	else if (currentUser.getType() == admin)
	{
		adminAnalytics(currentUser);
	}
	else
	{
		cerr << "[ERROR]当前用户组不支持查看统计数据！\n";
	}
}

void userAnalytics(User user)
{
	cout << "总借阅图书（不包含当前借阅中的，单位为本*次）："<< user.returnedInfo.size() << "\n";
	int fineSum = 0;
	for (int i = 0; i < user.returnedInfo.size(); i++)
	{
		fineSum += user.returnedInfo[i].fine;
	}
	cout << std::setiosflags(std::ios::fixed);
	cout << "缴纳罚金总数：" << std::setprecision(2) << fineSum /100 << "\n";
	float averageBorrowLen = 0;
	for (int i = 0; i < user.returnedInfo.size(); i++)
	{
		averageBorrowLen += user.returnedInfo[i].returnedTime - user.returnedInfo[i].borrowedTime;
	}
	averageBorrowLen = averageBorrowLen / user.returnedInfo.size() / 86400;
	cout << std::setiosflags(std::ios::fixed);
	cout << "平均单本书籍借阅时长（天）" << std::setprecision(2) << averageBorrowLen << "\n";
	int lostTotal = 0;
	for (int i = 0; i < user.returnedInfo.size(); i++)
	{
		if (user.returnedInfo[i].isLost) {
			lostTotal++;
		}
	}
	cout << "造成丢失/损坏图书总数：" << lostTotal << "\n";
}

void adminAnalytics(User user)
{
	string filename = "info\\books.json";
	std::ifstream in(filename);
	Json::Value root;
	Json::CharReaderBuilder rbuilder;
	rbuilder["collectComments"] = false;
	string errs;
	Json::parseFromStream(rbuilder, in, &root, &errs);
	in.close();

	cout << "当前馆藏书籍总量：" << root.size() << "\n";
	
	Json::Value::Members members = root.getMemberNames();
	int borrowing = 0;
	for (Json::Value::Members::iterator iterMember = members.begin(); iterMember != members.end(); iterMember++)
	{
		string key = *iterMember;
		if (root[key.c_str()]["bookStatus"].asInt() == 2)
		{
			borrowing++;
		}
	}
	cout << "已外借书籍数：" << borrowing << "\n";
	int broken = 0;
	for (Json::Value::Members::iterator iterMember = members.begin(); iterMember != members.end(); iterMember++)
	{
		string key = *iterMember;
		if (root[key.c_str()]["bookStatus"].asInt() == -1)
		{
			broken++;
		}
	}
	cout << "已销毁图书数：" << broken << "\n";
	int borrowTime = 0;
	for (Json::Value::Members::iterator iterMember = members.begin(); iterMember != members.end(); iterMember++)
	{
		string key = *iterMember;
		borrowTime += root[key.c_str()]["borrowedTime"].asInt();
	}
	cout << "总外借图书次数：" << borrowTime << "\n";
}
