#if !defined(_DBL_LINK_LIST_H_)
#define _DBL_LINK_LIST_H_
#include "LinkListBase.h"
//using namespace LinkListBase< ElemType, DbNode<ElemType>>;


template <class ElemType> 

class DblLkList :public LinkListBase< ElemType, DbNode<ElemType>>
{
public:
	DblLkList();//构造函数
	~DblLkList();//析构函数
	DblLkList(const DblLkList& source);//拷贝构造函数
	void Tail();//当前节点指向尾节点
	bool Prev(ElemType &e);//当前节点变更为前驱
protected:
	virtual bool Link(DbNode<ElemType>* pPreNode, DbNode<ElemType>* pCurNode);//重载纯虚函数Link
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
	*this = source;//基类中重载了'='，调用赋值运算，构造对象
}
template <class ElemType>
bool DblLkList< ElemType>::Link(DbNode<ElemType>* pPreNode, DbNode<ElemType>* pCurNode)
{
	if (!pPreNode)
		return false;
	pPreNode->m_pNext = pCurNode;//前驱的m_pNext指向pCurNode
	if(pCurNode)
		pCurNode->m_pPre = pPreNode;//pCurNode的m_pPre指针指向前驱
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
		return false;//当前节点为空,不能继续前移当前节点
	if (this->m_pNodeCur->m_pPre == this->m_pNodeHead)
		return false;//当前节点首元结点,不能继续前移当前节点
	this->m_pNodeCur = this->m_pNodeCur->m_pPre;//变更为前驱

	e = this->m_pNodeCur->m_TData;//取前驱节点的数据
	return true;

}
#endif