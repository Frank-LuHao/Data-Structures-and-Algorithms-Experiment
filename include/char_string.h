#if !defined(_CHAR_STRING_H_)
#define _CHAR_STRING_H_
#pragma warning(disable:4996)
#include "SqList.h"
#include <fstream>
#include <stdarg.h>
using namespace std;
#define FORCE_ANSI      0x10000
#define FORCE_UNICODE   0x20000
#define FORCE_INT64     0x40000
const char* _cstinc(const char* str)
{
	return ++str;
}
class CharString:public SqList<char>
{
public:
	CharString();
	CharString(const char* source);//c �ַ�������
	CharString(const CharString& source);// ���ƹ��캯��
	~CharString();
	CharString& operator =(const CharString& source);// ���ظ�ֵ�����
	CharString& operator +=(const CharString& source);// ���ء�+=�������
	CharString& operator +=(const char* source);// ���ء�+=�� �������sourceΪc����ַ���
	const char& operator [](int i) const;// �����±��������[]��	
	CharString& CopyN(const char* source, int n);//����c����ַ���n���ַ�
	int Index(const CharString& pattern, int pos = 0)const;//ƥ��ģʽ��
	int KMPIndex(const CharString& pattern, int pos = 0)const;//����KMP�㷨ƥ��ģʽ��
	const char* ToCStr() const;// ����ת����C���
	void Format(const char* lpszFormat, ...);//��ʽ����������
	int Compare(const CharString& source) const;//�ȶ�Դ�ַ�����Ŀ���ַ���
	int AddTail(const char ch);	//��Ҫĩβ���0������
	bool Delete(int position, char& ch);//��Ҫĩβ���0������
	bool Insert(int position, const char e);//��Ҫĩβ���0������
private:
	int SaveData(char* pTData, int nDataLen);//��Ҫĩβ���0������
	void  GetNextVal(const CharString& pattern, int* nextVal) const;//KMP�㷨��Next����
	void AppendZero();//׷���ַ���ĩβ�����ַ�'\0'


};

// ����ز���
CharString Read(istream& input);										// �����������봮
CharString Read(istream& input, char& terminalChar);					// �����������봮������terminalChar���ش������ַ�
CharString Read(ifstream& infile, char& terminalChar);				// ���ļ����봮������terminalChar���ش������ַ�
void Write(const CharString& s);										// �����
void Concat(CharString& target, const CharString& source);				// ����source���ӵ���target�ĺ���
void Copy(CharString& target, const CharString& source);				// ����source���Ƶ���target
void Copy(CharString& target, const CharString& source, int n);			// ����source����n���ַ�����target 
int Index(const CharString& target, const CharString& pattern, int pos = 0);// ����ģʽ��pattern��һ����Ŀ�괮target�дӵ�pos���ַ���ʼ���ֵ�λ��
CharString SubString(const CharString& s, int pos, int len);		// ��s�ĵ�pos���ַ���ʼ�ĳ���Ϊlen���Ӵ�
bool operator ==(const CharString& first, const CharString& second);	// ���ع�ϵ�����==
bool operator <(const CharString& first, const CharString& second);		// ���ع�ϵ�����<
bool operator >(const CharString& first, const CharString& second);		// ���ع�ϵ�����>
bool operator <=(const CharString& first, const CharString& second);	// ���ع�ϵ�����<=
bool operator >=(const CharString& first, const CharString& second);	// ���ع�ϵ�����>=
bool operator !=(const CharString& first, const CharString& second);	// ���ع�ϵ�����!=

// ���༰��ز�����ʵ�ֲ���
CharString::CharString(const char* source)
//��C����ַ�������
{
	int nLen = strlen(source);//��ȡԴ�ַ�������
	SaveData((char*)source,nLen);//���ø���ӿڴ�������
	AppendZero();//���ĩβ������'\0'
}
CharString& CharString::operator =(const CharString& source)
{
	int nSrcLen = source.Length();//��ȡԴ�ַ�������
	Clear();//���ԭ��������
	SaveData(source.m_pElemData, nSrcLen);//���ø���ӿڴ�������
	AppendZero();//���ĩβ������'\0'
	return *this;
}

