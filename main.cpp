//
// Created by icefox on 2020/1/5.
//
///file: user book finance
///isbn name author keyword
///index_i index_n index_a index_k
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

int const BN=100;
vector<string>Command;

///User:

class User{
public:
    char id[32]{},passwd[32]{},name[32]{};
    int rank;
    User(){rank=0;}
    void Logout(){
        rank=0;
    }
    void display(){
        cout<<right<<id<<"\t"<<left<<setw(20)<<passwd<<"\t"<<left<<name<<'\t'<<rank<<endl;
    }
}Cur_User;
void Login(){
    char _id[32],_passwd[32];
    memset(_id,0,sizeof(_id));
    memset(_passwd,0,sizeof(_passwd));
    if(Command.size()>3){puts("Invalid");return;}
    strcpy(_id,Command[1].c_str());
    if(Command.size()>2) strcpy(_passwd,Command[2].c_str());
    else strcpy(_passwd,"");
    ifstream Iuser("user");
    User tmp;
    bool flag=false;
    while(Iuser.read(reinterpret_cast<char *>(&tmp),sizeof(User))){
        if(tmp.rank&&strcmp(tmp.id,_id)==0){flag=true;break;}
    }
    Iuser.close();
    if(!flag||(Cur_User.rank<=tmp.rank&&strcmp(_passwd,tmp.passwd)!=0)) puts("Invalid");
    else Cur_User=tmp;
}
void Logout(){
    if(Command.size()>1||Cur_User.rank==0) puts("Invalid");
    else Cur_User.Logout();
}
void UserAdd(){
    if(Command.size()!=5||Cur_User.rank<3){puts("Invalid");return;}
    char _id[32],_passwd[32],_name[32];
    memset(_id,0,sizeof(_id));
    memset(_passwd,0,sizeof(_passwd));
    memset(_name,0,sizeof(_name));
    strcpy(_id,Command[1].c_str());
    strcpy(_passwd,Command[2].c_str());
    strcpy(_name,Command[4].c_str());
    int _rank=stoi(Command[3]);
    if((_rank!=7&&_rank!=3&&_rank!=1)||_rank>=Cur_User.rank){puts("Invalid");return;}
    fstream IOuser("user");
    User tmp;
    int p=-1;
    while(IOuser.read(reinterpret_cast<char *>(&tmp),sizeof(User))){
        if(tmp.rank&&strcmp(tmp.id,_id)==0){p=IOuser.tellg();break;}
    }
    if(p!=-1) puts("Invalid");
    else{
        strcpy(tmp.id,_id);
        strcpy(tmp.passwd,_passwd);
        tmp.rank=_rank;
        strcpy(tmp.name,_name);
        IOuser.clear(ios::goodbit);
        IOuser.seekp(0,ios::end);
        IOuser.write(reinterpret_cast<char *>(&tmp),sizeof(User));
    }
    IOuser.close();
}
void RegisterUser(){
    if(Command.size()!=4){puts("Invalid");return;}
    char _id[32],_passwd[32],_name[32];
    memset(_id,0,sizeof(_id));
    memset(_passwd,0,sizeof(_passwd));
    memset(_name,0,sizeof(_name));
    strcpy(_id,Command[1].c_str());
    strcpy(_passwd,Command[2].c_str());
    strcpy(_name,Command[3].c_str());
    fstream IOuser("user");
    User tmp;
    int p=-1;
    while(IOuser.read(reinterpret_cast<char *>(&tmp),sizeof(User))){
        if(tmp.rank&&strcmp(tmp.id,_id)==0){p=IOuser.tellg();break;}
    }
    if(p!=-1) puts("Invalid");
    else{
        strcpy(tmp.id,_id);
        strcpy(tmp.passwd,_passwd);
        tmp.rank=1;
        strcpy(tmp.name,_name);
        IOuser.clear(ios::goodbit);
        IOuser.seekp(0,ios::end);
        IOuser.write(reinterpret_cast<char *>(&tmp),sizeof(User));
    }
    IOuser.close();
}
void DeleteUser(){
    if(Command.size()!=2||Cur_User.rank<7) {puts("Invalid");return;}
    char _id[32];
    strcpy(_id,Command[1].c_str());
    fstream IOuser("user");
    User tmp;
    int p=-1;
    while(IOuser.read(reinterpret_cast<char *>(&tmp),sizeof(User))){
        if(tmp.rank&&strcmp(tmp.id,_id)==0){p=IOuser.tellg();break;}
    }
    if(p==-1) puts("Invalid");
    else{
        tmp.rank=0;
        IOuser.seekp(p-sizeof(User));
        IOuser.write(reinterpret_cast<char *>(&tmp),sizeof(User));
    }
    IOuser.close();
}
void ChangePasswd(){
    if(Cur_User.rank<1){puts("Invalid");return;}
    if(Command.size()!=3&&Command.size()!=4){puts("Invalid");return;}
    char _id[32],passwd1[32],passwd2[32];
    memset(_id,0,sizeof(_id));
    memset(passwd1,0,sizeof(passwd1));
    memset(passwd2,0,sizeof(passwd2));
    strcpy(_id,Command[1].c_str());
    strcpy(passwd1,Command[2].c_str());
    fstream IOuser("user");
    User tmp;
    int p=-1;
    while(IOuser.read(reinterpret_cast<char *>(&tmp),sizeof(User))){
        if(tmp.rank&&strcmp(tmp.id,_id)==0){p=IOuser.tellg();break;}
    }
    if(p==-1){puts("Invalid");IOuser.close();return;}
    if(Cur_User.rank!=7){
        if(Command.size()!=4||strcmp(tmp.passwd,passwd1)!=0){
            puts("Invalid");IOuser.close();return;
        }else strcpy(tmp.passwd,Command[3].c_str());
    }
    if(Cur_User.rank==7){
       if(Command.size()==4) strcpy(tmp.passwd,Command[3].c_str());
       else strcpy(tmp.passwd,passwd1);
    }
    IOuser.seekp(p-sizeof(User));
    IOuser.write(reinterpret_cast<char *>(&tmp),sizeof(User));
    IOuser.close();
}
void DisplayUser(int cnt){
    cout<<cnt<<":"<<endl;
    ifstream Iuser("user");
    User tmp;
    while(Iuser.read(reinterpret_cast<char *>(&tmp),sizeof(User))){
        tmp.display();
    }
    Iuser.close();
}

