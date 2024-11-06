#if !defined(_LK_QUEUE_H_)
#define  _LK_QUEUE_H_
#include "SimpleLkList.h"

template <class ElemType>
class LinkQueue :private SimpleLkList <ElemType>
{
public:
	LinkQueue();//���캯��
	LinkQueue(const LinkQueue<ElemType>& source);//�������캯��
	~LinkQueue();//��������
	int Length() const;//��ȡ��ʽ������Ԫ�ظ���
	bool IsEmpty() const;//�ж���ʽ�����Ƿ�Ϊ��
	void Clear();//�����ʽ����
	void Traverse(bool (*visit)(const ElemType& e))const;//������ʽ����
	bool OutQueue(ElemType& e);	// ���Ӳ���
	bool OutQueue();		// ���Ӳ���
	bool GetHead(ElemType& e) const;	// ȡ��ͷ����
	bool InQueue(const ElemType& e);	// ���
};


//��ʽ���е�ʵ�ִ���
template <class ElemType>
LinkQueue<ElemType>::LinkQueue()
{

}
template <class ElemType>
LinkQueue<ElemType>::LinkQueue(const LinkQueue<ElemType>& source)
{
	*this = source;//������������'='�����ø�ֵ���㣬�������
}
template <class ElemType>
LinkQueue<ElemType>::~LinkQueue()
{
}

template <class ElemType>
int LinkQueue<ElemType>::Length() const
{
	return SimpleLkList<ElemType>::Length();//���ø���ķ���
}
template <class ElemType>
void LinkQueue<ElemType>::Clear()
{
	return SimpleLkList<ElemType>::Clear();//���ø���ķ���
}
template <class ElemType>
bool LinkQueue<ElemType>::IsEmpty()const
{
	return SimpleLkList<ElemType>::IsEmpty();//���ø���ķ���
}

template <class ElemType>
void LinkQueue<ElemType>::Traverse(bool (*visit)(const ElemType& e)) const
{
	return SimpleLkList<ElemType>::Traverse(visit);//���ø���ķ���
}
template <class ElemType>
bool LinkQueue<ElemType>::OutQueue(ElemType& e)
{
	if (this->IsEmpty())
		return false;//�����Լ�飬�ն���ʱ����ʧ��
	return this->Delete(0, e);//���ø��෽������ͷ��ɾ������
}
template <class ElemType>
bool LinkQueue<ElemType>::OutQueue()
{
	if (this->IsEmpty())
		return false;//�����Լ�飬�ն���ʱ����ʧ��
	ElemType e;
	return this->Delete(0, e);//���ø��෽������ͷ��ɾ������
}
template <class ElemType>
bool LinkQueue<ElemType>::GetHead(ElemType& e) const
{
	if (this->IsEmpty())
		return false;//�����Լ�飬�ն���ʱ����ʧ��
	return this->GetElem(0,e);//��ȡ��ͷ����Ԫ��㣩������
}
template <class ElemType>
bool LinkQueue<ElemType>::InQueue(const ElemType& e)
{
	this->AddTail(e);//���ø��෽�����ڶ�β��β�ڵ㣩׷������
	return true;
}
#endif