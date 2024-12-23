#if !define	BOOK_MANAGE_SYSTEM_H
#define BOOK_MANAGE_SYSTEM_H

#include<iostream>
#include<fstream>
#include<iomanip>
#include "SimpleLkList.h"
#include "SqList.h"
#include "data_struct.h"
#define max(a, b) ((a)>(b)?(a):(b))
using namespace std;

// 书籍管理系统类
class BookManageSystem
{
public:
	BookManageSystem(); // 构造函数
	~BookManageSystem(); // 析构函数
	void Run(); //运行图书管理系统

private:
	// 数据成员
	// 文件
	fstream BookFile; // 书籍文件
	fstream IdIndexFile; // ISBN号索引文件
	fstream NameIndexFile; // 书名索引文件
	// 索引表
	SqList<IdIndex> IdIndexList; // ISBN号索引表, 升序
	SqList<NameIndex> NameIndexList; // 书名索引表, 升序

	//成员函数
	void InsertBook(); // 插入书籍
	void DeleteBook(); // 删除书籍
	void SearchBook(); // 查找书籍
	void SearchByTitle(); // 按书名查找
	void SearchByAuthor(); // 按作者查找
	bool BinSearchByTitle(char *name, SimpleLkList<int> BookList, int &mid); // 二分查找书名，得到链表，返回值为索引表中位置
	bool BinSearchById(char *id, int &mid); // 二分查找ISBN号，mid返回索引表中位置，bool返回是否找到
	
	// 未完成
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
	BookFile.open("bookdata.txt"); // 打开数书籍文件
	if (!BookFile.is_open())
	{
		cout << "书籍文件打开失败!\n" << endl;
		exit(1);
	}
	
	IdIndexFile.open("idindex.txt"); // 打开ISBN号索引文件
	if (!IdIndexFile.is_open())
	{
		cout << "主关键字索引文件打开失败!\n" << endl;
		exit(1);
	}

	NameIndexFile.open("nameindex.txt"); // 打开书名索引文件
	if (!NameIndexFile.is_open())
	{
		cout << "次关键字索引文件打开失败!\n" << endl;
		exit(1);
	}

	// 读取并建立索引表
	// 读取ISBN号索引表
	IdIndexFile.seekg(ios::beg); // 定位到文件开头
	if (IdIndexFile.peek() != EOF) 
	{ // 文件非空，建立主关键字索引表
		IdIndex TmpIdIndex;
		while (IdIndexFile.read((char*)&TmpIdIndex, sizeof(IdIndex)))
		{
			IdIndexList.AddTail(TmpIdIndex);
		}
	}
	// 读取书名索引表
	NameIndexFile.seekg(ios::beg); // 定位到文件开头
	if (NameIndexFile.peek() != EOF)
	{ // 文件非空，建立次关键字索引表
		NameIndex TmpNameIndex;
		while (NameIndexFile.read((char*)&TmpNameIndex, sizeof(NameIndex)))
		{
			NameIndexList.AddTail(TmpNameIndex);
		}
	}
	
};

// 析构函数
BookManageSystem::~BookManageSystem()
{
	if (BookFile.is_open())
		BookFile.close();
	if (IdIndexFile.is_open())
		IdIndexFile.close();
	if (NameIndexFile.is_open())
		NameIndexFile.close();
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

	// 查找书籍是否已经存在
	// 由于ISBN号唯一，可以直接在主表中查找（采用二分策略）
	int loc_id;
	if (BinSearchById(book.id, loc_id)) {
		cout << "该书籍已存在！\n" << endl;
		return;
	}
	else 
	{ // 书籍不存在, 将书籍写入文件尾部，并在索引表中插入书籍
		SimpleLkList<int> Bookoffset;
		int loc_name;
		BinSearchByTitle(book.name, Bookoffset, loc_name);
		IdIndex CurIdIndex;
		NameIndex CurNameIndex;

		strcpy(CurIdIndex.id, book.id);
		strcpy(CurNameIndex.name, book.name);
		BookFile.seekp(ios::end); // 定位到文件末尾
		CurIdIndex.offset = BookFile.tellp(); // 记录书籍在文件中的偏移量
		CurNameIndex.offsetList.Insert(0, CurIdIndex.offset);
		
		BookFile.write((char*)&book, sizeof(Book)); // 写入书籍
		IdIndexList.Insert(loc_id, CurIdIndex); // 插入主关键字索引表
		NameIndexList.Insert(loc_name, CurNameIndex); // 插入次关键字索引表
		cout << "插入书籍成功！\n" << endl;
	}

	return;
};

