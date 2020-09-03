#include <string>

using std::string;

enum bookStatus {
	free = 1,
	borrowed = 2,
	invalid = -1//下架等因素导致不可借
};

class book
{
	string name;
	string ISBN;
	string writer;
	string press;
	int price;//单位到分，即10元表示为1000
protected:

};

