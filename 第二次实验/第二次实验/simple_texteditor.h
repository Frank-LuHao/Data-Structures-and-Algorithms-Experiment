#pragma once
#if !defined(SIMPLE_TEXTEDITOR_H)
#define SIMPLE_TEXTEDITOR_H
#include "../../include/char_string.h"
#include "../../include/dbl_link_list.h"
using namespace std;

class SimpleTextEditor : public DblLkList<CharString*>
{
public:
	SimpleTextEditor(char fName[]);//构造函数
	~SimpleTextEditor();//析构函数
	void Run(); //运行文本编辑器
private:
	//函数操作
	void Clear();//重载Clear函数
	//bool NextLine();//下一行
	//bool PrevLine();//上一行
	//bool GotoLine();//转到指定行
	//bool InsertLine();//插入行
	//bool ChangeLine();//修改行
	bool ReadFile();//读文件
	bool WriteFile();//写文件
	//void FindString();//查找字符串
	void ViewFile();//查看文件
	void Help();//帮助
	
	//数据成员
	int m_nCurRow;//当前行
	CharString m_strFileName;//编辑文件名
};

SimpleTextEditor::SimpleTextEditor(char fName[])
{
	m_nCurRow = 0;
	CharString tmpName(fName);
	m_strFileName = tmpName;
	ReadFile();
}

SimpleTextEditor::~SimpleTextEditor()
{
	Clear();
	m_strFileName = "";
}

void SimpleTextEditor::Clear()
{
	while (Length() > 0)
	{//清空链表中保存的字符串
		CharString* pStr;//定义从链表删除的字符串指针
		if (Delete(0, pStr))//调用链表类的Delete方法，清除第一行的字符串指针
			delete pStr;//删除该指针指向的对象
	}
	m_nCurRow = 0;
}

bool SimpleTextEditor::ReadFile()
{
	bool bLoaded = false;
	CharString strMsg;
	if (m_strFileName.IsEmpty())
	{
		printf( "文件为空，读取失败！\n");
	}
	else
	{
		FILE* pFile = NULL;
		if (!(pFile = fopen(m_strFileName.ToCStr(), "rb")))
		{
			printf("文件 \"%s\" 未找到", m_strFileName.ToCStr());
		}
		else
		{
			fseek(pFile, 0, SEEK_END);//到文件尾
			int nFLen = ftell(pFile);//获得文件长度
			char* pBuf = new char[nFLen + 1];
			fseek(pFile, 0, SEEK_SET);//回到文件头
			fread(pBuf, 1, nFLen, pFile);//读取文件
			fclose(pFile);
			pBuf[nFLen] = 0;
			//判断文件是否是文本文件
			char* p = pBuf;
			while ('\0' != *p) p++;
			if (p - pBuf + 1 < nFLen)
			{//不是文本文件
				printf("文件 \"%s\" 并不是一个文本文件", m_strFileName.ToCStr());
			}
			else
			{
				//首先清空字符串链表
				Clear();
				const char* pLine = pBuf;
				p = (char*)pLine;
				while ('\0' != *p)
				{
					if ('\n' == *p)
					{//找到换行符
						*p = '\0';//生成C风格字符串
						CharString* pNewStr = new CharString(pLine);
						AddTail(pNewStr);//添加一行
						pLine = p + 1;
					}
					p++;
				}
				if (pLine < pBuf + nFLen)
				{//添加没有换行符的最后一行
					CharString* pNewStr = new CharString(pLine);
					AddTail(pNewStr);//添加一行
				}
				bLoaded = true;
				m_strFileName = m_strFileName;//当前编辑文件为参数
			}
			delete[]pBuf;//删除缓冲区
		}
	}
	return bLoaded;
}

bool SimpleTextEditor::WriteFile() {
	bool bSucessed = false;
	CharString strMsg;
	if (m_strFileName.IsEmpty())
	{
		printf("save command need file name,Usage save -<filename>");
	}
	else
	{
		CharString fileName = m_strFileName;
		if (fileName.IsEmpty())
			fileName = m_strFileName;
		FILE* pFile = NULL;
		if ((pFile = fopen(fileName.ToCStr(), "wb")) == NULL)
		{
			printf("文件 \"%s\" 打开失败", fileName.ToCStr());
		}
		else
		{
			CharString* pStrLine;
			Head();//当前节点回到头节点
			int nLineNum = 0;
			while (Next(pStrLine) && pStrLine)//遍历链表
			{
				if (pStrLine->Length() > 0)//串长度大于0时写入文件
					fwrite(pStrLine->ToCStr(), pStrLine->Length(), 1, pFile);
				if (nLineNum < Length() - 1)//不是最后一行写入换行符
					fwrite("\n", 1, 1, pFile);
				nLineNum++;
			}
			fclose(pFile);
			m_strFileName = fileName;
			bSucessed = true;
		}
	}
	return bSucessed;
}

void SimpleTextEditor::ViewFile()
{
	CharString* pStrLine;
	Head();
	while (Next(pStrLine) && pStrLine)
	{
		printf("%s\n", pStrLine->ToCStr());
	}
}

void SimpleTextEditor::Run() {
	Help();
	//ViewFile();
}

void SimpleTextEditor::Help()
{
	printf("命令列表：\n");
	printf("H/h: 显示全部帮助信息\n");
	printf("V/v: 查看文件内容\n");
	printf("S/s: 保存文件\n");
	printf("Q/q: 退出编辑器\n");
	return;
}
#endif