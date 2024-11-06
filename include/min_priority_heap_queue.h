#ifndef __MIN_PRIORITY_HEAP_QUEUE__H__
#define __MIN_PRIORITY_HEAP_QUEUE__H__

#ifndef DEFAULT_SIZE
#define DEFAULT_SIZE 128						// 缺省元素个数
#endif
#include "SqList.h"
// 最小优先堆队列类模板
template<class ElemType>
class MinPriorityHeapQueue : private SqList<ElemType>
{
protected:

// 辅助函数模板:
	void SiftAdjust(int low, int high);
		// 调整this->m_pElemData[low]使其this->m_pElemData[low .. high]按关键字成为一个小顶堆
	void BuildHeap();							// 建立堆

public:
// 抽象数据类型方法声明及重载编译系统默认方法声明:
	MinPriorityHeapQueue(int sz = DEFAULT_SIZE);// 构造最大元素个数为sz的堆
	MinPriorityHeapQueue(ElemType e[], int cnt = 0, int sz = DEFAULT_SIZE);	
		// 构造堆元素为e[0] ...e[cnt - 1], 最大元素个为sz的堆
	virtual ~MinPriorityHeapQueue();			// 析构函数模板
	int Length() const;							// 求优先队列长度			 
	bool IsEmpty() const;							// 判断优先队列是否为空
	void Clear();								// 将优先队列清空
	void Traverse(bool (*visit)(const ElemType &));		// 遍历优先队列
	bool OutQueue(ElemType &e);					// 出队操作
	bool GetHead(ElemType &e) const;			// 取队头操作
	bool InQueue(const ElemType &e);			// 入队操作
	MinPriorityHeapQueue(const MinPriorityHeapQueue<ElemType> &source); // 复制构造函数模板
	MinPriorityHeapQueue<ElemType> &operator =(const MinPriorityHeapQueue<ElemType> &source);
		// 重载赋值运算符
};


// 最小优先堆队列类模板的实现部分
template <class ElemType>
void MinPriorityHeapQueue<ElemType>::SiftAdjust(int low, int high)
// 操作结果:this->m_pElemData[low .. high]中记录的关键字除this->m_pElemData[low]以外都满足堆定义,调
//	整this->m_pElemData[low]使其this->m_pElemData[low .. high]按关键字成为一个小顶堆
{
	for (int f = low, i = 2 * low + 1; i <= high; i = 2 * i + 1)
	{	// f为被调整结点,i为f的最小关键字孩子
		if (i < high && this->m_pElemData[i] > this->m_pElemData[i + 1])
		{	// 右孩子关键字更小, i指向右孩子
			i++;
		}
		if (this->m_pElemData[f] <= this->m_pElemData[i])
		{	// 已成为小顶堆
			break;
		}
		ElemType temElem = this->m_pElemData[f]; this->m_pElemData[f] = this->m_pElemData[i]; this->m_pElemData[i] = temElem;			// 交换this->m_pElemData[f], this->m_pElemData[i]
		f = i;										// 成为新的调整结点		
	}
}

template<class ElemType>
void MinPriorityHeapQueue<ElemType>::BuildHeap()
// 操作结果:建立小顶堆
{
	int i;
	for (i = (this->m_nDataLen - 2) / 2; i >= 0; --i) 
	{	// 将this->m_pElemData[0 .. this->m_nDataLen - 1]调整成小顶堆
		SiftAdjust(i, this->m_nDataLen - 1);
	};
}

template<class ElemType>
MinPriorityHeapQueue<ElemType>::MinPriorityHeapQueue(int sz)
	:SqList<ElemType>(sz)
// 操作结果：构造一个最大元素个数为sz的空优先队列
{
	

}

template<class ElemType>
MinPriorityHeapQueue<ElemType>::MinPriorityHeapQueue(ElemType e[], int cnt, int sz)	
	:SqList<ElemType>(sz)
// 操作结果：构造堆元素为e[0] ...e[cnt - 1], 最大元素个数为sz的堆
{
	if (sz < cnt)
		this->SetBufferSize(cnt);
	for (int pos = 0; pos < cnt; pos++)
	{	// 将e[]赋值给this->m_pElemData[]
		this->m_pElemData[pos] = e[pos];
	}
	this->m_nDataLen = cnt;									// 堆元素个数
	BuildHeap();									// 建立堆
}

template<class ElemType>
MinPriorityHeapQueue<ElemType>::~MinPriorityHeapQueue()
// 操作结果：销毁优先队列
{

}

