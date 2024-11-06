#if !defined(_HUF_TREE_NODE_H_)
#define _HUF_TREE_NODE_H_
#include <stdlib.h>
// �������������ģ��
template <class ElemType,class wtType>
class HufTreeNode
{
public:
	HufTreeNode();
	HufTreeNode( HufTreeNode<ElemType, wtType>* pLeft, HufTreeNode<ElemType, wtType>* pRight);
	~HufTreeNode();
	bool IsLeaf();//�Ƿ���Ҷ�ӽڵ�
	HufTreeNode<ElemType, wtType>* leftChild;//����
	HufTreeNode<ElemType, wtType>* rightChild;//�Һ���
	HufTreeNode<ElemType, wtType>* parent;//˫��
	ElemType data;//�������ݵ��ֶ�
	wtType  m_Weight;//Ȩ��

};
template <class ElemType, class wtType>
HufTreeNode<ElemType, wtType>::HufTreeNode()
{
	leftChild = NULL;
	rightChild = NULL;
	parent = NULL;//˫��
}
template <class ElemType, class wtType>
HufTreeNode<ElemType, wtType>::HufTreeNode( HufTreeNode<ElemType, wtType>* pLeft, HufTreeNode<ElemType, wtType>* pRight)
{
	leftChild = pLeft;
	if (pLeft)
		pLeft->parent = this;//���ӵ�˫��Ϊ����
	rightChild = pRight;
	if (pRight)
		pRight->parent = this;//�Һ��ӵ�˫��Ϊ����
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