CharString& CharString::operator +=(const CharString& source)
{
	int nSrcLen = source.Length();//��ȡԴ�ַ�������
	SaveData(source.m_pElemData, nSrcLen);//���ø���ӿ�׷�Ӵ�������
	AppendZero();//���ĩβ������'\0'
	return *this;
}
CharString& CharString::operator +=(const char* source)
{
	int nSrcLen = strlen(source);//��ȡԴ�ַ�������
	SaveData((char*)source, nSrcLen);//���ø���ӿ�׷�Ӵ�������
	AppendZero();//���ĩβ������'\0'
	return *this;
}
void CharString::Format(const char* lpszFormat, ...)
{
	va_list argList, argListSave;
	va_start(argList, lpszFormat);

	//	FormatV(lpszFormat,argList);


	va_start(argListSave, lpszFormat);


	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	for (const char* lpsz = lpszFormat; *lpsz != '\0'; lpsz = _cstinc(lpsz))
	{
		// handle '%' character, but watch out for '%%'
		if (*lpsz != '%' || *(lpsz = _cstinc(lpsz)) == '%')
		{
			nMaxLen += strlen(lpsz);
			continue;
		}

		int nItemLen = 0;

		// handle '%' character with format
		int nWidth = 0;
		for (; *lpsz != '\0'; lpsz = _cstinc(lpsz))
		{
			// check for valid flags
			if (*lpsz == '#')
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(argList, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' ||
				*lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = atoi(lpsz);
			for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = _cstinc(lpsz))
				;
		}
		//ASSERT(nWidth >= 0);

		int nPrecision = 0;
		if (*lpsz == '.')
		{
			// skip past '.' separator (width.precision)
			lpsz = _cstinc(lpsz);

			// get precision and skip it
			if (*lpsz == '*')
			{
				nPrecision = va_arg(argList, int);
				lpsz = _cstinc(lpsz);
			}
			else
			{
				nPrecision = atoi(lpsz);
				for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = _cstinc(lpsz))
					;
			}
			//ASSERT(nPrecision >= 0);
		}

		// should be on type modifier or specifier
		int nModifier = 0;
		if (memcmp(lpsz, "I64", 3) == 0)
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
		}
		else
		{
			switch (*lpsz)
			{
				// modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				lpsz = _cstinc(lpsz);
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				lpsz = _cstinc(lpsz);
				break;

				// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				lpsz = _cstinc(lpsz);
				break;
			}
		}

		// now should be on specifier
		switch (*lpsz | nModifier)
		{
			// single characters
		case 'c':
		case 'C':
			nItemLen = 2;
			va_arg(argList, char);
			break;
		case 'c' | FORCE_ANSI:
		case 'C' | FORCE_ANSI:
			nItemLen = 2;
			va_arg(argList, char);
			break;
		case 'c' | FORCE_UNICODE:
		case 'C' | FORCE_UNICODE:
			nItemLen = 2;
			va_arg(argList, short);
			break;

			// strings
		case 's':
		case 'S':
		case 's' | FORCE_UNICODE:
		case 'S' | FORCE_UNICODE:
		case 's' | FORCE_ANSI:
		case 'S' | FORCE_ANSI:

		{
			const char* pstrNextArg = va_arg(argList, const char*);
			if (pstrNextArg == NULL)
				nItemLen = 6;  // "(null)"
			else
			{
				nItemLen = strlen(pstrNextArg);
				nItemLen = max(1, nItemLen);
			}
		}
		break;

		}

		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			if (nPrecision != 0)
				nItemLen = min(nItemLen, nPrecision);
			nItemLen = max(nItemLen, nWidth);
		}
		else
		{
			switch (*lpsz)
			{
				// integers
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				if (nModifier & FORCE_INT64)
					va_arg(argList, long long);
				else
					va_arg(argList, int);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;

			case 'e':
			case 'g':
			case 'G':
				va_arg(argList, double);
				nItemLen = 128;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;

			case 'f':
			{
				double f;
				char* pszTemp;

				// 312 == strlen("-1+(309 zeroes).")
				// 309 zeroes == max precision of a double
				// 6 == adjustment in case precision is not specified,
				//   which means that the precision defaults to 6
				pszTemp = (char*)malloc(max(nWidth, 312 + nPrecision + 6));

				f = va_arg(argList, double);
				sprintf(pszTemp, "%*.*f", nWidth, nPrecision + 6, f);
				nItemLen = strlen(pszTemp);
				free(pszTemp);
			}
			break;

			case 'p':
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;

				// no output
			case 'n':
				va_arg(argList, int*);
				break;

			default:
				//ASSERT(false);  // unknown formatting option
				break;
			}
		}

		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}


	SetBufferSize(nMaxLen + 1);
	m_nDataLen = vsprintf(m_pElemData, lpszFormat, argListSave);
	
	m_pElemData[m_nDataLen] = 0;


	va_end(argListSave);
}
int CharString::Compare(const CharString& source) const
{
	int nRet = 0;
	int nLen = Length();
	if (source.Length() > nLen)
		nLen = source.Length();
	const char* pDst = m_pElemData;
	const char* pSrc = source.ToCStr();
	int i;
	for (i = 0; i < nLen; i++)
	{
		if (pSrc[i] != pDst[i])
			break;
	}
	if (pDst[i] > pSrc[i])
		return 1;
	else if (pDst[i] < pSrc[i])
		return -1;
	return 0;//���
}
CharString::CharString(const CharString& source)
//��Դ�ַ���������
{
	*this = source;//�����������'='������ֱ�Ӹ�ֵ����
}
CharString::CharString()
//��ͨ���죬���ַ���
{
	AppendZero();//���ĩβ������'\0'
}

