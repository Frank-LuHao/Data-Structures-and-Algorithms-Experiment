/*******************************************************************
*文件名：TextEditor.h
*功能：实现文本编辑
*作者：陈延涛（四川大学计算机学院）
*说明：本程序为教学用源代码,未经允许，勿作他用
*******************************************************************/
#if !defined(_TEXT_EDITOR_H_)
#define _TEXT_EDITOR_H_
#include "char_string.h"
#include "dbl_link_list.h"
#include <conio.h>
#if defined(_WIN32)
#include <windows.h> //获取窗口尺寸的是非标准函数，不同操作系统不同
#elif defined(_gnu_linux_)
#include <unistd.h>
#include <sys/ioctl.h>
#endif
#pragma warning(disable:4996)
using namespace std;
typedef struct tagWND_SIZE
{
	int rows, cols;
}WND_SIZE;
WND_SIZE GetWindSize()
{
	WND_SIZE size;
	size.cols = 80;
	size.rows = 30;
	//获取console窗口分辨率需要调用非标准函数，不同操作系统不同，
	//采用条件编译自动根据操作系统选择需要编译的语句
#if defined(_WIN32)
	//windows操作系统
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	size.rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	size.cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#elif defined(_gnu_linux_)
	//linux操作系统
	int fd;
	struct winsize sz;

	fd = open(TTY_NAME, O_RDONLY);
	if (fd < 0)
		return size;

	if (ioctl(fd, TIOCGWINSZ, &sz) < 0)
		if (fd < 0)
			return size;
	size.rows = sz.ws_row;
	size.cols = sz.ws_col;
	close(fd);

#endif 
	return size;
}
class TextEditor:public DblLkList<CharString*>
{
public:
	TextEditor();
	~TextEditor();
	void Run(const CharString& strFileName);//启动文本编辑器
	bool Load(const CharString& strFileName);
private:
	void Clear();//重载Clear;
	void Edit();//编辑
	bool GetCommand();//获取命令行命令
	bool OnEditCommand(char chCtrl,CharString* pCurStr);//处理编辑命令，如光标左移、右移等
	bool Save(const CharString& strFileName);//存储到文件
	unsigned short GetStdCh(char& chCtrl);//获取一个字符并且不回显
	void Help(const CharString& strCommand);//帮助
	void ShowMessage(const CharString & strMsg,int nBkColor = 41);//显示信息
	void PrintStrings(bool bClearAll = false);//打印全部字符串
	void PrintLine(CharString * pStr,int nYCursor);//打印一行
	void MoveCursor(int nYCursor, int nXCursor,bool bPrintLineNum = true);//移动光标
	void CheckChinesCursor(CharString* pStr);//检查汉字光标
	bool MoveYCursor(int nOffset, int nWndRows);//上下移动当前行
	bool FindPatternStr(const CharString& strPattern, int nRow, int nCol);//查找模式串
	bool GotoLine(const CharString& strNumber);//光标跳转到指定行
	int GetTop(int nWndRows,bool &bPrintStrings);//获取当前显示窗口顶行行号
	int m_nXCursor;//当前光标横坐标
	int m_nYCursor;//当前光标纵坐标
	int m_nCurTop;//当前显示的顶行
	bool m_bReplace;//当前编辑字符是否是替换状态：false-插入状态,true-替换状态
	bool m_bEdit;//当前是否处于编辑状态：false-命令状态，tue-编辑状态
	CharString m_strFileName;//当前编辑的文件名
	CharString m_strPattern;//当前比对模式串
};

TextEditor::TextEditor()
{
	m_nXCursor = 0;
	m_nYCursor = 0;
	m_nCurTop = 0;
	m_bReplace = false;
	m_bEdit = false;
}

TextEditor::~TextEditor()
{
	Clear();
}
void TextEditor::Clear()
{
	while (Length() > 0)
	{//清空链表中保存的字符串
		CharString* pStr;//定义从链表删除的字符串指针
		if (Delete(0, pStr))//调用链表类的Delete方法，清除第一行的字符串指针
			delete pStr;//删除该指针指向的对象
	}
	m_nYCursor = 0;//当前行归零
	m_nXCursor = 0;//当前列归零
	m_nCurTop = 0;//当前显示顶部行归零
	m_strFileName = "";//清空当前文件名
	m_strPattern = "";//清空匹配模式串
}

