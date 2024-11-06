#ifndef __MIN_PRIORITY_HEAP_QUEUE__H__
#define __MIN_PRIORITY_HEAP_QUEUE__H__

#ifndef DEFAULT_SIZE
#define DEFAULT_SIZE 128						// ȱʡԪ�ظ���
#endif
#include "SqList.h"
// ��С���ȶѶ�����ģ��
template<class ElemType>
class MinPriorityHeapQueue : private SqList<ElemType>
{
protected:

// ��������ģ��:
	void SiftAdjust(int low, int high);
		// ����this->m_pElemData[low]ʹ��this->m_pElemData[low .. high]���ؼ��ֳ�Ϊһ��С����
	void BuildHeap();							// ������

public:
// �����������ͷ������������ر���ϵͳĬ�Ϸ�������:
	MinPriorityHeapQueue(int sz = DEFAULT_SIZE);// �������Ԫ�ظ���Ϊsz�Ķ�
	MinPriorityHeapQueue(ElemType e[], int cnt = 0, int sz = DEFAULT_SIZE);	
		// �����Ԫ��Ϊe[0] ...e[cnt - 1], ���Ԫ�ظ�Ϊsz�Ķ�
	virtual ~MinPriorityHeapQueue();			// ��������ģ��
	int Length() const;							// �����ȶ��г���			 
	bool IsEmpty() const;							// �ж����ȶ����Ƿ�Ϊ��
	void Clear();								// �����ȶ������
	void Traverse(bool (*visit)(const ElemType &));		// �������ȶ���
	bool OutQueue(ElemType &e);					// ���Ӳ���
	bool GetHead(ElemType &e) const;			// ȡ��ͷ����
	bool InQueue(const ElemType &e);			// ��Ӳ���
	MinPriorityHeapQueue(const MinPriorityHeapQueue<ElemType> &source); // ���ƹ��캯��ģ��
	MinPriorityHeapQueue<ElemType> &operator =(const MinPriorityHeapQueue<ElemType> &source);
		// ���ظ�ֵ�����
};


// ��С���ȶѶ�����ģ���ʵ�ֲ���
template <class ElemType>
void MinPriorityHeapQueue<ElemType>::SiftAdjust(int low, int high)
// �������:this->m_pElemData[low .. high]�м�¼�Ĺؼ��ֳ�this->m_pElemData[low]���ⶼ����Ѷ���,��
//	��this->m_pElemData[low]ʹ��this->m_pElemData[low .. high]���ؼ��ֳ�Ϊһ��С����
{
	for (int f = low, i = 2 * low + 1; i <= high; i = 2 * i + 1)
	{	// fΪ���������,iΪf����С�ؼ��ֺ���
		if (i < high && this->m_pElemData[i] > this->m_pElemData[i + 1])
		{	// �Һ��ӹؼ��ָ�С, iָ���Һ���
			i++;
		}
		if (this->m_pElemData[f] <= this->m_pElemData[i])
		{	// �ѳ�ΪС����
			break;
		}
		ElemType temElem = this->m_pElemData[f]; this->m_pElemData[f] = this->m_pElemData[i]; this->m_pElemData[i] = temElem;			// ����this->m_pElemData[f], this->m_pElemData[i]
		f = i;										// ��Ϊ�µĵ������		
	}
}

template<class ElemType>
void MinPriorityHeapQueue<ElemType>::BuildHeap()
// �������:����С����
{
	int i;
	for (i = (this->m_nDataLen - 2) / 2; i >= 0; --i) 
	{	// ��this->m_pElemData[0 .. this->m_nDataLen - 1]������С����
		SiftAdjust(i, this->m_nDataLen - 1);
	};
}

template<class ElemType>
MinPriorityHeapQueue<ElemType>::MinPriorityHeapQueue(int sz)
	:SqList<ElemType>(sz)
// �������������һ�����Ԫ�ظ���Ϊsz�Ŀ����ȶ���
{
	

}

template<class ElemType>
MinPriorityHeapQueue<ElemType>::MinPriorityHeapQueue(ElemType e[], int cnt, int sz)	
	:SqList<ElemType>(sz)
// ��������������Ԫ��Ϊe[0] ...e[cnt - 1], ���Ԫ�ظ���Ϊsz�Ķ�
{
	if (sz < cnt)
		this->SetBufferSize(cnt);
	for (int pos = 0; pos < cnt; pos++)
	{	// ��e[]��ֵ��this->m_pElemData[]
		this->m_pElemData[pos] = e[pos];
	}
	this->m_nDataLen = cnt;									// ��Ԫ�ظ���
	BuildHeap();									// ������
}

template<class ElemType>
MinPriorityHeapQueue<ElemType>::~MinPriorityHeapQueue()
// ����������������ȶ���
{

}

template<class ElemType>
int MinPriorityHeapQueue<ElemType>::Length() const
// ����������������ȶ��г���			 
{
	return SqList< ElemType>::Length();
}

