// 目标：建立一个用户表以及一个食物表（收藏表作为像歌单一样的存在加到目标7里）,达到:
//    1.对用户信息CRUD，包括{userid， username， number， password， favorlists[]}
//    2.对食物信息CRUD，包括{foodid， foodname， store， tags[]}
//    3.对店铺信息CRUD，包括{storeid， storename， canteen， floor， commentlist[]}
//    4.收藏功能、拉黑功能
//    5.随机食物，有尝鲜模式（未吃过的食物获得10倍权重）和普通模式（收藏食物获得3倍权重）
//    6.检索，包括{用户名检索， 用户id检索， 食物名检索， 餐厅楼层检索， 店铺名检索， tag检索}
//    7.分享、社区、猜你喜欢（有时间再说）

#include <iostream>
#include <string.h>
#include <fstream>
using namespace std;

struct favorlist{
    int listid=0;
    int userid=0;
    int foodid[20]={0};
};

struct tag{
    int tagid=0;
    char tagname[20]="";
};

struct comment{
    int commentid=0;
    int userid=0;
    char text[100]="";
};

struct user{
    int userid=0;
    int number=0;
    char username[20]="";
    char password[20]="";
    favorlist* favorlists[5]={nullptr};
};

struct food{
    int foodid=0;
    char foodname[20]="";
    int storeid=0;
    tag* tags[3]={nullptr};
};

struct store{
    int storeid=0;
    char storename[20]="";
    int canteen=0;
    int floor=0;
    comment *comments[20]={nullptr};
};

struct UserTrieNode{
    UserTrieNode* next[26]={nullptr};
    user* users[10]={nullptr};
    int TerminalSize=0;
};