///finance
struct Finance{
    double in,out,tin,tout;
    Finance(){
        in=0;out=0;tin=0;tout=0;
    }
    Finance & operator-=(const Finance &x){
        tin-=x.tin;
        tout-=x.tout;
        return *this;
    }
    void display(){
        cout<<"+ "<<fixed<<setprecision(2)<<tin;
        cout<<" - "<<fixed<<setprecision(2)<<tout<<endl;
    }
};
void DisplayFinance(int cnt){
    cout<<endl;
    cout<<cnt<<": "<<endl;
    ifstream file("finance");
    Finance tmp;
    while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Finance))){
        tmp.display();
    }
    cout<<endl;
}

///Book:

int CurBid=-1;
class Book{
public:
    char isbn[22]{},name[42]{},author[42]{},keyword[70]{};
    double price;
    int num;
    explicit Book(const char *_isbn=""){
        strcpy(isbn,_isbn);
        price=0;num=0;
    }
    friend bool operator<(const Book &a,const Book &b){
        return strcmp(a.isbn,b.isbn)<0;
    }
    void display(){
        cout<<isbn<<"\t"<<name<<"\t"<<author<<"\t"<<keyword<<"\t";
        cout<<fixed<<setprecision(2)<<price<<"\t";
        cout<<num<<"本"<<endl;
    }
};
void DisplayBook(int cnt){
    cout<<endl;
    cout<<cnt<<": "<<endl;
    ifstream file("book");
    Book bk;
    while(file.read(reinterpret_cast<char *>(&bk),sizeof(Book))){
        bk.display();
    }
    cout<<endl;
}
struct Isbn{
    char isbn[22]{};
    int Bid;//-1 -- deleted
    Isbn(){
        Bid=-1;
    }
    friend bool operator<(const Isbn &a,const Isbn &b){
        return strcmp(a.isbn,b.isbn)<0;
    }
    void display(){
        cout<<isbn<<'\t'<<Bid<<endl;
    }
};
struct Name{
    char name[42]{};
    int Bid;
    Name(){
        Bid=-1;
    }
    friend bool operator<(const Name &a,const Name &b){
        return strcmp(a.name,b.name)<0;
    }
};
struct Author{
    char author[42]{};
    int Bid;
    Author(){
        Bid=-1;
    }
    friend bool operator<(const Author &a,const Author &b){
        return strcmp(a.author,b.author)<0;
    }
};
struct Keyword{
    char keyword[70]{};
    int Bid;
    Keyword(){
        Bid=-1;
    }
    friend bool operator<(const Keyword &a,const Keyword &b){
        return strcmp(a.keyword,b.keyword)<0;
    }
};
struct Index_i{
    char isbn[22]{};//first key of the block
    int p;//begin position of the block  real size of the block
    explicit Index_i(const char *_="",int __=-1){
        strcpy(isbn,_);p=__;
    }
};
struct Index_n{
    char name[42]{};
    int p;
    explicit Index_n(const char *_="",int __=-1){
        strcpy(name,_);p=__;
    }
};
struct Index_a{
    char author[42]{};
    int p;
    explicit Index_a(const char *_="",int __=-1){
        strcpy(author,_);p=__;
    }
};
struct Index_k{
    char keyword[70]{};
    int p;
    explicit Index_k(const char *_="",int __=-1){
        strcpy(keyword,_);p=__;
    }
};
void DisplayIsbn(){
    cout<<"isbn: "<<endl;
    ifstream file("isbn");
    Isbn tmp;
    while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Isbn))){
        if(tmp.Bid==-1) break;
        tmp.display();
    }
}
int findisbn(const char *_isbn){
    ifstream file("index_i");
    ifstream file1("isbn");
    Index_i tmp;
    Isbn tmp1;
    int p=-1;
    while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_i))){
        if(tmp.p<0) continue;
        if(strcmp(tmp.isbn,_isbn)>0) break;
        p=tmp.p;
        if(strcmp(tmp.isbn,_isbn)==0) break;
    }
    if(p<0){
        file.close();
        file1.close();
        return -1;
    }
    file1.seekg(p);
    for(int i=0;i<BN;++i){
        file1.read(reinterpret_cast<char *>(&tmp1),sizeof(Isbn));
        if(strcmp(tmp1.isbn,_isbn)==0){
            file.close();
            file1.close();
            return tmp1.Bid;
        }
    }
    file.close();
    file1.close();
    return -1;
}
void findname(const char *_name){
    ifstream file("index_n");
    ifstream file1("name");
    Index_n tmp;
    Name tmp1;
    int p=-1;
    while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_n))){
        if(tmp.p<0) continue;
        if(strcmp(tmp.name,_name)>=0){
            file.seekg(-sizeof(Index_n),ios::cur);
            break;
        }
        p=tmp.p;
    }

    vector<int>a;
    if(p<0){p=0;file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_n));}
    if(file.eof()){//last block
        file.clear(ios::goodbit);
    }
    while(true){
        bool flag=false;
        file1.seekg(p);
        for(int i=0;i<BN;++i){
            file1.read(reinterpret_cast<char *>(&tmp1),sizeof(Name));
            if(tmp1.Bid==-1) break;
            if(strcmp(_name,tmp1.name)==0){
                a.push_back(tmp1.Bid);flag=true;
            }
        }
        if(!flag) break;
        if(!file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_n))) break;
        p=tmp.p;
    }
    file.close();
    file1.close();

    if(a.empty()) return;
    sort(a.begin(),a.end());
    file.open("book");
    vector<Book>b;
    b.clear();
    Book bk;
    for(int i : a){
        file.seekg(i*sizeof(Book));
        file.read(reinterpret_cast<char *>(&bk),sizeof(Book));
        b.push_back(bk);
    }
    sort(b.begin(),b.end());
    for(auto i : b) i.display();
    file.close();
}
void findauthor(const char *_author){
    ifstream file("index_a");
    ifstream file1("author");
    Index_a tmp;
    Author tmp1;
    int p=-1;
    while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_a))){
        if(tmp.p<0) continue;
        if(strcmp(tmp.author,_author)>=0){
            file.seekg(-sizeof(Index_a),ios::cur);
            break;
        }
        p=tmp.p;
    }

    vector<int>a;
    if(p<0){p=0;file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_a));}
    if(file.eof()){//last block
        file.clear(ios::goodbit);
    }
    while(true){
        bool flag=false;
        file1.seekg(p);
        for(int i=0;i<BN;++i){
            file1.read(reinterpret_cast<char *>(&tmp1),sizeof(Author));
            if(tmp1.Bid==-1) break;
            if(strcmp(_author,tmp1.author)==0){
                a.push_back(tmp1.Bid);flag=true;
            }
        }
        if(!flag) break;
        if(!file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_a))) break;
        p=tmp.p;
    }
    file.close();
    file1.close();

    if(a.empty()) return;
    sort(a.begin(),a.end());
    file.open("book");
    vector<Book>b;
    b.clear();
    Book bk;
    for(int i : a){
        file.seekg(i*sizeof(Book));
        file.read(reinterpret_cast<char *>(&bk),sizeof(Book));
        b.push_back(bk);
    }
    sort(b.begin(),b.end());
    for(auto i : b) i.display();
    file.close();
}
void findkeyword(const char *_keyword){
    ifstream file("index_k");
    ifstream file1("keyword");
    Index_k tmp;
    Keyword tmp1;
    int p=-1;
    while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_k))){
        if(tmp.p<0) continue;
        if(strcmp(tmp.keyword,_keyword)>=0){
            file.seekg(-sizeof(Index_k),ios::cur);
            break;
        }
        p=tmp.p;
    }

    vector<int>a;
    if(p<0){p=0;file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_k));}
    if(file.eof()){//last block
        file.clear(ios::goodbit);
    }
    while(true){
        bool flag=false;
        file1.seekg(p);
        for(int i=0;i<BN;++i){
            file1.read(reinterpret_cast<char *>(&tmp1),sizeof(Keyword));
            if(tmp1.Bid==-1) break;
            if(strcmp(_keyword,tmp1.keyword)==0){
                a.push_back(tmp1.Bid);flag=true;
            }
        }
        if(!flag) break;
        if(!file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_k))) break;
        p=tmp.p;
    }
    file.close();
    file1.close();

    if(a.empty()) return;
    sort(a.begin(),a.end());
    file.open("book");
    vector<Book>b;
    b.clear();
    Book bk;
    for(int i : a){
        file.seekg(i*sizeof(Book));
        file.read(reinterpret_cast<char *>(&bk),sizeof(Book));
        b.push_back(bk);
    }
    sort(b.begin(),b.end());
    for(auto i : b) i.display();
    file.close();
}
void addisbn(const char *_isbn){
 //   DisplayIsbn();
    fstream file("index_i");
    fstream file1("isbn");
    Index_i tmp;
    Isbn tmp1;
    int p=-1;
    while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_i))){
        if(tmp.p<0) continue;
        if(strcmp(tmp.isbn,_isbn)>0) break;
        p=tmp.p;
    }
    vector<Isbn>a;
    a.clear();
    if(p<0){//first block
        p=0;
        file.seekg(0);
    }
    else if(file.eof()){//last block
        file.clear(ios::goodbit);
        file.seekg(-sizeof(Index_i),ios::end);
    }else{
        file.seekg(-sizeof(Index_i)*2,ios::cur);
    }
    file1.seekg(p);
    for(int i=0;i<BN;++i){
        file1.read(reinterpret_cast<char *>(&tmp1),sizeof(Isbn));
        if(tmp1.Bid==-1) break;
        a.push_back(tmp1);
    }
    tmp1.Bid=CurBid;memcpy(tmp1.isbn,_isbn,sizeof(tmp1.isbn));
    a.push_back(tmp1);
    sort(a.begin(),a.end());
    file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_i));
    if(a.size()==BN){
        vector<Isbn>b;
        b.clear();
        for(int i=0;i<=BN/2;++i){
            b.push_back(a.back());a.pop_back();
        }
        reverse(b.begin(),b.end());
        a.size();strcpy(tmp.isbn, a[0].isbn);
        file.seekp(file.tellg());
        file.seekp(-sizeof(Index_i),ios::cur);
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_i));
        file1.seekp(p);
        for(auto & i : a) file1.write(reinterpret_cast<char *>(&i),sizeof(Isbn));
        tmp1.Bid=-1;strcpy(tmp1.isbn,"");
        for(int i=a.size();i<BN;++i) file1.write(reinterpret_cast<char *>(&tmp1),sizeof(Isbn));
        file1.seekp(0,ios::end);
        b.size();strcpy(tmp.isbn, b[0].isbn);tmp.p=file1.tellp();
        for(auto & i : b) file1.write(reinterpret_cast<char *>(&i),sizeof(Isbn));
        for(int i=b.size();i<BN;++i) file1.write(reinterpret_cast<char *>(&tmp1),sizeof(Isbn));
        Index_i tmp2;
        while(file.read(reinterpret_cast<char *>(&tmp2),sizeof(Index_i))){
            file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_i));
            tmp=tmp2;
        }
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_i));
    }else{
        strcpy(tmp.isbn, a[0].isbn);
        file.seekp(file.tellg());
        file.seekp(-sizeof(Index_i),ios::cur);
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_i));
        file1.seekp(p);
        for(auto & i : a) file1.write(reinterpret_cast<char *>(&i),sizeof(Isbn));
        tmp1.Bid=-1;strcpy(tmp1.isbn,"");
        for(int i=a.size();i<BN;++i) file1.write(reinterpret_cast<char *>(&tmp1),sizeof(Isbn));
    }
    file.close();
    file1.close();
 //   DisplayIsbn();
}
void delisbn(const char *_isbn){
    fstream file("index_i");
    fstream file1("isbn");
    Index_i tmp;
    Isbn tmp1;
    int p=-1;
    while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_i))){
        if(tmp.p<0) continue;
        if(strcmp(tmp.isbn,_isbn)>0){
            file.seekg(-sizeof(Index_i)*2,ios::cur);
            break;
        }
        p=tmp.p;
        if(strcmp(tmp.isbn,_isbn)==0){
            file.seekg(-sizeof(Index_i),ios::cur);
            break;
        }
    }

    vector<Isbn>a;
    a.clear();
    if(file.eof()){//last block
        file.clear(ios::goodbit);
        file.seekg(-sizeof(Index_i),ios::end);
    }
    file1.seekg(p);
    for(int i=0;i<BN;++i){
        file1.read(reinterpret_cast<char *>(&tmp1),sizeof(Isbn));
        if(tmp1.Bid==-1) break;
        if(strcmp(_isbn,tmp1.isbn)==0) continue;
        a.push_back(tmp1);
    }

    file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_i));
    if(a.empty()){
        file.seekp(file.tellg());
        file.seekp(-sizeof(Index_i),ios::cur);
        while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_i))){
            file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_i));
        }
        tmp.p=-1;
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_i));
    }else{
        a.size();strcpy(tmp.isbn, a[0].isbn);
        file.seekp(file.tellg());
        file.seekp(-sizeof(Index_i),ios::cur);
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_i));
        file1.seekp(p);
        for(auto & i : a) file1.write(reinterpret_cast<char *>(&i),sizeof(Isbn));
        tmp1.Bid=-1;strcpy(tmp1.isbn,"");
        for(int i=a.size();i<BN;++i) file1.write(reinterpret_cast<char *>(&tmp1),sizeof(Isbn));
    }
    file.close();
    file1.close();
}
void addname(const char *_name){
    fstream file("index_n");
    fstream file1("name");
    Index_n tmp;
    Name tmp1;
    int p=-1;
    while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_n))){
        if(tmp.p<0) continue;
        if(strcmp(tmp.name,_name)>0) break;
        p=tmp.p;
    }
    vector<Name>a;
    a.clear();
    if(p<0){//first block
        p=0;
        file.seekg(0);
    }
    else if(file.eof()){//last block
        file.clear(ios::goodbit);
        file.seekg(-sizeof(Index_n),ios::end);
    }else{
        file.seekg(-sizeof(Index_n)*2,ios::cur);
    }
    file1.seekg(p);
    for(int i=0;i<BN;++i){
        file1.read(reinterpret_cast<char *>(&tmp1),sizeof(Name));
        if(tmp1.Bid==-1) break;
        a.push_back(tmp1);
    }
    tmp1.Bid=CurBid;strcpy(tmp1.name,_name);
    a.push_back(tmp1);
    sort(a.begin(),a.end());
    file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_n));
    if(a.size()==BN){
        vector<Name>b;
        b.clear();
        for(int i=0;i<=BN/2;++i){
            b.push_back(a.back());a.pop_back();
        }
        reverse(b.begin(),b.end());
        a.size();strcpy(tmp.name, a[0].name);
        file.seekp(file.tellg());
        file.seekp(-sizeof(Index_n),ios::cur);
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_n));
        file1.seekp(p);
        for(auto & i : a) file1.write(reinterpret_cast<char *>(&i),sizeof(Name));
        tmp1.Bid=-1;strcpy(tmp1.name,"");
        for(int i=a.size();i<BN;++i) file1.write(reinterpret_cast<char *>(&tmp1),sizeof(Name));
        file1.seekp(0,ios::end);
        b.size();strcpy(tmp.name, b[0].name);tmp.p=file1.tellp();
        for(auto & i : b) file1.write(reinterpret_cast<char *>(&i),sizeof(Name));
        for(int i=b.size();i<BN;++i) file1.write(reinterpret_cast<char *>(&tmp1),sizeof(Name));
        Index_n tmp2;
        while(file.read(reinterpret_cast<char *>(&tmp2),sizeof(Index_n))){
            file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_n));
            tmp=tmp2;
        }
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_n));
    }else{
        a.size();strcpy(tmp.name, a[0].name);
        file.seekp(file.tellg());
        file.seekp(-sizeof(Index_n),ios::cur);
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_n));
        file1.seekp(p);
        for(auto & i : a) file1.write(reinterpret_cast<char *>(&i),sizeof(Name));
        tmp1.Bid=-1;strcpy(tmp1.name,"");
        for(int i=a.size();i<BN;++i) file1.write(reinterpret_cast<char *>(&tmp1),sizeof(Name));
    }
    file.close();
    file1.close();
}
void delname(const char *_name){
    fstream file("index_n");
    fstream file1("name");
    Index_n tmp;
    Name tmp1;
    int p=-1;
    while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_n))){
        if(tmp.p<0) continue;
        if(strcmp(tmp.name,_name)>=0){
            file.seekg(-sizeof(Index_n),ios::cur);
            break;
        }
        p=tmp.p;
    }
    
    vector<Name>a;
    if(p<0){p=0;file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_n));}
    if(file.eof()){//last block
        file.clear(ios::goodbit);
    }
    bool flag=false;
    while(true){
        a.clear();
        file1.seekg(p);
        for(int i=0;i<BN;++i){
            file1.read(reinterpret_cast<char *>(&tmp1),sizeof(Name));
            if(tmp1.Bid==-1) break;
            if(strcmp(_name,tmp1.name)==0&&tmp1.Bid==CurBid){
                flag=true;continue;
            }
            a.push_back(tmp1);
        }
        if(flag) break;
        file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_n));
        p=tmp.p;
    }
    
    if(a.empty()){
        file.seekp(file.tellg());
        file.seekp(-sizeof(Index_n),ios::cur);
        while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_n))){
            file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_n));
        }
        tmp.p=-1;
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_n));
    }else{
        a.size();strcpy(tmp.name, a[0].name);
        file.seekp(file.tellg());
        file.seekp(-sizeof(Index_n),ios::cur);
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_n));
        file1.seekp(p);
        for(auto & i : a) file1.write(reinterpret_cast<char *>(&i),sizeof(Name));
        tmp1.Bid=-1;strcpy(tmp1.name,"");
        for(int i=a.size();i<BN;++i) file1.write(reinterpret_cast<char *>(&tmp1),sizeof(Name));
    }
    file.close();
    file1.close();
}
void addauthor(const char *_author){
    fstream file("index_a");
    fstream file1("author");
    Index_a tmp;
    Author tmp1;
    int p=-1;
    while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_a))){
        if(tmp.p<0) continue;
        if(strcmp(tmp.author,_author)>0) break;
        p=tmp.p;
    }
    vector<Author>a;
    a.clear();
    if(p<0){//first block
        p=0;
        file.seekg(0);
    }
    else if(file.eof()){//last block
        file.clear(ios::goodbit);
        file.seekg(-sizeof(Index_a),ios::end);
    }else{
        file.seekg(-sizeof(Index_a)*2,ios::cur);
    }
    file1.seekg(p);
    for(int i=0;i<BN;++i){
        file1.read(reinterpret_cast<char *>(&tmp1),sizeof(Author));
        if(tmp1.Bid==-1) break;
        a.push_back(tmp1);
    }
    tmp1.Bid=CurBid;strcpy(tmp1.author,_author);
    a.push_back(tmp1);
    sort(a.begin(),a.end());
    file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_a));
    if(a.size()==BN){
        vector<Author>b;
        b.clear();
        for(int i=0;i<=BN/2;++i){
            b.push_back(a.back());a.pop_back();
        }
        reverse(b.begin(),b.end());
        a.size();strcpy(tmp.author, a[0].author);
        file.seekp(file.tellg());
        file.seekp(-sizeof(Index_a),ios::cur);
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_a));
        file1.seekp(p);
        for(auto & i : a) file1.write(reinterpret_cast<char *>(&i),sizeof(Author));
        tmp1.Bid=-1;strcpy(tmp1.author,"");
        for(int i=a.size();i<BN;++i) file1.write(reinterpret_cast<char *>(&tmp1),sizeof(Author));
        file1.seekp(0,ios::end);
        b.size();strcpy(tmp.author, b[0].author);tmp.p=file1.tellp();
        for(auto & i : b) file1.write(reinterpret_cast<char *>(&i),sizeof(Author));
        for(int i=b.size();i<BN;++i) file1.write(reinterpret_cast<char *>(&tmp1),sizeof(Author));
        Index_a tmp2;
        while(file.read(reinterpret_cast<char *>(&tmp2),sizeof(Index_a))){
            file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_a));
            tmp=tmp2;
        }
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_a));
    }else{
        a.size();strcpy(tmp.author, a[0].author);
        file.seekp(file.tellg());
        file.seekp(-sizeof(Index_a),ios::cur);
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_a));
        file1.seekp(p);
        for(auto & i : a) file1.write(reinterpret_cast<char *>(&i),sizeof(Author));
        tmp1.Bid=-1;strcpy(tmp1.author,"");
        for(int i=a.size();i<BN;++i) file1.write(reinterpret_cast<char *>(&tmp1),sizeof(Author));
    }
    file.close();
    file1.close();
}
void delauthor(const char *_author){
    fstream file("index_a");
    fstream file1("author");
    Index_a tmp;
    Author tmp1;
    int p=-1;
    while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_a))){
        if(tmp.p<0) continue;
        if(strcmp(tmp.author,_author)>=0){
            file.seekg(-sizeof(Index_a),ios::cur);
            break;
        }
        p=tmp.p;
    }

    vector<Author>a;
    if(p<0){p=0;file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_a));}
    if(file.eof()){//last block
        file.clear(ios::goodbit);
    }
    bool flag=false;
    while(true){
        a.clear();
        file1.seekg(p);
        for(int i=0;i<BN;++i){
            file1.read(reinterpret_cast<char *>(&tmp1),sizeof(Author));
            if(tmp1.Bid==-1) break;
            if(strcmp(_author,tmp1.author)==0&&tmp1.Bid==CurBid){
                flag=true;continue;
            }
            a.push_back(tmp1);
        }
        if(flag) break;
        file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_a));
        p=tmp.p;
    }

    if(a.empty()){
        file.seekp(file.tellg());
        file.seekp(-sizeof(Index_a),ios::cur);
        while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_a))){
            file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_a));
        }
        tmp.p=-1;
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_a));
    }else{
        a.size();strcpy(tmp.author, a[0].author);
        file.seekp(file.tellg());
        file.seekp(-sizeof(Index_a),ios::cur);
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_a));
        file1.seekp(p);
        for(auto & i : a) file1.write(reinterpret_cast<char *>(&i),sizeof(Author));
        tmp1.Bid=-1;strcpy(tmp1.author,"");
        for(int i=a.size();i<BN;++i) file1.write(reinterpret_cast<char *>(&tmp1),sizeof(Author));
    }
    file.close();
    file1.close();
}
void addkeyword(const char *_keyword){
    fstream file("index_k");
    fstream file1("keyword");
    Index_k tmp;
    Keyword tmp1;
    int p=-1;
    while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_k))){
        if(tmp.p<0) continue;
        if(strcmp(tmp.keyword,_keyword)>0) break;
        p=tmp.p;
    }
    vector<Keyword>a;
    a.clear();
    if(p<0){//first block
        p=0;
        file.seekg(0);
    }
    else if(file.eof()){//last block
        file.clear(ios::goodbit);
        file.seekg(-sizeof(Index_k),ios::end);
    }else{
        file.seekg(-sizeof(Index_k)*2,ios::cur);
    }
    file1.seekg(p);
    for(int i=0;i<BN;++i){
        file1.read(reinterpret_cast<char *>(&tmp1),sizeof(Keyword));
        if(tmp1.Bid==-1) break;
        a.push_back(tmp1);
    }
    tmp1.Bid=CurBid;strcpy(tmp1.keyword,_keyword);
    a.push_back(tmp1);
    sort(a.begin(),a.end());
    file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_k));
    if(a.size()==BN){
        vector<Keyword>b;
        b.clear();
        for(int i=0;i<=BN/2;++i){
            b.push_back(a.back());a.pop_back();
        }
        reverse(b.begin(),b.end());
        a.size();strcpy(tmp.keyword, a[0].keyword);
        file.seekp(file.tellg());
        file.seekp(-sizeof(Index_k),ios::cur);
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_k));
        file1.seekp(p);
        for(auto & i : a) file1.write(reinterpret_cast<char *>(&i),sizeof(Keyword));
        tmp1.Bid=-1;strcpy(tmp1.keyword,"");
        for(int i=a.size();i<BN;++i) file1.write(reinterpret_cast<char *>(&tmp1),sizeof(Keyword));
        file1.seekp(0,ios::end);
        b.size();strcpy(tmp.keyword, b[0].keyword);tmp.p=file1.tellp();
        for(auto & i : b) file1.write(reinterpret_cast<char *>(&i),sizeof(Keyword));
        for(int i=b.size();i<BN;++i) file1.write(reinterpret_cast<char *>(&tmp1),sizeof(Keyword));
        Index_k tmp2;
        while(file.read(reinterpret_cast<char *>(&tmp2),sizeof(Index_k))){
            file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_k));
            tmp=tmp2;
        }
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_k));
    }else{
        a.size();strcpy(tmp.keyword, a[0].keyword);
        file.seekp(file.tellg());
        file.seekp(-sizeof(Index_k),ios::cur);
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_k));
        file1.seekp(p);
        for(auto & i : a) file1.write(reinterpret_cast<char *>(&i),sizeof(Keyword));
        tmp1.Bid=-1;strcpy(tmp1.keyword,"");
        for(int i=a.size();i<BN;++i) file1.write(reinterpret_cast<char *>(&tmp1),sizeof(Keyword));
    }
    file.close();
    file1.close();
}
void delkeyword(const char *_keyword){
    fstream file("index_k");
    fstream file1("keyword");
    Index_k tmp;
    Keyword tmp1;
    int p=-1;
    while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_k))){
        if(tmp.p<0) continue;
        if(strcmp(tmp.keyword,_keyword)>=0){
            file.seekg(-sizeof(Index_k),ios::cur);
            break;
        }
        p=tmp.p;
    }

    vector<Keyword>a;
    if(p<0){p=0;file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_k));}
    if(file.eof()){//last block
        file.clear(ios::goodbit);
    }
    bool flag=false;
    while(true){
        a.clear();
        file1.seekg(p);
        for(int i=0;i<BN;++i){
            file1.read(reinterpret_cast<char *>(&tmp1),sizeof(Keyword));
            if(tmp1.Bid==-1) break;
            if(strcmp(_keyword,tmp1.keyword)==0&&tmp1.Bid==CurBid){
                flag=true;continue;
            }
            a.push_back(tmp1);
        }
        if(flag) break;
        file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_k));
        p=tmp.p;
    }

    if(a.empty()){
        file.seekp(file.tellg());
        file.seekp(-sizeof(Index_k),ios::cur);
        while(file.read(reinterpret_cast<char *>(&tmp),sizeof(Index_k))){
            file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_k));
        }
        tmp.p=-1;
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_k));
    }else{
        a.size();strcpy(tmp.keyword, a[0].keyword);
        file.seekp(file.tellg());
        file.seekp(-sizeof(Index_k),ios::cur);
        file.write(reinterpret_cast<char *>(&tmp),sizeof(Index_k));
        file1.seekp(p);
        for(auto & i : a) file1.write(reinterpret_cast<char *>(&i),sizeof(Keyword));
        tmp1.Bid=-1;strcpy(tmp1.keyword,"");
        for(int i=a.size();i<BN;++i) file1.write(reinterpret_cast<char *>(&tmp1),sizeof(Keyword));
    }
    file.close();
    file1.close();
}
void Select(){
    if(Command.size()!=2||Cur_User.rank<3){puts("Invalid");return;}
    char _isbn[22];
    memset(_isbn,0,sizeof(_isbn));
    strcpy(_isbn,Command[1].c_str());
    CurBid=findisbn(_isbn);
    if(CurBid!=-1) return;
    ofstream file("book",ios::app);
    CurBid=file.tellp()/sizeof(Book);
    Book bk(_isbn);
    file.write(reinterpret_cast<char *>(&bk),sizeof(Book));
    file.close();
    addisbn(_isbn);
}
void Modify(){
    if(Cur_User.rank<3||CurBid==-1){puts("Invalid");return;}

    fstream file("book");
    Book bk;
    file.seekg(CurBid*sizeof(Book));
    file.read(reinterpret_cast<char *>(&bk),sizeof(Book));

    for(int i=1;i<Command.size();++i){
        if(Command[i].find("-ISBN")!=string::npos){
            string ss=Command[i].substr(6);
            if(strcmp(ss.c_str(),bk.isbn)==0) continue;
            if(findisbn(ss.c_str())) continue;
        }
        if(Command[i].find("-name")!=string::npos) continue;
        if(Command[i].find("-author")!=string::npos) continue;
        if(Command[i].find("-keyword")!=string::npos) continue;
        if(Command[i].find("-price")!=string::npos) continue;
        puts("Invalid");file.close();return;
    }

    for(int i=1;i<Command.size();++i){
        if(Command[i].find("-ISBN")!=string::npos){
            Command[i]=Command[i].substr(6);
            if(strcmp(Command[i].c_str(),bk.isbn)==0) continue;
            addisbn(Command[i].c_str());
            delisbn(bk.isbn);
            strcpy(bk.isbn,Command[i].c_str());
            continue;
        }
        if(Command[i].find("-name")!=string::npos){
            Command[i]=Command[i].substr(7);
            Command[i].pop_back();
            if(strcmp(Command[i].c_str(),bk.name)==0) continue;
            addname(Command[i].c_str());
            if(strcmp(bk.name,"")!=0) delname(bk.name);
            strcpy(bk.name,Command[i].c_str());
            continue;
        }
        if(Command[i].find("-author")!=string::npos){
            Command[i]=Command[i].substr(9);
            Command[i].pop_back();
            if(strcmp(Command[i].c_str(),bk.author)==0) continue;
            addauthor(Command[i].c_str());
            if(strcmp(bk.author,"")!=0) delauthor(bk.author);
            strcpy(bk.author,Command[i].c_str());
            continue;
        }
        if(Command[i].find("-keyword")!=string::npos){
            Command[i]=Command[i].substr(10);
            Command[i].pop_back();
            map<string,int>mp;
            string ss=Command[i];
            int l=0,r=0;
            while((r=ss.find('|',l))!=string::npos){
                mp[ss.substr(l,r-l)]=1;
                l=r+1;
            }
            ss=bk.keyword;
            l=r=0;
            while((r=ss.find('|',l))!=string::npos){
                mp[ss.substr(l,r-l)]-=1;
                l=r+1;
            }
            for(auto & it : mp){
                if(it.second>0) addkeyword(it.first.c_str());
                if(it.second<0) delkeyword(it.first.c_str());
            }
            strcpy(bk.keyword,Command[i].c_str());
            continue;
        }
        if(Command[i].find("-price")!=string::npos) {
            Command[i] = Command[i].substr(7);
            bk.price=stod(Command[i]);
            continue;
        }
    }
    file.seekp(CurBid*sizeof(Book));
    file.write(reinterpret_cast<char *>(&bk),sizeof(Book));
    file.close();
}
void Import(){
    if(Cur_User.rank<3||CurBid==-1||Command.size()!=3){puts("Invalid");return;}
    fstream file("book");
    Book bk;
    file.seekg(CurBid*sizeof(Book));
    file.read(reinterpret_cast<char *>(&bk),sizeof(Book));
    bk.num+=stoi(Command[1]);
    file.seekp(CurBid*sizeof(Book));
    file.write(reinterpret_cast<char *>(&bk),sizeof(Book));
    file.close();

    file.open("finance");
    Finance tmp;
    file.seekg(-sizeof(Finance),ios::end);
    file.read(reinterpret_cast<char *>(&tmp),sizeof(Finance));
    tmp.in=0;tmp.out=stod(Command[2]);
    tmp.tin+=tmp.in;tmp.tout+=tmp.out;
    file.seekp(0,ios::end);
    file.write(reinterpret_cast<char *>(&tmp),sizeof(Finance));
    file.close();
}
void Buy(){
    if(Cur_User.rank<1||Command.size()!=3){puts("Invalid");return;}
    int p=findisbn(Command[1].c_str());
    if(p==-1){puts("Invalid");return;}

    fstream file("book");
    Book bk;
    file.seekg(p*sizeof(Book));
    file.read(reinterpret_cast<char *>(&bk),sizeof(Book));
    int q=stoi(Command[2]);
    double pri=bk.price;
    if(bk.num<q) {file.close();puts("Invalid");return;}
    bk.num-=q;
    file.seekp(p*sizeof(Book));
    file.write(reinterpret_cast<char *>(&bk),sizeof(Book));
    file.close();

    file.open("finance");
    Finance tmp;
    file.seekg(-sizeof(Finance),ios::end);
    file.read(reinterpret_cast<char *>(&tmp),sizeof(Finance));
    tmp.in=q*pri;tmp.out=0;
    tmp.tin+=tmp.in;tmp.tout+=tmp.out;
    file.seekp(0,ios::end);
    file.write(reinterpret_cast<char *>(&tmp),sizeof(Finance));
    file.close();
}
void Show_finance(){
    if(Cur_User.rank!=7||Command.size()>3){puts("Invalid");return;}
    ifstream file("finance");
    file.seekg(-sizeof(Finance),ios::end);
    Finance tmp,tmp1;
    file.read(reinterpret_cast<char *>(&tmp),sizeof(Finance));
    if(Command.size()==2){
        tmp.display();return;
    }
    int x=stoi(Command[2]);
    file.seekg(-sizeof(Finance)*(x+1),ios::end);
    file.read(reinterpret_cast<char *>(&tmp1),sizeof(Finance));
    tmp-=tmp1;
    tmp.display();
}
void Show(){
    if(Command.size()==1){
        ifstream file("book");
        vector<Book>a;
        Book bk;
        while(file.read(reinterpret_cast<char *>(&bk),sizeof(Book)))
            a.push_back(bk);
        sort(a.begin(),a.end());
        for(auto i :a) i.display();
        return;
    }
    if(Command[1]=="finance"){
        Show_finance();
        return;
    }
    if(Cur_User.rank<1||Command.size()>2){puts("Invalid");return;}
    if(Command[1].find("-ISBN")!=string::npos){
        Command[1]=Command[1].substr(6);
        int p=findisbn(Command[1].c_str());
        if(p==-1) return;
        ifstream file("book");
        Book bk;
        file.seekg(p*sizeof(Book));
        file.read(reinterpret_cast<char *>(&bk),sizeof(Book));
        bk.display();
        return;
    }
    if(Command[1].find("-name")!=string::npos){
        Command[1]=Command[1].substr(7);
        Command[1].pop_back();
        findname(Command[1].c_str());
        return;
    }
    if(Command[1].find("-author")!=string::npos){
        Command[1]=Command[1].substr(9);
        Command[1].pop_back();
        findauthor(Command[1].c_str());
        return;
    }
    if(Command[1].find("-keyword")!=string::npos){
        Command[1]=Command[1].substr(10);
        Command[1].pop_back();
        findkeyword(Command[1].c_str());
        return;
    }
    puts("Invalid");
}

