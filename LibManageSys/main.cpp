#include <iostream>
#include <vector>
#include <cstdlib>
#include "User.h"
#include "borrowInfo.h"
#include "book.h"
#include "analytics.h"
#include <conio.h>
#include <io.h>
#include <json/json.h>

using namespace std;

int UserRegister();
int UserLogin();
int userManage();
int userMenu();
int adminMenu();
string getPassword();
bool dupUsername(string name);

User currentUser;

int main()
{
    loadConfig();
    int user = userManage();

    int cmd = -1;
    if (user!=0)
    {
        do
        {
            if (user == 1) {
                cmd = userMenu();
            }
            else
            {
                cmd = adminMenu();
            }
        } while (cmd!=0);
    }
    return 0;
}

int userManage()
{
    int order = -1;
    do
    {
        cout << endl;
        cout << "1. 用户注册" << endl;
        cout << "2. 用户登录" << endl;
        cout << "0. 退出系统" << endl;
        cout << endl;
        cout << "请输入对应序号进行相应操作: ";
        cin >> order;

        switch (order)
        {
        case 1:
            system("cls");
            return UserRegister();
            break;
        case 2:
            system("cls");
            return UserLogin();
            return 1;
            break;
        case 0:
            return 0;
            break;
        default:
            cout << "错误指令，请重新输入！按任意键返回主界面…" << endl;
            cin.get();
            system("cls");
            break;
        }
    } while (true);
    return 0;
}

int adminMenu()
{
    int order = -1;
    string bookID;
    book se;
    do
    {
        cout << endl;
        cout << "1. 采购图书" << endl;
        cout << "2. 报废图书" << endl;
        cout << "3. 统计" << endl;
        cout << "0. 退出系统" << endl;
        cout << endl;
        cout << "请输入对应序号进行相应操作: ";
        cin >> order;
        cin.clear();
        switch (order)
        {
        case 1:
            system("cls");
            addBook();
            break;
        case 2:
            system("cls");
            cout << "请输入待报废图书bookID：";
            cin >> bookID;
            se = searchBookByID((string)bookID);
            if (se.bookID==bookID)
            {
                cout << "请确认待报废图书信息：\n" << \
                    "书名：" << se.name << ",\n" << \
                    "ISBN：" << se.ISBN << ",\n" << \
                    "出版社：" << se.press << ",\n" << \
                    "作者（若多作者则仅显示第一个）：" << se.writer[0] << endl \
                    << "输入Y确认，其他键返回";
                string s;
                cin >> s;
                if (s=="Y")
                {
                    se.deleteBook();
                }
                else
                {
                    system("cls");
                }
            }
            break;
        case 3:
            analytics(currentUser);
            cout << "按任意键返回主菜单……" << endl;
            cin.get();
            break;
        case 0:
            return 0;
            break;
        default:
            cerr << "错误指令，请重新输入！按任意键返回主界面…" << endl;
            cin.get();
            system("cls");
            break;
        }
    } while (true);
    return 0;
}

