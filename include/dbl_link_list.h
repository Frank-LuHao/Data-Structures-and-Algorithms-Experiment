#if !defined(_DBL_LINK_LIST_H_)
#define _DBL_LINK_LIST_H_
#include "LinkListBase.h"
//using namespace LinkListBase< ElemType, DbNode<ElemType>>;


template <class ElemType> 

class DblLkList :public LinkListBase< ElemType, DbNode<ElemType>>
{
public:
	DblLkList();//���캯��
	~DblLkList();//��������
	DblLkList(const DblLkList& source);//�������캯��
	void Tail();//��ǰ�ڵ�ָ��β�ڵ�
	bool Prev(ElemType &e);//��ǰ�ڵ���Ϊǰ��
protected:
	virtual bool Link(DbNode<ElemType>* pPreNode, DbNode<ElemType>* pCurNode);//���ش��麯��Link
};
template <class ElemType>
DblLkList< ElemType>::DblLkList()
{
}

template <class ElemType>
DblLkList< ElemType>::~DblLkList()
{
}
template <class ElemType>
DblLkList< ElemType>::DblLkList(const DblLkList& source)
{
	*this = source;//������������'='�����ø�ֵ���㣬�������
}
template <class ElemType>
bool DblLkList< ElemType>::Link(DbNode<ElemType>* pPreNode, DbNode<ElemType>* pCurNode)
{
	if (!pPreNode)
		return false;
	pPreNode->m_pNext = pCurNode;//ǰ����m_pNextָ��pCurNode
	if(pCurNode)
		pCurNode->m_pPre = pPreNode;//pCurNode��m_pPreָ��ָ��ǰ��
	return true;
}
template <class ElemType>
void DblLkList< ElemType>::Tail()
{
	LinkListBase < ElemType, DbNode<ElemType>>::m_pNodeCur = LinkListBase < ElemType, DbNode<ElemType>>::m_pNodeTail;
}
template <class ElemType>
bool DblLkList< ElemType>::Prev(ElemType& e)
{

	if (!this->m_pNodeCur)
		return false;//��ǰ�ڵ�Ϊ��,���ܼ���ǰ�Ƶ�ǰ�ڵ�
	if (this->m_pNodeCur->m_pPre == this->m_pNodeHead)
		return false;//��ǰ�ڵ���Ԫ���,���ܼ���ǰ�Ƶ�ǰ�ڵ�
	this->m_pNodeCur = this->m_pNodeCur->m_pPre;//���Ϊǰ��

	e = this->m_pNodeCur->m_TData;//ȡǰ���ڵ������
	return true;

}
#endif