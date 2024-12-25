#if !define	BOOK_MANAGE_SYSTEM_H
#define BOOK_MANAGE_SYSTEM_H

#include<iostream>
#include<fstream>
#include<iomanip>
#include<limits>
#include "SimpleLkList.h"
#include "SqList.h"
#include "data_struct.h"
#define MY_MAX(a, b) ((a)>(b)?(a):(b))
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
	void init(); // ��ʼ������
	void save(); // ���溯��
	void test(); // ���Ժ���
	void show();

	void InsertBook(); // �����鼮
	bool InsertBookAux(Book &book); // �����鼮��������

	void DeleteBook(); // ɾ���鼮
	bool DeleteBookAux(char *id); // ɾ���鼮��������

	void SearchBook(); // �����鼮
	void SearchByTitle(); // ����������
	void SearchByAuthor(); // �����߲���
	bool BinSearchByTitle(char *name, SimpleLkList<int> &BookList, int &mid); // ���ֲ����������õ���������ֵΪ��������λ��
	bool BinSearchById(char *id, int &mid); // ���ֲ���ISBN�ţ�mid������������λ�ã�bool�����Ƿ��ҵ�
	
	void UpdateBook(); // �����鼮
	void SortByAuthor(); // ������������

	void DisplayBook(Book &book); // ��ʾ�鼮��Ϣ
	void DisplayBook(SimpleLkList<Book> &bookList); // ��ʾ�鼮�б�
	void DisplayBookAux(Book &book, int numspace[]); // ��ʾ�鼮��Ϣ��������
};

// ���캯��
BookManageSystem::BookManageSystem()
{
	init(); // ��ʼ��
};

// ��������
BookManageSystem::~BookManageSystem()
{
	save(); // ����
};

void BookManageSystem::show()
{
	cout << "�ιؼ���������" << endl;
	for (int i = 0; i < NameIndexList.Length(); i++)
	{
		NameIndex tmp;
		NameIndexList.GetElem(i, tmp);
		cout << "������" << tmp.name << "  λ�ã�";
		//cout << tmp.offsetList.Length() << endl;
		for (int j = 0; j < tmp.offsetList.Length(); j++)
		{
			int CurOffset;
			tmp.offsetList.GetElem(j, CurOffset);
			cout << CurOffset << " ";
		}
		cout << endl;
	}
};

void BookManageSystem::init()
{
	cout << "�������������Ե�......" << endl;
	// ���ļ�
	BookFile.open("bookdata.data"); // �����鼮�ļ�
	if (!BookFile.is_open())
	{
		cerr << "�鼮�ļ���ʧ��!\n" << endl;
		exit(1);
	}

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
		cout << "���ڲ���:" << book.name << endl;

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
			NameIndex tmp;
			NameIndexList.GetElem(loc_name, tmp); // ��ȡԭ������������
			tmp.offsetList.AddTail(CurIdIndex.offset);// �����µ�ƫ����
			NameIndexList.SetElem(loc_name, tmp);// ������������
			//Bookoffset.AddTail(CurIdIndex.offset); // �������ò���޷���ȷ�޸�
		}
		else
		{ // ����������
			NameIndex CurNameIndex;
			strcpy(CurNameIndex.name, book.name);
			CurNameIndex.offsetList.AddTail(CurIdIndex.offset);
			NameIndexList.Insert(loc_name, CurNameIndex);
		}

		show();
	}

	cout << "�������ʼ�����" << endl;
};