template<class ElemType>
bool MinPriorityHeapQueue<ElemType>::IsEmpty() const
// ��������������ȶ���Ϊ�գ��򷵻�true�����򷵻�false
{
	return SqList< ElemType>::IsEmpty();
}

template<class ElemType>
void MinPriorityHeapQueue<ElemType>::Clear() 
// ���������������ȶ���
{
	SqList< ElemType>::Clear();										// �����ȶ���Ԫ�ظ���Ϊ0 
}

template <class ElemType>
void MinPriorityHeapQueue<ElemType>::Traverse(bool (*visit)(const ElemType &))
// ������������ζ����ȶ��е�ÿ��Ԫ�ص��ú���(*visit)
{
	for (int pos = 0; pos < this->m_nDataLen; pos++)
	{	// �����ȶ���ÿ��Ԫ�ص��ú���(*visit)
		if (!(*visit)(this->m_pElemData[pos]))
			break;
	}
}


template<class ElemType>
bool MinPriorityHeapQueue<ElemType>::OutQueue(ElemType &e)
// ���������������ȶ��зǿգ���ôɾ���Ѷ�Ԫ�أ�����e������ֵ������true, ���򷵻�false
{
	if (!IsEmpty()) 
	{	// ���ȶ��зǿ�
		e = this->m_pElemData[0];								// ��e���ضѶ�Ԫ��ֵ
		ElemType temElem = this->m_pElemData[0]; this->m_pElemData[0] = this->m_pElemData[this->m_nDataLen - 1]; this->m_pElemData[this->m_nDataLen - 1] = temElem;		// ���Ѷ�Ԫ�غ����һ��Ԫ�ؽ���
		SiftAdjust(0, this->m_nDataLen - 2);					// ��this->m_pElemData[0 .. this->m_nDataLen - 2]���µ���ΪС����
		this->m_nDataLen--;									// ɾ���Ѷ�Ԫ�غ�, Ԫ�ظ����Լ�1
		return true;								// �����ɹ�
	}
	else
	{	// ���ȶ���Ϊ��
		return false;								// ����ʧ��
	}
}

template<class ElemType>
bool MinPriorityHeapQueue<ElemType>::GetHead(ElemType &e) const
// ���������������зǿգ���ô��e���ضѶ�Ԫ�أ�����true, ���򷵻�false
{
	if (!IsEmpty()) 
	{	// ���ȶ��зǿ�
		e = this->m_pElemData[0];								// ��e���ضѶ�Ԫ��
		return true;
	}
	else
	{	// ����Ϊ��
		return false;
	}
}

template<class ElemType>
bool MinPriorityHeapQueue<ElemType>::InQueue(const ElemType &e)
// �������������Ԫ��e�������ɹ�����true,��񷵻�false
{
	if (this->m_nDataLen >= this->m_nBufferLen)
	{	//�����ߴ�
		this->SetBufferSize(this->m_nDataLen * 2 + 1);
	}
	int curPos = this->m_nDataLen++;						// ��ǰλ��
	int parent = (curPos - 1) / 2;				// ��ǰ��˫��	
	this->m_pElemData[curPos] = e;							// ��ʼʱ��Ԫ��e�����ڶѵ�ĩ��
	while (curPos > 0 && this->m_pElemData[curPos] < this->m_pElemData[parent])
	{	// this->m_pElemData[curPos]С��˫��, ��˫�׽���
		ElemType temElem = this->m_pElemData[curPos];  this->m_pElemData[curPos] = this->m_pElemData[parent]; this->m_pElemData[parent] = temElem;	// ����this->m_pElemData[curPos]��˫��Ԫ��this->m_pElemData[parent]
		curPos = parent;						// ��˫����Ϊ�µ�ǰλ��
		parent = (curPos - 1) / 2;				// ��ǰ˫��	
	}
	return true;								// �����ɹ�

}

template<class ElemType>
MinPriorityHeapQueue<ElemType>::MinPriorityHeapQueue(const MinPriorityHeapQueue<ElemType> &source)
// ��������������ȶ���source���������ȶ��С������ƹ��캯��ģ��
{
	*this = source;
	
}

template<class ElemType>
MinPriorityHeapQueue<ElemType> &MinPriorityHeapQueue<ElemType>::operator =(const MinPriorityHeapQueue<ElemType> &source)
// ��������������ȶ���source��ֵ����ǰ���ȶ��С������ظ�ֵ�����
{
	if (&source != this)
	{
		delete this->m_pElemData;								// �ͷſռ�
		this->m_pElemData = new ElemType[source.m_nBufferLen];						// Ϊ�ѷ���洢�ռ�
		this->m_nDataLen = source.m_nDataLen;							// �����ȶ��е�Ԫ�ظ���

		memcpy(this->m_pElemData, source.m_pElemData, sizeof(ElemType) * this->m_nDataLen);

	}
	return *this;
}

#endif