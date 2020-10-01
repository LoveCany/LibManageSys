#include <string>
#include <list>
#include <json/json.h>

using std::string;
using std::list;

enum bookStatus {
	free = 1,
	borrowed = 2,
	invalid = -1//下架销毁等因素导致不可借
};

class book
{
public:
	string bookID;//bookID应映射到每一本独立的书，对于复本bookID自然也应不一致（但原则上应当相邻）
	string name;
	string ISBN;
	list<string> writer;
	string press;
	int price;//单位到分，即10元表示为1000
	bookStatus status;
	/*新增图书应当调用addBook()方法，以方便main方法处理异常*/
	book();//无效查询的返回结果
	book(string bookID, string name, string ISBN, list<string> writer, string press, int price);
	book(string bookID, string name, string ISBN, list<string> writer, string press, int price, bookStatus status);
	void deleteBook();//报废销毁
	void operator=(const book &B);
};

book json2Book(Json::Value bookJSON, string bookID);
list<book> searchBook(string word);
book searchBookByID(string bookID);
bool addBook();