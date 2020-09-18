#include "book.h"
#include <json/json.h>
#include <io.h>
#include <fstream>
#include <iostream>
#include <ctime>

book::book(string name, string ISBN, string writer, string press, int price)
{
	this->bookID = (long long)time(0);
	this->name = name;
	this->ISBN = ISBN;
	this->writer = writer;
	this->press = press;
	this->price = price;
	this->status = free;
	Json::Value root;
	root["name"] = Json::Value(name);
	root["ISBN"] = Json::Value(ISBN);
	root["writer"] = Json::Value(writer);
	root["press"] = Json::Value(press);
	root["price"] = Json::Value(price);
	root["status"] = Json::Value((int)status);
	std::ofstream os;
	//long long转换到string
	std::ostringstream oss;
	oss << bookID;
	string bookID_str;
	std::istringstream iss(oss.str());
	iss >> bookID_str;

	std::string filename = "info\\books\\" + bookID_str + ".json";
	os.open(filename, std::ios::out | std::ios::app);
	if (!os.is_open())
	{
		std::cout << "ERROR: cannot create file \"" + filename + "\"." << std::endl;
	}
	os << root;
	os.close();
}

void book::deleteBook()
{
	//删除记录对于管理来说并不利于维护，因此这里只做更改状态处理
	status = invalid;
}

list<book> searchBook(string word)
{
	list<book> searchResult;

	FILE* filepath;
	

}
