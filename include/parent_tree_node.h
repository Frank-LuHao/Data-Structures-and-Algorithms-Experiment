#ifndef __PARENT_TREE_NODE_H__
#define __PARENT_TREE_NODE_H__

// 双亲法表示树结点类模板 
template <class ElemType>
struct ParentTreeNode 
{
// 数据成员:
	ElemType data;									// 数据成分
	int parent;										// 双亲位置成分

// 构造函数模板:
	ParentTreeNode();								// 无参数的构造函数模板
	ParentTreeNode(const ElemType &e, int pt = -1);	// 已知数据成分值和双亲位置建立结点
};

// 双法表示树结点类模板的实现部分
template<class ElemType>
ParentTreeNode<ElemType>::ParentTreeNode()
// 操作结果：构造双亲成分为-1的结点
{
	parent = -1;
}

template<class ElemType>
ParentTreeNode<ElemType>::ParentTreeNode(const ElemType &e, int pt)
// 操作结果：构造一个数据成分为item和双亲成分为pt的结点
{
	data = e;
	parent = pt;
}

#endif
