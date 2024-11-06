#ifndef __GEN_NODE_H__
#define __GEN_NODE_H__

#ifndef __REF_GEN_LIST_NODE_TYPE__
#define __REF_GEN_LIST_NODE_TYPE__
typedef enum {_HEAD, _ATOM,_LIST}RefGenListNodeType;
#endif

// 引用数法广义表结点类模样
template<class ElemType>
class RefGenListNode
{
public:
// 数据成员:

	RefGenListNodeType m_tag;				
		// 标志成分,HEAD(0):头结点, ATOM(1):原子结点, LIST(2):表结点
	RefGenListNode<ElemType> *m_pNext;	// 指向同一层中的下一个结点指针成分
	union
	{
		int m_nRef;						// tag=HEAD,头结点,存放引用数
		ElemType m_TData;					// tag=ATOM,存放原子结点的数据成分
		RefGenListNode<ElemType> *m_pSubLink;	// tag=LISK,存放指向子表的指针成分
	}; 

// 构造函数模板:
	RefGenListNode(RefGenListNodeType tg = _HEAD, RefGenListNode<ElemType> *next = NULL);
	// 由标志tg和指针next构造引用数法广义表结点
	void ClearLink();
};

// 引用数法广义表结点类模板的实现部分
template<class ElemType>
RefGenListNode<ElemType>::RefGenListNode(RefGenListNodeType tg, RefGenListNode<ElemType> *next)
// 操作结果：由标志tg和指针next构造引用数法广义表结点
{
	m_tag = tg;							// 标志
	m_pNext = next;					// 后继
}

template<class ElemType>
void RefGenListNode<ElemType>::ClearLink()
{
	m_pNext = NULL;
	m_nRef = 1;//头节点引用数为1
}
#endif
