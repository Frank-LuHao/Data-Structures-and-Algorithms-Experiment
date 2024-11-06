#ifndef __CHILD_SIBLING_FOREST_NODE_H__
#define __CHILD_SIBLING_FOREST_NODE_H__

// 孩子兄弟表示树结点类模板
template <class ElemType>
struct ChildSiblingTreeNode 
{
// 数据成员:
	ElemType data;								// 数据成分
	ChildSiblingTreeNode<ElemType> *firstChild;	// 指向首孩子指针成分
	ChildSiblingTreeNode<ElemType> *rightSibling;// 指向右兄弟指针成分

// 构造函数模板:
	ChildSiblingTreeNode();						// 无参数的构造函数模板
	ChildSiblingTreeNode(const ElemType &e,		// 已知数据成分值、指向首孩子与右兄弟指针建立结点
		ChildSiblingTreeNode<ElemType> *fChild = NULL,
		ChildSiblingTreeNode<ElemType> *rSibling = NULL);	
		
};

// 孩子兄弟表示树结点类模板的实现部分
template<class ElemType>
ChildSiblingTreeNode<ElemType>::ChildSiblingTreeNode()
// 操作结果：构造指针成分为空的结点
{
	firstChild = rightSibling = NULL;			// 指向首孩子及兄弟的指针都为空
}

template<class ElemType>
ChildSiblingTreeNode<ElemType>::ChildSiblingTreeNode(const ElemType &e,			// 已知数据域值和双亲位置建立结构
		ChildSiblingTreeNode<ElemType> *fChild,
		ChildSiblingTreeNode<ElemType> *rSibling)
// 操作结果：构造一个数据成分为item和首孩子为fChild，右兄弟为rChild的结点
{
	data = e;									// 数据成分
	firstChild = fChild;						// 首孩子
	rightSibling = rSibling;					// 右兄弟
}

#endif
