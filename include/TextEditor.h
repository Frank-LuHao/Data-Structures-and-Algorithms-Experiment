/*******************************************************************
*�ļ�����TextEditor.h
*���ܣ�ʵ���ı��༭
*���ߣ������Σ��Ĵ���ѧ�����ѧԺ��
*˵����������Ϊ��ѧ��Դ����,δ��������������
*******************************************************************/
#if !defined(_TEXT_EDITOR_H_)
#define _TEXT_EDITOR_H_
#include "char_string.h"
#include "dbl_link_list.h"
#include <conio.h>
#if defined(_WIN32)
#include <windows.h> //��ȡ���ڳߴ���ǷǱ�׼��������ͬ����ϵͳ��ͬ
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
	//��ȡconsole���ڷֱ�����Ҫ���÷Ǳ�׼��������ͬ����ϵͳ��ͬ��
	//�������������Զ����ݲ���ϵͳѡ����Ҫ��������
#if defined(_WIN32)
	//windows����ϵͳ
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	size.rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	size.cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#elif defined(_gnu_linux_)
	//linux����ϵͳ
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
	void Run(const CharString& strFileName);//�����ı��༭��
	bool Load(const CharString& strFileName);
private:
	void Clear();//����Clear;
	void Edit();//�༭
	bool GetCommand();//��ȡ����������
	bool OnEditCommand(char chCtrl,CharString* pCurStr);//����༭����������ơ����Ƶ�
	bool Save(const CharString& strFileName);//�洢���ļ�
	unsigned short GetStdCh(char& chCtrl);//��ȡһ���ַ����Ҳ�����
	void Help(const CharString& strCommand);//����
	void ShowMessage(const CharString & strMsg,int nBkColor = 41);//��ʾ��Ϣ
	void PrintStrings(bool bClearAll = false);//��ӡȫ���ַ���
	void PrintLine(CharString * pStr,int nYCursor);//��ӡһ��
	void MoveCursor(int nYCursor, int nXCursor,bool bPrintLineNum = true);//�ƶ����
	void CheckChinesCursor(CharString* pStr);//��麺�ֹ��
	bool MoveYCursor(int nOffset, int nWndRows);//�����ƶ���ǰ��
	bool FindPatternStr(const CharString& strPattern, int nRow, int nCol);//����ģʽ��
	bool GotoLine(const CharString& strNumber);//�����ת��ָ����
	int GetTop(int nWndRows,bool &bPrintStrings);//��ȡ��ǰ��ʾ���ڶ����к�
	int m_nXCursor;//��ǰ��������
	int m_nYCursor;//��ǰ���������
	int m_nCurTop;//��ǰ��ʾ�Ķ���
	bool m_bReplace;//��ǰ�༭�ַ��Ƿ����滻״̬��false-����״̬,true-�滻״̬
	bool m_bEdit;//��ǰ�Ƿ��ڱ༭״̬��false-����״̬��tue-�༭״̬
	CharString m_strFileName;//��ǰ�༭���ļ���
	CharString m_strPattern;//��ǰ�ȶ�ģʽ��
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
	{//��������б�����ַ���
		CharString* pStr;//���������ɾ�����ַ���ָ��
		if (Delete(0, pStr))//�����������Delete�����������һ�е��ַ���ָ��
			delete pStr;//ɾ����ָ��ָ��Ķ���
	}
	m_nYCursor = 0;//��ǰ�й���
	m_nXCursor = 0;//��ǰ�й���
	m_nCurTop = 0;//��ǰ��ʾ�����й���
	m_strFileName = "";//��յ�ǰ�ļ���
	m_strPattern = "";//���ƥ��ģʽ��
}

