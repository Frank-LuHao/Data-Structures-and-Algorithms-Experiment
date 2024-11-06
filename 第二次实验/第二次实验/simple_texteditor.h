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
	//bool NextLine();//��һ��
	//bool PrevLine();//��һ��
	//bool GotoLine();//ת��ָ����
	//bool InsertLine();//������
	//bool ChangeLine();//�޸���
	bool ReadFile();//���ļ�
	bool WriteFile();//д�ļ�
	//void FindString();//�����ַ���
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
	printf("�����б�\n");
	printf("H/h: ��ʾȫ��������Ϣ\n");
	printf("V/v: �鿴�ļ�����\n");
	printf("S/s: �����ļ�\n");
	printf("Q/q: �˳��༭��\n");
	return;
}
#endif