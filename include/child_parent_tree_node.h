#ifndef __CHILD_PARENT_TREE_NODE_H__
#define __CHILD_PARENT_TREE_NODE_H__

#include "SimpleLkList.h"				// 线性链表

// 孩子双法表示树结点类模板
template <class ElemType>
struct ChildParentTreeNode 
{
// 数据成员:
	ElemType data;					// 数据成分
	SimpleLkList<int> childLkList;		// 孩子链表	
	int parent;						// 双亲位置成分

// 构造函数模板:
	ChildParentTreeNode();			// 无参数的构造函数模板
	ChildParentTreeNode(const ElemType &e, int pt = -1);// 已知数据域值和双亲位置建立结构
};

// 孩子双法表示树结点类模板的实现部分
template<class ElemType>
ChildParentTreeNode<ElemType>::ChildParentTreeNode()
// 操作结果：构造双亲成分为-1的结点
{
	parent = -1;
}

template<class ElemType>
ChildParentTreeNode<ElemType>::ChildParentTreeNode(const ElemType &e, int pt)
// 操作结果：构造一个数据成分为item和双亲域为pt的结点
{
	data = e;						// 数据元素值
	parent = pt;					// 双亲
}

#endif
