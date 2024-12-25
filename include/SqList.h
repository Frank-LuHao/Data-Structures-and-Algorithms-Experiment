#if !defined(_SQ_LIST_H_)
#define  _SQ_LIST_H_
#include "ListBase.h"
template <class ElemType> 
class SqList: public ListBase <ElemType>
{
public:
	SqList(int nBufferSize = 16);//构造函数
	SqList(const SqList& other);//拷贝构造函数
	~SqList();//析构函数
	int Length() const;//获取线性表中元素个数
	bool IsEmpty() const;//判断线性表是否为空
	void Clear();//清空线性表
	bool GetElem(int position, ElemType& e) const;//获取指定位置元素
	bool SetElem(int position, const ElemType e);//设置指定位置元素
	bool Delete(int position,ElemType& e);//删除指定位置元素
	bool Insert(int position,const ElemType e);//在指定位置插入元素
	void Traverse(bool (*visit)(const ElemType&))const;//遍历线性表
	int AddTail(const ElemType e) ;	//添加数据到线性表末尾
	virtual SqList& operator = (const SqList<ElemType>& source); // 重载赋值运算符
protected:
	int SaveData(ElemType* pTData, int nDataLen);//内部存储数据的私有函数
	int SetBufferSize(int nSize);//内部调整缓冲区大小的私有函数
	ElemType* m_pElemData;//内部数据缓冲区
	int m_nDataLen;//内部存储的有效数据数量个数
	int m_nBufferLen;//内部缓冲区大小
};

//顺序表实现代码
template <class ElemType> 
SqList<ElemType>::SqList(int nBufferSize)
{
	m_pElemData = new ElemType[nBufferSize];//初始化时能够存储4个元素

	m_nBufferLen = nBufferSize;
	m_nDataLen = 0;
	//memset(m_pElemData, 0, sizeof(ElemType) * m_nBufferLen);//如果是对象，该句会把对象清零，屏蔽
}

template <class ElemType> 
SqList<ElemType>::~SqList()
{
	if (m_pElemData)
	{
		delete[]m_pElemData;
		m_pElemData = NULL;
	}
	m_nDataLen = 0;
	m_nBufferLen = 0;
}
template <class ElemType>
SqList<ElemType>& SqList<ElemType>::operator = (const SqList<ElemType>& source)
{
	Clear();//清空数据
	int nDataLen = source.m_nDataLen;
	ElemType* pSrcBuf = source.m_pElemData;
	if (nDataLen > 0)
	{//被拷贝的对象有效数据长度不为0
		SaveData(pSrcBuf, nDataLen);//拷贝数据
	}
	return *this;
}
template <class ElemType>
SqList<ElemType>::SqList(const SqList<ElemType>& other)
{
	int nDataLen = other.m_nDataLen;
	m_nBufferLen = other.m_nBufferLen;
	m_pElemData = new ElemType[m_nBufferLen];//分配缓冲
	m_nDataLen = 0;//有效数据长度为0
	ElemType* pOtherBuf = other.m_pElemData;
	if (nDataLen > 0)
	{//被拷贝的对象有效数据长度不为0
		SaveData(pOtherBuf, nDataLen);//拷贝数据
	}
}

template <class ElemType> 
int SqList<ElemType>::SetBufferSize(int nSize)
{

	if (nSize <= m_nBufferLen)
		return m_nBufferLen;
	ElemType* pData = new ElemType[nSize];//重新分配缓冲区
	memcpy(pData, m_pElemData, sizeof(ElemType) * m_nDataLen);//把旧缓冲区的内容拷贝到新缓冲区中
	memset(pData + m_nDataLen, 0, (m_nBufferLen - m_nDataLen) * sizeof(ElemType));//缓冲中的空闲数据清零
	delete[] m_pElemData;//释放旧的缓冲区
	m_pElemData = pData;
	m_nBufferLen = nSize;
	return m_nBufferLen;//返回缓冲区尺寸
}
template <class ElemType> 
int SqList<ElemType>::Length() const
{
	return m_nDataLen;
}
template <class ElemType> 
void SqList<ElemType>::Clear()
{
	m_nDataLen = 0;
}
template <class ElemType>
bool SqList<ElemType>::IsEmpty()const
{
	if (m_nDataLen <= 0)
		return true;
	return false;
}

