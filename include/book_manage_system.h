#if !define	BOOK_MANAGE_SYSTEM_H
#define BOOK_MANAGE_SYSTEM_H

#include<iostream>
#include<fstream>
#include "SqList.h"
using namespace std;

// һЩ����ṹ��Ķ���
struct Date {
	int year, month, day;
};

struct Book {
	char id[14]; //ISBN��
	char name[128]; //����
	char author[128]; //����
	float price; //�۸�
	Date buy_date; //��������
};

//�����ṹ
struct IdIndex { //���ؼ���
	char id[14]; //ISBN��
	long offset; //ƫ����
};

struct NameIndex { //�ιؼ���
	char name[128]; //����
	long offset; //ƫ����
};

// �鼮����ϵͳ��
class BookManageSystem
{
public:
	BookManageSystem(); // ���캯��
	~BookManageSystem(); // ��������
	//Run(); //����ͼ�����ϵͳ

private:
	//���ݳ�Ա
	fstream bookFile; // �鼮�ļ�
	SqList<IdIndex> idIndexList; // ISBN��������
	SqList<NameIndex> nameIndexList; // ����������

	//��Ա����
	void InsertBook(); // �����鼮
	void InsertBookAux(Book &book, int offset); // �����鼮��������

};
// ���캯��
BookManageSystem::BookManageSystem()
{
	bookFile.open("bookdata.txt");
};
// ��������
BookManageSystem::~BookManageSystem()
{
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
	cout << "�������鼮�Ĺ�������(��λyear ��λmonth ��λday���Կո����)��";
	cin >> book.buy_date.year >> book.buy_date.month >> book.buy_date.day;
	// �����鼮
	InsertBookAux(book, bookFile.tellp());
};
// �����鼮��������

#endif