CharString::~CharString()
{
}
void CharString::AppendZero()
{
	if (m_nDataLen >= m_nBufferLen)//�ж��Ƿ�������
	{
		SetBufferSize(m_nDataLen * 2+8);//�����෽����չ�ڲ�����
	}
	m_pElemData[m_nDataLen] = '\0';//ĩβ��'\0'�ַ��������ڴ������ڣ�����m_nDataLen����Ҫ��1
}
int CharString::AddTail(const char ch)
{
	int nRet = SqList<char>::AddTail(ch);//���ø��෽��
	AppendZero();
	return nRet;
}



bool CharString::Delete(int position, char& ch)
{
	bool bRet = SqList<char>::Delete(position, ch);
	AppendZero();
	return bRet;

}
bool CharString::Insert(int position, const char ch)
{
	bool bRet = SqList<char>::Insert(position, ch);//���ø��෽��
	AppendZero();
	return bRet;
}
int CharString::SaveData(char* pChData, int nDataLen)
{
	int nRet = SqList<char>::SaveData(pChData, nDataLen);
	AppendZero();
	return nRet;
}

CharString& CharString::CopyN(const char* source, int n)
{
	Clear();//�ȵ��ø��෽������ڲ�����
	int nSrcLen = strlen(source);
	if (n > nSrcLen)
		n = nSrcLen;//�����������ַ������ȣ�ȡ�ַ�������
	SaveData((char*)source, n);
	AppendZero();//���ĩβ������'\0'
	return *this;
}
const char* CharString::ToCStr() const
{
	return m_pElemData;//���ظ�������ݴ洢�ڴ�
}
const char& CharString::operator [](int index) const
{
	if (index < 0 || index>m_nDataLen)
	{
		throw std::runtime_error("out of bound!");
		return 0;
	}
	return m_pElemData[index];
}
int CharString::Index(const CharString& pattern, int pos) const
{
	int nPatLen = pattern.Length();//��ȡģʽ���ĳ���
	if (m_nDataLen - pos+1 < nPatLen)
		return -1;//����������Ȳ���������-1
	const char* pCstrPat = pattern.ToCStr();//ģʽ��ת��ΪC����ַ���
	int i,j;
	int nSumChar = 0;//���������ַ����ֵ,��ֵΪ0
	int nSumPat = 0;//ģʽ���ַ����ֵ,��ֵΪ0

	bool bMatched = true;//ƥ���־
	//��������״μ���nSumChar��nSumPat�������״�ƥ��
	for (i = pos,j= 0; j < nPatLen; i++,j++)
	{
		nSumPat += pCstrPat[j];//ģʽ�����
		nSumChar += m_pElemData[i];//����ǰnPatLen���ַ����
		if(m_pElemData[i] != pCstrPat[j])
			bMatched = false;
	}
	if (bMatched)
		return pos;//�״μ�ƥ��ɹ�
	int nMatchedPos = -1;//�洢ƥ�䴰��λ�ñ�������ֵΪ0
	char chPatHead = pCstrPat[0],chPatTail = pCstrPat[nPatLen-1];//����ģʽ����β�ַ�
	for (i = pos+1; i <= m_nDataLen - nPatLen; i++)
	{
		nSumChar += m_pElemData[i + nPatLen-1] - m_pElemData[i-1];//�󷽽�һ���ַ���ǰ����һ���ַ�
		if (nSumChar != nSumPat || m_pElemData[i] != chPatHead || m_pElemData[i + nPatLen - 1]!= chPatTail)
			continue;//���ֵ��ͬ����β�ַ���ͬ��ֱ����һ��ѭ��
		bMatched = true;//ƥ��ǰ����ƥ���־��ƥ��ʱ�����ƥ�䣬����ñ�־
		//�����ѭ��ƥ�䴮���ס�ĩ��λ�Ѿ�ƥ�䣬������ƥ����λ
		for (j = 1; j < nPatLen-1; j++)
		{
			if (m_pElemData[i + j] != pCstrPat[j])
			{//���ַ�����ȣ����ò�ƥ���־������Ҫ��ѭ��
				bMatched = false;
				break;
			}
		}
		if (bMatched)
		{
			nMatchedPos = i;//�ҵ�ƥ��λ��
			break;//�ҵ�ƥ��λ�ú󣬽���ѭ��
		}
	}
	return nMatchedPos;
}

