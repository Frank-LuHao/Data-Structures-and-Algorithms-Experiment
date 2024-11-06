#if !defined(_CIRCLE_LIST_BASE_H_)
#define _CIRCLE_LIST_BASE_H_
#include "LinkListBase.h"
template <class ElemType,class NodeType>
class CircleListBase :public LinkListBase< ElemType, NodeType>
{
public:
	CircleListBase();//���캯��
	~CircleListBase();//��������
	CircleListBase(const CircleListBase& source);//�������캯��
	virtual bool Delete(int position, ElemType& e);//����ɾ��ָ��λ��Ԫ��
	virtual bool Insert(int position, const ElemType e);//������ָ��λ�ò���Ԫ��
	virtual int AddTail(const ElemType e);	//����������ݵ�����ĩβ
	virtual bool IsEmpty() const;//�ж����Ա��Ƿ�Ϊ��
	virtual CircleListBase& operator = (const CircleListBase<ElemType, NodeType>& source); // ���ظ�ֵ�����
	virtual void Clear();//������Ա�
	virtual bool Next(ElemType& e);//��ǰ�ڵ����,ͬʱ��ȡ�������
protected:

	virtual NodeType* GetFirstNode() const;//��ȡ��Ԫ���
	virtual void LinkTailToHead();//����β�ڵ㵽ͷ�ڵ�
};

template <class ElemType, class NodeType>
CircleListBase< ElemType, NodeType>::CircleListBase()
{

}
template <class ElemType, class NodeType>
CircleListBase<ElemType,NodeType>::CircleListBase(const CircleListBase& source)
{
	*this = source;

}
template <class ElemType, class NodeType>
CircleListBase<ElemType,NodeType>& CircleListBase<ElemType,NodeType>::operator = (const CircleListBase<ElemType,NodeType>& source)
{
	LinkListBase<ElemType,NodeType>::operator=(source);//�ȵ��û�������ء�=������
	LinkTailToHead();//ʵ��ͷ��β����
	return *this;
}
template <class ElemType, class NodeType>
CircleListBase<ElemType, NodeType>::~CircleListBase()
{

}
template <class ElemType, class NodeType>
void CircleListBase< ElemType, NodeType>::LinkTailToHead()
{
	//����β�ڵ㵽ͷ�ڵ�
	this->Link(this->m_pNodeTail, this->m_pNodeHead);
}
template <class ElemType, class NodeType>
void CircleListBase<ElemType, NodeType>::Clear()
{
	//���ȵ��û����Clear����
	LinkListBase<ElemType,NodeType>::Clear();
	// Ȼ���β�ڵ��ָ�벿��ָ��ͷ�ڵ�
	LinkTailToHead();
}
template <class ElemType, class NodeType>
bool CircleListBase<ElemType,NodeType>::Next(ElemType& e)
{
	if (IsEmpty())
		return false;//ѭ������Ϊ��ʱ������false
	if (!this->m_pNodeCur || !this->m_pNodeCur->m_pNext)
		return false;//��ǰ�ڵ�Ϊ�ջ�ǰ�ڵ�ĺ��Ϊ�գ������ܼ������Ƶ�ǰ�ڵ�
	this->m_pNodeCur = this->m_pNodeCur->m_pNext;//��ǰ�ڵ���Ϊ���	
	//��������Ľڵ�Ϊͷ�ڵ㣬�������Ϊ���
	if (this->m_pNodeCur == this->m_pNodeHead)
		this->m_pNodeCur = this->m_pNodeCur->m_pNext;
	e = this->m_pNodeCur->m_TData;
	return true;

}
template <class ElemType, class NodeType>
bool CircleListBase<ElemType, NodeType>::Delete(int position, ElemType& e)
{
	//���ȵ��û����Delete����
	bool b = LinkListBase<ElemType,NodeType>::Delete(position, e);
	//Ȼ���β�ڵ��ָ�벿��ָ��ͷ�ڵ�
	LinkTailToHead();
	return b;
}
template <class ElemType, class NodeType>
bool CircleListBase<ElemType, NodeType>::Insert(int position, const ElemType e)
{
	//���ȵ��û����Insert����
	bool b = LinkListBase<ElemType,NodeType>::Insert(position, e);
	//Ȼ���β�ڵ��ָ�벿��ָ��ͷ�ڵ�
	LinkTailToHead();
	return b;
}
template <class ElemType, class NodeType>
int CircleListBase<ElemType, NodeType>::AddTail(const ElemType e)
{
	//���ȵ��û����AddTail����
	int index = LinkListBase<ElemType,NodeType>::AddTail( e);
	//Ȼ���β�ڵ��ָ�벿��ָ��ͷ�ڵ�
	LinkTailToHead();
	return index;
}
template <class ElemType, class NodeType>
bool CircleListBase<ElemType,NodeType>::IsEmpty() const
{
	if (this->m_pNodeHead->m_pNext == this->m_pNodeHead)
		return true;//ͷ�ڵ�ָ�벿��ָ��ͷ�ڵ�����ʱΪ��
	return false;
}
template <class ElemType, class NodeType>
NodeType* CircleListBase<ElemType, NodeType>::GetFirstNode() const
{
	if (IsEmpty())
		return NULL;//�ձ�ʱ����NULL
	return this->m_pNodeHead->m_pNext;//�ǿ�ʱ������Ԫ���
}
#endif