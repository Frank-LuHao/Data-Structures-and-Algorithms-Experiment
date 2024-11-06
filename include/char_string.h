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
	CharString(const char* source);//c 字符串构造
	CharString(const CharString& source);// 复制构造函数
	~CharString();
	CharString& operator =(const CharString& source);// 重载赋值运算符
	CharString& operator +=(const CharString& source);// 重载‘+=’运算符
	CharString& operator +=(const char* source);// 重载‘+=’ 运算符，source为c风格字符串
	const char& operator [](int i) const;// 重载下标运算符‘[]’	
	CharString& CopyN(const char* source, int n);//拷贝c风格字符串n个字符
	int Index(const CharString& pattern, int pos = 0)const;//匹配模式串
	int KMPIndex(const CharString& pattern, int pos = 0)const;//采用KMP算法匹配模式串
	const char* ToCStr() const;// 将串转换成C风格串
	void Format(const char* lpszFormat, ...);//格式化输入数据
	int Compare(const CharString& source) const;//比对源字符串和目的字符串
	int AddTail(const char ch);	//需要末尾添加0，重载
	bool Delete(int position, char& ch);//需要末尾添加0，重载
	bool Insert(int position, const char e);//需要末尾添加0，重载
private:
	int SaveData(char* pTData, int nDataLen);//需要末尾添加0，重载
	void  GetNextVal(const CharString& pattern, int* nextVal) const;//KMP算法求Next数组
	void AppendZero();//追加字符串末尾结束字符'\0'


};

// 串相关操作
CharString Read(istream& input);										// 从输入流读入串
CharString Read(istream& input, char& terminalChar);					// 从输入流读入串，并用terminalChar返回串结束字符
CharString Read(ifstream& infile, char& terminalChar);				// 从文件读入串，并用terminalChar返回串结束字符
void Write(const CharString& s);										// 输出串
void Concat(CharString& target, const CharString& source);				// 将串source连接到串target的后面
void Copy(CharString& target, const CharString& source);				// 将串source复制到串target
void Copy(CharString& target, const CharString& source, int n);			// 将串source复制n个字符到串target 
int Index(const CharString& target, const CharString& pattern, int pos = 0);// 查找模式串pattern第一次在目标串target中从第pos个字符开始出现的位置
CharString SubString(const CharString& s, int pos, int len);		// 求串s的第pos个字符开始的长度为len的子串
bool operator ==(const CharString& first, const CharString& second);	// 重载关系运算符==
bool operator <(const CharString& first, const CharString& second);		// 重载关系运算符<
bool operator >(const CharString& first, const CharString& second);		// 重载关系运算符>
bool operator <=(const CharString& first, const CharString& second);	// 重载关系运算符<=
bool operator >=(const CharString& first, const CharString& second);	// 重载关系运算符>=
bool operator !=(const CharString& first, const CharString& second);	// 重载关系运算符!=

// 串类及相关操作的实现部分
CharString::CharString(const char* source)
//由C风格字符串构造
{
	int nLen = strlen(source);//获取源字符串长度
	SaveData((char*)source,nLen);//调用父类接口存入数据
	AppendZero();//添加末尾结束符'\0'
}
CharString& CharString::operator =(const CharString& source)
{
	int nSrcLen = source.Length();//获取源字符串长度
	Clear();//清空原来的数据
	SaveData(source.m_pElemData, nSrcLen);//调用父类接口存入数据
	AppendZero();//添加末尾结束符'\0'
	return *this;
}

CharString& CharString::operator +=(const CharString& source)
{
	int nSrcLen = source.Length();//获取源字符串长度
	SaveData(source.m_pElemData, nSrcLen);//调用父类接口追加存入数据
	AppendZero();//添加末尾结束符'\0'
	return *this;
}
CharString& CharString::operator +=(const char* source)
{
	int nSrcLen = strlen(source);//获取源字符串长度
	SaveData((char*)source, nSrcLen);//调用父类接口追加存入数据
	AppendZero();//添加末尾结束符'\0'
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
	return 0;//相等
}
CharString::CharString(const CharString& source)
//由源字符串对象构造
{
	*this = source;//重载了运算符'='，可以直接赋值拷贝
}
CharString::CharString()
//普通构造，空字符串
{
	AppendZero();//添加末尾结束符'\0'
}

