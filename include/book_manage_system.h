#if !define	BOOK_MANAGE_SYSTEM_H
#define BOOK_MANAGE_SYSTEM_H

#include<iostream>
#include<fstream>
#include<iomanip>
#include "SimpleLkList.h"
#include "SqList.h"
#define max(a, b) ((a)>(b)?(a):(b))
using namespace std;

// һЩ����ṹ��Ķ���:
// ���ڽṹ
struct Date {
	int year, month, day;
};

// �鼮�ṹ
struct Book {
	char id[14]; //ISBN��
	char name[128]; //����
	char author[128]; //����
	float price; //�۸�
	Date buyDate; //��������
	bool isDeleted; //�Ƿ�ɾ��
};

//�����ṹ:
// ���������
struct IdIndex { //���ؼ���
	char id[14]; //ISBN��
	int offset; //�鼮��¼���ļ��е�ƫ����
};

// ����������
struct NameIndex { //�ιؼ���
	char name[128]; //����
	SimpleLkList<int> offsetList; //�鼮��¼���ļ��е�ƫ��������
};

// �鼮����ϵͳ��
class BookManageSystem
{
public:
	BookManageSystem(); // ���캯��
	~BookManageSystem(); // ��������
	void Run(); //����ͼ�����ϵͳ

private:
	//���ݳ�Ա
	fstream bookFile; // �鼮�ļ�
	SqList<IdIndex> idIndexList; // ISBN��������, ����
	SqList<NameIndex> nameIndexList; // ����������, ����

	//��Ա����
	void InsertBook(); // �����鼮
	bool InsertBookAux(Book &book, int offset); // �����鼮��������
	
	void DeleteBook(); // ɾ���鼮
	void DeleteBookAux(char *id); // ɾ���鼮��������
	
	void SearchBook(); // �����鼮
	void SearchByTitle(); // ����������
	void SearchByAuthor(); // �����߲���
	bool BinSearchByTitle(char *name, SimpleLkList<int> &BookList); // ���ֲ�����������������
	
	void UpdateBook(); // �����鼮
	
	void SortByAuthor(); // ������������

	void DisplayBook(Book &book); // ��ʾ�鼮��Ϣ
	void DisplayBook(SimpleLkList<Book> &bookList); // ��ʾ�鼮�б�
	void DisplayBookAux(Book &book, int numspace[]); // ��ʾ�鼮��Ϣ��������
};

// ���캯��
BookManageSystem::BookManageSystem()
{
	// ���ļ�
	bookFile.open("bookdata.txt");
	if (!bookFile.is_open())
	{
		cout << "�ļ���ʧ��" << endl;
		exit(1);
	}

	// ��ȡ�ļ��е��鼮��Ϣ
	// �ж��ļ��Ƿ�Ϊ��
	bookFile.seekg(0, ios::end);
	if (bookFile.tellg() != 0)
	{ // �ļ���Ϊ��
		bookFile.seekg(0, ios::beg);
		while (!bookFile.eof())
		{
			Book book;
			IdIndex CurIdIndex;
			NameIndex CurNameIndex;
			CurIdIndex.offset = bookFile.tellg(); // ��¼�鼮���ļ��е�ƫ����
			bookFile.read((char*)&book, sizeof(Book)); // ��ȡ�鼮��Ϣ
			// �����鼮
			
			// �������в����鼮
			int pos;
			// �����ҵ�����λ��
			strcpy(CurIdIndex.id, book.id);
			idIndexList.Insert(pos, CurIdIndex);
			
			// �ڴα��в����鼮
		}
	}
};

// ��������
BookManageSystem::~BookManageSystem()
{
	if (bookFile.is_open())
		bookFile.close();
};

// �����鼮
void BookManageSystem::InsertBook()
{
	Book book;
	cout << "�������鼮��ISBN�ţ�";
	cin >> book.id;
	cout << "�������鼮��������";
	cin >> book.name;
	cout << "�������鼮�����ߣ�";
	cin >> book.author;
	cout << "�������鼮�ļ۸�";
	cin >> book.price;
	cout << "�������鼮�Ĺ�������(year month day���Կո����)��";
	cin >> book.buyDate.year >> book.buyDate.month >> book.buyDate.day;

	//�����鼮�Ƿ��Ѿ�����

	// ���ļ���д���鼮
	
	// �����鼮

	
};

// �����鼮��������
bool BookManageSystem::InsertBookAux(Book &book, int offset)
{
	// �������в����鼮
	// �ڴα��в����鼮
};

// ɾ���鼮
void BookManageSystem::DeleteBook()
{
	char id[14];
	cout << "������Ҫɾ�����鼮��ISBN�ţ�";
	cin >> id;
	DeleteBookAux(id);
};

