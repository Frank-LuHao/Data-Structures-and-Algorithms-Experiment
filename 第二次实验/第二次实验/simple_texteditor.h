#pragma once
#if !defined(SIMPLE_TEXTEDITOR_H)
#define SIMPLE_TEXTEDITOR_H
#include "../../include/char_string.h"
#include "../../include/dbl_link_list.h"
using namespace std;

class SimpleTextEditor : public DblLkList<CharString*>
{
public:
	SimpleTextEditor(char fName[]);//���캯��
	~SimpleTextEditor();//��������
	void Run(); //�����ı��༭��
private:
	//��������
	void Clear();//����Clear����
	bool NextLine();//��һ��
	bool PrevLine();//��һ��
	void ShowCurRow();//��ʾ��ǰ��
	bool GotoLine();//ת��ָ����
	bool GotoFirstLine();//ת����һ��
	bool GotoLastLine();//ת�����һ��
	bool GotoLineAux(int nLineNum); //ת��ָ���и�������
	bool InsertLine();//������
	bool DeleteLine();//ɾ����ǰ��
	bool ReadFile();//���ļ�
	bool WriteFile(bool OutPutFlag = 1);//д�ļ�
	bool FindString();//�����ַ���
	bool FindStringAux(const CharString& strFind, int &nLineNum, int &nIndex);//�����ַ�����������
	bool ChangeLine();//�滻��ǰ�л������е�ָ���ı���
	bool ReplaceLineAux(const CharString& strReplace, int &nLineNum, int &nIndex);//�滻�ַ�����������
	void ViewFile();//�鿴�ļ�
	void Help();//����
	
	//���ݳ�Ա
	int m_nCurRow;//��ǰ��
	CharString m_strFileName;//�༭�ļ���
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
	{//��������б�����ַ���
		CharString* pStr;//���������ɾ�����ַ���ָ��
		if (Delete(0, pStr))//�����������Delete�����������һ�е��ַ���ָ��
			delete pStr;//ɾ����ָ��ָ��Ķ���
	}
	m_nCurRow = 0;
}

bool SimpleTextEditor::ReadFile()
{
	bool bLoaded = false;
	CharString strMsg;
	if (m_strFileName.IsEmpty())
	{
		printf( "�ļ�Ϊ�գ���ȡʧ�ܣ�\n");
	}
	else
	{
		FILE* pFile = NULL;
		if (!(pFile = fopen(m_strFileName.ToCStr(), "rb"))) 
		{
			printf("�ļ� \"%s\" δ�ҵ�", m_strFileName.ToCStr());
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
				printf("�ļ� \"%s\" ������һ���ı��ļ�", m_strFileName.ToCStr());
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
				m_strFileName = m_strFileName;//��ǰ�༭�ļ�Ϊ����
			}
			delete[]pBuf;//ɾ��������
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
			printf("�ļ� \"%s\" ��ʧ��", fileName.ToCStr());
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
	if (OutPutFlag)
	{
		printf("�ļ� \"%s\" ����ɹ�\n", m_strFileName.ToCStr());
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
		// ��ȡ�û����������
		do {
			printf("command:");
			do {
				UserCommand = getchar();
			} while (' ' == UserCommand);
		} while ('\n' == UserCommand );
		//���������
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
	
	//�˳�ʱ���Զ�����ѯ��
	char charSaveCommand;
	do {
		printf("�Ƿ�ѡ�񱣴棿��y/n��:");
		charSaveCommand = getchar();
		while ((CharTmp = getchar()) != EOF && CharTmp != '\n');  //���������
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
	printf("�����б�\n");
	printf("H/h: ��ʾȫ��������Ϣ\n");
	printf("V/v: �鿴�ļ�����\n");
	printf("S/s: �����ļ�\n");
	printf("Q/q: �˳��༭��\n");
	printf("N/n: ��һ��\n");
	printf("P/p: ��һ��\n");
	printf("G/g: ת��ָ����\n");
	printf("B/b: ת����һ��\n");
	printf("E/e: ת�����һ��\n");
	printf("I/i: ������\n");
	printf("D/d: ɾ����\n");
	printf("F/f: �����ַ���\n");
	printf("C/c: �滻�ַ���\n");
	return;
}

void SimpleTextEditor::ShowCurRow()
{
	CharString* pStrMsg;
	if (GetElem(m_nCurRow, pStrMsg))
		printf("%d: %s\n", m_nCurRow+1, pStrMsg->ToCStr());
	else 
		printf("�ļ�Ϊ��\n");
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
		printf("�Ѿ������ļ�ĩβ\n");
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
		printf("�Ѿ������ļ���ͷ\n");
	}
	return bPrevFlag;
}

bool SimpleTextEditor::GotoLine()
{
	int nLineNum;
	printf("�������кţ�");
	scanf("%d", &nLineNum);
	getchar(); //�����β���з�
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
		printf("�кų�����Χ\n");
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
	printf("������Ҫ������кţ�Ĭ�ϵ�ǰ�У���");
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
			printf("�кų�����Χ\n");
			return bInsertFlag;
		}
		nLineNum--;
	}
	printf("������Ҫ��������ݣ�");
	CharString StrMsg = Read(cin);
	CharString* pStrMsg = new CharString(StrMsg);
	bInsertFlag = Insert(nLineNum, pStrMsg);
	if (bInsertFlag)
	{
		m_nCurRow = nLineNum;
		printf("����ɹ�\n");
	}
	else
	{
		delete pStrMsg;
		printf("����ʧ��\n");
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
		printf("ɾ���ɹ�\n");
	}
	else
	{
		printf("ɾ��ʧ��\n");
	}
	return bDeleteFlag;
}

