#include "book.h"
#include <io.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

using std::cin;
using std::cout;

bool addBook()
{
	string filename = "info\\books.json";
	std::ifstream in(filename, std::ios::binary);
	Json::Value root;
	Json::CharReaderBuilder rbuilder;
	rbuilder["collectComments"] = false;
	string errs;
	Json::parseFromStream(rbuilder, in, &root, &errs);
	in.close();
	
	string bookID, name, ISBN, writer_input, writer_tmp, press;
	int price;
	cout << "分配bookID：";
	cin >> bookID;
	
	//需要先判断当前bookID是否已被分配
	if (root.isMember(bookID))
	{
		std::cerr << "[ERROR]当前bookID已存在，请重新分配！\n";
		return false;
	}
	else
	{
		cout << "书名：";
		cin >> name;
		cout << "ISBN：";
		cin >> ISBN;
		cout << "作者（多个作者之间请用英文逗号分隔）：";
		cin >> writer_input;
		vector<string> writers;
		std::stringstream input(writer_input);
		while (std::getline(input, writer_tmp, ','))
		{
			writers.push_back(writer_tmp);
		}
		cout << "标价（以单位为人民币分输入）：";
		cin >> price;
		cout << "出版社：";
		cin >> press;
		book newBook = book(bookID, name, ISBN, writers, press, price);
		Json::Value cur = root[bookID];
		root[bookID.c_str()]["name"] = Json::Value(name);
		root[bookID.c_str()]["ISBN"] = Json::Value(ISBN);
		for (int i = 0; i < writers.size(); i++)
		{
			root[bookID.c_str()]["writer"][i] = Json::Value(writers[i]);
		}
		root[bookID.c_str()]["press"] = Json::Value(press);
		root[bookID.c_str()]["price"] = Json::Value(price);
		root[bookID.c_str()]["bookStatus"] = Json::Value((int)1);
		root[bookID.c_str()]["borrowedTime"] = Json::Value((int)0);

		Json::StreamWriterBuilder wBuilder;
		wBuilder.setDefaults(&root);
		root["emitUTF8"] = true;
		std::ofstream os;
		os.open(filename, std::ios::out | std::ios::app);
		if (!os.is_open())
		{
			std::cerr << "[ERROR]cannot create file \"" + filename + "\"." << std::endl;
		}
		os << root;
		os.close();
	}
	return true;
}

void book::updateBookInfo()
{
	string filename = "info\\books.json";
	std::ifstream in(filename, std::ios::binary);
	Json::Value root;
	Json::CharReaderBuilder rbuilder;
	rbuilder["collectComments"] = false;
	string errs;
	Json::parseFromStream(rbuilder, in, &root, &errs);
	in.close();
	
	for (int i = 0; i < writer.size(); i++)
	{
		root[bookID.c_str()]["writer"][i] = Json::Value(writer[i]);
	}
	Json::Value cur = root[bookID];
	root[bookID.c_str()]["name"] = Json::Value(name);
	root[bookID.c_str()]["ISBN"] = Json::Value(ISBN);
	root[bookID.c_str()]["press"] = Json::Value(press);
	root[bookID.c_str()]["price"] = Json::Value(price);
	root[bookID.c_str()]["bookStatus"] = Json::Value(bookStatus);
	root[bookID.c_str()]["borrowedTime"] = Json::Value(borrowedTime);
	Json::StreamWriterBuilder wBuilder;
	wBuilder.setDefaults(&root);
	root["emitUTF8"] = true;

	std::ofstream os;
	os.open(filename, std::ios::out | std::ios::app);
	if (!os.is_open())
	{
		std::cerr << "[ERROR]cannot create file \"" + filename + "\"." << std::endl;
	}
	os << root;
	os.close();
}


book::book()
{
	this->bookID = "";
	this->name = "";
	this->ISBN = "";
	this->press = "";
	this->price = 0;
	this->bookStatus = invalid;
	this->borrowedTime = 0;
}