unsigned short TextEditor::GetStdCh(char& chCtrl)
{
	unsigned short usCh = 0;
	unsigned char ch = getch();//从标准输入流获取一个字符，不回显
	chCtrl = 0;//控制码赋初值0，表示不是控制码
	usCh = ch;
	if (ch == 0xe0 || ch==0)
	{//当获取的字符ascii码值为0xe0或0时，是控制字符
		chCtrl = getch();//再从标准输入流中获取控制字符
	}
	else if(ch<' ' && ch !='\t')
	{//当获取的字符ascii码值小于空格时(如Enter键),也等同于快捷键
		chCtrl = ch;
	}
	else if (ch >= 0xa0)
	{//是汉字
		ch = getch();//汉字由两个字符组成，需要再次获取第二个字符
		//把两个字符打包到一个short型(16位)字符中
		usCh <<= 8;//第一个字符左移8位，占据高8位
		usCh |= ch;//第二个字符战绝低8位
	}
	return usCh;
}
int TextEditor::GetTop(int nWndRows, bool& bPrintStrings)
{
	int nTopRow = m_nCurTop;
	bPrintStrings = false;
	if (m_nYCursor -nTopRow >= nWndRows-3)
	{
		m_nCurTop = nTopRow = (m_nYCursor + 3) - nWndRows;//需要留下1行给命令行
		bPrintStrings = true;//需要重新打印所有字符串
	}
	else if(m_nYCursor - nTopRow<0)
	{
		if (nTopRow > 0)
		{
			nTopRow--;
			m_nCurTop = nTopRow;
			bPrintStrings = true;//需要重新打印所有字符串
		}
	}
	return nTopRow;
}
void TextEditor::MoveCursor(int nYCursor, int nXCursor, bool bPrintLineNum)
{
	//WND_SIZE size = GetWindSize();
	//bool bPrintStrings;
	int nTop = m_nCurTop;// GetTop(size.rows, bPrintStrings);
	if (!m_bEdit)
	{//非编辑状态，直接移动到光标位置
		printf("\033[%d;%dH", nYCursor + 1, nXCursor + 1);
		return;
	}
	//编辑状态
	char strLineHead[32] = { 0 };
	if (bPrintLineNum)
	{

		CharString* pCursorStr;

		sprintf(strLineHead, "%-3d", nYCursor + 1);
		int nHeadLen = strlen(strLineHead);
		if (GetElem(nYCursor, pCursorStr))
		{//获取光标所在行的字符串
			//计算光标需要移动的位置
			const char* pStr = pCursorStr->ToCStr();
			int x = nHeadLen;
			for (int i = 0; i < nXCursor; i++)
			{
				if (pStr[i] == '\t')
				{
					x += 8;
					x = x / 8 * 8;

				}
				else
					x++;
			}
			nXCursor = x;
		}
		else
			bPrintLineNum = false;
	}
	if (bPrintLineNum)
	{//需要打印行号
		printf("\033[?25l");//隐藏光标
		printf("\033[%d;%dH\033[46m%s", nYCursor + 1 - nTop, 1, strLineHead);
		printf("\033[?25h\033[40m");//恢复光标
	}
	printf("\033[%d;%dH", nYCursor + 1- nTop, nXCursor +1);

}
void TextEditor::PrintStrings(bool bClearAll)
{
	printf("\033[?25l");//隐藏光标
	printf("\033[0;0H");//光标回到原点
	CharString* pStrLine;
	int nLineNo = 0;//当前行
	WND_SIZE size = GetWindSize();
	bool bPrintStrings;
	int nTop = GetTop(size.rows, bPrintStrings);
	Head();//当前节点回到头节点
	while (Next(pStrLine) &&pStrLine)//遍历链表
	{
		if (nLineNo < nTop)
		{
			nLineNo++;
			continue;
		}
		if (nLineNo - nTop > size.rows - 3)
			break;
		MoveCursor(nLineNo, 0);
		printf("\033[K%s\n", pStrLine->ToCStr());//打印一行，并换行
		nLineNo++;
	}
	if (!bClearAll)
	{
		MoveCursor(nLineNo, 0, false);//不需要打印行号
		printf("\033[K");//多清除一行，避免删除行后最后一行显示没有清除
	}
	else
	{
		while (nLineNo - nTop <= size.rows - 3)
		{
			MoveCursor(nLineNo, 0, false);//不需要打印行号
			printf("\033[K");
			nLineNo++;
		}
	}
	
	if (m_bReplace)
		printf("\033[%d;%dH\033[K%s", size.rows, 0, "--Replace,Press \'ESC\' to return command mode--");
	else
		printf("\033[%d;%dH\033[K%s", size.rows, 0, "--Insert,Press \'ESC\' to return command mode--");
	MoveCursor(m_nYCursor, m_nXCursor);//光标回到当前位置
	printf("\033[?25h");//恢复光标
}
void TextEditor::PrintLine(CharString* pStr,int nYCursor)
{
	if (!pStr)
		return;	
	printf("\033[?25l");//隐藏光标
	MoveCursor(nYCursor,0);//光标到打印行首
	printf("\033[K");//清除光标到行尾的内容
	printf("%s", pStr->ToCStr());//打印一行,不换行
	printf("\033[?25h");//恢复光标
	MoveCursor(m_nYCursor , m_nXCursor);//光标回到当前位置
}
void TextEditor::ShowMessage(const CharString& strMsg,int nBkColor)
{
	if (m_bEdit)
		return;
	printf("\033[?25l");//隐藏光标
	WND_SIZE size = GetWindSize();
	MoveCursor(size.rows - 1, 0);
	printf("\033[K");//清除光标到行尾
	printf("\033[%dm%s", nBkColor,strMsg.ToCStr());//字体背景颜色红色，显示提示信息
	MoveCursor(size.rows - 2, 0);
	printf("\033[40m\033[K>>");//清除命令行
	printf("\033[?25h");//恢复光标

}
bool TextEditor::GotoLine(const CharString& strNumber)
{
	bool bGo = false;
	CharString strMsg;
	if (strNumber.IsEmpty())
	{
		strMsg.Format("go command must have param,usage: g -<Line number>");
		ShowMessage(strMsg);
	}
	else
	{
		char* pStop;
		int nLineNumber = strtol(strNumber.ToCStr(), &pStop, 10);
		if (nLineNumber < 1 || nLineNumber >= Length())
		{
			ShowMessage("Invalid line number");
		}
		else
		{
			m_nYCursor = nLineNumber - 1;
			m_nXCursor = 0;
			bGo = true;
		}
	}
	return bGo;
}
bool TextEditor::FindPatternStr(const CharString& strPattern, int nRow, int nCol)
{
	bool bFind = false;
	CharString strMsg;
	if (strPattern.IsEmpty())
	{//当模式串为空时，打印出错信息
		strMsg.Format("find command must have param, usage: f - <pattern string>");
		ShowMessage(strMsg);
	}
	else
	{
		CharString* pStrLine;
		Head();//当前节点回到头节点
		int nLineNum = 0;

		while (Next(pStrLine) && pStrLine)
		{//通过链表的Next()方法遍历链表
			if (nLineNum < nRow)
			{//当前行小于查找起始行时，继续遍历
				nLineNum++;//行号加1
				continue;//继续往下遍历
			}
			int pos = 0;
			if (nLineNum == nRow)
				pos = nCol;//行号为查找行时，列号为起始列，否则为0
			int index = pStrLine->KMPIndex(strPattern, pos);//调用KMP模式串匹配算法匹配字符串
			if (index >= 0)
			{//找到匹配字符
				bFind = true;//设置找到标志
				m_strPattern = strPattern;//设置当前模式串，方便在编辑状态时按F3键查找下一个串
				m_nXCursor = index;//设置当前光标横坐标
				m_nYCursor = nLineNum; //设置当前光标纵坐标
				break;//中断遍历
			}
			nLineNum++;//行号加1
		}
		if (!bFind && !m_bEdit)
		{//没有找到匹配字符,且不是编辑模式时，打印信息
			strMsg = "can\'t find string: ";
			strMsg += strPattern;
			ShowMessage(strMsg);
		}
		else if (!m_bEdit && bFind)
		{//找到且不为编辑状态时，在命令行提示栏打印信息
			strMsg.Format("find string %s,press F3 to find next string,press any key to return edit mode", strPattern.ToCStr());
			ShowMessage(strMsg,40);//打印信息
			getchar();//等待输入任意键
		}
	}
	return bFind;
}
bool TextEditor::Load(const CharString& strFileName)
{
	bool bLoaded = false;
	CharString strMsg;
	if (strFileName.IsEmpty())
	{
		strMsg = "load command must have param,usage: l -<filename>";
		ShowMessage(strMsg);
	}
	else
	{
		FILE* pFile = NULL;
		if (!(pFile = fopen(strFileName.ToCStr(), "rb")))
		{
			strMsg.Format("file \"%s\" not found", strFileName.ToCStr());
			ShowMessage(strMsg);
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
				strMsg.Format("file \"%s\" is not a text file", strFileName.ToCStr());
				ShowMessage(strMsg);
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
				m_strFileName = strFileName;//当前编辑文件为参数
			}
			delete[]pBuf;//删除缓冲区
		}
	}
	return bLoaded;
}
bool TextEditor::Save(const CharString& strFileName)
{
	bool bSucessed = false;
	CharString strMsg;
	if (strFileName.IsEmpty() && m_strFileName.IsEmpty())
	{
		strMsg = "save command need file name,Usage save -<filename>";
		ShowMessage(strMsg);
	}
	else
	{
		CharString fileName = strFileName;
		if (fileName.IsEmpty())
			fileName = m_strFileName;
		FILE* pFile = NULL;
		if ((pFile = fopen(fileName.ToCStr(), "wb")) == NULL)
		{
			strMsg.Format("file \"%s\" open failed",fileName.ToCStr());
			ShowMessage(strMsg);
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
bool TextEditor::GetCommand()
{
	printf("\033[?25l");//隐藏光标

	WND_SIZE size = GetWindSize();
	MoveCursor(size.rows-2,0);
	printf("\033[32m%s\033[37m>>",m_strFileName.ToCStr());
	printf("\033[?25h");//恢复光标
	printf("\033[K");//清除光标到行尾
	char szBuffer[256] = { 0 };
	scanf("%255[^\n]", szBuffer);//采用逆向字符集方式输入包含空格的字符串
	getchar();//丢弃\n
	char* pCmd = szBuffer;
	while ( *pCmd <= ' ' && *pCmd !='\0')pCmd++;//去除掉字符串开始的不可见字符和空格
	char* pPara = pCmd + 1;
	while (*pPara > ' ' && *pPara != '\0') pPara++;//找到可控制字符或空格
	while ((*pPara <= ' '|| *pPara=='=') && *pPara != '\0')
	{//去除掉参数中的不可见控制字符
		*pPara = '\0';
		pPara++;
	}
	
	CharString strCommand = pCmd, strParam = pPara;//定义命令和参数
	CharString strMsg;
	bool bContinue = true;//继续初值为真
	if (strCommand == "e" || strCommand == "edit")
	{//切换到编辑状，不需要参数
		m_bEdit = true;
	}
	else if (strCommand == "?" || strCommand == "help" || strCommand =="h")
	{//help 命令解释
		Help(strParam);
	}
	else if (strCommand == "l" || strCommand == "load")
	{//调文件，需要参数
		if (Load(strParam))
			m_bEdit = true;//调用文件成功，转为编辑模式
	}
	else if (strCommand == "q" || strCommand == "quit")
		bContinue = false;
	else if (strCommand == "s" || strCommand == "save")
	{//保存文件
		if (Save(strParam))
			m_bEdit = true;//存储文件成功，转为编辑模式
	}
	else if (strCommand == "f" || strCommand == "find")
	{
		if (FindPatternStr(strParam, m_nYCursor, 0))//查找字符串
			m_bEdit = true;//查找字符串成功，切换到编辑模式
	}
	else if (strCommand == "g" || strCommand == "go")
	{
		if (GotoLine(strParam))
			m_bEdit = true;//跳转到指定行成功，切换到编辑模式
	}
	else
	{
		strMsg.Format("Not an editor command:%s",strCommand.ToCStr());
		ShowMessage(strMsg);
	}
	return bContinue;
}
void TextEditor::CheckChinesCursor(CharString* pCurStr)
{
	if (m_nXCursor > 0)
	{//检查当前光标处后的汉字
		int nCurMax = pCurStr->Length();
		if (m_bReplace && pCurStr->Length() > 0)
			nCurMax--;
		if (m_nXCursor > nCurMax)
			m_nXCursor = nCurMax;

		unsigned char* pStrCh = (unsigned char*)pCurStr->ToCStr();
		unsigned char* pStrEnd = pStrCh + pCurStr->Length();
		unsigned char* p = pStrCh + m_nXCursor;
		while (p < pStrEnd)
		{
			if (*p < 0xa0)
				break;
			p++;
		}
		int nChinesCh = p - (pStrCh + m_nXCursor);
		if (nChinesCh % 2)
			m_nXCursor--;
	}
}
bool TextEditor::MoveYCursor(int nOffset, int nWndRows)
{
	CharString* pCurStr = NULL;
	bool bSuccessed = false;
	bool bPage = false;

	if (nOffset < -1 || nOffset>1)
	{//是翻页
		bPage = true;
		if (m_nYCursor + nOffset < 0)
			nOffset = -m_nYCursor;
		else if (m_nYCursor + nOffset >= Length() - 1)
			nOffset = Length() - 1 - m_nYCursor;
	}
	if (GetElem(m_nYCursor + nOffset, pCurStr) && pCurStr)
	{
		bSuccessed = true;
		bool bPrintStrings;
		m_nYCursor+= nOffset;
		CheckChinesCursor(pCurStr);
		if(!bPage)
			GetTop(nWndRows, bPrintStrings);
		else
		{
			bPrintStrings = true;
			m_nCurTop += nOffset;
			if (m_nCurTop < 0)
				m_nCurTop = 0;
	
		}
		if (bPrintStrings)
			PrintStrings(bPage);
		else
			MoveCursor(m_nYCursor, m_nXCursor);
	}
	return bSuccessed;
}
bool TextEditor::OnEditCommand(char chCtrl,  CharString* pCurStr)
{
	WND_SIZE size = GetWindSize();
	bool bSucessed = true;
	int nLines = Length();
	switch (chCtrl)
	{
	case 27:
		m_bEdit = false;
		break;
	case 'R'://insert键
		m_bReplace = !m_bReplace;
		PrintStrings();
		break;
	case 'G'://home键
		m_nYCursor = 0;//起始行
		m_nXCursor = 0;//起始列
		m_nCurTop = 0;//显示起始行为0
		PrintStrings();
		break;
	case 'O'://End键
		if (!GetElem(nLines - 1, pCurStr))
			break;
		m_nYCursor = nLines - 1;//最后一行
		m_nXCursor = pCurStr->Length();//最后一行的最后一个字符之后
		PrintStrings();
		break;
	case 'H'://up 键
		if (m_nYCursor > 0)
			bSucessed = MoveYCursor(-1,size.rows);
		break;
	case 'P'://down 键
		if (m_nYCursor < Length() - 1)
			bSucessed = MoveYCursor(+1, size.rows);
		break;
	case 'I'://page up键
		if (m_nYCursor >0)
			bSucessed = MoveYCursor(-(size.rows - 2), size.rows);
		break;
	case 'Q'://page down键
		if (m_nYCursor + size.rows - 2 < nLines)
			bSucessed = MoveYCursor(size.rows - 2, size.rows);
		else
			MoveCursor(nLines - 1, 0);

		break;
	case 'K'://left键
		if (pCurStr && m_nXCursor > 0)
		{
			unsigned char* pStrCh = (unsigned char*)pCurStr->ToCStr();
			if (pStrCh[m_nXCursor - 1] >= 0xa0)
				m_nXCursor -= 2;//是汉字,占用两个字符
			else
				m_nXCursor--;

			MoveCursor(m_nYCursor, m_nXCursor);
		}
		break;
	case 'M'://right键
		if (pCurStr)
		{
			int nCurMax = pCurStr->Length() + 1;
			if (m_bReplace && m_nXCursor > 0)
				nCurMax--;
			if (m_nXCursor + 1 < nCurMax)
			{
				unsigned char* pStrCh = (unsigned char*)pCurStr->ToCStr();
				if (pStrCh[m_nXCursor] > 0xa0)
					m_nXCursor += 2;//是汉字,占用两个字符
				else
					m_nXCursor++;

				MoveCursor(m_nYCursor, m_nXCursor);
			}
		}
		break;
	case '='://F3键
		if (!m_strPattern.IsEmpty())
		{//模式串不为空时,查找下一个匹配字符串
			if (FindPatternStr(m_strPattern, m_nYCursor, m_nXCursor + 1))
				PrintStrings();
		}
		break;
	case '\b':// 删除前一个字符
		if (m_nXCursor == 0)
		{//把换行删除
			CharString* pPreStr = NULL;
			if (GetElem(m_nYCursor - 1, pPreStr))
			{
				if (Delete(m_nYCursor, pCurStr))
				{
					*pPreStr += *pCurStr;//拼接到上一行
					delete pCurStr;
					m_nYCursor--;
					pCurStr = pPreStr;
					m_nXCursor = pCurStr->Length();
					printf("\033[?25l");//隐藏光标
					PrintStrings();//需要重新打印
					printf("\033[?25h");//恢复光标
				}
			}
		}
		else
		{
			char chDel;
			pCurStr->Delete(m_nXCursor - 1, chDel);//当前行删除掉前一个字符
			m_nXCursor--;//光标左移
			if ((unsigned char)chDel >= 0xa0)
			{//是一个汉字
				pCurStr->Delete(m_nXCursor - 1, chDel);
				m_nXCursor--;//光标左移
			}

			PrintLine(pCurStr, m_nYCursor);
		}
		break;
	case 'S'://Delete键
		if (m_nXCursor >= pCurStr->Length())
		{//把换行删除
			CharString* pNextStr = NULL;
			if (Delete(m_nYCursor + 1, pNextStr))
			{
				*pCurStr += *pNextStr;//拼接到上一行
				delete pNextStr;
				PrintStrings();//需要重新打印
			}
		}
		else
		{
			char chDel;
			pCurStr->Delete(m_nXCursor, chDel);//当前行删除掉后一个字符
			if ((unsigned char)chDel >= 0xa0)
			{//是一个汉字，再删除一个
				pCurStr->Delete(m_nXCursor, chDel);
			}
			PrintLine(pCurStr, m_nYCursor);
		}
		break;
	case '\r'://回车键
		if (m_bReplace)
		{//换行
			if (m_nYCursor < nLines - 1)
			{
				m_nYCursor++;
				m_nXCursor = 0;
				MoveCursor(m_nYCursor, m_nXCursor);
			}

		}
		else
		{//插入一行
			CharString* pStr = new CharString;
			//把当前光标后的数据删除并加入到新行
			while (pCurStr->Length() > m_nXCursor)
			{
				char chDel;
				if (!pCurStr->Delete(m_nXCursor, chDel))
					break;
				pStr->AddTail(chDel);
			}
			Insert(m_nYCursor + 1, pStr);

			m_nYCursor++;
			m_nXCursor = 0;

			printf("\033[?25l");//隐藏光标
			PrintStrings();//不是最后一行换行需要重新打印
			printf("\033[?25h");//恢复光标

		}
		break;
	default:
		break;
	}

	return bSucessed;
}
void TextEditor::Edit()
{
	int nLines = Length();
	printf("\033[2J");//清除屏幕
	PrintStrings();//打印字符串
	if (nLines == 0)
	{//插入首行
		CharString* pStr = new CharString;
		Insert(0, pStr);
		m_nXCursor = 0;
		m_nYCursor = 0;
	}
	char chCtrl;//控制字符
	unsigned short ch;//文本字符，(由于可能是汉字，所以为16位的short类型)
	do
	{
		CharString* pCurStr = NULL;//当前行字符串指针
		if (!GetElem(m_nYCursor, pCurStr) || !pCurStr)//调用链表的方法，获取当前行字符串指针
			break;//获取当前行的字符串失败，中断循环
		ch = GetStdCh(chCtrl);//从标准输入设备读取字符，不回显
		if (chCtrl > 0 )
		{//是控制字符
			if (!OnEditCommand(chCtrl, pCurStr))//处理编辑状态快捷键
				break;
		}
		else if (ch >=' ' || ch == '\t')
		{//获取的字符是可显示字符或制表键
			if (ch & 0xff00)//汉字时ch的高8位不为0
			{//是汉字，由两个字符组成
				char chHi = ch>> 8;//取高位字符
				char chLo = ch & 0xff;//取低位字符
				if (m_bReplace)
				{//当前处于替换状态
					unsigned char chOld = (*pCurStr)[m_nXCursor];
					if (chOld >= 0xa0)
					{//原来的字符是汉字
						pCurStr->SetElem(m_nXCursor, chHi);//高位替换原有字符
						m_nXCursor++;
						pCurStr->SetElem(m_nXCursor, chLo);//插入低位字符
						m_nXCursor++;
					}
					else
					{
						pCurStr->SetElem(m_nXCursor, chHi);//高位替换原有字符
						m_nXCursor++;
						pCurStr->Insert(m_nXCursor, chLo);//插入低位字符
						m_nXCursor++;
					}
				}
				else
				{//当前处于插入状态
					pCurStr->Insert(m_nXCursor, chHi);//插入高位字符
					m_nXCursor++;
					pCurStr->Insert(m_nXCursor, chLo);//插入低位字符
					m_nXCursor++;
				}
			}
			else
			{
				if (m_bReplace)
				{
					unsigned char chOld = (*pCurStr)[m_nXCursor];
					if (chOld >= 0xa0)
					{//原来的字符是汉字
						pCurStr->SetElem(m_nXCursor, ch);//替换原有字符
						char chNext;
						pCurStr->Delete(m_nXCursor + 1, chNext);//删除掉汉字的第二个字符
						
					}
					else
						pCurStr->SetElem(m_nXCursor, ch);//替换原有字符
				}
				else
					pCurStr->Insert(m_nXCursor, ch);//插入模式
				m_nXCursor++;

			}
			PrintLine(pCurStr, m_nYCursor);//打印当前行
		}

	} while (ch !=27);//esc键的ascii码是27，输入字符是esc时返回命令状态
	WND_SIZE size = GetWindSize();
	MoveCursor(size.rows - 1, 0);
	printf("\033[K--command mode,input your command--");//清除光标到行尾
}
void TextEditor::Help(const CharString& strCommand)
{
	printf("\033[2J");//清除屏幕
	WND_SIZE size = GetWindSize();
	bool bHelpAll = false;//
	MoveCursor(0, 0);
	if (strCommand == "e" || strCommand == "edit")
	{//edit 命令的解释
		printf("command:\tedit\nfunction:\tchange to edit mode\nparam:\t\tunnecessary\ncommand with equivalent function:e");
	}
	else if (strCommand == "?" || strCommand == "help" || strCommand == "h")
	{//help 命令帮助
		printf("command:\thelp\nfunction:\tshow help message\nparam:\t\tcommand name\ncommand with equivalent function:？ or h\nusage:\t\thelp [command]\nexample:\thelp load");
	}
	else if (strCommand == "l" || strCommand == "load")
	{//调文件命令帮助
		printf("command:\tload\nfunction:\tload a text file into memory\nparam:\t\tfilename\ncommand with equivalent function: l\nusage:\t\tload <filename>\nexample:\tload test.txt");

	}
	else if (strCommand == "q" || strCommand == "quit")
	{
		printf("command:\tquit\nfunction:\texit program\nparam:\t\tunnecessary\ncommand with equivalent function:q");
	}
	else if (strCommand == "s" || strCommand == "save")
	{//保存文件帮组
		printf("command:\tsave\nfunction:\tsave memory strings to text file\nparam:\t\tfilename or unnecessary\ncommand with equivalent function: s\nnotes:if no param,save to loaded filename\nusage:\t\tsave [filename]\nexample:\tsave test.txt");

	}
	else if (strCommand == "f" || strCommand == "find")
	{//查找字符串帮助
		printf("command:\tfind\nfunction:\tfind pattern string\nparam:\t\tpattern string\ncommand with equivalent function: f\nusage:\t\tfind <pattern string>\nexample:\tfind abc");

	}
	else if (strCommand == "g" || strCommand == "go")
	{
		printf("command:\tgo\nfunction:\tcurrent edit line goto line number\nparam:\t\tline number\ncommand with equivalent function: g\nusage:\t\tgo <line number>\nexample:\tgo 2");
	}
	else
		bHelpAll = true;
	if (bHelpAll)
	{

		printf("**********************************************\nwellcom using text editor,command:\ne: change to edit mode\nl <filename>: load a file\ng <line-number>:cursor goto line number \nd <line-number>: delete a line \nf <pattern string>: find a string\ns [filename]: save to file\nq: quit\n(edit mode change to command mode input esc)\nCopy right: Sichuan university\n**********************************************\n");
	}

}

void TextEditor::Run(const CharString& strFileName)
{
	Help("");//启动时显示全部帮助信息
	if(!strFileName.IsEmpty())
		Load(strFileName);//如果启动时的文件名不为空，调文件
	WND_SIZE size = GetWindSize();//获取当前窗口尺寸
	//移动光标到窗口的倒数第一行
	MoveCursor(size.rows - 1, 0);
	//在倒数第一行清除光标到行尾，并打印提示信息
	printf("\033[K--command mode,input your command--");
	while (GetCommand())
	{//命令行状态循环输入命令
		if (m_bEdit)
			Edit();//当编辑标志为真时，进入编辑状态
	} 
}
#endif