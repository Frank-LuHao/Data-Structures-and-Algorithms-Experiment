#if !define	BOOK_MANAGE_SYSTEM_H
#define BOOK_MANAGE_SYSTEM_H

#include<iostream>
#include<fstream>
#include<iomanip>
#include "SimpleLkList.h"
#include "SqList.h"
#define max(a, b) ((a)>(b)?(a):(b))
using namespace std;

// 一些所需结构体的定义:
// 日期结构
struct Date {
	int year, month, day;
};

// 书籍结构
struct Book {
	char id[14]; //ISBN号
	char name[128]; //书名
	char author[128]; //作者
	float price; //价格
	Date buyDate; //购买日期
	bool isDeleted; //是否删除
};

//索引结构:
// 书号主索引
struct IdIndex { //主关键字
	char id[14]; //ISBN号
	int offset; //书籍记录在文件中的偏移量
};

// 书名次索引
struct NameIndex { //次关键字
	char name[128]; //书名
	SimpleLkList<int> offsetList; //书籍记录在文件中的偏移量链表
};

// 书籍管理系统类
class BookManageSystem
{
public:
	BookManageSystem(); // 构造函数
	~BookManageSystem(); // 析构函数
	void Run(); //运行图书管理系统

private:
	//数据成员
	fstream bookFile; // 书籍文件
	SqList<IdIndex> idIndexList; // ISBN号索引表, 升序
	SqList<NameIndex> nameIndexList; // 书名索引表, 升序

	//成员函数
	void InsertBook(); // 插入书籍
	bool InsertBookAux(Book &book, int offset); // 插入书籍辅助函数
	
	void DeleteBook(); // 删除书籍
	void DeleteBookAux(char *id); // 删除书籍辅助函数
	
	void SearchBook(); // 查找书籍
	void SearchByTitle(); // 按书名查找
	void SearchByAuthor(); // 按作者查找
	bool BinSearchByTitle(char *name, SimpleLkList<int> &BookList); // 二分查找书名，返回链表
	
	void UpdateBook(); // 更新书籍
	
	void SortByAuthor(); // 按作者名排序

	void DisplayBook(Book &book); // 显示书籍信息
	void DisplayBook(SimpleLkList<Book> &bookList); // 显示书籍列表
	void DisplayBookAux(Book &book, int numspace[]); // 显示书籍信息辅助函数
};

// 构造函数
BookManageSystem::BookManageSystem()
{
	// 打开文件
	bookFile.open("bookdata.txt");
	if (!bookFile.is_open())
	{
		cout << "文件打开失败" << endl;
		exit(1);
	}

	// 读取文件中的书籍信息
	// 判断文件是否为空
	bookFile.seekg(0, ios::end);
	if (bookFile.tellg() != 0)
	{ // 文件不为空
		bookFile.seekg(0, ios::beg);
		while (!bookFile.eof())
		{
			Book book;
			IdIndex CurIdIndex;
			NameIndex CurNameIndex;
			CurIdIndex.offset = bookFile.tellg(); // 记录书籍在文件中的偏移量
			bookFile.read((char*)&book, sizeof(Book)); // 读取书籍信息
			// 插入书籍
			
			// 在主表中插入书籍
			int pos;
			// 二分找到插入位置
			strcpy(CurIdIndex.id, book.id);
			idIndexList.Insert(pos, CurIdIndex);
			
			// 在次表中插入书籍
		}
	}
};

// 析构函数
BookManageSystem::~BookManageSystem()
{
	if (bookFile.is_open())
		bookFile.close();
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
	cout << "请输入书籍的购买日期(year month day，以空格隔开)：";
	cin >> book.buyDate.year >> book.buyDate.month >> book.buyDate.day;

	//查找书籍是否已经存在

	// 在文件中写入书籍
	
	// 插入书籍

	
};

// 插入书籍辅助函数
bool BookManageSystem::InsertBookAux(Book &book, int offset)
{
	// 在主表中插入书籍
	// 在次表中插入书籍
};

// 删除书籍
void BookManageSystem::DeleteBook()
{
	char id[14];
	cout << "请输入要删除的书籍的ISBN号：";
	cin >> id;
	DeleteBookAux(id);
};

// 删除书籍辅助函数
void BookManageSystem::DeleteBookAux(char *id)
{
	// 在主表中查找书籍

	// 在文件中删除书籍

	// 在主表中删除书籍
};

// 查找书籍
void BookManageSystem::SearchBook()
{
	do {
		cout << "1.按书名查找 2.按作者查找 3.退出查找\n";
		int choice;
		cout << "请输入选择：";
		cin >> choice;
		switch (choice)
		{
		case 1:
			SearchByTitle();
			break;
		case 2:
			SearchByAuthor();
			break;
		case 3:
			return;
		}
	} while (true);
};

// 按书名查找
void BookManageSystem::SearchByTitle()
{ // 由于有书名索引，可以直接在索引表中查找，并进一步通过偏移量找到书籍信息
	char name[128];
	cout << "请输入要查找的书名：";
	cin >> name;

	// 在书名索引表中查找
	// 二分查找
	SimpleLkList<int> Bookoffset;
	if (BinSearchByTitle(name, Bookoffset) == 0) {
		cout << "未找到相关书籍!\n" << endl;
		return;
	}

	// 根据位移量，找到书籍信息
	cout << "查找到的书籍信息如下：\n" << endl;
	SimpleLkList<Book> BookList;
	for (int i = 0; i < Bookoffset.Length(); i++) {
		Book book;
		int CurOffset;
		Bookoffset.GetElem(i, CurOffset);
		bookFile.seekg(CurOffset, ios::beg);
		bookFile.read((char*)&book, sizeof(Book));
		BookList.AddTail(book);
	}
	// 输出书籍信息
	DisplayBook(BookList);
	
	return;
};

