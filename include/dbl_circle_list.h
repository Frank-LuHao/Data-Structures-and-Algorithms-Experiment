#if !defined(_DOUBLE_CIRCLE_LIST_H_)
#define _DOUBLE_CIRCLE_LIST_H_
#include "node.h"
#include "circle_list_base.h"
template <class ElemType>
class DblCircleList :public CircleListBase< ElemType, DbNode<ElemType>>
{
public:
	DblCircleList();//构造函数
	~DblCircleList();//析构函数
	DblCircleList(const DblCircleList& source);//拷贝构造函数
	void Tail();//当前节点指针指向尾节点
	bool Prev(ElemType& e);//当前节点变更为前驱 
	bool Reverse();//链表中所有的元素位置翻转
protected:
	virtual bool Link(DbNode<ElemType>* pPreNode, DbNode<ElemType>* pCurNode);//重载纯虚函数Link	

};

template <class ElemType>
DblCircleList< ElemType>::DblCircleList()
{
	this->LinkTailToHead();//调用父类方法首先首尾相连
}
template <class ElemType>
DblCircleList< ElemType>::~DblCircleList()
{

}
template <class ElemType>
DblCircleList< ElemType>::DblCircleList(const DblCircleList& source)
{
	*this = source;//基类中重载了'='，调用赋值运算，构造对象
	this->LinkTailToHead();//调用父类方法首先首尾相连
}
template <class ElemType>
bool DblCircleList< ElemType>::Link(DbNode<ElemType>* pPreNode, DbNode<ElemType>* pCurNode)
{
	if (!pPreNode)
		return false;
	pPreNode->m_pNext = pCurNode;//前驱的m_pNext指向pCurNode
	if (pCurNode)
	{
		pCurNode->m_pPre = pPreNode;//如果pCurNode不为空，其m_pPre链接前驱节点
	}
	return true;
}
template <class ElemType>
//操作结果:当前节点指针指向尾节点
void DblCircleList< ElemType>::Tail()
{
	this->m_pCurNode = this->m_pNodeTail;//当前节点指向尾节点
}
template <class ElemType>
//操作结果:当前节点变更为前驱,同时获取前驱的存在值
bool DblCircleList< ElemType>::Prev(ElemType& e)
{
	if (this->IsEmpty())
		return false;//链表为空，不能获取当前节点的前驱
	if (!this->m_pNodeCur || !this->m_pNodeCur->m_pPre)
		return false;//当前节点为空或当前节点的前驱为空，都不能继续前移当前节点
	this->m_pNodeCur = this->m_pNodeCur->m_pPre;//当前节点变更为前驱
	//如果变更后的节点为头节点，继续变更为前驱
	if (this->m_pNodeCur == this->m_pNodeHead)
		this->m_pNodeCur = this->m_pNodeCur->m_pPre;
	e = this->m_pNodeCur->m_TData;
	return true;
}

template <class ElemType>
//操作结果:链表中所有的元素位置翻转
bool DblCircleList< ElemType>::Reverse()
{
	if (this->IsEmpty())
		return false;//空链表不用翻转
	DbNode<ElemType>* pFirstNode = this->GetFirstNode();//获取首元节点
	DbNode<ElemType>* pNode = this->m_pNodeHead;//从头节点开始
	//从头节点开始翻转，节点的前驱和后继互换，遍历链表
	do
	{
		DbNode<ElemType>* pNext = pNode->m_pNext;//暂存pNode的后继
		//交换pNode的前驱和后继
		pNode->m_pNext = pNode->m_pPre;
		pNode->m_pPre = pNext;
		pNode = pNext;//变更为后继
	} while (pNode !=this->m_pNodeHead);
	//翻转后尾节点为翻转前的首元节点
	this->m_pNodeTail = pFirstNode;
	//也可以是：this->m_pNodeTail = this->m_pNodeHead->m_pPre;
	
}
#endif
 