CharString::~CharString()
{
}
void CharString::AppendZero()
{
	if (m_nDataLen >= m_nBufferLen)//判断是否数据满
	{
		SetBufferSize(m_nDataLen * 2+8);//调父类方法扩展内部缓冲
	}
	m_pElemData[m_nDataLen] = '\0';//末尾的'\0'字符不计算在串长度内，所以m_nDataLen不需要加1
}
int CharString::AddTail(const char ch)
{
	int nRet = SqList<char>::AddTail(ch);//调用父类方法
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
	bool bRet = SqList<char>::Insert(position, ch);//调用父类方法
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
	Clear();//先调用父类方法清空内部缓冲
	int nSrcLen = strlen(source);
	if (n > nSrcLen)
		n = nSrcLen;//拷贝数大于字符串长度，取字符串长度
	SaveData((char*)source, n);
	AppendZero();//添加末尾结束符'\0'
	return *this;
}
const char* CharString::ToCStr() const
{
	return m_pElemData;//返回父类的数据存储内存
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
	int nPatLen = pattern.Length();//获取模式串的长度
	if (m_nDataLen - pos+1 < nPatLen)
		return -1;//如果主串长度不够，返回-1
	const char* pCstrPat = pattern.ToCStr();//模式串转换为C风格字符串
	int i,j;
	int nSumChar = 0;//主串窗口字符求和值,初值为0
	int nSumPat = 0;//模式串字符求和值,初值为0

	bool bMatched = true;//匹配标志
	//下面进行首次计算nSumChar、nSumPat及进行首次匹配
	for (i = pos,j= 0; j < nPatLen; i++,j++)
	{
		nSumPat += pCstrPat[j];//模式串求和
		nSumChar += m_pElemData[i];//主串前nPatLen个字符求和
		if(m_pElemData[i] != pCstrPat[j])
			bMatched = false;
	}
	if (bMatched)
		return pos;//首次即匹配成功
	int nMatchedPos = -1;//存储匹配窗口位置变量，初值为0
	char chPatHead = pCstrPat[0],chPatTail = pCstrPat[nPatLen-1];//定义模式串首尾字符
	for (i = pos+1; i <= m_nDataLen - nPatLen; i++)
	{
		nSumChar += m_pElemData[i + nPatLen-1] - m_pElemData[i-1];//后方进一个字符，前方出一个字符
		if (nSumChar != nSumPat || m_pElemData[i] != chPatHead || m_pElemData[i + nPatLen - 1]!= chPatTail)
			continue;//求和值不同或首尾字符不同，直接下一次循环
		bMatched = true;//匹配前设置匹配标志，匹配时如果不匹配，清除该标志
		//下面的循环匹配串，首、末两位已经匹配，不必再匹配首位
		for (j = 1; j < nPatLen-1; j++)
		{
			if (m_pElemData[i + j] != pCstrPat[j])
			{//有字符不相等，设置不匹配标志，不需要再循环
				bMatched = false;
				break;
			}
		}
		if (bMatched)
		{
			nMatchedPos = i;//找到匹配位置
			break;//找到匹配位置后，结束循环
		}
	}
	return nMatchedPos;
}

void  CharString::GetNextVal(const CharString& pattern,int* nextVal) const
{
	//int  j = 0, k = -1;
	//nextVal[0] = -1;//首位为-1开始迭代
	const char* pPattern = pattern.ToCStr();
	int nLen = pattern.Length();
	
	nextVal[0] = -1;//初始值设为-1
	int j = 0, k = -1;
	while (j < nLen-1)
	{
		if (k == -1 || pPattern[j] == pPattern[k]) 
			nextVal[++j] = ++k;
		else k = nextVal[k];//失配时k回溯
	}

}
int  CharString::KMPIndex(const CharString& pattern, int pos)const
{
	int nPatLen = pattern.Length();//获取模式串的长度
	if (m_nDataLen - pos + 1 < nPatLen)
		return -1;//如果主串长度不够，返回-1
	const char* pCstrPattern = pattern.ToCStr();
	int* pnNext = new int[nPatLen];
	GetNextVal(pattern, pnNext);//获取Next数组
	int i = pos,j =0;
	while (i < m_nDataLen && j < nPatLen) 
	{
		if (j == -1 || m_pElemData[i] == pCstrPattern[j])
		{//初始或者匹配成功,j为-1时比对指针直接下移，下一个字符比对时模式串从0开始
			// 指针后移
			i++;
			j++;
		}
		else
			j = pnNext[j];// 模式串j指针根据next数组回溯
	}
	delete[]pnNext;//删除next数组
	if (j < nPatLen) return -1;	// 匹配失败
	return i - j;			// 匹配成功

}

