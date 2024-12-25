#if !defined(_STRUCT_H_)
#define _STRUCT_H_
#include "../../include/SimpleLkList.h"
// һЩ����ṹ��Ķ���:
// ���ڽṹ
struct Date {
	int year, month, day;
};

// �鼮�ṹ
struct Book {
	char id[14]; //ISBN��
	char name[32]; //����
	char author[32]; //����
	float price; //�۸�
	Date buyDate; //��������
	bool IsDeleted; //�Ƿ�ɾ��
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
#endif