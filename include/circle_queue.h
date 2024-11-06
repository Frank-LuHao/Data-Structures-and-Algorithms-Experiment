/*******************************************************************
*�ļ�����circle_queue.h
*���ܣ�ʵ��ѭ��˳�����ģ����
*���ߣ������Σ��Ĵ���ѧ�����ѧԺ��
*˵����������Ϊ��ѧ��Դ���룬δ��������������
*******************************************************************/
#if !defined(_CIRCLE_QUEUE_H_)
#define  _CIRCLE_QUEUE_H_
#include <memory.h>
template <class ElemType>
#define DEFAULT_SIZE 32
class CircleQueue 
{
public:
	CircleQueue(int nMaxSize= DEFAULT_SIZE);//���캯��
	CircleQueue(const CircleQueue<ElemType>& source);//�������캯��
	~CircleQueue();//��������
	int Length() const;//��ȡ��ʽ������Ԫ�ظ���
	bool IsEmpty() const;//�ж���ʽ�����Ƿ�Ϊ��
	void Clear();//�����ʽ����
	void Traverse(bool (*visit)(const ElemType& e))const;//������ʽ����
	bool OutQueue(ElemType& e);	// ���Ӳ���
	bool OutQueue();		// ���Ӳ���
	bool GetHead(ElemType& e) const;	// ȡ��ͷ����
	bool InQueue(const ElemType& e);	// ���
	CircleQueue& operator = (const CircleQueue<ElemType>& source);//����'='�����
	bool BatchInQueue(const ElemType* pElemBuf, int nElemCounts);//�������
	int BatchOutQueue(ElemType* pElemBuf, int nElemCounts);//���γ���
private:
	bool IsFull() const;//�Ƿ����
	bool Resize(int nMaxSize);//���·����ڲ�����
	ElemType* m_pElems;		// Ԫ�ش洢�ռ�
	int m_nFront, m_nRear;		// ��ͷ��β
	int m_nMaxSize;		// �������Ԫ�ظ���
	int m_nElemCounts;//Ԫ�ظ���
};

