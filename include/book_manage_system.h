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
	// 索引表
	SqList<IdIndex> IdIndexList; // ISBN号索引表, 升序
	SqList<NameIndex> NameIndexList; // 书名索引表, 升序

	//成员函数
	void InsertBook(); // 插入书籍
	void DeleteBook(); // 删除书籍
	void SearchBook(); // 查找书籍
	void SearchByTitle(); // 按书名查找
	void SearchByAuthor(); // 按作者查找
	bool BinSearchByTitle(char *name, SimpleLkList<int> &BookList, int &mid); // 二分查找书名，得到链表，返回值为索引表中位置
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
	BookFile.open("bookdata.data"); // 打开数书籍文件
	if (!BookFile.is_open())
	{
		cerr << "书籍文件打开失败!\n" << endl;
		exit(1);
	}

	cout << "索引表初始化" << endl;

	// 获取书籍数量
	BookFile.clear(); // 清除文件流状态
	BookFile.seekg(0, ios::end); // 定位到文件末尾
	int FileLen = BookFile.tellg(); // 文件长度
	int BookNums = FileLen / sizeof(Book); // 书籍数量

	// 读取书籍信息，建立索引表
	BookFile.clear(); // 清除文件流状态
	BookFile.seekg(0, ios::beg); // 定位到文件开头
	Book book;
	for (int i = 0; i < BookNums; i++)
	{
		// 建立索引表
		IdIndex CurIdIndex;
		CurIdIndex.offset = BookFile.tellg(); // 记录书籍在文件中的偏移量
		
		// 读取书籍信息
		BookFile.read((char*)&book, sizeof(Book));
		cout << book.id << " " << book.name << " " << book.author << " " << book.price << " " << book.buyDate.year << " " << book.buyDate.month << " " << book.buyDate.day << endl;

		// 插入主关键字索引表
		int idloc;
		if (BinSearchById(book.id, idloc))
		{ // 书籍已存在
			cout << "书籍已存在" << endl;
			continue;
		}
		else 
		{ // 书籍不存在
			strcpy(CurIdIndex.id, book.id);
			IdIndexList.Insert(idloc, CurIdIndex); // 插入主关键字索引表	
		}

		// 插入次关键字索引表
		SimpleLkList<int> Bookoffset;
		int loc_name;
		if (BinSearchByTitle(book.name, Bookoffset, loc_name))
		{ // 书名存在
			cout << "书名存在" << endl;
			cout << "在次索引中位置" << loc_name << endl;
			NameIndex tmp;
			NameIndexList.GetElem(loc_name, tmp);
			tmp.offsetList.AddTail(CurIdIndex.offset);

			// 显示链表元素
			//cout << "链表元素个数：" << tmp.offsetList.Length() << endl;

			NameIndexList.SetElem(loc_name, tmp);

			//NameIndexList.GetElem(loc_name, tmp);
			// 显示链表元素
			//cout << "修改后链表元素个数：" << tmp.offsetList.Length() << endl;
		}
		else
		{ // 书名不存在
			cout << "书名不存在" << endl;
			NameIndex CurNameIndex;
			strcpy(CurNameIndex.name, book.name);
			CurNameIndex.offsetList.Insert(0, CurIdIndex.offset);
			cout << "插入新书籍：" << CurNameIndex.name << " " << CurNameIndex.offsetList.Length() << endl;
			cout << "在次索引中插入位置" << loc_name << endl;
			NameIndexList.Insert(loc_name, CurNameIndex);
		}
	}

	cout << "索引表初始化完成" << endl;
};

// 析构函数
BookManageSystem::~BookManageSystem()
{
	if (BookFile.is_open())
	{
		// 更新书籍文件 （剔除已删除文件）
		BookFile.close();
	}
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
		
		IdIndex CurIdIndex;
		strcpy(CurIdIndex.id, book.id);
		BookFile.seekp(0, ios::end); // 定位到文件末尾
		CurIdIndex.offset = BookFile.tellp(); // 记录书籍在文件中的偏移量
		//cout << "原偏移量 " << CurIdIndex.offset << endl;

		BookFile.write((char*)&book, sizeof(Book)); // 写入书籍
		
		//int tmpoffset = BookFile.tellp();
		//cout << "插入后偏移量 " << tmpoffset << endl;

		//BookFile.seekg(0, ios::end); // 定位到文件末尾
		//tmpoffset = BookFile.tellp();
		//cout << "插入后文件尾部偏移量 " << tmpoffset << endl;
		
		IdIndexList.Insert(loc_id, CurIdIndex); // 插入主关键字索引表

		// 插入次关键字索引表
		SimpleLkList<int> Bookoffset;
		int loc_name;
		if (BinSearchByTitle(book.name, Bookoffset, loc_name))
		{ // 书名存在
			cout << "书名存在，插入" << endl;
			NameIndex tmp;
			NameIndexList.GetElem(loc_name, tmp);
			
			//cout << tmp.offsetList.Length() << endl;
			/*
			cout << "原来偏移量" << endl;
			for (int k = 0; k < tmp.offsetList.Length(); k++)
			{
				int ans;
				tmp.offsetList.GetElem(k, ans);
				cout << ans << endl;
			}
			*/

			tmp.offsetList.AddTail(CurIdIndex.offset);
			NameIndexList.SetElem(loc_name, tmp);
			
			//cout << tmp.offsetList.Length() << endl;
			/*
			cout << "插入后偏移量" << endl;
			for (int k = 0; k < tmp.offsetList.Length(); k++)
			{
				int ans;
				tmp.offsetList.GetElem(k, ans);
				cout << ans << endl;
			}
			*/
		}
		else
		{ // 书名不存在
			cout << "书名不存在" << endl;
			NameIndex CurNameIndex;
			strcpy(CurNameIndex.name, book.name);
			CurNameIndex.offsetList.Insert(0, CurIdIndex.offset);
			NameIndexList.Insert(loc_name, CurNameIndex);
		}
		cout << "插入书籍成功!" << endl;
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
		
		IdIndex tmp;
		IdIndexList.GetElem(loc_id, tmp);
		BookFile.seekg(tmp.offset, ios::beg);
		BookFile.read((char*)&book, sizeof(Book));

		IdIndexList.Delete(loc_id, CurIdIndex); // 删除主关键字索引表中的记录
		SimpleLkList<int> Bookoffset;
		int loc_name;
		BinSearchByTitle(book.name, Bookoffset, loc_name);
		NameIndexList.Delete(loc_name, CurNameIndex); // 删除次关键字索引表中的记录
		cout << "删除书籍成功!" << endl;
	}
	return;
};