bool SimpleTextEditor::FindStringAux(const CharString& strFind, int &nLineNum, int &nIndex)
{
	bool bFindFlag = false;
	CharString* pStrMsg;
	nIndex = 0;
	Head();
	for (int i = 0; i < nLineNum; i++) Next(pStrMsg); //��λ����ǰ�е�ǰһ��
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
		printf("�Ƿ�ӵ�ǰ�п�ʼ���ң������ͷ��ʼ������y/n��:");
		charSearchBegin = getchar();
		while ((CharTmp = getchar()) != EOF && CharTmp != '\n');  //���������
		if (charSearchBegin == 'y') {
			nLineNum = m_nCurRow;
		}
		else if (charSearchBegin == 'n') {
			nLineNum = 0;
		}
	} while (charSearchBegin != 'y' && charSearchBegin != 'n');

	printf("������Ҫ���ҵ��ַ�����");
	strFind = Read(cin);
	if (strFind.IsEmpty())
	{
		printf("�����ַ���Ϊ��\n");
		return bFindFlag;
	}
	else if (FindStringAux(strFind, nLineNum, nIndex))
	{
		m_nCurRow = nLineNum;
		CharString* pStrMsg;
		GetElem(m_nCurRow, pStrMsg);
		printf("%d: %s\n", m_nCurRow + 1, pStrMsg->ToCStr());
		printf("�ڵ�%d�е�%d���ҵ��ַ���\n", m_nCurRow + 1, nIndex + 1);
		bFindFlag = true;
	}
	else
	{
		printf("δ�ҵ��ַ���\n");
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
		printf("�Ƿ�ֻ�滻��ǰ�У������滻�����У�����y/n��:");
		charChangeBegin = getchar();
		while ((CharTmp = getchar()) != EOF && CharTmp != '\n');  //���������
		if (charChangeBegin == 'y') {
			nLineNum = m_nCurRow;
		}
		else if (charChangeBegin == 'n') {
			nLineNum = 0;
		}
	} while (charChangeBegin != 'y' && charChangeBegin != 'n');

	printf("������Ҫ���ҵ��ַ�����");
	strFind = Read(cin);
	if (strFind.IsEmpty())
	{
		printf("�����ַ���Ϊ��\n");
		return bChangeFlag;
	}
	printf("������Ҫ�滻���ַ�����");
	strReplace = Read(cin);
	if (strReplace.IsEmpty())
	{
		printf("�滻�ַ���Ϊ��\n");
		return bChangeFlag;
	}

	if (charChangeBegin == 'y') 	{
		if (FindStringAux(strFind, nLineNum, nIndex))
		{
			if (ReplaceLineAux(strReplace, nLineNum, nIndex)) {
				printf("�滻�ɹ�\n");
				bChangeFlag = true;
			}
			else {
				printf("�滻ʧ��\n");}
			}
		else
		{
			printf("δ�ҵ��ַ���\n");
		}
	}
	else
	{
		while (FindStringAux(strFind, nLineNum, nIndex))
		{
				bChangeFlag &= ReplaceLineAux(strReplace, nLineNum, nIndex);
		}
		if (bChangeFlag) {
			printf("�滻�ɹ�\n");
		}
		else
		{
			printf("�滻ʧ��\n");
		}
	}
	
	return bChangeFlag;
}
#endif