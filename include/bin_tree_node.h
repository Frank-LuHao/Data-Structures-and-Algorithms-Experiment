#ifndef __BIN_TREE_NODE_H__
#define __BIN_TREE_NODE_H__

// 二叉树结点类模板
template <class ElemType>
struct BinTreeNode
{
// 数据成员:
	ElemType data;						// 数据成分
	BinTreeNode<ElemType>  *leftChild;	// 左孩子指针成分
	BinTreeNode<ElemType>  *rightChild;	// 右孩子指针成分

//	构造函数模板:
	BinTreeNode();		// 无参数的构造函数模板 
	BinTreeNode(const ElemType &ee,		// 已知数据元素值,指向左右孩子的指针构造一个结点
		BinTreeNode<ElemType> *lChild = NULL, 
		BinTreeNode<ElemType> *rChild = NULL);
};

// 二叉树结点类模板的实现部分
template <class ElemType>
BinTreeNode<ElemType>::BinTreeNode()
// 操作结果：构造一个叶结点
{
	leftChild = rightChild = NULL;	// 叶结点左右孩子为空
}

template <class ElemType>
BinTreeNode<ElemType>::BinTreeNode(const ElemType &e, 
	BinTreeNode<ElemType> *lChild, BinTreeNode<ElemType> *rChild)
// 操作结果：构造一个数据成分为val,左孩子为lChild,右孩子为rChild的结点
{	
	data = e;					// 数据元素值
	leftChild = lChild;			// 左孩子
	rightChild = rChild;		// 右孩子
}

#endif

