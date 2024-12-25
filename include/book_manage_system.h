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

// �鼮����ϵͳ��
class BookManageSystem
{
public:
	BookManageSystem(); // ���캯��
	~BookManageSystem(); // ��������
	void Run(); //����ͼ�����ϵͳ

private:
	// ���ݳ�Ա
	// �ļ�
	fstream BookFile; // �鼮�ļ�
	// ������
	SqList<IdIndex> IdIndexList; // ISBN��������, ����
	SqList<NameIndex> NameIndexList; // ����������, ����

	//��Ա����
	void InsertBook(); // �����鼮
	void DeleteBook(); // ɾ���鼮
	void SearchBook(); // �����鼮
	void SearchByTitle(); // ����������
	void SearchByAuthor(); // �����߲���
	bool BinSearchByTitle(char *name, SimpleLkList<int> &BookList, int &mid); // ���ֲ����������õ���������ֵΪ��������λ��
	bool BinSearchById(char *id, int &mid); // ���ֲ���ISBN�ţ�mid������������λ�ã�bool�����Ƿ��ҵ�
	
	// δ���
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
	BookFile.open("bookdata.data"); // �����鼮�ļ�
	if (!BookFile.is_open())
	{
		cerr << "�鼮�ļ���ʧ��!\n" << endl;
		exit(1);
	}

	cout << "�������ʼ��" << endl;

	// ��ȡ�鼮����
	BookFile.clear(); // ����ļ���״̬
	BookFile.seekg(0, ios::end); // ��λ���ļ�ĩβ
	int FileLen = BookFile.tellg(); // �ļ�����
	int BookNums = FileLen / sizeof(Book); // �鼮����

	// ��ȡ�鼮��Ϣ������������
	BookFile.clear(); // ����ļ���״̬
	BookFile.seekg(0, ios::beg); // ��λ���ļ���ͷ
	Book book;
	for (int i = 0; i < BookNums; i++)
	{
		// ����������
		IdIndex CurIdIndex;
		CurIdIndex.offset = BookFile.tellg(); // ��¼�鼮���ļ��е�ƫ����
		
		// ��ȡ�鼮��Ϣ
		BookFile.read((char*)&book, sizeof(Book));
		cout << book.id << " " << book.name << " " << book.author << " " << book.price << " " << book.buyDate.year << " " << book.buyDate.month << " " << book.buyDate.day << endl;

		// �������ؼ���������
		int idloc;
		if (BinSearchById(book.id, idloc))
		{ // �鼮�Ѵ���
			cout << "�鼮�Ѵ���" << endl;
			continue;
		}
		else 
		{ // �鼮������
			strcpy(CurIdIndex.id, book.id);
			IdIndexList.Insert(idloc, CurIdIndex); // �������ؼ���������	
		}

		// ����ιؼ���������
		SimpleLkList<int> Bookoffset;
		int loc_name;
		if (BinSearchByTitle(book.name, Bookoffset, loc_name))
		{ // ��������
			cout << "��������" << endl;
			cout << "�ڴ�������λ��" << loc_name << endl;
			NameIndex tmp;
			NameIndexList.GetElem(loc_name, tmp);
			tmp.offsetList.AddTail(CurIdIndex.offset);

			// ��ʾ����Ԫ��
			//cout << "����Ԫ�ظ�����" << tmp.offsetList.Length() << endl;

			NameIndexList.SetElem(loc_name, tmp);

			//NameIndexList.GetElem(loc_name, tmp);
			// ��ʾ����Ԫ��
			//cout << "�޸ĺ�����Ԫ�ظ�����" << tmp.offsetList.Length() << endl;
		}
		else
		{ // ����������
			cout << "����������" << endl;
			NameIndex CurNameIndex;
			strcpy(CurNameIndex.name, book.name);
			CurNameIndex.offsetList.Insert(0, CurIdIndex.offset);
			cout << "�������鼮��" << CurNameIndex.name << " " << CurNameIndex.offsetList.Length() << endl;
			cout << "�ڴ������в���λ��" << loc_name << endl;
			NameIndexList.Insert(loc_name, CurNameIndex);
		}
	}