//ʵ�ִ���
template <class ElemType>
CircleQueue<ElemType>::CircleQueue(int nMaxSize)
{
	m_pElems = new ElemType[nMaxSize];
	m_nMaxSize = nMaxSize;//��������СΪnMaxSize
	m_nFront = m_nRear = 0;//��ʼ��ͷβ��Ϊ0
	m_nElemCounts = 0;
}
template <class ElemType>
CircleQueue<ElemType>::CircleQueue(const CircleQueue<ElemType>& source)
{
	*this = source;//�ع���'='�����
}
template <class ElemType>
CircleQueue<ElemType>::~CircleQueue()
{
	if (m_pElems)
	{
		delete[]m_pElems;//ɾ���ڲ�����
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
	����ı��������ڶ���ʱ������
	for (int temPos = m_nFront; temPos != m_nRear;
		temPos = (temPos + 1) % m_nMaxSize)
	{	// �Զ��е�ÿ��Ԫ�ص��ú���(*visit)
		(*visit)(m_pElems[temPos]);
	}
*/
	int postion, counts = 0;
	for (postion = m_nFront; counts < m_nElemCounts; counts++, postion = (postion + 1) % m_nMaxSize)
	{// �Զ��е�ÿ��Ԫ�ص��ú���(*visit)
		(*visit)(m_pElems[postion]);
	}

}
template <class ElemType>
bool CircleQueue<ElemType>::OutQueue(ElemType& e)
{
	if (IsEmpty())
		return false;//�����Լ�飬�ն���ʱ����ʧ��
	e = m_pElems[m_nFront];//��ȡ��ͷ����
	m_nFront = (m_nFront + 1) % m_nMaxSize;
	m_nElemCounts--;//����������1
	return true;
}
template <class ElemType>
bool CircleQueue<ElemType>::OutQueue()
{
	if (IsEmpty())
		return false;//�����Լ�飬�ն���ʱ����ʧ��
	m_nFront = (m_nFront + 1) % m_nMaxSize;
	m_nElemCounts--;//����������1
	return true;

}
template <class ElemType>
bool CircleQueue<ElemType>::GetHead(ElemType& e) const
{
	if (IsEmpty())
		return false;//�����Լ�飬�ն���ʱ����ʧ��
	e = m_pElems[m_nFront];//��ȡ��ͷ����
	return true;
}
template <class ElemType>
bool CircleQueue<ElemType>::Resize(int nMaxSize)
{
	if (nMaxSize <= m_nMaxSize)
		return false;//�»�������СС��ԭ���Ļ����С������ʧ��
	ElemType* pNewElem = new ElemType[nMaxSize];//���·��仺����
	if (!pNewElem)
		return false;//�����ڴ�ʧ��
	if (m_nFront >= m_nRear && m_nElemCounts>0)
	{
		memcpy(pNewElem, m_pElems + m_nFront, (m_nMaxSize - m_nFront) * sizeof(ElemType));//�ȿ���front��������β������
		memcpy(pNewElem+ m_nMaxSize - m_nFront, m_pElems , m_nRear * sizeof(ElemType));//�ٿ���������ͷ����β������
	}
	else if(m_nElemCounts > 0)
	{
		memcpy(pNewElem, m_pElems + m_nFront, (m_nRear - m_nFront) * sizeof(ElemType));//������ͷ����β������
	}
	delete[] m_pElems;//ɾ�����ɵĻ�����
	m_pElems = pNewElem;//���»���ָ��
	m_nFront = 0;//��ͷ��0;
	m_nRear = m_nElemCounts;//��β��m_nElemCounts
	m_nMaxSize = nMaxSize;//�����»����С
}
template <class ElemType>
bool CircleQueue<ElemType>::InQueue(const ElemType& e)
{
	if (IsFull())//�����ж�
	{
		if (!Resize(m_nMaxSize * 2))//�ߴ�����1�������·��仺��
			return false;//���·��仺��ʧ��
	}
	m_pElems[m_nRear] = e;
	m_nRear = (m_nRear + 1) % m_nMaxSize;
	m_nElemCounts++;
	return true;
}
template <class ElemType>
/************************************************************************
* ��������bool CircleQueue<ElemType>::BatchInQueue(const ElemType* pElemBuf, int nElemCounts)
* ���ò����� pElemBuf-���������ڴ滺����
*			 nElemCounts -������ӵ�������
* ����ֵ��   �Ƿ�ɹ������������
* ���ܣ�	������������ӵ�ѭ��������
* ע���������ڲ��Զ����ڲ������С���м�⣬���������ݻᵼ�¶����ڴ����ʱ�������·����ڴ�
***************************************************************************/
bool CircleQueue<ElemType>::BatchInQueue(const ElemType* pElemBuf, int nElemCounts)
{
	if (m_nElemCounts + nElemCounts > m_nMaxSize)
	{//�������ߴ粻�������·��仺����
		if (!Resize((m_nElemCounts + nElemCounts) * 2))
			return false;//�����ڴ�ʧ�ܣ����ش���
	}
	if (m_nRear >= m_nFront)
	{
		if (m_nRear + nElemCounts > m_nMaxSize)
		{
			int nCpyLen = m_nMaxSize - m_nRear;//�������������������ݳ���
			memcpy(m_pElems + m_nRear, pElemBuf, nCpyLen * sizeof(ElemType));
			//�ٰ�ʣ������ݿ�����������ͷ
			memcpy(m_pElems , pElemBuf+nCpyLen, (nElemCounts-nCpyLen) * sizeof(ElemType));
		}
		else
		{
			memcpy(m_pElems + m_nRear, pElemBuf, nElemCounts * sizeof(ElemType));//������ֱ�ӿ�����������
		}
	}
	else
	{
		memcpy(m_pElems + m_nRear, pElemBuf, nElemCounts * sizeof(ElemType));//������ֱ�ӿ�����������
	}
	m_nElemCounts += nElemCounts;
	m_nRear = (m_nRear+nElemCounts)%m_nMaxSize;
}
template <class ElemType>
/************************************************************************
* ��������int CircleQueue<ElemType>::BatchOutQueue(ElemType* pElemBuf, int nElemCounts)
* ���ò����� pElemBuf-��������ڴ滺����
*			 nElemCounts -��Ҫ���ӵ�������
* ����ֵ��   ʵ�ʳ��ӵ������� 
* ���ܣ�	��ѭ��������������������
***************************************************************************/
int CircleQueue<ElemType>::BatchOutQueue(ElemType* pElemBuf, int nElemCounts)
{
	int nOutCounts = nElemCounts;
	if (nOutCounts > m_nElemCounts)
		nOutCounts = m_nElemCounts;//��������������ڶ��������������³���������
	if (nOutCounts <= 0)
		return 0;//��������������ȷ������0
	if (m_nFront >= m_nRear)
	{
		if (m_nFront + nOutCounts > m_nMaxSize)
		{
			int nCpyLen = m_nMaxSize - m_nFront;//������ͷ������β��������
			memcpy(pElemBuf,m_pElems + m_nFront,  nCpyLen * sizeof(ElemType));
			//�ٿ���ʣ�������
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