void BookManageSystem::save()
{
	if (BookFile.is_open())
	{
		// �����鼮�ļ� ���޳���ɾ���ļ���
		BookFile.clear();
		BookFile.seekp(0, ios::beg); // ��λ���ļ���ͷ
		int BookNums = IdIndexList.Length();
		SimpleLkList<Book> BookList;
		for (int i = 0; i < BookNums; i++)
		{
			IdIndex tmp;
			IdIndexList.GetElem(i, tmp);
			BookFile.seekg(tmp.offset, ios::beg);
			Book book;
			BookFile.read((char*)&book, sizeof(Book));
			if (!book.IsDeleted)
			{ // δɾ��
				BookList.AddTail(book);
			}
		}
		BookFile.close();
		// ����д���ļ�
		ofstream ofs("bookdata.data", std::ios::out | std::ios::trunc);
		for (int i = 0; i < BookList.Length(); i++)
		{
			Book tmp;
			BookList.GetElem(i, tmp);
			ofs.write((char*)&tmp, sizeof(Book));
		}
		ofs.close();
	}
};

bool BookManageSystem::InsertBookAux(Book& book)
{ // �鼮���ڷ���0���鼮����ɹ�����1
	// �����鼮�Ƿ��Ѿ�����
	// ����ISBN��Ψһ������ֱ���������в��ң����ö��ֲ��ԣ�
	int loc_id;
	if (BinSearchById(book.id, loc_id)) {
		return 0;
	}
	else
	{ // �鼮������, ���鼮д���ļ�β���������������в����鼮

		IdIndex CurIdIndex;
		strcpy(CurIdIndex.id, book.id);
		BookFile.seekp(0, ios::end); // ��λ���ļ�ĩβ
		CurIdIndex.offset = BookFile.tellp(); // ��¼�鼮���ļ��е�ƫ����

		BookFile.write((char*)&book, sizeof(Book)); // д���鼮

		IdIndexList.Insert(loc_id, CurIdIndex); // �������ؼ���������

		// ����ιؼ���������
		SimpleLkList<int> Bookoffset;
		int loc_name;
		if (BinSearchByTitle(book.name, Bookoffset, loc_name))
		{ // ��������
			//cout << "�������ڣ�����" << endl;
			NameIndex tmp;
			NameIndexList.GetElem(loc_name, tmp);

			tmp.offsetList.AddTail(CurIdIndex.offset);
			NameIndexList.SetElem(loc_name, tmp);
		}
		else
		{ // ����������
			//cout << "����������" << endl;
			NameIndex CurNameIndex;
			strcpy(CurNameIndex.name, book.name);
			CurNameIndex.offsetList.Insert(0, CurIdIndex.offset);
			NameIndexList.Insert(loc_name, CurNameIndex);
		}
		return 1;
	}
}

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
	book.IsDeleted = 0; // δɾ����ʼ��

	if (InsertBookAux(book))
		cout << "�����鼮�ɹ�!" << endl;
	else
		cout << "�鼮�Ѿ�����!" << endl;

	return;
};

bool BookManageSystem::DeleteBookAux(char* id)
{ // 0�����鼮�����ڣ�1����ɾ���ɹ�
	// �����鼮�Ƿ��Ѿ�����
	// ����ISBN��Ψһ������ֱ���������в��ң����ö��ֲ��ԣ�
	int loc_id;
	if (!BinSearchById(id, loc_id)) {
		cout << "���鼮�����ڣ�\n" << endl;
		return 0;
	}
	else
	{ // �鼮����, ����������ɾ���鼮
		IdIndex CurIdIndex;
		NameIndex CurNameIndex;
		Book book;

		IdIndex tmp;
		IdIndexList.GetElem(loc_id, tmp);
		BookFile.clear(); // ����ļ���״̬
		BookFile.seekg(tmp.offset, ios::beg);
		BookFile.read((char*)&book, sizeof(Book));

		//���ļ���ɾ���鼮
		book.IsDeleted = 1;
		BookFile.seekg(tmp.offset, ios::beg);
		BookFile.write((char*)&book, sizeof(Book));

		IdIndexList.Delete(loc_id, CurIdIndex); // ɾ�����ؼ����������еļ�¼
		SimpleLkList<int> Bookoffset;
		int loc_name;
		BinSearchByTitle(book.name, Bookoffset, loc_name);
		if (Bookoffset.Length() == 1)
		{ // ��Ӧ����ֻ��һ����
			NameIndexList.Delete(loc_name, CurNameIndex); // ɾ���ιؼ����������еļ�¼
		}
		else
		{ // ��Ӧ�����ж౾��
			NameIndexList.GetElem(loc_name, CurNameIndex);
			for (int i = 0; i < CurNameIndex.offsetList.Length(); i++)
			{
				int CurOffset;
				CurNameIndex.offsetList.GetElem(i, CurOffset);
				if (CurOffset == tmp.offset)
				{
					CurNameIndex.offsetList.Delete(i, CurOffset);
					break;
				}
			}
			NameIndexList.SetElem(loc_name, CurNameIndex); // ������������
		}
	}
	return 1;
}

