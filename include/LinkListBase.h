#if !defined(_LINK_LIST_BASE_H_)
#define  _LINK_LIST_BASE_H_
#include "ListBase.h"
#include "node.h"

template <class ElemType, class NodeType>  
class LinkListBase:public ListBase<ElemType>
{
public:
	LinkListBase();//构造函数
	virtual ~LinkListBase();//析构函数
	virtual int Length() const;//获取线性表中元素个数
	virtual bool IsEmpty() const;//判断线性表是否为空
	virtual void Clear();//清空线性表
	virtual bool GetElem(int position, ElemType& e) const;//获取指定位置元素
	virtual bool SetElem(int position, const ElemType e);//设置指定位置元素
	virtual bool Delete(int position, ElemType& e);//删除指定位置元素
	virtual bool Insert(int position, const ElemType e) ;//在指定位置插入元素
	virtual void Traverse(bool (*visit)(const ElemType&))const ;//遍历线性表
	virtual int AddTail(const ElemType e);	//添加数据到线性表末尾
	virtual LinkListBase& operator = (const LinkListBase<ElemType, NodeType>& source); // 重载赋值运算符
	virtual void Head();//当前节点指向头节点
	virtual bool Next(ElemType& e);//当前节点后移,同时获取后继数据
	virtual bool DeleteCur(ElemType& e);//删除当前节点元素
protected:
	virtual bool Link(NodeType* pPreNode,NodeType* pNextNode) =0;//链接两个节点
	virtual NodeType* FindNode(int position, NodeType*&  pPreNode) const;//根据位置号寻找节点
	virtual NodeType* GetFirstNode() const;//获取首元结点
	virtual bool IsEndWhile(NodeType* pNode) const;//是否结束循环语句
	NodeType* m_pNodeHead;//头节点指针
	NodeType* m_pNodeTail;//尾节点指针
	NodeType* m_pNodeCur;//当前节点指针
	int m_nNodeCounts;//当前节点数
	bool m_bSpecialDetrcutiom;//特殊析构方式，如广义表
	
};
//LinkListBase实现
template <class ElemType,class NodeType>
LinkListBase<ElemType, NodeType>::LinkListBase()
{
	m_bSpecialDetrcutiom = false;
	m_pNodeHead = new NodeType;
	m_nNodeCounts = 0;
	m_pNodeTail = m_pNodeHead;//初始化时头节点就是尾节点
	m_pNodeCur = m_pNodeHead;//当前节点为头节点
}

template <class ElemType, class NodeType>
LinkListBase<ElemType, NodeType>& LinkListBase<ElemType, NodeType>::operator = (const LinkListBase<ElemType, NodeType>& source)
{

	Clear();//清空节点
	m_bSpecialDetrcutiom = false;
	NodeType* pSrcNodeHead = source.m_pNodeHead;//获取源链表头节点
	NodeType* pSrcNode = source.GetFirstNode();//pSrcNode为source的首元结点
	NodeType* pPreNode = m_pNodeHead;
	while (!source.IsEndWhile(pSrcNode))
	{
		NodeType* pNewNode = new NodeType;
		pNewNode->m_TData = pSrcNode->m_TData;//新节点赋值
		Link(pPreNode, pNewNode);//链接前节点和新节点
		pSrcNode = pSrcNode->m_pNext;//源链表的当前节点后移
		pPreNode = pNewNode;//前节点变更为新节点
		m_nNodeCounts++;
	}
	m_pNodeTail = pPreNode;
	m_pNodeCur = m_pNodeHead;
	return *this;
}

template <class ElemType, class NodeType>
LinkListBase<ElemType, NodeType>::~LinkListBase()
{
	if (!m_bSpecialDetrcutiom)
	{

		Clear();//清空节点
		if (m_pNodeHead)
		{
			delete m_pNodeHead;
			m_pNodeHead = NULL;
		}
	}
}


template <class ElemType, class NodeType>
int LinkListBase<ElemType, NodeType>::Length() const
{
	int nNodeCounts = 0;//节点数目计数器

/*
	如果没有m_nNodeCounts作为节点数目计数器，需要遍历节点
	NodeType* pNode = GetFirstNode();
	while (!IsEndWhile(pNode))
	{
		nNodeCounts++;
		pNode = pNode->m_pNext;
	}
*/
	nNodeCounts = m_nNodeCounts;
	return nNodeCounts;
}

