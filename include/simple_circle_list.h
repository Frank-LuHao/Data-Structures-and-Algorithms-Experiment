#if !defined(_SIMPLE_CIRCLE_LIST_H_)
#define _SIMPLE_CIRCLE_LIST_H_
#include "circle_list_base.h"
template <class ElemType> 
class SimpleCircleList :public CircleListBase< ElemType, Node<ElemType>>
{
public:
	SimpleCircleList();//构造函数
	~SimpleCircleList();//析构函数
	SimpleCircleList(const SimpleCircleList& source);//拷贝构造函数
protected:
	virtual bool Link(Node<ElemType>* pPreNode, Node<ElemType>* pCurNode);//重载纯虚函数Link	
	
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
	*this = source;//基类中重载了'='，调用赋值运算，构造对象
	this->LinkTailToHead();
}
template <class ElemType>
bool SimpleCircleList< ElemType>::Link(Node<ElemType>* pPreNode, Node<ElemType>* pCurNode)
{
	if (!pPreNode)
		return false;
	pPreNode->m_pNext = pCurNode;//前驱的m_pNext指向pCurNode
	return true;
}
#endif