unsigned short TextEditor::GetStdCh(char& chCtrl)
{
	unsigned short usCh = 0;
	unsigned char ch = getch();//�ӱ�׼��������ȡһ���ַ���������
	chCtrl = 0;//�����븳��ֵ0����ʾ���ǿ�����
	usCh = ch;
	if (ch == 0xe0 || ch==0)
	{//����ȡ���ַ�ascii��ֵΪ0xe0��0ʱ���ǿ����ַ�
		chCtrl = getch();//�ٴӱ�׼�������л�ȡ�����ַ�
	}
	else if(ch<' ' && ch !='\t')
	{//����ȡ���ַ�ascii��ֵС�ڿո�ʱ(��Enter��),Ҳ��ͬ�ڿ�ݼ�
		chCtrl = ch;
	}
	else if (ch >= 0xa0)
	{//�Ǻ���
		ch = getch();//�����������ַ���ɣ���Ҫ�ٴλ�ȡ�ڶ����ַ�
		//�������ַ������һ��short��(16λ)�ַ���
		usCh <<= 8;//��һ���ַ�����8λ��ռ�ݸ�8λ
		usCh |= ch;//�ڶ����ַ�ս����8λ
	}
	return usCh;
}
int TextEditor::GetTop(int nWndRows, bool& bPrintStrings)
{
	int nTopRow = m_nCurTop;
	bPrintStrings = false;
	if (m_nYCursor -nTopRow >= nWndRows-3)
	{
		m_nCurTop = nTopRow = (m_nYCursor + 3) - nWndRows;//��Ҫ����1�и�������
		bPrintStrings = true;//��Ҫ���´�ӡ�����ַ���
	}
	else if(m_nYCursor - nTopRow<0)
	{
		if (nTopRow > 0)
		{
			nTopRow--;
			m_nCurTop = nTopRow;
			bPrintStrings = true;//��Ҫ���´�ӡ�����ַ���
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
	{//�Ǳ༭״̬��ֱ���ƶ������λ��
		printf("\033[%d;%dH", nYCursor + 1, nXCursor + 1);
		return;
	}
	//�༭״̬
	char strLineHead[32] = { 0 };
	if (bPrintLineNum)
	{

		CharString* pCursorStr;

		sprintf(strLineHead, "%-3d", nYCursor + 1);
		int nHeadLen = strlen(strLineHead);
		if (GetElem(nYCursor, pCursorStr))
		{//��ȡ��������е��ַ���
			//��������Ҫ�ƶ���λ��
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
	{//��Ҫ��ӡ�к�
		printf("\033[?25l");//���ع��
		printf("\033[%d;%dH\033[46m%s", nYCursor + 1 - nTop, 1, strLineHead);
		printf("\033[?25h\033[40m");//�ָ����
	}
	printf("\033[%d;%dH", nYCursor + 1- nTop, nXCursor +1);

}
void TextEditor::PrintStrings(bool bClearAll)
{
	printf("\033[?25l");//���ع��
	printf("\033[0;0H");//���ص�ԭ��
	CharString* pStrLine;
	int nLineNo = 0;//��ǰ��
	WND_SIZE size = GetWindSize();
	bool bPrintStrings;
	int nTop = GetTop(size.rows, bPrintStrings);
	Head();//��ǰ�ڵ�ص�ͷ�ڵ�
	while (Next(pStrLine) &&pStrLine)//��������
	{
		if (nLineNo < nTop)
		{
			nLineNo++;
			continue;
		}
		if (nLineNo - nTop > size.rows - 3)
			break;
		MoveCursor(nLineNo, 0);
		printf("\033[K%s\n", pStrLine->ToCStr());//��ӡһ�У�������
		nLineNo++;
	}
	if (!bClearAll)
	{
		MoveCursor(nLineNo, 0, false);//����Ҫ��ӡ�к�
		printf("\033[K");//�����һ�У�����ɾ���к����һ����ʾû�����
	}
	else
	{
		while (nLineNo - nTop <= size.rows - 3)
		{
			MoveCursor(nLineNo, 0, false);//����Ҫ��ӡ�к�
			printf("\033[K");
			nLineNo++;
		}
	}
	
	if (m_bReplace)
		printf("\033[%d;%dH\033[K%s", size.rows, 0, "--Replace,Press \'ESC\' to return command mode--");
	else
		printf("\033[%d;%dH\033[K%s", size.rows, 0, "--Insert,Press \'ESC\' to return command mode--");
	MoveCursor(m_nYCursor, m_nXCursor);//���ص���ǰλ��
	printf("\033[?25h");//�ָ����
}
void TextEditor::PrintLine(CharString* pStr,int nYCursor)
{
	if (!pStr)
		return;	
	printf("\033[?25l");//���ع��
	MoveCursor(nYCursor,0);//��굽��ӡ����
	printf("\033[K");//�����굽��β������
	printf("%s", pStr->ToCStr());//��ӡһ��,������
	printf("\033[?25h");//�ָ����
	MoveCursor(m_nYCursor , m_nXCursor);//���ص���ǰλ��
}
void TextEditor::ShowMessage(const CharString& strMsg,int nBkColor)
{
	if (m_bEdit)
		return;
	printf("\033[?25l");//���ع��
	WND_SIZE size = GetWindSize();
	MoveCursor(size.rows - 1, 0);
	printf("\033[K");//�����굽��β
	printf("\033[%dm%s", nBkColor,strMsg.ToCStr());//���屳����ɫ��ɫ����ʾ��ʾ��Ϣ
	MoveCursor(size.rows - 2, 0);
	printf("\033[40m\033[K>>");//���������
	printf("\033[?25h");//�ָ����

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
	{//��ģʽ��Ϊ��ʱ����ӡ������Ϣ
		strMsg.Format("find command must have param, usage: f - <pattern string>");
		ShowMessage(strMsg);
	}
	else
	{
		CharString* pStrLine;
		Head();//��ǰ�ڵ�ص�ͷ�ڵ�
		int nLineNum = 0;

		while (Next(pStrLine) && pStrLine)
		{//ͨ�������Next()������������
			if (nLineNum < nRow)
			{//��ǰ��С�ڲ�����ʼ��ʱ����������
				nLineNum++;//�кż�1
				continue;//�������±���
			}
			int pos = 0;
			if (nLineNum == nRow)
				pos = nCol;//�к�Ϊ������ʱ���к�Ϊ��ʼ�У�����Ϊ0
			int index = pStrLine->KMPIndex(strPattern, pos);//����KMPģʽ��ƥ���㷨ƥ���ַ���
			if (index >= 0)
			{//�ҵ�ƥ���ַ�
				bFind = true;//�����ҵ���־
				m_strPattern = strPattern;//���õ�ǰģʽ���������ڱ༭״̬ʱ��F3��������һ����
				m_nXCursor = index;//���õ�ǰ��������
				m_nYCursor = nLineNum; //���õ�ǰ���������
				break;//�жϱ���
			}
			nLineNum++;//�кż�1
		}
		if (!bFind && !m_bEdit)
		{//û���ҵ�ƥ���ַ�,�Ҳ��Ǳ༭ģʽʱ����ӡ��Ϣ
			strMsg = "can\'t find string: ";
			strMsg += strPattern;
			ShowMessage(strMsg);
		}
		else if (!m_bEdit && bFind)
		{//�ҵ��Ҳ�Ϊ�༭״̬ʱ������������ʾ����ӡ��Ϣ
			strMsg.Format("find string %s,press F3 to find next string,press any key to return edit mode", strPattern.ToCStr());
			ShowMessage(strMsg,40);//��ӡ��Ϣ
			getchar();//�ȴ����������
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
			fseek(pFile, 0, SEEK_END);//���ļ�β
			int nFLen = ftell(pFile);//����ļ�����
			char* pBuf = new char[nFLen + 1];
			fseek(pFile, 0, SEEK_SET);//�ص��ļ�ͷ
			fread(pBuf, 1, nFLen, pFile);//��ȡ�ļ�
			fclose(pFile);
			pBuf[nFLen] = 0;
			//�ж��ļ��Ƿ����ı��ļ�
			char* p = pBuf;
			while ('\0' != *p) p++;
			if (p - pBuf + 1 < nFLen)
			{//�����ı��ļ�
				strMsg.Format("file \"%s\" is not a text file", strFileName.ToCStr());
				ShowMessage(strMsg);
			}
			else
			{
				//��������ַ�������
				Clear();
				const char* pLine = pBuf;
				p = (char*)pLine;
				while ('\0' != *p)
				{
					if ('\n' == *p)
					{//�ҵ����з�
						*p = '\0';//����C����ַ���
						CharString* pNewStr = new CharString(pLine);
						AddTail(pNewStr);//���һ��
						pLine = p + 1;
					}
					p++;
				}
				if (pLine < pBuf + nFLen)
				{//���û�л��з������һ��
					CharString* pNewStr = new CharString(pLine);
					AddTail(pNewStr);//���һ��
				}
				bLoaded = true;
				m_strFileName = strFileName;//��ǰ�༭�ļ�Ϊ����
			}
			delete[]pBuf;//ɾ��������
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
			Head();//��ǰ�ڵ�ص�ͷ�ڵ�
			int nLineNum = 0;
			while (Next(pStrLine) && pStrLine)//��������
			{
				if (pStrLine->Length() > 0)//�����ȴ���0ʱд���ļ�
					fwrite(pStrLine->ToCStr(), pStrLine->Length(), 1, pFile);
				if (nLineNum < Length() - 1)//�������һ��д�뻻�з�
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
	printf("\033[?25l");//���ع��

	WND_SIZE size = GetWindSize();
	MoveCursor(size.rows-2,0);
	printf("\033[32m%s\033[37m>>",m_strFileName.ToCStr());
	printf("\033[?25h");//�ָ����
	printf("\033[K");//�����굽��β
	char szBuffer[256] = { 0 };
	scanf("%255[^\n]", szBuffer);//���������ַ�����ʽ��������ո���ַ���
	getchar();//����\n
	char* pCmd = szBuffer;
	while ( *pCmd <= ' ' && *pCmd !='\0')pCmd++;//ȥ�����ַ�����ʼ�Ĳ��ɼ��ַ��Ϳո�
	char* pPara = pCmd + 1;
	while (*pPara > ' ' && *pPara != '\0') pPara++;//�ҵ��ɿ����ַ���ո�
	while ((*pPara <= ' '|| *pPara=='=') && *pPara != '\0')
	{//ȥ���������еĲ��ɼ������ַ�
		*pPara = '\0';
		pPara++;
	}
	
	CharString strCommand = pCmd, strParam = pPara;//��������Ͳ���
	CharString strMsg;
	bool bContinue = true;//������ֵΪ��
	if (strCommand == "e" || strCommand == "edit")
	{//�л����༭״������Ҫ����
		m_bEdit = true;
	}
	else if (strCommand == "?" || strCommand == "help" || strCommand =="h")
	{//help �������
		Help(strParam);
	}
	else if (strCommand == "l" || strCommand == "load")
	{//���ļ�����Ҫ����
		if (Load(strParam))
			m_bEdit = true;//�����ļ��ɹ���תΪ�༭ģʽ
	}
	else if (strCommand == "q" || strCommand == "quit")
		bContinue = false;
	else if (strCommand == "s" || strCommand == "save")
	{//�����ļ�
		if (Save(strParam))
			m_bEdit = true;//�洢�ļ��ɹ���תΪ�༭ģʽ
	}
	else if (strCommand == "f" || strCommand == "find")
	{
		if (FindPatternStr(strParam, m_nYCursor, 0))//�����ַ���
			m_bEdit = true;//�����ַ����ɹ����л����༭ģʽ
	}
	else if (strCommand == "g" || strCommand == "go")
	{
		if (GotoLine(strParam))
			m_bEdit = true;//��ת��ָ���гɹ����л����༭ģʽ
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
	{//��鵱ǰ��괦��ĺ���
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
	{//�Ƿ�ҳ
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
	case 'R'://insert��
		m_bReplace = !m_bReplace;
		PrintStrings();
		break;
	case 'G'://home��
		m_nYCursor = 0;//��ʼ��
		m_nXCursor = 0;//��ʼ��
		m_nCurTop = 0;//��ʾ��ʼ��Ϊ0
		PrintStrings();
		break;
	case 'O'://End��
		if (!GetElem(nLines - 1, pCurStr))
			break;
		m_nYCursor = nLines - 1;//���һ��
		m_nXCursor = pCurStr->Length();//���һ�е����һ���ַ�֮��
		PrintStrings();
		break;
	case 'H'://up ��
		if (m_nYCursor > 0)
			bSucessed = MoveYCursor(-1,size.rows);
		break;
	case 'P'://down ��
		if (m_nYCursor < Length() - 1)
			bSucessed = MoveYCursor(+1, size.rows);
		break;
	case 'I'://page up��
		if (m_nYCursor >0)
			bSucessed = MoveYCursor(-(size.rows - 2), size.rows);
		break;
	case 'Q'://page down��
		if (m_nYCursor + size.rows - 2 < nLines)
			bSucessed = MoveYCursor(size.rows - 2, size.rows);
		else
			MoveCursor(nLines - 1, 0);

		break;
	case 'K'://left��
		if (pCurStr && m_nXCursor > 0)
		{
			unsigned char* pStrCh = (unsigned char*)pCurStr->ToCStr();
			if (pStrCh[m_nXCursor - 1] >= 0xa0)
				m_nXCursor -= 2;//�Ǻ���,ռ�������ַ�
			else
				m_nXCursor--;

			MoveCursor(m_nYCursor, m_nXCursor);
		}
		break;
	case 'M'://right��
		if (pCurStr)
		{
			int nCurMax = pCurStr->Length() + 1;
			if (m_bReplace && m_nXCursor > 0)
				nCurMax--;
			if (m_nXCursor + 1 < nCurMax)
			{
				unsigned char* pStrCh = (unsigned char*)pCurStr->ToCStr();
				if (pStrCh[m_nXCursor] > 0xa0)
					m_nXCursor += 2;//�Ǻ���,ռ�������ַ�
				else
					m_nXCursor++;

				MoveCursor(m_nYCursor, m_nXCursor);
			}
		}
		break;
	case '='://F3��
		if (!m_strPattern.IsEmpty())
		{//ģʽ����Ϊ��ʱ,������һ��ƥ���ַ���
			if (FindPatternStr(m_strPattern, m_nYCursor, m_nXCursor + 1))
				PrintStrings();
		}
		break;
	case '\b':// ɾ��ǰһ���ַ�
		if (m_nXCursor == 0)
		{//�ѻ���ɾ��
			CharString* pPreStr = NULL;
			if (GetElem(m_nYCursor - 1, pPreStr))
			{
				if (Delete(m_nYCursor, pCurStr))
				{
					*pPreStr += *pCurStr;//ƴ�ӵ���һ��
					delete pCurStr;
					m_nYCursor--;
					pCurStr = pPreStr;
					m_nXCursor = pCurStr->Length();
					printf("\033[?25l");//���ع��
					PrintStrings();//��Ҫ���´�ӡ
					printf("\033[?25h");//�ָ����
				}
			}
		}
		else
		{
			char chDel;
			pCurStr->Delete(m_nXCursor - 1, chDel);//��ǰ��ɾ����ǰһ���ַ�
			m_nXCursor--;//�������
			if ((unsigned char)chDel >= 0xa0)
			{//��һ������
				pCurStr->Delete(m_nXCursor - 1, chDel);
				m_nXCursor--;//�������
			}

			PrintLine(pCurStr, m_nYCursor);
		}
		break;
	case 'S'://Delete��
		if (m_nXCursor >= pCurStr->Length())
		{//�ѻ���ɾ��
			CharString* pNextStr = NULL;
			if (Delete(m_nYCursor + 1, pNextStr))
			{
				*pCurStr += *pNextStr;//ƴ�ӵ���һ��
				delete pNextStr;
				PrintStrings();//��Ҫ���´�ӡ
			}
		}
		else
		{
			char chDel;
			pCurStr->Delete(m_nXCursor, chDel);//��ǰ��ɾ������һ���ַ�
			if ((unsigned char)chDel >= 0xa0)
			{//��һ�����֣���ɾ��һ��
				pCurStr->Delete(m_nXCursor, chDel);
			}
			PrintLine(pCurStr, m_nYCursor);
		}
		break;
	case '\r'://�س���
		if (m_bReplace)
		{//����
			if (m_nYCursor < nLines - 1)
			{
				m_nYCursor++;
				m_nXCursor = 0;
				MoveCursor(m_nYCursor, m_nXCursor);
			}

		}
		else
		{//����һ��
			CharString* pStr = new CharString;
			//�ѵ�ǰ���������ɾ�������뵽����
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

			printf("\033[?25l");//���ع��
			PrintStrings();//�������һ�л�����Ҫ���´�ӡ
			printf("\033[?25h");//�ָ����

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
	printf("\033[2J");//�����Ļ
	PrintStrings();//��ӡ�ַ���
	if (nLines == 0)
	{//��������
		CharString* pStr = new CharString;
		Insert(0, pStr);
		m_nXCursor = 0;
		m_nYCursor = 0;
	}
	char chCtrl;//�����ַ�
	unsigned short ch;//�ı��ַ���(���ڿ����Ǻ��֣�����Ϊ16λ��short����)
	do
	{
		CharString* pCurStr = NULL;//��ǰ���ַ���ָ��
		if (!GetElem(m_nYCursor, pCurStr) || !pCurStr)//��������ķ�������ȡ��ǰ���ַ���ָ��
			break;//��ȡ��ǰ�е��ַ���ʧ�ܣ��ж�ѭ��
		ch = GetStdCh(chCtrl);//�ӱ�׼�����豸��ȡ�ַ���������
		if (chCtrl > 0 )
		{//�ǿ����ַ�
			if (!OnEditCommand(chCtrl, pCurStr))//����༭״̬��ݼ�
				break;
		}
		else if (ch >=' ' || ch == '\t')
		{//��ȡ���ַ��ǿ���ʾ�ַ����Ʊ��
			if (ch & 0xff00)//����ʱch�ĸ�8λ��Ϊ0
			{//�Ǻ��֣��������ַ����
				char chHi = ch>> 8;//ȡ��λ�ַ�
				char chLo = ch & 0xff;//ȡ��λ�ַ�
				if (m_bReplace)
				{//��ǰ�����滻״̬
					unsigned char chOld = (*pCurStr)[m_nXCursor];
					if (chOld >= 0xa0)
					{//ԭ�����ַ��Ǻ���
						pCurStr->SetElem(m_nXCursor, chHi);//��λ�滻ԭ���ַ�
						m_nXCursor++;
						pCurStr->SetElem(m_nXCursor, chLo);//�����λ�ַ�
						m_nXCursor++;
					}
					else
					{
						pCurStr->SetElem(m_nXCursor, chHi);//��λ�滻ԭ���ַ�
						m_nXCursor++;
						pCurStr->Insert(m_nXCursor, chLo);//�����λ�ַ�
						m_nXCursor++;
					}
				}
				else
				{//��ǰ���ڲ���״̬
					pCurStr->Insert(m_nXCursor, chHi);//�����λ�ַ�
					m_nXCursor++;
					pCurStr->Insert(m_nXCursor, chLo);//�����λ�ַ�
					m_nXCursor++;
				}
			}
			else
			{
				if (m_bReplace)
				{
					unsigned char chOld = (*pCurStr)[m_nXCursor];
					if (chOld >= 0xa0)
					{//ԭ�����ַ��Ǻ���
						pCurStr->SetElem(m_nXCursor, ch);//�滻ԭ���ַ�
						char chNext;
						pCurStr->Delete(m_nXCursor + 1, chNext);//ɾ�������ֵĵڶ����ַ�
						
					}
					else
						pCurStr->SetElem(m_nXCursor, ch);//�滻ԭ���ַ�
				}
				else
					pCurStr->Insert(m_nXCursor, ch);//����ģʽ
				m_nXCursor++;

			}
			PrintLine(pCurStr, m_nYCursor);//��ӡ��ǰ��
		}

	} while (ch !=27);//esc����ascii����27�������ַ���escʱ��������״̬
	WND_SIZE size = GetWindSize();
	MoveCursor(size.rows - 1, 0);
	printf("\033[K--command mode,input your command--");//�����굽��β
}
void TextEditor::Help(const CharString& strCommand)
{
	printf("\033[2J");//�����Ļ
	WND_SIZE size = GetWindSize();
	bool bHelpAll = false;//
	MoveCursor(0, 0);
	if (strCommand == "e" || strCommand == "edit")
	{//edit ����Ľ���
		printf("command:\tedit\nfunction:\tchange to edit mode\nparam:\t\tunnecessary\ncommand with equivalent function:e");
	}
	else if (strCommand == "?" || strCommand == "help" || strCommand == "h")
	{//help �������
		printf("command:\thelp\nfunction:\tshow help message\nparam:\t\tcommand name\ncommand with equivalent function:�� or h\nusage:\t\thelp [command]\nexample:\thelp load");
	}
	else if (strCommand == "l" || strCommand == "load")
	{//���ļ��������
		printf("command:\tload\nfunction:\tload a text file into memory\nparam:\t\tfilename\ncommand with equivalent function: l\nusage:\t\tload <filename>\nexample:\tload test.txt");

	}
	else if (strCommand == "q" || strCommand == "quit")
	{
		printf("command:\tquit\nfunction:\texit program\nparam:\t\tunnecessary\ncommand with equivalent function:q");
	}
	else if (strCommand == "s" || strCommand == "save")
	{//�����ļ�����
		printf("command:\tsave\nfunction:\tsave memory strings to text file\nparam:\t\tfilename or unnecessary\ncommand with equivalent function: s\nnotes:if no param,save to loaded filename\nusage:\t\tsave [filename]\nexample:\tsave test.txt");

	}
	else if (strCommand == "f" || strCommand == "find")
	{//�����ַ�������
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
	Help("");//����ʱ��ʾȫ��������Ϣ
	if(!strFileName.IsEmpty())
		Load(strFileName);//�������ʱ���ļ�����Ϊ�գ����ļ�
	WND_SIZE size = GetWindSize();//��ȡ��ǰ���ڳߴ�
	//�ƶ���굽���ڵĵ�����һ��
	MoveCursor(size.rows - 1, 0);
	//�ڵ�����һ�������굽��β������ӡ��ʾ��Ϣ
	printf("\033[K--command mode,input your command--");
	while (GetCommand())
	{//������״̬ѭ����������
		if (m_bEdit)
			Edit();//���༭��־Ϊ��ʱ������༭״̬
	} 
}
#endif