int userMenu()
{
    int order = -1;
    do
    {
        cout << endl;
        /*催还功能*/
        if (currentUser.borrowingInfo.size() > 0)
        {
            cout << "您当前已借 " << currentUser.borrowingInfo.size() << "本书，请记得及时归还哦~\n";
            cout << "已租借图书：\n";
            cout << "bookID\t书名\tISBN\t归还期限\t可否申请续期\n";
            for (int i = 0; i < currentUser.borrowingInfo.size(); i++)
            {
                book curBook = searchBookByID(currentUser.borrowingInfo[i].bookID);
                char returnDue[64];
                strftime(returnDue, sizeof(returnDue), "%Y-%m-%d %H:%M:%S", localtime(&(currentUser.borrowingInfo[i].dueTime)));
                cout << curBook.bookID << "\t" << curBook.name << "\t" << curBook.ISBN << "\t" \
                    << returnDue << "\t";
                if (currentUser.borrowingInfo[i].isRenewed)
                {
                    cout << "否\n";
                }
                else
                {
                    cout << "是\n";
                }
            }
            cout << "------\n";
        }
        
        cout << "1. 借书" << endl;
        cout << "2. 还书" << endl;
        cout << "3. 申请续期" << endl;
        cout << "4. 统计" << endl;
        cout << "0. 退出系统" << endl;
        cout << endl;
        cout << "请输入对应序号进行相应操作: ";
        cin >> order;
        cin.clear();
        book ret;
        int toRenew;
        bool renewed = false;
        bool returned = false;
        bool found = false;

        switch (order)
        {
        case 1:
            system("cls");
            do
            {
                cout << "请输入查询书籍的单个关键词（标题，作者，出版社，ISBN等），搜索到对应图书后请记下对应bookID，并输入\"~\"退出搜索：";
                string keyword;
                cin >> keyword;
                if (keyword == "~")
                {
                    break;
                }
                vector<book> result = searchBook(keyword);
                if (result.size()==0)
                {
                    cerr << "未搜索到相关书籍！\n";
                }
                else
                {
                    cout << "搜索结果共 " << result.size() << " 条：\n";
                    cout << "bookID\t书名\t作者\t出版社\tISBN\n";
                    for (int i = 0; i < result.size(); i++)
                    {
                        cout << result[i].bookID << "\t" << result[i].name << "\t";
                        for (int j = 0; j < result[i].writer.size(); j++)
                        {
                            cout << result[i].writer[j];
                            if (result[i].writer.size() > 1 && j < result[i].writer.size() - 1)
                            {
                                cout << ", ";
                            }
                        }
                        cout << "\t" << result[i].press << "\t" << result[i].ISBN << "\n";
                    }
                }
            } while (true);
            do
            {
                cout << "请输入bookID，输入\"~\"返回主页：";
                string bookID;
                cin >> bookID;
                if (bookID=="~")
                {
                    break;
                }
                ret = searchBookByID(bookID);
                if (ret.bookID!="")
                {
                    found = true;
                }
            } while (!found);
            if (found)
            {
                cout << "请确认书籍信息：\n";
                cout << "bookID\t书名\t作者\t出版社\tISBN\n";
                cout << ret.bookID << "\t" << ret.name << "\t";
                for (int j = 0; j < ret.writer.size(); j++)
                {
                    cout << ret.writer[j];
                    if (ret.writer.size() > 1 && j < ret.writer.size() - 1)
                    {
                        cout << ", ";
                    }
                }
                cout << "\t" << ret.press << "\t" << ret.ISBN << "\n";
                cout << "信息正确请输入Y借阅，错误请输入其他任意键返回主页：";
                string choose;
                cin >> choose;
                if (choose == "Y")
                {
                    currentUser.newBorrow(ret);
                    currentUser.updateInfo();
                }
            }
            break;
        case 2:
            system("cls");
            int toReturn;
            do
            {
                cout << "您当前的待归还图书共有 " << currentUser.borrowingInfo.size() << "本：";
                cout << "序号\tbookID\t书名\tISBN\t归还期限\t可否申请续期\n";
                for (int i = 0; i < currentUser.borrowingInfo.size(); i++)
                {
                    book curBook = searchBookByID(currentUser.borrowingInfo[i].bookID);
                    char returnDue[64];
                    strftime(returnDue, sizeof(returnDue), "%Y-%m-%d %H:%M:%S", localtime(&(currentUser.borrowingInfo[i].dueTime)));
                    cout << (i + 1) <<curBook.bookID << "\t" << curBook.name << "\t" << curBook.ISBN << "\t" \
                        << returnDue << "\t";
                    if (currentUser.borrowingInfo[i].isRenewed)
                    {
                        cout << "否\n";
                    }
                    else
                    {
                        cout << "是\n";
                    }
                }
                cout << "------\n";
                cout << "请输入需要归还的图书序号：";
                int num;
                cin >> num;
                if (0>=num || num > currentUser.borrowingInfo.size())
                {
                    cerr << "序号输入有误，请重新输入！\n";
                }
                else
                {
                    toReturn = num;
                    returned = true;
                }
            } while (!returned);
            currentUser.newReturn(currentUser.borrowingInfo[toReturn]);
            break;
        case 3:
            do
            {
                cout << "您当前的待归还图书共有 " << currentUser.borrowingInfo.size() << "本：";
                cout << "序号\tbookID\t书名\tISBN\t归还期限\t可否申请续期\n";
                for (int i = 0; i < currentUser.borrowingInfo.size(); i++)
                {
                    book curBook = searchBookByID(currentUser.borrowingInfo[i].bookID);
                    char returnDue[64];
                    strftime(returnDue, sizeof(returnDue), "%Y-%m-%d %H:%M:%S", localtime(&(currentUser.borrowingInfo[i].dueTime)));
                    cout << (i + 1) << curBook.bookID << "\t" << curBook.name << "\t" << curBook.ISBN << "\t" \
                        << returnDue << "\t";
                    if (currentUser.borrowingInfo[i].isRenewed)
                    {
                        cout << "否\n";
                    }
                    else
                    {
                        cout << "是\n";
                    }
                }
                cout << "------\n";
                cout << "请输入需要延期的图书序号（注意：已经续期过的图书不可再次申请续期！），输入\"0\"返回主菜单：";
                int num;
                cin >> num;
                if (num==0)
                {
                    break;
                }
                else if (0 > num || num > currentUser.borrowingInfo.size())
                {
                    cerr << "序号输入有误，请重新输入！\n";
                }
                else if (currentUser.borrowingInfo[num].isRenewed)
                {
                    cerr << "已经续期过的图书不可再次申请续期！\n";
                }
                else
                {
                    toRenew = num;
                    renewed = true;
                }
            } while (!renewed);
            currentUser.borrowingInfo[toRenew].isRenewed = true;
            currentUser.borrowingInfo[toRenew].dueTime += 86400 * 30;
            char returnDue[64];
            strftime(returnDue, sizeof(returnDue), "%Y-%m-%d %H:%M:%S", localtime(&(currentUser.borrowingInfo[toRenew].returnedTime)));
            cout << "续期成功，当前归还期限已变更至：" << (string)returnDue << "，请记得如期归还~";
            break;
        case 4:
            analytics(currentUser);
            cout << "按任意键返回主菜单……" << endl;
            cin.get();
            break;
        case 0:
            return 0;
            break;
        default:
            cerr << "错误指令，请重新输入！按任意键返回主界面…" << endl;
            cin.get();
            system("cls");
            break;
        }
    } while (true);
    return 0;
}

