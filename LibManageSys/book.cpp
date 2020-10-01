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
	std::ifstream in(filename);
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
	
	list<string> writers;
	std::stringstream input(writer_input);
	while (std::getline(input, writer_tmp, ','))
	{
		writers.push_back(writer_tmp);
	}
	
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
		cout << "标价（以单位为人民币分输入）：";
		cin >> price;
		cout << "出版社：";
		cin >> press;
		book newBook = book(bookID, name, ISBN, writers, press, price);
		/*为写入json文件定义变量声明长度的数组，但需要及时delete避免内存泄露*/
		string* writerArr = new string[writers.size()];
		Json::Value cur = root[bookID];
		cur["name"] = Json::Value(name);
		cur["ISBN"] = Json::Value(ISBN);
		cur["writer"] = Json::Value(writerArr);
		cur["press"] = Json::Value(press);
		cur["price"] = Json::Value(price);
		cur["status"] = Json::Value((int)1);
		std::ofstream os;
		os.open(filename, std::ios::out | std::ios::app);
		if (!os.is_open())
		{
			std::cerr << "[ERROR]cannot create file \"" + filename + "\"." << std::endl;
		}
		os << root;
		os.close();
		delete(writerArr);//及时释放内存
	}
	return true;
}

book::book()
{
	this->bookID = "";
	this->name = "";
	this->ISBN = "";
	this->press = "";
	this->price = 0;
	this->status = invalid;
}

book::book(string bookID, string name, string ISBN, list<string> writer, string press, int price)
{
	this->bookID = bookID;
	this->name = name;
	this->ISBN = ISBN;
	this->writer = writer;
	this->press = press;
	this->price = price;
	this->status = free;
}

book::book(string bookID, string name, string ISBN, list<string> writer, string press, int price, bookStatus status)
{
	this->bookID = bookID;
	this->name = name;
	this->ISBN = ISBN;
	this->writer = writer;
	this->press = press;
	this->price = price;
	this->status = status;
}

void book::deleteBook()
{
	//删除记录对于管理来说并不利于维护，这里只做更改状态处理
	//因此图书销毁后bookID并不释放，在最初采购图书标记ID时管理员应当留出一定冗余给后续购入的复本
	status = invalid;
}

void book::operator=(const book& B)
{
	bookID = B.bookID;
	name = B.name;
	ISBN = B.ISBN;
	writer = B.writer;
	press = B.press;
	price = B.price;
	status = B.status;
}

book json2Book(Json::Value bookJSON, string bookID)
{
	string name, ISBN, press;
	int price;
	bookStatus status;
	list<string> writers;
	name=bookJSON["name"].asString();
	ISBN=bookJSON["ISBN"].asString();
	Json::Value writerArr=bookJSON["writer"];
	for (int i = 0; i < writerArr.size(); i++)
	{
		writers.push_back(writerArr[i].asString());
	}
	press=bookJSON["press"].asString();
	price=bookJSON["price"].asInt();
	status=(bookStatus)bookJSON["status"].asInt();
	return book(bookID, name, ISBN, writers, press, price, status);
}

list<book> searchBook(string word)
{
	string filename = "info\\books.json";
	std::ifstream in(filename);
	Json::Value root;
	Json::CharReaderBuilder rbuilder;
	rbuilder["collectComments"] = false;
	string errs;
	Json::parseFromStream(rbuilder, in, &root, &errs);
	in.close();

	list<book> result;
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
			for (list<string>::iterator it = cur.writer.begin(); it != cur.writer.end(); it++)
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
