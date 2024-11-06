#if !defined(__NODE_H__)
#define __NODE_H__

// 单链表结点类模板
template <class ElemType>
class Node 
{

public:
// 构造函数模板:
	Node();						// 无参数的构造函数模板
	Node(const ElemType &e, Node<ElemType> *link = nullptr);	// 已知数据元素值和指针建立结点
	~Node();//析构函数
	// 数据成员:
	ElemType m_TData;				// 数据成分
	Node<ElemType>* m_pNext;		// 指针成分

	virtual void ClearLink(); //清空链接状态
};

// 结点类模板的实现部分
template<class ElemType>
//操作结果：析构函数
Node<ElemType>::~Node()
{

}

template<class ElemType>
Node<ElemType>::Node()
// 操作结果：构造指针成分为空的结点
{
	m_pNext = nullptr;
}

template<class ElemType>
Node<ElemType>::Node(const ElemType &e, Node<ElemType> *link)
// 操作结果：构造一个数据成分为e和指针成分为link的结点
{
	m_TData = e;
	m_pNext = link;
}
template<class ElemType>
//操作结果：清空链接状态
void Node<ElemType>::ClearLink()
{
	m_pNext = nullptr;
}
// 双链表结点类模板
template <class ElemType>
class DbNode 
{

public:
	// 构造函数模板:
	DbNode();						// 无参数的构造函数模板
	DbNode(const ElemType& e, DbNode<ElemType>* prelink = nullptr, DbNode<ElemType>* nextlink = nullptr);	// 已知数据元素值和指针建立结点
	~DbNode();//析构函数
	//m_TData,m_pNext等数据继承于Node
	// 数据成员:
	DbNode<ElemType>* m_pPre;		// 指针向前的节点的指针成分
	DbNode<ElemType>* m_pNext;		//指向后节点的指针成分
	ElemType m_TData;				// 数据成分
	virtual void ClearLink(); //清空链接状态

};
template<class ElemType>
DbNode<ElemType>::DbNode()
// 操作结果：构造指针成分为空的结点
{
	m_pPre = nullptr;
	m_pNext = nullptr;
}
template<class ElemType>
DbNode<ElemType>::DbNode(const ElemType& e, DbNode<ElemType>* prelink , DbNode<ElemType>* nextlink )
// 操作结果：构造指针成分为空的结点
{
	this->m_TData = e;
	m_pPre = prelink;
	m_pNext = nextlink;

}

template<class ElemType>
DbNode<ElemType>::~DbNode()
// 操作结果：析构函数
{
}

template<class ElemType>
//操作结果：清空链接状态
void DbNode<ElemType>::ClearLink()
{
	m_pNext = nullptr;
	m_pPre = nullptr;
}
#endif