class userclass{
    private:
    user* userlist[100]={nullptr};
    UserTrieNode root;
    int useridMax=0;
    public:
    int creat(){
        int number;
        char username[20];
        char password[20];
        cout<<"please input stu number:";
        while(1){
            cin>>number;
            if (TestNum(number)) break;
        }
        cout<<"please input username:";
        while(1){
        cin>>username;
            if (TestUsername(username)) break;
        }
        cout<<"please input password:";
        while(1){
            cin>>password;
            if (TestPassword(password)) break;
        }
        if(AddUser(useridMax+1,number,username,password)) useridMax++;
        return 1;
    }
    int AddUser(int id, int num, char *uname, char *pw){
        user* newuser = new user; 
        newuser->userid = id;
        newuser->number = num;
        strcpy(newuser->username,uname);
        strcpy(newuser->password,pw);
        userlist[id] = newuser;
        UserTrieInsert(id);
        return 1;
    }
    int PrintNewUser(){
        cout<<"ID:"<<useridMax<<endl<<"number:"<<userlist[useridMax]->number<<endl<<"username:"<<userlist[useridMax]->username<<endl;
        return 1;
    }
    int UpdateUserName(int id){
        if (!CheckPassword(id)) return 0;
        char uname[20]="";
        cout << "please input your new username:";
        while(1){
            cin>>uname;
            if (TestUsername(uname)) break;
        }
        UserTrieDelete(id);
        strcpy(userlist[id]->username,uname);
        UserTrieInsert(id);
    }
    int CheckPassword(int id){
        char pw[20]="";
        cout << "please input your password:";
        cin >> pw;
        if (strcmp(userlist[id]->password,pw)==0) return 1;
        cout << "wrong password" << endl;
        return 0;
    }
    int TestNum(int num){
        if ((num>=1000000)||(num<=99999999)) return 1;
        cout<<"wrong stu number,please input again:";
        return 0;
    }
    int TestUsername(char* uname){
        for (int i=0;uname[i]!=0;i++)
            if (!(((int('A')<=int(uname[i]))&&(int(uname[i])<=int('Z')))||((int('a')<=int(uname[i]))&&(int(uname[i])<=int('z'))))){
                cout<<"illegal username, please input again:";
                return 0;
            }
        return 1;
    }
    int TestPassword(char* pw){
        int ischar=0,isnum=0;
        for (int i=0;pw[i]!=0;i++){
            if (!(((int('A')<=int(pw[i]))&&(int(pw[i])<=int('Z')))||((int('a')<=int(pw[i]))&&(int(pw[i])<=int('z')))||((int('0')<=int(pw[i]))&&(int(pw[i])<=int('9'))))){
                cout<<"illegal password, please input again:";
                return 0;
            }
            if (((int('A')<=int(pw[i]))&&(int(pw[i])<=int('Z')))||((int('a')<=int(pw[i]))&&(int(pw[i])<=int('z'))))
                ischar = 1;
            if ((int('0')<=int(pw[i]))&&(int(pw[i])<=int('9')))
                isnum = 1;
        }
        if (! ischar){
            cout<<"your password has no character, please input again:";
            return 0;
        }
        if (! isnum){
            cout<<"your password has no number, please input again:";
            return 0;
        }
        return 1;
    }
    int savelist(){
        ofstream  userlistfile;
        userlistfile.open("userlistfile.txt");
        for (int i=1;i<=useridMax;i++){
            userlistfile << "###" << endl;
            userlistfile << userlist[i]->userid << endl;
            userlistfile << userlist[i]->number <<endl;
            userlistfile << userlist[i]->username << endl;
            userlistfile << userlist[i]->password << endl;
            userlistfile << "###" << endl << endl;
        }
        userlistfile << "!!!" << endl;
        userlistfile.close();
        return 1;
    }
    int loadlist(){
        ifstream userlistfile;
        char text[20];
        userlistfile.open("userlistfile.txt");
        while(1){
            userlistfile >> text;
            if (strcmp(text,"!!!")==0) return 1;
            if (strcmp(text,"###")==0){
                while(1){
                    int id,num;
                    char uname[20],pw[20];
                    userlistfile >> id >> num >> uname >> pw >> text;
                    if(AddUser(id,num,uname,pw)) useridMax=id;
                    break;
                }
            }
        }
        return 1;
    }
    int UserTrieInsert(int id){
        UserTrieNode* T = &root;
        for (int i=0;userlist[id]->username[i]!=0;i++){
            int idx = userlist[id]->username[i] - 'a';
            if (idx<0) idx += 'a' - 'A';
            if(T->next[idx] == nullptr){
                T->next[idx] = new UserTrieNode;
            }
            T = T->next[idx];
        }
        T->users[T->TerminalSize]=userlist[id];
        T->TerminalSize += 1;
        return 1;
    }
    int UserTrieDelete(int id){
        char* uname= userlist[id]->username;
        UserTrieNode* T = &root;
        for (int i=0;uname[i]!=0;i++) {
            int idx = uname[i] - 'a';
            if (idx < 0) idx += 'a' - 'A';
            T = T->next[idx];
            if (T == nullptr) {
                cout<<"nullptr"<<endl;
                return 0;
            }
        }
        for (int i=0;i<T->TerminalSize;i++){
            if (T->users[i]->userid==id){
                for (int j = i;j < T->TerminalSize-1;j++)
                    T->users[j]=T->users[j+1];
                T->users[T->TerminalSize-1]=nullptr;
                T->TerminalSize--;
                break;
            }
        }
        return 1;
    }
    int search(char* uname) {
        UserTrieNode* T = &root;
        for (int i=0;uname[i]!=0;i++) {
            int idx = uname[i] - 'a';
            if (idx < 0) idx += 'a' - 'A';
            T = T->next[idx];
            if (T == nullptr) {
                cout<<"nullptr"<<endl;
                return 0;
            }
        }
        for (int i=0;i<T->TerminalSize;i++){
            cout << T->users[i]->number << endl;
        }
        return 1;
    }
};

int main(){
    userclass UserList;
    //服务端开机
    if (UserList.loadlist()) cout<<"load all complete"<<endl;

    //调试区
    //创建新用户
    //UserList.creat();
    //UserList.PrintNewUser();
    //查找用户
    //UserList.search("minato");
    //UserList.UserTrieDelete(1);
    //UserList.search("minato");
    //更新用户
    //UserList.search("minato");
    //UserList.search("spectral");
    //UserList.UpdateUserName(2);
    //UserList.search("minato");
    //UserList.search("spectral");
    
    //服务端关机
    if (UserList.savelist()) cout<<"save all complete"<<endl;
}