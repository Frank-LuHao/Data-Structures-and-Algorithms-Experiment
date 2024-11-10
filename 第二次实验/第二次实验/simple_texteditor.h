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
	bool NextLine();//下一行
	bool PrevLine();//上一行
	void ShowCurRow();//显示当前行
	bool GotoLine();//转到指定行
	bool GotoFirstLine();//转到第一行
	bool GotoLastLine();//转到最后一行
	bool GotoLineAux(int nLineNum); //转到指定行辅助函数
	bool InsertLine();//插入行
	bool DeleteLine();//删除当前行
	bool ReadFile();//读文件
	bool WriteFile(bool OutPutFlag = 1);//写文件
	bool FindString();//查找字符串
	bool FindStringAux(const CharString& strFind, int &nLineNum, int &nIndex);//查找字符串辅助函数
	bool ChangeLine();//替换当前行或所有行的指定文本串
	bool ReplaceLineAux(const CharString& strReplace, int &nLineNum, int &nIndex);//替换字符串辅助函数
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

bool SimpleTextEditor::WriteFile(bool OutPutFlag) {
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
	if (OutPutFlag)
	{
		printf("文件 \"%s\" 保存成功\n", m_strFileName.ToCStr());
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
	ShowCurRow();
	char UserCommand, CharTmp;
	bool bRunFlag = 1;
	while (bRunFlag)
	{
		// 获取用户输入的命令
		do {
			printf("command:");
			do {
				UserCommand = getchar();
			} while (' ' == UserCommand);
		} while ('\n' == UserCommand );
		//清空输入流
		while ((CharTmp = getchar()) != EOF && CharTmp != '\n');

		if (UserCommand >= 'A' && UserCommand <= 'Z')
			UserCommand = tolower(UserCommand);

		switch (UserCommand)
		{
		case 'h':
		case '?':
			Help();
			ShowCurRow();
			break;
		case 'v':
			ViewFile();
			ShowCurRow();
			break;
		case 's':
			WriteFile();
			ShowCurRow();
			break;
		case 'n':
			NextLine();
			break;
		case 'p':
			PrevLine();
			break;
		case 'g':
			GotoLine();
			break;
		case 'b':
			GotoFirstLine();
			break;
		case 'e':
			GotoLastLine();
			break;
		case 'i':
			InsertLine();
			ShowCurRow();
			break;
		case 'd':
			DeleteLine();
			ShowCurRow();
			break;
		case 'f':
			FindString();
			break;
		case 'c':
			ChangeLine();
			ShowCurRow();
			break;
		case 'q':
			bRunFlag = 0;
			break;
		default:
			printf("invaild command!(use \'h/H\' to get help)\n");
			break;
		}
	}
	
	//退出时的自动保存询问
	char charSaveCommand;
	do {
		printf("是否选择保存？（y/n）:");
		charSaveCommand = getchar();
		while ((CharTmp = getchar()) != EOF && CharTmp != '\n');  //清空输入流
		if (charSaveCommand == 'y') {
			if (WriteFile(0))
				printf("Successful Exit with file saved automaticly!\n");
			else
				printf("Fail to save the file !\n");
		}
		else if (charSaveCommand == 'n') {
			printf("Successful quit.\n");
		}
	} while (charSaveCommand != 'y' && charSaveCommand != 'n');

	return;
}

void SimpleTextEditor::Help()
{
	printf("命令列表：\n");
	printf("H/h: 显示全部帮助信息\n");
	printf("V/v: 查看文件内容\n");
	printf("S/s: 保存文件\n");
	printf("Q/q: 退出编辑器\n");
	printf("N/n: 下一行\n");
	printf("P/p: 上一行\n");
	printf("G/g: 转到指定行\n");
	printf("B/b: 转到第一行\n");
	printf("E/e: 转到最后一行\n");
	printf("I/i: 插入行\n");
	printf("D/d: 删除行\n");
	printf("F/f: 查找字符串\n");
	printf("C/c: 替换字符串\n");
	return;
}

void SimpleTextEditor::ShowCurRow()
{
	CharString* pStrMsg;
	if (GetElem(m_nCurRow, pStrMsg))
		printf("%d: %s\n", m_nCurRow+1, pStrMsg->ToCStr());
	else 
		printf("文件为空\n");
	return;
}

bool SimpleTextEditor::NextLine()
{
	bool bNextFlag = false;
	CharString* pStrMsg;
	if (GetElem(m_nCurRow+1, pStrMsg))
	{
		m_nCurRow++;
		printf("%d: %s\n", m_nCurRow + 1, pStrMsg->ToCStr());
	}
	else
	{
		printf("已经到达文件末尾\n");
	}
	return bNextFlag;
}

bool SimpleTextEditor::PrevLine()
{
	bool bPrevFlag = false;
	CharString* pStrMsg;
	if (GetElem(m_nCurRow - 1, pStrMsg))
	{
		m_nCurRow--;
		printf("%d: %s\n", m_nCurRow + 1, pStrMsg->ToCStr());
	}
	else
	{
		printf("已经到达文件开头\n");
	}
	return bPrevFlag;
}

bool SimpleTextEditor::GotoLine()
{
	int nLineNum;
	printf("请输入行号：");
	scanf("%d", &nLineNum);
	getchar(); //清空行尾换行符
	return GotoLineAux(nLineNum);
}

bool SimpleTextEditor::GotoLineAux(int nLineNum)
{
	bool bGotoFlag = false;
	CharString* pStrMsg;
	if (GetElem(nLineNum - 1, pStrMsg))
	{
		m_nCurRow = nLineNum - 1;
		printf("%d: %s\n", m_nCurRow + 1, pStrMsg->ToCStr());
	}
	else
	{
		printf("行号超出范围\n");
	}
	return bGotoFlag;
}

bool SimpleTextEditor::GotoFirstLine()
{
	return GotoLineAux(1);
}

bool SimpleTextEditor::GotoLastLine()
{
	return GotoLineAux(Length());
}

bool SimpleTextEditor::InsertLine()
{
	bool bInsertFlag = false;
	char strLineNum[128];
	char charTmp;
	int nLineNum;
	printf("请输入要插入的行号（默认当前行）：");
	strcpy(strLineNum, Read(cin).ToCStr());
	if (strlen(strLineNum) == 0)
	{
		nLineNum = m_nCurRow;
	}
	else
	{
		nLineNum = atoi(strLineNum);
		if (nLineNum < 1 || nLineNum > Length() + 1)
		{
			printf("行号超出范围\n");
			return bInsertFlag;
		}
		nLineNum--;
	}
	printf("请输入要插入的内容：");
	CharString StrMsg = Read(cin);
	CharString* pStrMsg = new CharString(StrMsg);
	bInsertFlag = Insert(nLineNum, pStrMsg);
	if (bInsertFlag)
	{
		m_nCurRow = nLineNum;
		printf("插入成功\n");
	}
	else
	{
		delete pStrMsg;
		printf("插入失败\n");
	}
	
	return bInsertFlag;
}

bool SimpleTextEditor::DeleteLine()
{
	bool bDeleteFlag = false;
	CharString* pStrMsg;
	if (Delete(m_nCurRow, pStrMsg))
	{
		delete pStrMsg;
		bDeleteFlag = true;
		printf("删除成功\n");
	}
	else
	{
		printf("删除失败\n");
	}
	return bDeleteFlag;
}

bool SimpleTextEditor::FindStringAux(const CharString& strFind, int &nLineNum, int &nIndex)
{
	bool bFindFlag = false;
	CharString* pStrMsg;
	nIndex = 0;
	Head();
	for (int i = 0; i < nLineNum; i++) Next(pStrMsg); //定位到当前行的前一行
	while (Next(pStrMsg) && pStrMsg)
	{
		nLineNum++;
		nIndex = pStrMsg->KMPIndex(strFind);
		if (nIndex >= 0)
		{
			bFindFlag = true;
			break;
		}
	}
	nLineNum--;
	return bFindFlag;
}

bool SimpleTextEditor::FindString()
{
	bool bFindFlag = false;
	CharString strFind;
	int nLineNum, nIndex;

	char charSearchBegin, CharTmp;
	do {
		printf("是否从当前行开始查找（否则从头开始）？（y/n）:");
		charSearchBegin = getchar();
		while ((CharTmp = getchar()) != EOF && CharTmp != '\n');  //清空输入流
		if (charSearchBegin == 'y') {
			nLineNum = m_nCurRow;
		}
		else if (charSearchBegin == 'n') {
			nLineNum = 0;
		}
	} while (charSearchBegin != 'y' && charSearchBegin != 'n');

	printf("请输入要查找的字符串：");
	strFind = Read(cin);
	if (strFind.IsEmpty())
	{
		printf("查找字符串为空\n");
		return bFindFlag;
	}
	else if (FindStringAux(strFind, nLineNum, nIndex))
	{
		m_nCurRow = nLineNum;
		CharString* pStrMsg;
		GetElem(m_nCurRow, pStrMsg);
		printf("%d: %s\n", m_nCurRow + 1, pStrMsg->ToCStr());
		printf("在第%d行第%d列找到字符串\n", m_nCurRow + 1, nIndex + 1);
		bFindFlag = true;
	}
	else
	{
		printf("未找到字符串\n");
	}
	return bFindFlag;
}

bool SimpleTextEditor::ReplaceLineAux(const CharString& strReplace, int &nLineNum, int &nIndex)
{
	bool bReplaceFlag = false;
	CharString TmpStr;
	CharString* pStrMsg;
	GetElem(nLineNum, pStrMsg);
	TmpStr = SubString(*pStrMsg, 0, nIndex);
	TmpStr += strReplace;
	TmpStr += SubString(*pStrMsg, nIndex + strReplace.Length(), pStrMsg->Length() - nIndex - strReplace.Length());
	Copy(*pStrMsg, TmpStr);
	bReplaceFlag = true;
	return bReplaceFlag;
}

bool SimpleTextEditor::ChangeLine()
{
	bool bChangeFlag = true;
	char charChangeBegin, CharTmp;
	CharString strFind, strReplace;
	int nLineNum = 0, nIndex;

	do {
		printf("是否只替换当前行（否则替换所有行）？（y/n）:");
		charChangeBegin = getchar();
		while ((CharTmp = getchar()) != EOF && CharTmp != '\n');  //清空输入流
		if (charChangeBegin == 'y') {
			nLineNum = m_nCurRow;
		}
		else if (charChangeBegin == 'n') {
			nLineNum = 0;
		}
	} while (charChangeBegin != 'y' && charChangeBegin != 'n');

	printf("请输入要查找的字符串：");
	strFind = Read(cin);
	if (strFind.IsEmpty())
	{
		printf("查找字符串为空\n");
		return bChangeFlag;
	}
	printf("请输入要替换的字符串：");
	strReplace = Read(cin);
	if (strReplace.IsEmpty())
	{
		printf("替换字符串为空\n");
		return bChangeFlag;
	}

	if (charChangeBegin == 'y') 	{
		if (FindStringAux(strFind, nLineNum, nIndex))
		{
			if (ReplaceLineAux(strReplace, nLineNum, nIndex)) {
				printf("替换成功\n");
				bChangeFlag = true;
			}
			else {
				printf("替换失败\n");}
			}
		else
		{
			printf("未找到字符串\n");
		}
	}
	else
	{
		while (FindStringAux(strFind, nLineNum, nIndex))
		{
				bChangeFlag &= ReplaceLineAux(strReplace, nLineNum, nIndex);
		}
		if (bChangeFlag) {
			printf("替换成功\n");
		}
		else
		{
			printf("替换失败\n");
		}
	}
	
	return bChangeFlag;
}
#endif