#ifndef __CHILD_SIBLING_FOREST_NODE_H__
#define __CHILD_SIBLING_FOREST_NODE_H__

// �����ֵܱ�ʾ�������ģ��
template <class ElemType>
struct ChildSiblingTreeNode 
{
// ���ݳ�Ա:
	ElemType data;								// ���ݳɷ�
	ChildSiblingTreeNode<ElemType> *firstChild;	// ָ���׺���ָ��ɷ�
	ChildSiblingTreeNode<ElemType> *rightSibling;// ָ�����ֵ�ָ��ɷ�

// ���캯��ģ��:
	ChildSiblingTreeNode();						// �޲����Ĺ��캯��ģ��
	ChildSiblingTreeNode(const ElemType &e,		// ��֪���ݳɷ�ֵ��ָ���׺��������ֵ�ָ�뽨�����
		ChildSiblingTreeNode<ElemType> *fChild = NULL,
		ChildSiblingTreeNode<ElemType> *rSibling = NULL);	
		
};

// �����ֵܱ�ʾ�������ģ���ʵ�ֲ���
template<class ElemType>
ChildSiblingTreeNode<ElemType>::ChildSiblingTreeNode()
// �������������ָ��ɷ�Ϊ�յĽ��
{
	firstChild = rightSibling = NULL;			// ָ���׺��Ӽ��ֵܵ�ָ�붼Ϊ��
}

template<class ElemType>
ChildSiblingTreeNode<ElemType>::ChildSiblingTreeNode(const ElemType &e,			// ��֪������ֵ��˫��λ�ý����ṹ
		ChildSiblingTreeNode<ElemType> *fChild,
		ChildSiblingTreeNode<ElemType> *rSibling)
// �������������һ�����ݳɷ�Ϊitem���׺���ΪfChild�����ֵ�ΪrChild�Ľ��
{
	data = e;									// ���ݳɷ�
	firstChild = fChild;						// �׺���
	rightSibling = rSibling;					// ���ֵ�
}

#endif