int UserRegister()
{
    int typeNumber=-1;
    userType type = undefined;
    string realName, username, password, confirmPassword;
    bool registered = false;
    do
    {
        do
        {
            cout << "请输入注册用户类型，1代表普通用户，2代表管理员：";
            cin >> typeNumber;
            switch (typeNumber)
            {
            case 1:
                type = user;
                break;
            case 2:
                type = admin;
                break;
            default:
                cerr << "错误指令，请重新输入！" << endl;
                break;
            }
        } while (typeNumber != 1 && typeNumber != 2);
        cin.get();
        cout << "姓名：";
        getline(cin, realName);
        cout << "用户名（登录时使用）：";
        cin >> username;
        cout << "密码：";
        password = getPassword();
        cout << "确认你的密码：";
        confirmPassword = getPassword();
        if (password.compare(confirmPassword) != 0)
        {
            cerr << "前后密码不一致，请重新输入！" << endl;
        }
        else if (dupUsername(username))
        {
            cerr << "当前用户名已存在，请重新输入！" << endl;
        }
        else
        {
            registered = true;
        }
    } while (!registered);
    User newUser(type, username, realName, password);
    currentUser = newUser;
    cin.get();
    cout << "注册成功！已为您自动登录，按任意键进入管理界面…" << endl;
    cin.get();
    system("cls");
    return typeNumber;
}

int UserLogin()
{
    string username, password;
    bool logined = false;
    userType loginType;
    do 
    {
        cout << "用户名：";
        cin >> username;
        cout << "密码：";
        password = getPassword();
        currentUser = User(username, generateMD5(password));
        loginType = currentUser.getType();
        if (loginType == undefined)
        {//登陆失败
            cout << "用户名或密码错误，请重新输入！" << endl;
        }
        else
        {//登陆成功
            cin.get();
            cout << "登陆成功！按任意键进入系统…" << endl;
            logined = true;
            cin.get();
            system("cls");
        }
    } while (!logined);
    if (loginType==user)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

string getPassword()
{
    string ret;
    char ch;
    ch = _getch();
    while (ch!='\n'&&ch!='\r')
    {
        ret += ch;
        ch = _getch();
    }
    cout << endl;
    return ret;
}

bool dupUsername(string name)
{
    _finddata_t file;
    int k;
    long HANDLE;
    string fileName = name + ".json";
    k = HANDLE = _findfirst((char*)fileName.data(), &file);
    if (k != -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}
