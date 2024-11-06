#if !defined(_HUF_TREE_NODE_H_)
#define _HUF_TREE_NODE_H_
#include <stdlib.h>
// 哈夫曼树结点类模板
template <class ElemType,class wtType>
class HufTreeNode
{
public:
	HufTreeNode();
	HufTreeNode( HufTreeNode<ElemType, wtType>* pLeft, HufTreeNode<ElemType, wtType>* pRight);
	~HufTreeNode();
	bool IsLeaf();//是否是叶子节点
	HufTreeNode<ElemType, wtType>* leftChild;//左孩子
	HufTreeNode<ElemType, wtType>* rightChild;//右孩子
	HufTreeNode<ElemType, wtType>* parent;//双亲
	ElemType data;//保存数据的字段
	wtType  m_Weight;//权重

};
template <class ElemType, class wtType>
HufTreeNode<ElemType, wtType>::HufTreeNode()
{
	leftChild = NULL;
	rightChild = NULL;
	parent = NULL;//双亲
}
template <class ElemType, class wtType>
HufTreeNode<ElemType, wtType>::HufTreeNode( HufTreeNode<ElemType, wtType>* pLeft, HufTreeNode<ElemType, wtType>* pRight)
{
	leftChild = pLeft;
	if (pLeft)
		pLeft->parent = this;//左孩子的双亲为自身
	rightChild = pRight;
	if (pRight)
		pRight->parent = this;//右孩子的双亲为自身
	parent = NULL;
}
template <class ElemType, class wtType>
HufTreeNode<ElemType, wtType>::~HufTreeNode()
{
}
template <class ElemType, class wtType>
bool HufTreeNode<ElemType, wtType>::IsLeaf()
{
	if (!leftChild && !rightChild)
		return true;
	return false;
}
#endif