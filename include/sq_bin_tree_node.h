#ifndef __SQ_BIN_TREE_NODE_H__
#define __SQ_BIN_TREE_NODE_H__

#ifndef __SQ_BIN_TREE_NODE_TAG_TYPE__
#define __SQ_BIN_TREE_NODE_TAG_TYPE__
enum SqBinTreeNodeTagType {EMPTY_NODE, USED_NODE};
#endif

// 顺序存储二叉树结点类模板
template <class ElemType>
struct SqBinTreeNode 
{
// 数据成员:
	ElemType data;				// 数据成分
	SqBinTreeNodeTagType tag;	// 结点使用标志

// 构造函数模板:
	SqBinTreeNode();			// 无参数的构造函数模板
	SqBinTreeNode(const ElemType &e, SqBinTreeNodeTagType tg = EMPTY_NODE);	
		// 已知数据成分和使用标志建立结构
};

// 顺序存储二叉树结点类模板的实现部分
template<class ElemType>
SqBinTreeNode<ElemType>::SqBinTreeNode() 
// 操作结果：构造空结点
{
   tag = EMPTY_NODE;			// 标一个空结点
}

template<class ElemType>
SqBinTreeNode<ElemType>::SqBinTreeNode(const ElemType &e, SqBinTreeNodeTagType tg)
// 操作结果：构造一个数据成分为item和使用标志成分为tg的结点
{
   data = e;					// 数据成分
   tag = tg;					// 标志成分
}

#endif
