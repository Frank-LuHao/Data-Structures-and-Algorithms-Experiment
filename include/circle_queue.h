/*******************************************************************
*文件名：circle_queue.h
*功能：实现循环顺序队列模版类
*作者：陈延涛（四川大学计算机学院）
*说明：本程序为教学用源代码，未经允许，勿作他用
*******************************************************************/
#if !defined(_CIRCLE_QUEUE_H_)
#define  _CIRCLE_QUEUE_H_
#include <memory.h>
template <class ElemType>
#define DEFAULT_SIZE 32
class CircleQueue 
{
public:
	CircleQueue(int nMaxSize= DEFAULT_SIZE);//构造函数
	CircleQueue(const CircleQueue<ElemType>& source);//拷贝构造函数
	~CircleQueue();//析构函数
	int Length() const;//获取链式队列中元素个数
	bool IsEmpty() const;//判断链式队列是否为空
	void Clear();//清空链式队列
	void Traverse(bool (*visit)(const ElemType& e))const;//遍历链式队列
	bool OutQueue(ElemType& e);	// 出队操作
	bool OutQueue();		// 出队操作
	bool GetHead(ElemType& e) const;	// 取队头操作
	bool InQueue(const ElemType& e);	// 入队
	CircleQueue& operator = (const CircleQueue<ElemType>& source);//重载'='运算符
	bool BatchInQueue(const ElemType* pElemBuf, int nElemCounts);//批次入队
	int BatchOutQueue(ElemType* pElemBuf, int nElemCounts);//批次出队
private:
	bool IsFull() const;//是否队满
	bool Resize(int nMaxSize);//重新分配内部缓冲
	ElemType* m_pElems;		// 元素存储空间
	int m_nFront, m_nRear;		// 队头队尾
	int m_nMaxSize;		// 队列最大元素个数
	int m_nElemCounts;//元素个数
};

//实现代码
template <class ElemType>
CircleQueue<ElemType>::CircleQueue(int nMaxSize)
{
	m_pElems = new ElemType[nMaxSize];
	m_nMaxSize = nMaxSize;//缓冲区大小为nMaxSize
	m_nFront = m_nRear = 0;//初始化头尾都为0
	m_nElemCounts = 0;
}
template <class ElemType>
CircleQueue<ElemType>::CircleQueue(const CircleQueue<ElemType>& source)
{
	*this = source;//重构了'='运算符
}
template <class ElemType>
CircleQueue<ElemType>::~CircleQueue()
{
	if (m_pElems)
	{
		delete[]m_pElems;//删除内部缓冲
		m_pElems = NULL;
	}
	m_nFront = m_nRear = 0;
	m_nElemCounts = 0;
}
template <class ElemType>
CircleQueue<ElemType>& CircleQueue<ElemType>::operator = (const CircleQueue<ElemType>& source)
{
	Clear();
	if (m_pElems)
		delete[] m_pElems;
	m_nMaxSize = source.m_nMaxSize;
	m_pElems = new ElemType[m_nMaxSize];
	ElemType* pSrcElem = source.m_pElems;
	memcpy(m_pElems, pSrcElem, m_nMaxSize * sizeof(ElemType));
	m_nFront = source.m_nFront;
	m_nRear = source.m_nRear;
	m_nElemCounts = source.m_nElemCounts;
	return *this;
}
template <class ElemType>
int CircleQueue<ElemType>::Length() const
{
	return m_nElemCounts;
}
template <class ElemType>
void CircleQueue<ElemType>::Clear()
{
	m_nFront = m_nRear = 0;
	m_nElemCounts = 0;
}
template <class ElemType>
bool CircleQueue<ElemType>::IsEmpty()const
{
	if (m_nElemCounts == 0)
		return true;
	return false;
}
template <class ElemType>
bool CircleQueue<ElemType>::IsFull()const
{
	if (m_nElemCounts >= m_nMaxSize)
		return true;
	return false;
}