template <class ElemType> 
int SqList<ElemType>::SaveData(ElemType* pTData, int nDataLen)
{
	//返回一批数据追加首地址的索引
	if (!pTData || nDataLen <= 0)
		return -1;

	if (m_nDataLen + nDataLen >= m_nBufferLen)
	{
		if (!SetBufferSize((m_nDataLen + nDataLen)*2))
			return -1;
	}
	int position = m_nDataLen;
	memcpy(m_pElemData + position, pTData, nDataLen * sizeof(ElemType));
	m_nDataLen += nDataLen;

	return position;
}
template <class ElemType> 
bool SqList<ElemType>::Insert(int position, const ElemType eData)
{
	//返回插入位置索引
	if (position < 0 )
		return false;
	//检查缓冲区
	if (m_nDataLen >= m_nBufferLen)
	{
		if (!SetBufferSize(m_nDataLen*2))//缓冲区扩充1倍
			return false;
	}
	if (position <= m_nDataLen - 1)
	{//后面有数据
		// 该循环语句块被memmove替代,速度更快,但适用数据类型为没有重载'='的对象
		for (int i = m_nDataLen-1; i >=position; i--)
		{
			m_pElemData[i+1] = m_pElemData[i];
		}
		//memmove(m_pElemData + position + 1, m_pElemData + position, sizeof(ElemType) * (m_nDataLen - position));
		m_pElemData[position] = eData;
		m_nDataLen++;
	}
	else
	{//追加到末尾
		position = m_nDataLen;
		m_pElemData[position] = eData;
		m_nDataLen++;
	}
	return true;
}
template <class ElemType>
int SqList<ElemType>::AddTail(const ElemType e)
{
	//检查缓冲区
	if (m_nDataLen >= m_nBufferLen)
	{
		if (!SetBufferSize(m_nDataLen * 2))//缓冲区扩充1倍
			return -1;
	}
	int position = m_nDataLen;//追加到末尾
	m_pElemData[position] = e; 
	m_nDataLen++;
	return position;
}

template <class ElemType> 
bool SqList<ElemType>::Delete(int position,ElemType& eData)
{
	if (position < 0 || position >= m_nDataLen)
		return false;

	eData = m_pElemData[position];//删除的数据赋值给tData
	
	if (position < m_nDataLen - 1) //如果是最后一条数据，不需要移动后面的数据了
	{
		//该循环语句块被memmove替代,速度更快,但适用于没有重载'='的数据对象
		for (int i = position + 1; i < m_nDataLen; i++)
		{
			m_pElemData[i - 1] = m_pElemData[i];
		}
		//memmove(m_pElemData + position, m_pElemData + position + 1, (m_nDataLen - position - 1) * sizeof(ElemType));//后面的所有数据前移
	}
	m_nDataLen--;
	return true;
}
template <class ElemType>
bool SqList<ElemType>::GetElem(int position, ElemType& e) const
{
	if (position < 0 || position >= m_nDataLen)
		return false;
	e = m_pElemData[position];
	return true;
}
template <class ElemType> 
bool SqList<ElemType>::SetElem(int position, const ElemType e)
{
	if (position < 0)
		return false;
	if (position >= m_nBufferLen)
	{
		if (!SetBufferSize(position * 2))//缓冲区扩充1倍
			return false;
	}
	m_pElemData[position] = e;
	if (position >= m_nDataLen)
		m_nDataLen = position + 1;
	return true;
}
template <class ElemType>
void SqList<ElemType>::Traverse(bool (*visit)(const ElemType&)) const
{
	if (!visit)
		return;
	int position;
	for (position = 0; position < m_nDataLen; position++)
	{
		if(!(*visit)(m_pElemData[position]))
			break;//函数指针访问函数返回为false退出循环
	}
}
#endif