book::book(string bookID, string name, string ISBN, vector<string> writer, string press, int price)
{
	this->bookID = bookID;
	this->name = name;
	this->ISBN = ISBN;
	this->writer = writer;
	this->press = press;
	this->price = price;
	this->bookStatus = available;
	this->borrowedTime = 0;
}

book::book(string bookID, string name, string ISBN, vector<string> writer, string press, int price, int status, int borrowedTime)
{
	this->bookID = bookID;
	this->name = name;
	this->ISBN = ISBN;
	this->writer = writer;
	this->press = press;
	this->price = price;
	this->bookStatus = status;
	this->borrowedTime = borrowedTime;
}

void book::deleteBook()
{
	//删除记录对于管理来说并不利于维护，这里只做更改状态处理
	//因此图书销毁后bookID并不释放，在最初采购图书标记ID时管理员应当留出一定冗余给后续购入的复本
	bookStatus = invalid;
	updateBookInfo();
}

void book::operator=(const book& B)
{
	bookID = B.bookID;
	name = B.name;
	ISBN = B.ISBN;
	writer = B.writer;
	press = B.press;
	price = B.price;
	bookStatus = B.bookStatus;
	borrowedTime = B.borrowedTime;
}

book json2Book(Json::Value bookJSON, string bookID)
{
	string name, ISBN, press;
	int price, borrowedTime;
	int status;
	vector<string> writers;
	name=bookJSON["name"].asString();
	ISBN=bookJSON["ISBN"].asString();
	Json::Value writerArr=bookJSON["writer"];
	for (int i = 0; i < writerArr.size(); i++)
	{
		writers.push_back(writerArr[i].asString());
	}
	press=bookJSON["press"].asString();
	price=bookJSON["price"].asInt();
	status=bookJSON["bookStatus"].asInt();
	borrowedTime = bookJSON["borrowedTime"].asInt();
	return book(bookID, name, ISBN, writers, press, price, status, borrowedTime);
}

vector<book> searchBook(string word)
{
	string filename = "info\\books.json";
	std::ifstream in(filename);
	Json::Value root;
	Json::CharReaderBuilder rbuilder;
	rbuilder["collectComments"] = false;
	string errs;
	Json::parseFromStream(rbuilder, in, &root, &errs);
	in.close();

	vector<book> result;
	Json::Value::Members member = root.getMemberNames();
	for (auto iter = member.begin(); iter != member.end(); iter++)
	{
		book cur = json2Book(root[*iter], *iter);
		if (cur.name.find(word)!=cur.name.npos)
		{
			result.push_back(cur);
		}
		else if (cur.ISBN.find(word)!=cur.ISBN.npos)
		{
			result.push_back(cur);
		}
		else if (cur.press.find(word)!=cur.press.npos)
		{
			result.push_back(cur);
		}
		else
		{
			for (vector<string>::iterator it = cur.writer.begin(); it != cur.writer.end(); it++)
			{
				if (it->find(word) != it->npos)
				{
					result.push_back(cur);
					break;
				}
			}
		}
	}
	return result;
}

book searchBookByID(string bookID)
{
	string filename = "info\\books.json";
	std::ifstream in(filename);
	Json::Value root;
	Json::CharReaderBuilder rbuilder;
	Json::StreamWriterBuilder wBuilder;
	wBuilder.setDefaults(&root);
	root["emitUTF8"] = true;
	rbuilder["collectComments"] = false;
	string errs;
	Json::parseFromStream(rbuilder, in, &root, &errs);
	in.close();
	if (root.isMember(bookID))
	{
		if (root[bookID]["status"] == invalid) {
			std::cerr << "[ERROR]该图书不可用，请确认图书状态！\n";
			return book();
		}
		return json2Book(root[bookID], bookID);
	}
	else
	{
		std::cerr << "[ERROR]未找到对应图书，请确认bookID！\n";
		return book();
	}
}