template <class ElemType>
void CircleQueue<ElemType>::Traverse(bool (*visit)(const ElemType& )) const
{
	if (IsEmpty())
		return;
/*
	下面的遍历方法在队满时有问题
	for (int temPos = m_nFront; temPos != m_nRear;
		temPos = (temPos + 1) % m_nMaxSize)
	{	// 对队列的每个元素调用函数(*visit)
		(*visit)(m_pElems[temPos]);
	}
*/
	int postion, counts = 0;
	for (postion = m_nFront; counts < m_nElemCounts; counts++, postion = (postion + 1) % m_nMaxSize)
	{// 对队列的每个元素调用函数(*visit)
		(*visit)(m_pElems[postion]);
	}

}
template <class ElemType>
bool CircleQueue<ElemType>::OutQueue(ElemType& e)
{
	if (IsEmpty())
		return false;//保护性检查，空队列时返回失败
	e = m_pElems[m_nFront];//获取队头数据
	m_nFront = (m_nFront + 1) % m_nMaxSize;
	m_nElemCounts--;//数据总数减1
	return true;
}
template <class ElemType>
bool CircleQueue<ElemType>::OutQueue()
{
	if (IsEmpty())
		return false;//保护性检查，空队列时返回失败
	m_nFront = (m_nFront + 1) % m_nMaxSize;
	m_nElemCounts--;//数据总数减1
	return true;

}
template <class ElemType>
bool CircleQueue<ElemType>::GetHead(ElemType& e) const
{
	if (IsEmpty())
		return false;//保护性检查，空队列时返回失败
	e = m_pElems[m_nFront];//获取队头数据
	return true;
}
template <class ElemType>
bool CircleQueue<ElemType>::Resize(int nMaxSize)
{
	if (nMaxSize <= m_nMaxSize)
		return false;//新缓冲区大小小于原来的缓冲大小，返回失败
	ElemType* pNewElem = new ElemType[nMaxSize];//重新分配缓冲区
	if (!pNewElem)
		return false;//分配内存失败
	if (m_nFront >= m_nRear && m_nElemCounts>0)
	{
		memcpy(pNewElem, m_pElems + m_nFront, (m_nMaxSize - m_nFront) * sizeof(ElemType));//先拷贝front到缓冲区尾的数据
		memcpy(pNewElem+ m_nMaxSize - m_nFront, m_pElems , m_nRear * sizeof(ElemType));//再拷贝缓冲区头到队尾的数据
	}
	else if(m_nElemCounts > 0)
	{
		memcpy(pNewElem, m_pElems + m_nFront, (m_nRear - m_nFront) * sizeof(ElemType));//拷贝队头到队尾的数据
	}
	delete[] m_pElems;//删除掉旧的缓冲区
	m_pElems = pNewElem;//更新缓冲指针
	m_nFront = 0;//队头是0;
	m_nRear = m_nElemCounts;//队尾是m_nElemCounts
	m_nMaxSize = nMaxSize;//更新新缓冲大小
}
template <class ElemType>
bool CircleQueue<ElemType>::InQueue(const ElemType& e)
{
	if (IsFull())//队满判断
	{
		if (!Resize(m_nMaxSize * 2))//尺寸扩大1倍，重新分配缓冲
			return false;//重新分配缓冲失败
	}
	m_pElems[m_nRear] = e;
	m_nRear = (m_nRear + 1) % m_nMaxSize;
	m_nElemCounts++;
	return true;
}
template <class ElemType>
/************************************************************************
* 函数名：bool CircleQueue<ElemType>::BatchInQueue(const ElemType* pElemBuf, int nElemCounts)
* 调用参数： pElemBuf-输入数据内存缓冲区
*			 nElemCounts -批量入队的数据量
* 返回值：   是否成功批量入队数据
* 功能：	把数据批量入队到循环队列中
* 注：本函数内部自动对内部缓冲大小进行检测，当输入数据会导致队列内存溢出时，会重新分配内存
***************************************************************************/
bool CircleQueue<ElemType>::BatchInQueue(const ElemType* pElemBuf, int nElemCounts)
{
	if (m_nElemCounts + nElemCounts > m_nMaxSize)
	{//缓冲区尺寸不够，重新分配缓冲区
		if (!Resize((m_nElemCounts + nElemCounts) * 2))
			return false;//分配内存失败，返回错误
	}
	if (m_nRear >= m_nFront)
	{
		if (m_nRear + nElemCounts > m_nMaxSize)
		{
			int nCpyLen = m_nMaxSize - m_nRear;//拷贝填满缓冲区的数据长度
			memcpy(m_pElems + m_nRear, pElemBuf, nCpyLen * sizeof(ElemType));
			//再把剩余的数据拷贝到缓冲区头
			memcpy(m_pElems , pElemBuf+nCpyLen, (nElemCounts-nCpyLen) * sizeof(ElemType));
		}
		else
		{
			memcpy(m_pElems + m_nRear, pElemBuf, nElemCounts * sizeof(ElemType));//把数据直接拷贝到缓冲区
		}
	}
	else
	{
		memcpy(m_pElems + m_nRear, pElemBuf, nElemCounts * sizeof(ElemType));//把数据直接拷贝到缓冲区
	}
	m_nElemCounts += nElemCounts;
	m_nRear = (m_nRear+nElemCounts)%m_nMaxSize;
}
template <class ElemType>
/************************************************************************
* 函数名：int CircleQueue<ElemType>::BatchOutQueue(ElemType* pElemBuf, int nElemCounts)
* 调用参数： pElemBuf-输出数据内存缓冲区
*			 nElemCounts -需要出队的数据量
* 返回值：   实际出队的数据量 
* 功能：	从循环队列中批量出队数据
***************************************************************************/
int CircleQueue<ElemType>::BatchOutQueue(ElemType* pElemBuf, int nElemCounts)
{
	int nOutCounts = nElemCounts;
	if (nOutCounts > m_nElemCounts)
		nOutCounts = m_nElemCounts;//需求的数据量大于队列数据量，更新出队数据量
	if (nOutCounts <= 0)
		return 0;//出队数据量不正确，返回0
	if (m_nFront >= m_nRear)
	{
		if (m_nFront + nOutCounts > m_nMaxSize)
		{
			int nCpyLen = m_nMaxSize - m_nFront;//拷贝队头到缓冲尾部的数据
			memcpy(pElemBuf,m_pElems + m_nFront,  nCpyLen * sizeof(ElemType));
			//再拷贝剩余的数据
			memcpy(pElemBuf+nCpyLen,m_pElems, (nOutCounts - nCpyLen) * sizeof(ElemType));
		}
		else
		{
			memcpy(pElemBuf , m_pElems+m_nFront, nOutCounts * sizeof(ElemType));
		}
	}
	else
	{
		memcpy(pElemBuf, m_pElems + m_nFront, nOutCounts * sizeof(ElemType));
	}
	m_nElemCounts -= nOutCounts;
	m_nFront = (m_nFront + nOutCounts) % m_nMaxSize;
	return nOutCounts;
}

#endif