template<class ElemType>
int MinPriorityHeapQueue<ElemType>::Length() const
// 操作结果：返回优先队列长度			 
{
	return SqList< ElemType>::Length();
}

template<class ElemType>
bool MinPriorityHeapQueue<ElemType>::IsEmpty() const
// 操作结果：如优先队列为空，则返回true，否则返回false
{
	return SqList< ElemType>::IsEmpty();
}

template<class ElemType>
void MinPriorityHeapQueue<ElemType>::Clear() 
// 操作结果：清空优先队列
{
	SqList< ElemType>::Clear();										// 空优先队列元素个数为0 
}

template <class ElemType>
void MinPriorityHeapQueue<ElemType>::Traverse(bool (*visit)(const ElemType &))
// 操作结果：依次对优先队列的每个元素调用函数(*visit)
{
	for (int pos = 0; pos < this->m_nDataLen; pos++)
	{	// 对优先队列每个元素调用函数(*visit)
		if (!(*visit)(this->m_pElemData[pos]))
			break;
	}
}


template<class ElemType>
bool MinPriorityHeapQueue<ElemType>::OutQueue(ElemType &e)
// 操作结果：如果优先队列非空，那么删除堆顶元素，并用e返回其值，返回true, 否则返回false
{
	if (!IsEmpty()) 
	{	// 优先队列非空
		e = this->m_pElemData[0];								// 用e返回堆顶元素值
		ElemType temElem = this->m_pElemData[0]; this->m_pElemData[0] = this->m_pElemData[this->m_nDataLen - 1]; this->m_pElemData[this->m_nDataLen - 1] = temElem;		// 将堆顶元素和最后一个元素交换
		SiftAdjust(0, this->m_nDataLen - 2);					// 将this->m_pElemData[0 .. this->m_nDataLen - 2]重新调整为小顶堆
		this->m_nDataLen--;									// 删除堆顶元素后, 元素个数自减1
		return true;								// 操作成功
	}
	else
	{	// 优先队列为空
		return false;								// 操作失败
	}
}

template<class ElemType>
bool MinPriorityHeapQueue<ElemType>::GetHead(ElemType &e) const
// 操作结果：如果队列非空，那么用e返回堆顶元素，返回true, 否则返回false
{
	if (!IsEmpty()) 
	{	// 优先队列非空
		e = this->m_pElemData[0];								// 用e返回堆顶元素
		return true;
	}
	else
	{	// 队列为空
		return false;
	}
}

template<class ElemType>
bool MinPriorityHeapQueue<ElemType>::InQueue(const ElemType &e)
// 操作结果：插入元素e，操作成功返回true,则否返回false
{
	if (this->m_nDataLen >= this->m_nBufferLen)
	{	//调整尺寸
		this->SetBufferSize(this->m_nDataLen * 2 + 1);
	}
	int curPos = this->m_nDataLen++;						// 当前位置
	int parent = (curPos - 1) / 2;				// 当前的双亲	
	this->m_pElemData[curPos] = e;							// 初始时将元素e插入在堆的末端
	while (curPos > 0 && this->m_pElemData[curPos] < this->m_pElemData[parent])
	{	// this->m_pElemData[curPos]小于双亲, 与双亲交换
		ElemType temElem = this->m_pElemData[curPos];  this->m_pElemData[curPos] = this->m_pElemData[parent]; this->m_pElemData[parent] = temElem;	// 交换this->m_pElemData[curPos]与双亲元素this->m_pElemData[parent]
		curPos = parent;						// 以双亲作为新当前位置
		parent = (curPos - 1) / 2;				// 当前双亲	
	}
	return true;								// 操作成功

}

template<class ElemType>
MinPriorityHeapQueue<ElemType>::MinPriorityHeapQueue(const MinPriorityHeapQueue<ElemType> &source)
// 操作结果：由优先队列source构造新优先队列——复制构造函数模板
{
	*this = source;
	
}

template<class ElemType>
MinPriorityHeapQueue<ElemType> &MinPriorityHeapQueue<ElemType>::operator =(const MinPriorityHeapQueue<ElemType> &source)
// 操作结果：将优先队列source赋值给当前优先队列——重载赋值运算符
{
	if (&source != this)
	{
		delete this->m_pElemData;								// 释放空间
		this->m_pElemData = new ElemType[source.m_nBufferLen];						// 为堆分配存储空间
		this->m_nDataLen = source.m_nDataLen;							// 新优先队列的元素个数

		memcpy(this->m_pElemData, source.m_pElemData, sizeof(ElemType) * this->m_nDataLen);

	}
	return *this;
}

#endif