//相关串操作实现

CharString Read(istream& input)
// 操作结果：从输入流读入串
{
	CharString strChar;												// 临时线性表
	char ch;															// 临时字符
	while ((ch = input.peek()) != EOF &&								// peek()从输入流中取一个字符,输入流指针不变
		(ch = input.get()) != '\n')										// get()从输入流中取一个字符,输入流指针指向下一字符
	{	// 将输入的字符追加线性表中
		strChar.AddTail(ch);
	}
	return strChar;														// 返回由temList生成的串
}

CharString Read(istream& input, char& terminalChar)
// 操作结果：从输入流读入串，并用terminalChar返回串结束字符
{
	CharString strChar;								// 临时字符串
	char ch;										// 临时字符
	while ((ch = input.peek()) != EOF &&			// peek()从输入流中取一个字符,输入流指针不变
		(ch = input.get()) != '\n')					// get()从输入流中取一个字符,输入流指针指向下一字符
	{	// 将输入的字符追加线性表中
		strChar.AddTail(ch);;
	}
	terminalChar = ch;													// 用terminalChar返回串结束字符
	return strChar;														// 返回由temList生成的串
}

CharString Read(ifstream& input, char& terminalChar)
{
	CharString strChar;													// 临时字符串
	char ch;
	// 从文件读入串
	while ((ch = input.peek()) != EOF &&								// peek()从输入流中取一个字符,输入流指针不变
		(ch = input.get()) != '\n')										// get()从输入流中取一个字符,输入流指针指向下一字符
	{	// 将输入的字符追加线性表中
		strChar.AddTail(ch);
	}
	terminalChar = ch;
	return strChar;
}

void Write(const CharString& s)
// 操作结果：输出串
{
	cout << s.ToCStr() << endl;											// 输出串值
}

void Concat(CharString& target, const CharString& source)
// 操作结果：将串source连接到串target的后面
{
	target += source;//重载了‘+=’运算符
}

void Copy(CharString& target, const CharString& source)
// 操作结果：将串source复制到串target
{
	target = source;//CharString 重载了'=',可以直接赋值
}

void Copy(CharString& target, const CharString& source, int n)
// 操作结果：将串source复制n个字符到串target 
{
	target.CopyN(source.ToCStr(), n);//拷贝source的n个字符
}

int Index(const CharString& target, const CharString& pattern, int pos)
// 操作结果：如果匹配成功，返回模式串pattern第一次在目标串target中从第pos
//	个字符开始出现的位置, 否则返回-1
{
	return target.Index(pattern, pos);
}

CharString SubString(const CharString& s, int pos, int len)
// 初始条件：串s存在，0 <= pos < s.Length()且0 <= len <= s.Length() - pos
// 操作结果：返回串s的第pos个字符开始的长度为len的子串
{
	CharString sub = "";//子串初始化为空串
	if (0 <= pos && pos < s.Length() && 0 <= len)
	{	// 返回串s的第pos个字符开始的长度为len的子串
		len = (len < s.Length() - pos) ? len : (s.Length() - pos);		// 子串长串sub
		sub.CopyN(s.ToCStr() + pos, len);
	}
	return sub;
}

bool operator ==(const CharString& first, const CharString& second)
// 操作结果：重载关系运算符==
{
	return first.Compare(second) == 0;
}

bool operator <(const CharString& first, const CharString& second)
	// 操作结果：重载关系运算符<
{
	return first.Compare(second) < 0;
}

bool operator >(const CharString& first, const CharString& second)
// 操作结果：重载关系运算符>
{
	return first.Compare(second) > 0;
}

bool operator <=(const CharString& first, const CharString& second)
// 操作结果：重载关系运算符<=
{
	return first.Compare(second) <= 0;
}

bool operator >=(const CharString& first, const CharString& second)
// 操作结果：重载关系运算符>=
{
	return first.Compare(second) >= 0;
}

bool operator !=(const CharString& first, const CharString& second)
// 操作结果：重载关系运算符!=
{
	return first.Compare(second) != 0;
}
#endif