// 二分查找书名，返回链表
bool BookManageSystem::BinSearchByTitle(char *name, SimpleLkList<int> &Bookoffset)
{
	// 在书名索引表中查找
	// 二分查找
	int low = 0, high = nameIndexList.Length() - 1;
	while (low <= high) {
		int mid = (low + high) >> 1;
		if (strcmp(nameIndexList[mid].name, name) == 0) {
			// 找到书名
			Bookoffset = nameIndexList[mid].offsetList;
			return true;
		}
		else if (strcmp(nameIndexList[mid].name, name) < 0) {
			low = mid + 1;
		}
		else {
			high = mid;
		}
	}
	return false;
};

// 按作者查找
void BookManageSystem::SearchByAuthor()
{ // 由于没有作者索引，只能遍历所有书籍文件
	char author[128];
	cout << "请输入要查找的作者：";
	cin >> author;
	bookFile.clear(); // 清除文件流状态
	bookFile.seekg(0, ios::beg); // 定位到文件开头
	Book book;
	SimpleLkList<Book> bookList;
	bool flag = 0;
	do {
		bookFile.read((char*) &book, sizeof(Book));
		if (strcmp(book.author, author) == 0)
		{
			flag = 1;
			bookList.AddTail(book);
		}
	} while(!bookFile.eof());
	if (flag) {
		cout << "查找到的书籍信息如下：\n" << endl;
		DisplayBook(bookList);
	}
	else {
		cout << "未找到相关书籍!\n" << endl;
	}
	
	return;
};

// 运行图书管理系统
void BookManageSystem::Run()
{
	int choice;
	while (true)
	{
		cout << "请选择操作：1.插入书籍 2.删除书籍 3.更新记录 4.查找书籍 5.按作者名排序 6.退出\n" << endl;
		cout << "请输入选择：";
		cin >> choice;
		switch (choice)
		{
		case 1:
			InsertBook();
			break;
		case 2:
			DeleteBook();
			break;
		case 3:
			//UpdateBook();
			break;
		case 4:
			SearchBook();
			break;
		case 5:
			break;
		case 6:
			return;
		default:
			cout << "输入错误，请重新输入" << endl;
			break;
		}
	}
};

// 显示书籍信息
void BookManageSystem::DisplayBook(Book &book)
{
	// 统计各个字段的最大长度
	int numspace[4];
	numspace[0] = max(strlen(book.id), 6);
	numspace[1] = max(strlen(book.name), 4);
	numspace[2] = max(strlen(book.author), 4);
	int price = book.price, len = 3;
	while(price)
	{
		price /= 10;
		len++;
	}
	numspace[3] = max(len, 8);
	
	// 输出表头
	cout << setw(numspace[0]) << "ISBN号" << "\t";
	cout << setw(numspace[1]) << "书名" << "\t";
	cout << setw(numspace[2]) << "作者" << "\t";
	cout << setw(numspace[3]) << "价格" << "\t";
	cout << "购买日期" << endl;
	
	// 输出书籍信息
	DisplayBookAux(book, numspace);
};

// 显示书籍列表
void BookManageSystem::DisplayBook(SimpleLkList<Book> &bookList)
{
	// 统计各个字段的最大长度
	int numspace[4] = {6, 4, 4, 8};
for (int i = 0; i < bookList.Length(); i++)
	{
		Book tmp;
		bookList.GetElem(i, tmp);
		numspace[0] = max(numspace[0], strlen(tmp.id));
		numspace[1] = max(numspace[1], strlen(tmp.name));
		numspace[2] = max(numspace[2], strlen(tmp.author));
		int price = tmp.price, len = 3;
		while(price)
		{
			price /= 10;
			len++;
		}
		numspace[3] = max(numspace[3], len);
	}

	// 输出表头
	cout << setw(numspace[0]) << "ISBN号" << "\t";
	cout << setw(numspace[1]) << "书名" << "\t";
	cout << setw(numspace[2]) << "作者" << "\t";
	cout << setw(numspace[3]) << "价格" << "\t";
	cout << "购买日期" << endl;
	
	// 输出书籍信息
	for (int i = 0; i < bookList.Length(); i++)
	{
		Book tmp;
		bookList.GetElem(i, tmp);
		DisplayBookAux(tmp, numspace);
	}
};

// 显示书籍信息辅助函数
void BookManageSystem::DisplayBookAux(Book &book, int numspace[])
{
	cout << setw(numspace[0]) << book.id << "\t";
	cout << setw(numspace[1]) << book.name << "\t";
	cout << setw(numspace[2]) << book.author << "\t";
	cout << setw(numspace[3]) << std::setprecision(2) << book.price << "\t";
	cout << book.buyDate.year << " " << book.buyDate.month << " " << book.buyDate.day << endl;
};

// 更新书籍
void BookManageSystem::UpdateBook()
{

};

// 按作者名排序
void BookManageSystem::SortByAuthor()
{

};
#endif