// 删除书籍
void BookManageSystem::DeleteBook()
{
	char id[14];
	cout << "请输入要删除的书籍的ISBN号：";
	cin >> id;
	
	// 查找书籍是否已经存在
	// 由于ISBN号唯一，可以直接在主表中查找（采用二分策略）
	int loc_id;
	if (!BinSearchById(id, loc_id)) {
		cout << "该书籍不存在！\n" << endl;
		return;
	}
	else
	{ // 书籍存在, 在索引表中删除书籍
		IdIndex CurIdIndex;
		NameIndex CurNameIndex;
		Book book;
		
		BookFile.seekg(IdIndexList[loc_id].offset, ios::beg);
		BookFile.read((char*)&book, sizeof(Book));

		IdIndexList.Delete(loc_id, CurIdIndex); // 删除主关键字索引表中的记录
		SimpleLkList<int> Bookoffset;
		int loc_name;
		BinSearchByTitle(book.name, Bookoffset, loc_name);
		NameIndexList.Delete(loc_name, CurNameIndex); // 删除次关键字索引表中的记录
		cout << "删除书籍成功！\n" << endl;
	}
	return;
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
		default:
			cout << "输入错误，请重新输入" << endl;
			break;
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
	int mid;
	if (!BinSearchByTitle(name, Bookoffset, mid)) {
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
		BookFile.seekg(CurOffset, ios::beg);
		BookFile.read((char*)&book, sizeof(Book));
		BookList.AddTail(book);
	}
	// 输出书籍信息
	DisplayBook(BookList);
	
	return;
};

// 二分查找书名，返回链表
bool BookManageSystem::BinSearchByTitle(char *name, SimpleLkList<int> Bookoffset, int &mid)
{
	if (NameIndexList.Length() == 0)
	{
		mid = 0;
		return 0;
	}
	// 在书名索引表中查找
	// 二分查找
	int low = 0, high = NameIndexList.Length() - 1;
	while (low <= high) {
		mid = (low + high) >> 1;
		if (strcmp(NameIndexList[mid].name, name) == 0) {
			// 找到书名
			Bookoffset = NameIndexList[mid].offsetList;
			return 1;
		}
		else if (strcmp(NameIndexList[mid].name, name) < 0) {
			low = mid + 1;
		}
		else {
			high = mid;
		}
	}
	return 0;
};

// 按作者查找
void BookManageSystem::SearchByAuthor()
{ // 由于没有作者索引，只能遍历所有书籍文件
	char author[128];
	cout << "请输入要查找的作者：";
	cin >> author;
	BookFile.clear(); // 清除文件流状态
	BookFile.seekg(0, ios::beg); // 定位到文件开头
	Book book;
	SimpleLkList<Book> bookList;
	bool flag = 0;
	do {
		BookFile.read((char*) &book, sizeof(Book));
		if (strcmp(book.author, author) == 0)
		{
			flag = 1;
			bookList.AddTail(book);
		}
	} while(!BookFile.eof());
	if (flag) {
		cout << "查找到的书籍信息如下：" << endl;
		DisplayBook(bookList);
	}
	else {
		cout << "未找到相关书籍!" << endl;
	}
	
	return;
};

// 二分查找ISBN号，返回索引表中位置，若未找到返回-1
bool BookManageSystem::BinSearchById(char *id, int &mid)
{
	if (IdIndexList.Length() == 0)
	{
		mid = 0;
		return 0;
	}
	int low = 0, high = IdIndexList.Length() - 1;
	while (low <= high) {
		mid = (low + high) >> 1;
		if (strcmp(IdIndexList[mid].id, id) == 0) {
			// 找到书籍
			return 1;
		}
		else if (strcmp(IdIndexList[mid].id, id) < 0) {
			low = mid + 1;
		}
		else {
			high = mid;
		}
	}
	return 0;
};

// 运行图书管理系统
void BookManageSystem::Run()
{
	int choice;
	while (true)
	{
		cout << "请选择操作：1.插入书籍 2.删除书籍 3.更新记录 4.查找书籍 5.按作者名排序 6.退出\n";
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
			cout << "成功退出系统!\n";
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