void Init(){
    strcpy(Cur_User.id,"root");
    strcpy(Cur_User.name,"root");
    strcpy(Cur_User.passwd,"sjtu");
    Cur_User.rank=7;
    ofstream Ofile("user");
    Ofile.write(reinterpret_cast<char *>(&Cur_User),sizeof(User));
    Ofile.close();

    Ofile.open("book");
    Ofile.close();

    Ofile.open("isbn");
    Isbn tmp;
    for(int i=0;i<BN;++i) Ofile.write(reinterpret_cast<char *>(&tmp),sizeof(Isbn));
    Ofile.close();

    Ofile.open("name");
    Name tmp1;
    for(int i=0;i<BN;++i) Ofile.write(reinterpret_cast<char *>(&tmp1),sizeof(Name));
    Ofile.close();

    Ofile.open("author");
    Author tmp2;
    for(int i=0;i<BN;++i) Ofile.write(reinterpret_cast<char *>(&tmp2),sizeof(Author));
    Ofile.close();

    Ofile.open("keyword");
    Keyword tmp3;
    for(int i=0;i<BN;++i) Ofile.write(reinterpret_cast<char *>(&tmp3),sizeof(Keyword));
    Ofile.close();

    Ofile.open("index_i");
    Index_i tmp4("",0);
    Ofile.write(reinterpret_cast<char *>(&tmp4),sizeof(Index_i));
    Ofile.close();

    Ofile.open("index_n");
    Index_n tmp5("",0);
    Ofile.write(reinterpret_cast<char *>(&tmp5),sizeof(Index_n));
    Ofile.close();

    Ofile.open("index_a");
    Index_a tmp6("",0);
    Ofile.write(reinterpret_cast<char *>(&tmp6),sizeof(Index_a));
    Ofile.close();

    Ofile.open("index_k");
    Index_k tmp7("",0);
    Ofile.write(reinterpret_cast<char *>(&tmp7),sizeof(Index_k));
    Ofile.close();

    Ofile.open("finance");
    Finance tmp8;
    Ofile.write(reinterpret_cast<char *>(&tmp8),sizeof(Finance));
    Ofile.close();
}
int main(){
//   freopen("test_data/testcase6.txt","r",stdin);
//   freopen("a.out","w",stdout);
    Init();
//    int cnt=0;
    while(true){
    //    DisplayUser(cnt++);
    //    DisplayBook(cnt++);
    //    DisplayFinance(cnt++);
        string op;
        getline(cin,op);
        stringstream ss(op);
        Command.clear();
        while(ss>>op) Command.push_back(op);
        if(Command[0]=="exit") break;
        if(Command[0]=="su"){Login();continue;}
        if(Command[0]=="logout"){Logout();continue;}
        if(Command[0]=="useradd"){UserAdd();continue;}
        if(Command[0]=="register"){RegisterUser();continue;}
        if(Command[0]=="delete"){DeleteUser();continue;}
        if(Command[0]=="passwd"){ChangePasswd();continue;}
        if(Command[0]=="select"){Select();continue;}
        if(Command[0]=="modify"){Modify();continue;}
        if(Command[0]=="import"){Import();continue;}
        if(Command[0]=="show"){Show();continue;}
        if(Command[0]=="buy"){Buy();continue;}
        puts("Invalid");
    }
 //   DisplayUser(cnt);
    return 0;
}
