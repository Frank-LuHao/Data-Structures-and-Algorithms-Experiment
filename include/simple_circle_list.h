#if !defined(_SIMPLE_CIRCLE_LIST_H_)
#define _SIMPLE_CIRCLE_LIST_H_
#include "circle_list_base.h"
template <class ElemType> 
class SimpleCircleList :public CircleListBase< ElemType, Node<ElemType>>
{
public:
	SimpleCircleList();//���캯��
	~SimpleCircleList();//��������
	SimpleCircleList(const SimpleCircleList& source);//�������캯��
protected:
	virtual bool Link(Node<ElemType>* pPreNode, Node<ElemType>* pCurNode);//���ش��麯��Link	
	
};

template <class ElemType>
SimpleCircleList< ElemType>::SimpleCircleList()
{
	this->LinkTailToHead();
}
template <class ElemType>
SimpleCircleList< ElemType>::~SimpleCircleList()
{

}
template <class ElemType>
SimpleCircleList< ElemType>::SimpleCircleList(const SimpleCircleList& source)
{
	*this = source;//������������'='�����ø�ֵ���㣬�������
	this->LinkTailToHead();
}
template <class ElemType>
bool SimpleCircleList< ElemType>::Link(Node<ElemType>* pPreNode, Node<ElemType>* pCurNode)
{
	if (!pPreNode)
		return false;
	pPreNode->m_pNext = pCurNode;//ǰ����m_pNextָ��pCurNode
	return true;
}
#endif