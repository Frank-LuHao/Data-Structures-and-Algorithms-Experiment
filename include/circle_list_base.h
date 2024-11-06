#if !defined(_CIRCLE_LIST_BASE_H_)
#define _CIRCLE_LIST_BASE_H_
#include "LinkListBase.h"
template <class ElemType,class NodeType>
class CircleListBase :public LinkListBase< ElemType, NodeType>
{
public:
	CircleListBase();//构造函数
	~CircleListBase();//析构函数
	CircleListBase(const CircleListBase& source);//拷贝构造函数
	virtual bool Delete(int position, ElemType& e);//重载删除指定位置元素
	virtual bool Insert(int position, const ElemType e);//重载在指定位置插入元素
	virtual int AddTail(const ElemType e);	//重载添加数据到链表末尾
	virtual bool IsEmpty() const;//判断线性表是否为空
	virtual CircleListBase& operator = (const CircleListBase<ElemType, NodeType>& source); // 重载赋值运算符
	virtual void Clear();//清空线性表
	virtual bool Next(ElemType& e);//当前节点后移,同时获取后继数据
protected:

	virtual NodeType* GetFirstNode() const;//获取首元结点
	virtual void LinkTailToHead();//链接尾节点到头节点
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
	LinkListBase<ElemType,NodeType>::operator=(source);//先调用基类的重载‘=’方法
	LinkTailToHead();//实现头、尾相连
	return *this;
}
template <class ElemType, class NodeType>
CircleListBase<ElemType, NodeType>::~CircleListBase()
{

}
template <class ElemType, class NodeType>
void CircleListBase< ElemType, NodeType>::LinkTailToHead()
{
	//链接尾节点到头节点
	this->Link(this->m_pNodeTail, this->m_pNodeHead);
}
template <class ElemType, class NodeType>
void CircleListBase<ElemType, NodeType>::Clear()
{
	//首先调用基类的Clear方法
	LinkListBase<ElemType,NodeType>::Clear();
	// 然后把尾节点的指针部分指向头节点
	LinkTailToHead();
}
template <class ElemType, class NodeType>
bool CircleListBase<ElemType,NodeType>::Next(ElemType& e)
{
	if (IsEmpty())
		return false;//循环链表为空时，返回false
	if (!this->m_pNodeCur || !this->m_pNodeCur->m_pNext)
		return false;//当前节点为空或当前节点的后继为空，都不能继续后移当前节点
	this->m_pNodeCur = this->m_pNodeCur->m_pNext;//当前节点变更为后继	
	//如果变更后的节点为头节点，继续变更为后继
	if (this->m_pNodeCur == this->m_pNodeHead)
		this->m_pNodeCur = this->m_pNodeCur->m_pNext;
	e = this->m_pNodeCur->m_TData;
	return true;

}
template <class ElemType, class NodeType>
bool CircleListBase<ElemType, NodeType>::Delete(int position, ElemType& e)
{
	//首先调用基类的Delete方法
	bool b = LinkListBase<ElemType,NodeType>::Delete(position, e);
	//然后把尾节点的指针部分指向头节点
	LinkTailToHead();
	return b;
}
template <class ElemType, class NodeType>
bool CircleListBase<ElemType, NodeType>::Insert(int position, const ElemType e)
{
	//首先调用基类的Insert方法
	bool b = LinkListBase<ElemType,NodeType>::Insert(position, e);
	//然后把尾节点的指针部分指向头节点
	LinkTailToHead();
	return b;
}
template <class ElemType, class NodeType>
int CircleListBase<ElemType, NodeType>::AddTail(const ElemType e)
{
	//首先调用基类的AddTail方法
	int index = LinkListBase<ElemType,NodeType>::AddTail( e);
	//然后把尾节点的指针部分指向头节点
	LinkTailToHead();
	return index;
}
template <class ElemType, class NodeType>
bool CircleListBase<ElemType,NodeType>::IsEmpty() const
{
	if (this->m_pNodeHead->m_pNext == this->m_pNodeHead)
		return true;//头节点指针部分指向头节点自身时为空
	return false;
}
template <class ElemType, class NodeType>
NodeType* CircleListBase<ElemType, NodeType>::GetFirstNode() const
{
	if (IsEmpty())
		return NULL;//空表时返回NULL
	return this->m_pNodeHead->m_pNext;//非空时返回首元结点
}
#endif