template <class ElemType, class NodeType>
NodeType* LinkListBase<ElemType, NodeType>::GetFirstNode() const
{
	NodeType* pNode = m_pNodeHead->m_pNext;
	return pNode;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::IsEndWhile(NodeType* pNode) const
{
	if (!pNode || pNode == m_pNodeHead)
		return true;
	return false;
}
template <class ElemType, class NodeType>
void LinkListBase<ElemType, NodeType>::Head()
{
	m_pNodeCur = m_pNodeHead;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::Next(ElemType &e)
{
	if (!m_pNodeCur || !m_pNodeCur->m_pNext)
		return false;//当前节点为空或当前节点的后继为空，都不能继续后移当前节点
	m_pNodeCur = m_pNodeCur->m_pNext;
	e = m_pNodeCur->m_TData;
	return true;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::DeleteCur(ElemType& e)
{
	if (!m_pNodeCur || m_pNodeCur == m_pNodeHead)
		return false;//当前节点为空，或当前节点为头节点，都不能删除
	NodeType* pNextNode = m_pNodeCur->m_pNext;//后继可以直接得到
	NodeType* pPreNode = m_pNodeHead;//前驱初始化为头节点
	NodeType* pNode = GetFirstNode();
	bool bFindPre = false;
	while (!IsEndWhile(pNode))
	{
		if (pNode == m_pNodeCur)
		{
			bFindPre = true;
			break;//找到前驱，退出循环
		}
		pNode = pNode->m_pNext;
		pPreNode = pPreNode->m_pNext;
	}
	if (!bFindPre)
		return false;
	Link(pPreNode, pNode->m_pNext);//链接前驱和后继节点
	e = pNode->m_TData;

	if (pNode == m_pNodeTail)
	{//如果删除的是尾节点
		m_pNodeTail = pPreNode; //变更尾节点为删除节点的前驱
	}
	m_pNodeCur = pPreNode;//当前节点变更为前驱
	delete pNode;//删除节点
	m_nNodeCounts--;//总节点数减一
	return true;
}
template <class ElemType, class NodeType>
void LinkListBase<ElemType, NodeType>::Clear()
{
	if (!m_pNodeHead)
		return;
	NodeType* pNode = GetFirstNode();
	//下面的循环清空节点
	while (!IsEndWhile(pNode))
	{
		NodeType* pNodeNext = pNode->m_pNext;
		delete pNode;
		pNode = pNodeNext;
	}
	m_pNodeHead->ClearLink();//清空头节点的链接状态
	m_pNodeTail = m_pNodeHead;//清空后头节点为尾节点
	m_pNodeCur = m_pNodeHead;//当前节点为头节点
	m_nNodeCounts = 0;//节点数目为0
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::IsEmpty() const
{
	if (m_pNodeHead->m_pNext)
		return false;
	return true;
}
template <class ElemType, class NodeType>
NodeType* LinkListBase<ElemType, NodeType>::FindNode(int position, NodeType*& pPreNode) const
{
	if (position < 0)
		return NULL;
	NodeType* pNode = GetFirstNode();//获取首元结点
	pPreNode = m_pNodeHead;//首元结点的前驱是头节点
	NodeType* pFindNode = NULL;//定义找到的节点，初始值为空
	int nIndex = 0;//定义节点位置号变量，初始值为0
	while (!IsEndWhile(pNode))
	{
		if (nIndex == position)
		{//当前位置号为查找位置号
			pFindNode = pNode;//找到节点
			break;//退出循环
		}
		pPreNode = pNode;//前驱变更为当前节点
		pNode = pNode->m_pNext;//当前节点变更为后驱
		nIndex++;//位置号变量加1
	}
	return pFindNode;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::SetElem(int position, const ElemType e) 
{
	NodeType* pPreNode;
	NodeType* pNode = FindNode(position, pPreNode);//根据位置号查找节点
	if (!pNode)
		return false;//没有找到节点，返回false;
	pNode->m_TData = e;
	return true;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::GetElem(int position, ElemType& e) const
{
	NodeType* pPreNode;
	NodeType* pNode =  FindNode(position, pPreNode);//根据位置号查找节点
	if (!pNode)
		return false;//没有找到节点，返回false;
	e = pNode->m_TData;
	return true;
}

template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::Delete(int position, ElemType& e)
{
	NodeType* pPreNode;
	NodeType* pNode = FindNode(position, pPreNode);//根据位置号查找节点
	if (!pNode)
		return false;//没有找到节点，返回false;
	
	Link(pPreNode, pNode->m_pNext);//链接前驱和后继节点
	e = pNode->m_TData;
	
	if (pNode == m_pNodeTail)
	{//如果删除的是尾节点
		m_pNodeTail = pPreNode; //变更尾节点为删除节点的前驱
	}
	m_pNodeCur = pPreNode;//当前节点变更为前驱
	delete pNode;//删除节点
	m_nNodeCounts--;//总节点数减一
	return true;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::Insert(int position, const ElemType e)
{
	NodeType* pPreNode;//待插入位置的前驱节点指针
	NodeType* pNode = FindNode(position, pPreNode);//根据位置号查找节点
	if (!pNode)
	{ //没有找到节点
		if (position == m_nNodeCounts)//尾部插入判断
			pPreNode = m_pNodeTail;//尾部插入时，前驱为尾节点
		else
			return false;//位置号错误，返回false
	}
	NodeType* pNewNode = new NodeType;//创建新节点
	pNewNode->m_TData = e;//给新节点的数据赋值
	Link(pPreNode, pNewNode);//链接前节点和新节点
	Link(pNewNode, pNode);//链接新节点和插入位置的节点
	if (position == m_nNodeCounts)//尾部插入判断
		m_pNodeTail = pNewNode;//尾部插入时，尾节点为新节点
	m_pNodeCur = pNewNode;//变更当前节点为新节点
	m_nNodeCounts++;
	return true;
}


template <class ElemType, class NodeType>
void LinkListBase<ElemType, NodeType>::Traverse(bool (*visit)(const ElemType&))const
{
	if (!visit)
		return;
	NodeType* pNode =GetFirstNode();
	while (!IsEndWhile(pNode))
	{
		if (!(*visit)(pNode->m_TData))
			break;//函数指针访问函数返回为false退出循环
		pNode = pNode->m_pNext;
	}
}
template <class ElemType, class NodeType>
int LinkListBase<ElemType, NodeType>::AddTail(const ElemType e)
{
	NodeType* pNewNode = new NodeType;
	pNewNode->m_TData = e;//给新节点的数据赋值
	Link(m_pNodeTail, pNewNode);//链接尾节点和新节点
	m_pNodeTail = pNewNode;//尾节点变更为新节点
	m_pNodeCur = pNewNode;//变更当前节点为新节点
	m_nNodeCounts++;//节点数目加一
	return m_nNodeCounts - 1;// 返回新加节点的位置号
}

#endif
