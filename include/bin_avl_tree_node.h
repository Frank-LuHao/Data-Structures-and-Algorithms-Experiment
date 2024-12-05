#if !defined(_BIN_AVL_TREE_NODE_H_)
#define _BIN_AVL_TREE_NODE_H_

#define LH 1								// 左高
#define EH 0								// 等高
#define RH -1								// 右高

// 二叉平衡树结点类模板
template <class ElemType>
struct BinAVLTreeNode
{
// 数据成员:
	ElemType data;							// 数据成分
	int bf;									// 结点的平衡因子
	BinAVLTreeNode<ElemType> *leftChild;	// 左孩子指针成分
	BinAVLTreeNode<ElemType> *rightChild;	// 右孩子指针成分

// 构造函数模板:
	BinAVLTreeNode();						// 无参数的构造函数模板 
	BinAVLTreeNode(const ElemType &e,		// 已知数据元素值,平衡因子和指向左右孩子的指针构造一个结点
		int bFactor = 0,
		BinAVLTreeNode<ElemType> *lChild = NULL, 
		BinAVLTreeNode<ElemType> *rChild = NULL);
};

// 二叉平衡树结点类模板的实现部分
template <class ElemType>
BinAVLTreeNode<ElemType>::BinAVLTreeNode()
// 操作结果：构造一个叶结点
{
	bf = 0;							// 平衡因子
	leftChild = rightChild = NULL;	// 叶结点左右孩子为空
}

template <class ElemType>
BinAVLTreeNode<ElemType>::BinAVLTreeNode(const ElemType &e, int bFactor, 
								   BinAVLTreeNode<ElemType> *lChild, 
								   BinAVLTreeNode<ElemType> *rChild)

// 操作结果：构造一个数据成分为val,平衡因子为bFactor,左孩子为lChild,右孩子为rChild的结点
{	
	data = e;						// 数据元素值
	bf = bFactor;					// 平衡因子
	leftChild = lChild;				// 左孩子
	rightChild = rChild;			// 右孩子
}

#endif