	cout << "�������ʼ�����" << endl;
};

// ��������
BookManageSystem::~BookManageSystem()
{
	if (BookFile.is_open())
	{
		// �����鼮�ļ� ���޳���ɾ���ļ���
		BookFile.close();
	}
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

	// �����鼮�Ƿ��Ѿ�����
	// ����ISBN��Ψһ������ֱ���������в��ң����ö��ֲ��ԣ�
	int loc_id;
	if (BinSearchById(book.id, loc_id)) {
		cout << "���鼮�Ѵ��ڣ�\n" << endl;
		return;
	}
	else 
	{ // �鼮������, ���鼮д���ļ�β���������������в����鼮
		
		IdIndex CurIdIndex;
		strcpy(CurIdIndex.id, book.id);
		BookFile.seekp(0, ios::end); // ��λ���ļ�ĩβ
		CurIdIndex.offset = BookFile.tellp(); // ��¼�鼮���ļ��е�ƫ����
		//cout << "ԭƫ���� " << CurIdIndex.offset << endl;

		BookFile.write((char*)&book, sizeof(Book)); // д���鼮
		
		//int tmpoffset = BookFile.tellp();
		//cout << "�����ƫ���� " << tmpoffset << endl;

		//BookFile.seekg(0, ios::end); // ��λ���ļ�ĩβ
		//tmpoffset = BookFile.tellp();
		//cout << "������ļ�β��ƫ���� " << tmpoffset << endl;
		
		IdIndexList.Insert(loc_id, CurIdIndex); // �������ؼ���������

		// ����ιؼ���������
		SimpleLkList<int> Bookoffset;
		int loc_name;
		if (BinSearchByTitle(book.name, Bookoffset, loc_name))
		{ // ��������
			cout << "�������ڣ�����" << endl;
			NameIndex tmp;
			NameIndexList.GetElem(loc_name, tmp);
			
			//cout << tmp.offsetList.Length() << endl;
			/*
			cout << "ԭ��ƫ����" << endl;
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
			cout << "�����ƫ����" << endl;
			for (int k = 0; k < tmp.offsetList.Length(); k++)
			{
				int ans;
				tmp.offsetList.GetElem(k, ans);
				cout << ans << endl;
			}
			*/
		}
		else
		{ // ����������
			cout << "����������" << endl;
			NameIndex CurNameIndex;
			strcpy(CurNameIndex.name, book.name);
			CurNameIndex.offsetList.Insert(0, CurIdIndex.offset);
			NameIndexList.Insert(loc_name, CurNameIndex);
		}
		cout << "�����鼮�ɹ�!" << endl;
	}

	return;
};

// ɾ���鼮
void BookManageSystem::DeleteBook()
{
	char id[14];
	cout << "������Ҫɾ�����鼮��ISBN�ţ�";
	cin >> id;
	
	// �����鼮�Ƿ��Ѿ�����
	// ����ISBN��Ψһ������ֱ���������в��ң����ö��ֲ��ԣ�
	int loc_id;
	if (!BinSearchById(id, loc_id)) {
		cout << "���鼮�����ڣ�\n" << endl;
		return;
	}
	else
	{ // �鼮����, ����������ɾ���鼮
		IdIndex CurIdIndex;
		NameIndex CurNameIndex;
		Book book;
		
		IdIndex tmp;
		IdIndexList.GetElem(loc_id, tmp);
		BookFile.seekg(tmp.offset, ios::beg);
		BookFile.read((char*)&book, sizeof(Book));

		IdIndexList.Delete(loc_id, CurIdIndex); // ɾ�����ؼ����������еļ�¼
		SimpleLkList<int> Bookoffset;
		int loc_name;
		BinSearchByTitle(book.name, Bookoffset, loc_name);
		NameIndexList.Delete(loc_name, CurNameIndex); // ɾ���ιؼ����������еļ�¼
		cout << "ɾ���鼮�ɹ�!" << endl;
	}
	return;
};

// �����鼮
void BookManageSystem::SearchBook()
{
	cout << "������Һ���" << endl;
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
		default:
			cout << "�����������������" << endl;
			break;
		}
	} while (true);
	cout << "�˳����Һ���" << endl;
};

