#include <string>
#include <list>

using std::string;
using std::list;

enum bookStatus {
	free = 1,
	borrowed = 2,
	invalid = -1//下架销毁等因素导致不可借
};

class book
{
	long long bookID;
	string name;
	string ISBN;
	string writer;
	string press;
	bookStatus status;
protected:
	int price;//单位到分，即10元表示为1000
public:
	book(string name, string ISBN, string writer, string press, int price);
	void deleteBook();//报废销毁
};

list<book> searchBook(string word);