// ɾ���鼮
void BookManageSystem::DeleteBook()
{
	char id[14];
	cout << "������Ҫɾ�����鼮��ISBN�ţ�";
	cin >> id;
	
	if (DeleteBookAux(id))
		cout << "ɾ���鼮�ɹ�!" << endl;
	else
		cout << "ɾ���鼮������!" << endl;

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

		if (!(cin >> choice)) { // ���Զ�ȡ���룬���ʧ�������if���
			cin.clear(); // �������״̬
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n'); // ����ֱ�����з�����������
			cout << "�������������һ������ѡ�" << endl; // ��ʾ�û��������
			continue; // ����ѭ���Ŀ�ʼ���ٴ���ʾ�û�����
		}

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
			cout << "�������ִ�������������" << endl;
			break;
		}
	} while (true);
	cout << "�˳����Һ���" << endl;
};

// ����������
void BookManageSystem::SearchByTitle()
{ // ��������������������ֱ�����������в��ң�����һ��ͨ��ƫ�����ҵ��鼮��Ϣ
	
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
		BookList.AddTail(book);
	}
	// ����鼮��Ϣ
	DisplayBook(BookList);

	return;
};

// ���ֲ�����������������
bool BookManageSystem::BinSearchByTitle(char *name, SimpleLkList<int> &Bookoffset, int &mid)
{
	if (NameIndexList.Length() == 0)
	{
		mid = 0;
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
			//cout << "��ֵǰԪ�ظ�����" << tmp.offsetList.Length() << endl;
			Bookoffset = tmp.offsetList; // ��������
			//cout << "��ֵ��Ԫ�ظ�����" << Bookoffset.Length() << endl;
			return 1;
		}
		else if (strcmp(tmp.name, name) < 0) {
			low = mid + 1;
		}
		else {
			high = mid;
		}
	}

	mid = low; // ���ز���λ��
	
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
		if (strcmp(book.author, author) == 0 && !book.IsDeleted)
		{ // ������ͬ��δɾ��
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
	int choice;
	while (true)
	{
		cout << "��ѡ�������1.�����鼮 2.ɾ���鼮 3.���¼�¼ 4.�����鼮 5.������������ 6.�˳�\n";
		cout << "������ѡ��";

		if (!(cin >> choice)) { // ���Զ�ȡ���룬���ʧ�������if���
			cin.clear(); // �������״̬
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n'); // ����ֱ�����з�����������
			cout << "�������������һ������ѡ�" << endl; // ��ʾ�û��������
			continue; // ����ѭ���Ŀ�ʼ���ٴ���ʾ�û�����
		}

		switch (choice)
		{
		case 1:
			InsertBook();
			break;
		case 2:
			DeleteBook();
			break;
		case 3:
			UpdateBook();

			break;
		case 4:
			SearchBook();
			break;
		case 5:
			SortByAuthor();
			break;
		case 6:
			cout << "�ɹ��˳�ϵͳ!\n";
			return;
		case 7:
			test();
			break;
		default:
			cout << "�������ִ�������������" << endl;
			break;
		}
	}
};