// 查找书籍
void BookManageSystem::SearchBook()
{
	cout << "进入查找函数" << endl;
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
	cout << "退出查找函数" << endl;
};

// 按书名查找
void BookManageSystem::SearchByTitle()
{ // 由于有书名索引，可以直接在索引表中查找，并进一步通过偏移量找到书籍信息
	cout << "进入按书名查找函数" << endl;
	
	char name[64];
	cout << "请输入要查找的书名：";
	cin >> name;

	// 在书名索引表中查找
	// 二分查找
	SimpleLkList<int> Bookoffset;
	int mid;
	if (!BinSearchByTitle(name, Bookoffset, mid)) {
		cout << "未找到相关书籍!" << endl;
		return;
	}

	// 根据位移量，找到书籍信息
	cout << "查找到的书籍信息如下：" << endl;
	SimpleLkList<Book> BookList;
	for (int i = 0; i < Bookoffset.Length(); i++) {
		Book book;
		int CurOffset;
		Bookoffset.GetElem(i, CurOffset);
		BookFile.seekg(CurOffset, ios::beg);
		BookFile.read((char*)&book, sizeof(Book));

		//cout << "偏移量" << CurOffset << endl;
		//cout << "查找的书籍信息" << book.id << " " << book.name << " " << book.author << endl;
		
		BookList.AddTail(book);
	}
	// 输出书籍信息
	DisplayBook(BookList);

	cout << "退出按书名查找函数" << endl;
	
	return;
};

// 二分查找书名，返回链表
bool BookManageSystem::BinSearchByTitle(char *name, SimpleLkList<int> &Bookoffset, int &mid)
{
	cout << "进入二分书名查找函数" << endl;

	if (NameIndexList.Length() == 0)
	{
		mid = 0;
		cout << "退出二分书名查找函数" << endl;
		return 0;
	}
	// 在书名索引表中查找
	// 二分查找
	int low = 0, high = NameIndexList.Length();
	while (low < high) {
		mid = (low + high) >> 1;
		NameIndex tmp;
		NameIndexList.GetElem(mid, tmp);
		if (strcmp(tmp.name, name) == 0)
		{ // 找到书名
			cout << "赋值前元素个数：" << tmp.offsetList.Length() << endl;
			Bookoffset = tmp.offsetList; // 返回链表
			cout << "赋值后元素个数：" << Bookoffset.Length() << endl;
			cout << "退出二分书名查找函数" << endl;
			return 1;
		}
		else if (strcmp(tmp.name, name) < 0) {
			low = mid + 1;
		}
		else {
			high = mid;
		}
	}

	mid = low;
	cout << "退出二分书名查找函数" << endl;

	return 0;
};

// 按作者查找
void BookManageSystem::SearchByAuthor()
{ // 由于没有作者索引，只能遍历所有书籍文件
	char author[128];
	cout << "请输入要查找的作者：";
	cin >> author;

	BookFile.clear(); // 清除文件流状态
	BookFile.seekg(0, ios::end); // 定位到文件末尾
	int FileLen = BookFile.tellg(); // 文件长度
	int BookNums = FileLen / sizeof(Book); // 书籍数量
	cout << "书籍数量" << BookNums << endl;

	BookFile.clear(); // 清除文件流状态
	BookFile.seekg(0, ios::beg); // 定位到文件开头
	Book book;
	SimpleLkList<Book> bookList;
	bool flag = 0;
	while (BookNums--)
	{
		BookFile.read((char*) &book, sizeof(Book));
		if (strcmp(book.author, author) == 0)
		{
			flag = 1;
			bookList.AddTail(book);
		}
	}
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
	int low = 0, high = IdIndexList.Length();
	while (low < high) {
		mid = (low + high) >> 1;
		IdIndex tmp;
		IdIndexList.GetElem(mid, tmp);
		if (strcmp(tmp.id, id) == 0) {
			// 找到书籍
			return 1;
		}
		else if (strcmp(tmp.id, id) < 0) {
			low = mid + 1;
		}
		else {
			high = mid;
		}
	}
	mid = low;
	return 0;
};

// 运行图书管理系统
void BookManageSystem::Run()
{
	//cout << sizeof(Book) << endl;
	//cout << sizeof(int) << endl;
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
	cout << setw(numspace[3]) << std::fixed << std::setprecision(2) << book.price << "\t";
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