#if !defined(_SIMPLE_LK_LIST_H_)
#define _SIMPLE_LK_LIST_H_
#include "LinkListBase.h"
template <class ElemType> class SimpleLkList :public LinkListBase< ElemType, Node<ElemType>>
{
public:
	SimpleLkList();//���캯��
	~SimpleLkList();//��������
	SimpleLkList(const SimpleLkList& source);//�������캯��
protected:
	virtual bool Link(Node<ElemType>* pPreNode,Node<ElemType>* pCurNode) ;//���ش��麯��Link
};
template <class ElemType>
SimpleLkList< ElemType>::SimpleLkList()
{
}
template <class ElemType>
SimpleLkList< ElemType>::SimpleLkList(const SimpleLkList& source)
{
	*this = source;//������������'='�����ø�ֵ���㣬�������
}
template <class ElemType>
SimpleLkList< ElemType>::~SimpleLkList()
{
}

template <class ElemType>
bool SimpleLkList< ElemType>::Link(Node<ElemType>* pPreNode, Node<ElemType>* pCurNode)
{
	if (!pPreNode)
		return false;
	pPreNode->m_pNext = pCurNode;//ǰ����m_pNextָ��pCurNode
	return true;
}
#endif