// ɾ���鼮��������
void BookManageSystem::DeleteBookAux(char *id)
{
	// �������в����鼮

	// ���ļ���ɾ���鼮

	// ��������ɾ���鼮
};

// �����鼮
void BookManageSystem::SearchBook()
{
	do {
		cout << "1.���������� 2.�����߲��� 3.�˳�����\n";
		int choice;
		cout << "������ѡ��";
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

// ����������
void BookManageSystem::SearchByTitle()
{ // ��������������������ֱ�����������в��ң�����һ��ͨ��ƫ�����ҵ��鼮��Ϣ
	char name[128];
	cout << "������Ҫ���ҵ�������";
	cin >> name;

	// �������������в���
	// ���ֲ���
	SimpleLkList<int> Bookoffset;
	if (BinSearchByTitle(name, Bookoffset) == 0) {
		cout << "δ�ҵ�����鼮!\n" << endl;
		return;
	}

	// ����λ�������ҵ��鼮��Ϣ
	cout << "���ҵ����鼮��Ϣ���£�\n" << endl;
	SimpleLkList<Book> BookList;
	for (int i = 0; i < Bookoffset.Length(); i++) {
		Book book;
		int CurOffset;
		Bookoffset.GetElem(i, CurOffset);
		bookFile.seekg(CurOffset, ios::beg);
		bookFile.read((char*)&book, sizeof(Book));
		BookList.AddTail(book);
	}
	// ����鼮��Ϣ
	DisplayBook(BookList);
	
	return;
};

// ���ֲ�����������������
bool BookManageSystem::BinSearchByTitle(char *name, SimpleLkList<int> &Bookoffset)
{
	// �������������в���
	// ���ֲ���
	int low = 0, high = nameIndexList.Length() - 1;
	while (low <= high) {
		int mid = (low + high) >> 1;
		if (strcmp(nameIndexList[mid].name, name) == 0) {
			// �ҵ�����
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

// �����߲���
void BookManageSystem::SearchByAuthor()
{ // ����û������������ֻ�ܱ��������鼮�ļ�
	char author[128];
	cout << "������Ҫ���ҵ����ߣ�";
	cin >> author;
	bookFile.clear(); // ����ļ���״̬
	bookFile.seekg(0, ios::beg); // ��λ���ļ���ͷ
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
		cout << "���ҵ����鼮��Ϣ���£�\n" << endl;
		DisplayBook(bookList);
	}
	else {
		cout << "δ�ҵ�����鼮!\n" << endl;
	}
	
	return;
};

// ����ͼ�����ϵͳ
void BookManageSystem::Run()
{
	int choice;
	while (true)
	{
		cout << "��ѡ�������1.�����鼮 2.ɾ���鼮 3.���¼�¼ 4.�����鼮 5.������������ 6.�˳�\n" << endl;
		cout << "������ѡ��";
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
			cout << "�����������������" << endl;
			break;
		}
	}
};

// ��ʾ�鼮��Ϣ
void BookManageSystem::DisplayBook(Book &book)
{
	// ͳ�Ƹ����ֶε���󳤶�
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
	
	// �����ͷ
	cout << setw(numspace[0]) << "ISBN��" << "\t";
	cout << setw(numspace[1]) << "����" << "\t";
	cout << setw(numspace[2]) << "����" << "\t";
	cout << setw(numspace[3]) << "�۸�" << "\t";
	cout << "��������" << endl;
	
	// ����鼮��Ϣ
	DisplayBookAux(book, numspace);
};

// ��ʾ�鼮�б�
void BookManageSystem::DisplayBook(SimpleLkList<Book> &bookList)
{
	// ͳ�Ƹ����ֶε���󳤶�
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

	// �����ͷ
	cout << setw(numspace[0]) << "ISBN��" << "\t";
	cout << setw(numspace[1]) << "����" << "\t";
	cout << setw(numspace[2]) << "����" << "\t";
	cout << setw(numspace[3]) << "�۸�" << "\t";
	cout << "��������" << endl;
	
	// ����鼮��Ϣ
	for (int i = 0; i < bookList.Length(); i++)
	{
		Book tmp;
		bookList.GetElem(i, tmp);
		DisplayBookAux(tmp, numspace);
	}
};

// ��ʾ�鼮��Ϣ��������
void BookManageSystem::DisplayBookAux(Book &book, int numspace[])
{
	cout << setw(numspace[0]) << book.id << "\t";
	cout << setw(numspace[1]) << book.name << "\t";
	cout << setw(numspace[2]) << book.author << "\t";
	cout << setw(numspace[3]) << std::setprecision(2) << book.price << "\t";
	cout << book.buyDate.year << " " << book.buyDate.month << " " << book.buyDate.day << endl;
};

// �����鼮
void BookManageSystem::UpdateBook()
{

};

// ������������
void BookManageSystem::SortByAuthor()
{

};
#endif