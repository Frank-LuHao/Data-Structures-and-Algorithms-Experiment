#if !defined(_STRUCT_H_)
#define _STRUCT_H_
#include "../../include/SimpleLkList.h"
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
#endif