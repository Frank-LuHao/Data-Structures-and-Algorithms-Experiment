#if !defined(_SIMPLE_LK_LIST_H_)
#define _SIMPLE_LK_LIST_H_
#include "LinkListBase.h"
template <class ElemType> class SimpleLkList :public LinkListBase< ElemType, Node<ElemType>>
{
public:
	SimpleLkList();//构造函数
	~SimpleLkList();//析构函数
	SimpleLkList(const SimpleLkList& source);//拷贝构造函数
protected:
	virtual bool Link(Node<ElemType>* pPreNode,Node<ElemType>* pCurNode) ;//重载纯虚函数Link
};
template <class ElemType>
SimpleLkList< ElemType>::SimpleLkList()
{
}
template <class ElemType>
SimpleLkList< ElemType>::SimpleLkList(const SimpleLkList& source)
{
	*this = source;//基类中重载了'='，调用赋值运算，构造对象
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
	pPreNode->m_pNext = pCurNode;//前驱的m_pNext指向pCurNode
	return true;
}
#endif