// ����������
void BookManageSystem::SearchByTitle()
{ // ��������������������ֱ�����������в��ң�����һ��ͨ��ƫ�����ҵ��鼮��Ϣ
	cout << "���밴�������Һ���" << endl;
	
	char name[64];
	cout << "������Ҫ���ҵ�������";
	cin >> name;

	// �������������в���
	// ���ֲ���
	SimpleLkList<int> Bookoffset;
	int mid;
	if (!BinSearchByTitle(name, Bookoffset, mid)) {
		cout << "δ�ҵ�����鼮!" << endl;
		return;
	}

	// ����λ�������ҵ��鼮��Ϣ
	cout << "���ҵ����鼮��Ϣ���£�" << endl;
	SimpleLkList<Book> BookList;
	for (int i = 0; i < Bookoffset.Length(); i++) {
		Book book;
		int CurOffset;
		Bookoffset.GetElem(i, CurOffset);
		BookFile.seekg(CurOffset, ios::beg);
		BookFile.read((char*)&book, sizeof(Book));

		//cout << "ƫ����" << CurOffset << endl;
		//cout << "���ҵ��鼮��Ϣ" << book.id << " " << book.name << " " << book.author << endl;
		
		BookList.AddTail(book);
	}
	// ����鼮��Ϣ
	DisplayBook(BookList);

	cout << "�˳����������Һ���" << endl;
	
	return;
};

// ���ֲ�����������������
bool BookManageSystem::BinSearchByTitle(char *name, SimpleLkList<int> &Bookoffset, int &mid)
{
	cout << "��������������Һ���" << endl;

	if (NameIndexList.Length() == 0)
	{
		mid = 0;
		cout << "�˳������������Һ���" << endl;
		return 0;
	}
	// �������������в���
	// ���ֲ���
	int low = 0, high = NameIndexList.Length();
	while (low < high) {
		mid = (low + high) >> 1;
		NameIndex tmp;
		NameIndexList.GetElem(mid, tmp);
		if (strcmp(tmp.name, name) == 0)
		{ // �ҵ�����
			cout << "��ֵǰԪ�ظ�����" << tmp.offsetList.Length() << endl;
			Bookoffset = tmp.offsetList; // ��������
			cout << "��ֵ��Ԫ�ظ�����" << Bookoffset.Length() << endl;
			cout << "�˳������������Һ���" << endl;
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
	cout << "�˳������������Һ���" << endl;

	return 0;
};

// �����߲���
void BookManageSystem::SearchByAuthor()
{ // ����û������������ֻ�ܱ��������鼮�ļ�
	char author[128];
	cout << "������Ҫ���ҵ����ߣ�";
	cin >> author;

	BookFile.clear(); // ����ļ���״̬
	BookFile.seekg(0, ios::end); // ��λ���ļ�ĩβ
	int FileLen = BookFile.tellg(); // �ļ�����
	int BookNums = FileLen / sizeof(Book); // �鼮����
	cout << "�鼮����" << BookNums << endl;

	BookFile.clear(); // ����ļ���״̬
	BookFile.seekg(0, ios::beg); // ��λ���ļ���ͷ
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
		cout << "���ҵ����鼮��Ϣ���£�" << endl;
		DisplayBook(bookList);
	}
	else {
		cout << "δ�ҵ�����鼮!" << endl;
	}
	
	return;
};

// ���ֲ���ISBN�ţ�������������λ�ã���δ�ҵ�����-1
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
			// �ҵ��鼮
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

// ����ͼ�����ϵͳ
void BookManageSystem::Run()
{
	//cout << sizeof(Book) << endl;
	//cout << sizeof(int) << endl;
	int choice;
	while (true)
	{
		cout << "��ѡ�������1.�����鼮 2.ɾ���鼮 3.���¼�¼ 4.�����鼮 5.������������ 6.�˳�\n";
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
			cout << "�ɹ��˳�ϵͳ!\n";
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
	cout << setw(numspace[3]) << std::fixed << std::setprecision(2) << book.price << "\t";
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