void  CharString::GetNextVal(const CharString& pattern,int* nextVal) const
{
	//int  j = 0, k = -1;
	//nextVal[0] = -1;//��λΪ-1��ʼ����
	const char* pPattern = pattern.ToCStr();
	int nLen = pattern.Length();
	
	nextVal[0] = -1;//��ʼֵ��Ϊ-1
	int j = 0, k = -1;
	while (j < nLen-1)
	{
		if (k == -1 || pPattern[j] == pPattern[k]) 
			nextVal[++j] = ++k;
		else k = nextVal[k];//ʧ��ʱk����
	}

}
int  CharString::KMPIndex(const CharString& pattern, int pos)const
{
	int nPatLen = pattern.Length();//��ȡģʽ���ĳ���
	if (m_nDataLen - pos + 1 < nPatLen)
		return -1;//����������Ȳ���������-1
	const char* pCstrPattern = pattern.ToCStr();
	int* pnNext = new int[nPatLen];
	GetNextVal(pattern, pnNext);//��ȡNext����
	int i = pos,j =0;
	while (i < m_nDataLen && j < nPatLen) 
	{
		if (j == -1 || m_pElemData[i] == pCstrPattern[j])
		{//��ʼ����ƥ��ɹ�,jΪ-1ʱ�ȶ�ָ��ֱ�����ƣ���һ���ַ��ȶ�ʱģʽ����0��ʼ
			// ָ�����
			i++;
			j++;
		}
		else
			j = pnNext[j];// ģʽ��jָ�����next�������
	}
	delete[]pnNext;//ɾ��next����
	if (j < nPatLen) return -1;	// ƥ��ʧ��
	return i - j;			// ƥ��ɹ�

}

//��ش�����ʵ��

