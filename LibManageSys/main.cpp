#include <iostream>
#include <vector>
#include <cstdlib>
#include "User.h"
#include "config.h"
#include <conio.h>
#include <io.h>
#include <json/json.h>

using namespace std;

void UserRegister();
void UserLogin();
int userManage();
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
            UserRegister();
            return 1;
            break;
        case 2:
            system("cls");
            UserLogin();
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
    do
    {
        cout << endl;
        cout << "1. 采购图书" << endl;
        cout << "2. 报废图书" << endl;
        cout << "3. 查看荐购" << endl;
        cout << "4. 系统设置" << endl;
        cout << "5. 统计" << endl;
        cout << "0. 退出系统" << endl;
        cout << endl;
        cout << "请输入对应序号进行相应操作: ";
        cin >> order;

        switch (order)
        {
        case 1:
            system("cls");
            UserRegister();
            break;
        case 2:
            system("cls");
            UserLogin();
            break;
        case 3:

            break;
        case 4:

            break;
        case 5:

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

int userMenu()
{
    int order = -1;
    do
    {
        cout << endl;
        cout << "1. 借书" << endl;
        cout << "2. 还书" << endl;
        cout << "3. 申请续期" << endl;
        cout << "4. 荐购" << endl;
        cout << "5. 统计" << endl;
        cout << "0. 退出系统" << endl;
        cout << endl;
        cout << "请输入对应序号进行相应操作: ";
        cin >> order;

        switch (order)
        {
        case 1:
            system("cls");
            UserRegister();
            break;
        case 2:
            system("cls");
            UserLogin();
            break;
        case 3:

            break;
        case 4:

            break;
        case 5:

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

void UserRegister()
{
    int typeNumber=-1;
    userType type = undefined;
    string realName, username, password, confirmPassword;
    bool registered = false;
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
            cout << "错误指令，请重新输入！" << endl;
            break;
        }
    } while (typeNumber!=1&&typeNumber!=2);
    cin.get();
    cout << "姓名：";
    getline(cin, realName);
    cout << "用户名（登录时使用）：";
    cin >> username;
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
                cout << "错误指令，请重新输入！" << endl;
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
            cout << "前后密码不一致，请重新输入！" << endl;
        }
        else if (dupUsername(username))
        {
            cout << "当前用户名已存在，请重新输入！" << endl;
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
}

void UserLogin()
{
    string username, password;
    bool logined = false;
    do 
    {
        cout << "用户名：";
        cin >> username;
        cout << "密码：";
        password = getPassword();
        User loginUser(username, generateMD5(password));
        userType loginType = loginUser.getType();
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
