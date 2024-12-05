#if !define	BOOK_MANAGE_SYSTEM_H
#define BOOK_MANAGE_SYSTEM_H

#include<iostream>
#include<fstream>
#include "SqList.h"
using namespace std;

// 一些所需结构体的定义
struct Date {
	int year, month, day;
};

struct Book {
	char id[14]; //ISBN号
	char name[128]; //书名
	char author[128]; //作者
	float price; //价格
	Date buy_date; //购买日期
};

//索引结构
struct IdIndex { //主关键字
	char id[14]; //ISBN号
	long offset; //偏移量
};

struct NameIndex { //次关键字
	char name[128]; //书名
	long offset; //偏移量
};

// 书籍管理系统类
class BookManageSystem
{
public:
	BookManageSystem(); // 构造函数
	~BookManageSystem(); // 析构函数
	//Run(); //运行图书管理系统

private:
	//数据成员
	fstream bookFile; // 书籍文件
	SqList<IdIndex> idIndexList; // ISBN号索引表
	SqList<NameIndex> nameIndexList; // 书名索引表

	//成员函数
	void InsertBook(); // 插入书籍
	void InsertBookAux(Book &book, int offset); // 插入书籍辅助函数

};
// 构造函数
BookManageSystem::BookManageSystem()
{
	bookFile.open("bookdata.txt");
};
// 析构函数
BookManageSystem::~BookManageSystem()
{
};
// 插入书籍
void BookManageSystem::InsertBook()
{
	Book book;
	cout << "请输入书籍的ISBN号：";
	cin >> book.id;
	cout << "请输入书籍的书名：";
	cin >> book.name;
	cout << "请输入书籍的作者：";
	cin >> book.author;
	cout << "请输入书籍的价格：";
	cin >> book.price;
	cout << "请输入书籍的购买日期(四位year 两位month 两位day，以空格隔开)：";
	cin >> book.buy_date.year >> book.buy_date.month >> book.buy_date.day;
	// 插入书籍
	InsertBookAux(book, bookFile.tellp());
};
// 插入书籍辅助函数

#endif