CharString Read(istream& input)
// ��������������������봮
{
	CharString strChar;												// ��ʱ���Ա�
	char ch;															// ��ʱ�ַ�
	while ((ch = input.peek()) != EOF &&								// peek()����������ȡһ���ַ�,������ָ�벻��
		(ch = input.get()) != '\n')										// get()����������ȡһ���ַ�,������ָ��ָ����һ�ַ�
	{	// ��������ַ�׷�����Ա���
		strChar.AddTail(ch);
	}
	return strChar;														// ������temList���ɵĴ�
}

CharString Read(istream& input, char& terminalChar)
// ��������������������봮������terminalChar���ش������ַ�
{
	CharString strChar;								// ��ʱ�ַ���
	char ch;										// ��ʱ�ַ�
	while ((ch = input.peek()) != EOF &&			// peek()����������ȡһ���ַ�,������ָ�벻��
		(ch = input.get()) != '\n')					// get()����������ȡһ���ַ�,������ָ��ָ����һ�ַ�
	{	// ��������ַ�׷�����Ա���
		strChar.AddTail(ch);;
	}
	terminalChar = ch;													// ��terminalChar���ش������ַ�
	return strChar;														// ������temList���ɵĴ�
}

CharString Read(ifstream& input, char& terminalChar)
{
	CharString strChar;													// ��ʱ�ַ���
	char ch;
	// ���ļ����봮
	while ((ch = input.peek()) != EOF &&								// peek()����������ȡһ���ַ�,������ָ�벻��
		(ch = input.get()) != '\n')										// get()����������ȡһ���ַ�,������ָ��ָ����һ�ַ�
	{	// ��������ַ�׷�����Ա���
		strChar.AddTail(ch);
	}
	terminalChar = ch;
	return strChar;
}

void Write(const CharString& s)
// ��������������
{
	cout << s.ToCStr() << endl;											// �����ֵ
}

void Concat(CharString& target, const CharString& source)
// �������������source���ӵ���target�ĺ���
{
	target += source;//�����ˡ�+=�������
}

void Copy(CharString& target, const CharString& source)
// �������������source���Ƶ���target
{
	target = source;//CharString ������'=',����ֱ�Ӹ�ֵ
}

void Copy(CharString& target, const CharString& source, int n)
// �������������source����n���ַ�����target 
{
	target.CopyN(source.ToCStr(), n);//����source��n���ַ�
}

int Index(const CharString& target, const CharString& pattern, int pos)
// ������������ƥ��ɹ�������ģʽ��pattern��һ����Ŀ�괮target�дӵ�pos
//	���ַ���ʼ���ֵ�λ��, ���򷵻�-1
{
	return target.Index(pattern, pos);
}

CharString SubString(const CharString& s, int pos, int len)
// ��ʼ��������s���ڣ�0 <= pos < s.Length()��0 <= len <= s.Length() - pos
// ������������ش�s�ĵ�pos���ַ���ʼ�ĳ���Ϊlen���Ӵ�
{
	CharString sub = "";//�Ӵ���ʼ��Ϊ�մ�
	if (0 <= pos && pos < s.Length() && 0 <= len)
	{	// ���ش�s�ĵ�pos���ַ���ʼ�ĳ���Ϊlen���Ӵ�
		len = (len < s.Length() - pos) ? len : (s.Length() - pos);		// �Ӵ�����sub
		sub.CopyN(s.ToCStr() + pos, len);
	}
	return sub;
}

bool operator ==(const CharString& first, const CharString& second)
// ������������ع�ϵ�����==
{
	return first.Compare(second) == 0;
}

bool operator <(const CharString& first, const CharString& second)
	// ������������ع�ϵ�����<
{
	return first.Compare(second) < 0;
}

bool operator >(const CharString& first, const CharString& second)
// ������������ع�ϵ�����>
{
	return first.Compare(second) > 0;
}

bool operator <=(const CharString& first, const CharString& second)
// ������������ع�ϵ�����<=
{
	return first.Compare(second) <= 0;
}

bool operator >=(const CharString& first, const CharString& second)
// ������������ع�ϵ�����>=
{
	return first.Compare(second) >= 0;
}

bool operator !=(const CharString& first, const CharString& second)
// ������������ع�ϵ�����!=
{
	return first.Compare(second) != 0;
}
#endif