// ��ʾ�鼮��Ϣ
void BookManageSystem::DisplayBook(Book &book)
{
	// ͳ�Ƹ����ֶε���󳤶�
	int numspace[4];
	numspace[0] = MY_MAX(strlen(book.id), 6);
	numspace[1] = MY_MAX(strlen(book.name), 4);
	numspace[2] = MY_MAX(strlen(book.author), 4);
	int price = book.price, len = 3;
	while(price)
	{
		price /= 10;
		len++;
	}
	numspace[3] = MY_MAX(len, 8);
	
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
		numspace[0] = MY_MAX(numspace[0], strlen(tmp.id));
		numspace[1] = MY_MAX(numspace[1], strlen(tmp.name));
		numspace[2] = MY_MAX(numspace[2], strlen(tmp.author));
		int price = tmp.price, len = 3;
		while(price)
		{
			price /= 10;
			len++;
		}
		numspace[3] = MY_MAX(numspace[3], len);
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
	char id[14];
	cout << "������Ҫ���µ��鼮��ISBN�ţ�";
	cin >> id;
	if (DeleteBookAux(id))
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
		book.IsDeleted = 0; // δɾ����ʼ��

		if (InsertBookAux(book))
			cout << "�����鼮�ɹ�!" << endl;
		else
			cout << "�����鼮ʧ��!" << endl;
	}
	else
	{
		cout << "�����鼮������!" << endl;
	}
	return;
};

// ������������
void BookManageSystem::SortByAuthor()
{
	// ����û������������ֻ�ܱ��������鼮�ļ�
	BookFile.clear(); // ����ļ���״̬
	BookFile.seekg(0, ios::end); // ��λ���ļ�ĩβ
	int FileLen = BookFile.tellg(); // �ļ�����
	int BookNums = FileLen / sizeof(Book); // �鼮����
	
	BookFile.clear(); // ����ļ���״̬
	BookFile.seekg(0, ios::beg); // ��λ���ļ���ͷ
	Book book;
	SimpleLkList<Book> bookList;
	while (BookNums--)
	{
		BookFile.read((char*)&book, sizeof(Book));
		if (!book.IsDeleted)
		{ // δɾ��
			bookList.AddTail(book);
		}
	}
	if (bookList.Length() == 0)
	{
		cout << "Ŀǰ�����鼮�鼮!" << endl;
		return;
	}
	// ����
	// ѡ������
	for (int i = 0; i < bookList.Length(); i++)
	{
		Book tmp;
		bookList.GetElem(i, tmp);
		int min = i;
		for (int j = i + 1; j < bookList.Length(); j++)
		{
			Book tmp2;
			bookList.GetElem(j, tmp2);
			if (strcmp(tmp.author, tmp2.author) > 0)
			{
				min = j;
			}
		}
		if (min != i)
		{
			Book tmp2;
			bookList.GetElem(min, tmp2);
			bookList.SetElem(min, tmp);
			bookList.SetElem(i, tmp2);
		}
	}
	// ���
	cout << "���������������鼮��Ϣ���£�" << endl;
	DisplayBook(bookList);
	return;
};

// ���Ժ���
void BookManageSystem::test()
{
	cout << "���Ժ���" << endl;
	cout << "���ؼ���������" << endl;
	for (int i = 0; i < IdIndexList.Length(); i++)
	{
		IdIndex tmp;
		IdIndexList.GetElem(i, tmp);
		cout << "ISBN�ţ�" << tmp.id << "  λ�ã�" << tmp.offset << endl;
	}
	cout << "�ιؼ���������" << endl;
	for (int i = 0; i < NameIndexList.Length(); i++)
	{
		NameIndex tmp;
		NameIndexList.GetElem(i, tmp);
		cout << "������" << tmp.name << "  λ�ã�";
		//cout << tmp.offsetList.Length() << endl;
		for (int j = 0; j < tmp.offsetList.Length(); j++)
		{
			int CurOffset;
			tmp.offsetList.GetElem(j, CurOffset);
			cout << CurOffset << " ";
		}
		cout << endl;
	}
};
#endif