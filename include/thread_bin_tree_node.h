#ifndef __IN_THREAD_BIN_TREE_NODE_H__
#define __IN_THREAD_BIN_TREE_NODE_H__

#ifndef __POINTER_TAG_TYPE__
#define __POINTER_TAG_TYPE__
enum PointerTagType {CHILD_PTR, THREAD_PTR};
	// ָ���־����,CHILD_PTR(0):ָ���ӵ�ָ��,THREAD_PTR(1):ָ��������ָ��
#endif

// ���������������ģ��
template <class ElemType>
struct ThreadBinTreeNode
{
// ���ݳ�Ա:
	ElemType data;							// ���ݳɷ�
	ThreadBinTreeNode<ElemType> *leftChild;	// ����ָ��
	ThreadBinTreeNode<ElemType> *rightChild;// �Һ���ָ��
	ThreadBinTreeNode<ElemType>* parent;	// ˫��ָ����
	PointerTagType leftTag, rightTag;		// ���ұ�־

// ���캯��ģ��:
	ThreadBinTreeNode();					// �޲����Ĺ��캯��ģ��
	ThreadBinTreeNode(const ElemType &e,	// �����ݳɷ�ֵ,ָ�뼰��־������
		ThreadBinTreeNode<ElemType> *lChild = NULL, 
		ThreadBinTreeNode<ElemType> *rChild = NULL,
		ThreadBinTreeNode<ElemType>* pParent = NULL,
		PointerTagType leftTag = CHILD_PTR,
		PointerTagType rightTag = CHILD_PTR);
};

// ���������������ģ���ʵ�ֲ���
template <class ElemType>
ThreadBinTreeNode<ElemType>::ThreadBinTreeNode()
// �������������һ��Ҷ���
{
	leftChild = rightChild = parent = NULL;			// ָ��Ϊ��
	leftTag = rightTag = CHILD_PTR;			// ��־
}

template <class ElemType>
ThreadBinTreeNode<ElemType>::ThreadBinTreeNode(const ElemType &e, 
		ThreadBinTreeNode<ElemType> *lChild, 
		ThreadBinTreeNode<ElemType> *rChild,
		ThreadBinTreeNode<ElemType> *pParent,
		PointerTagType lTag,
		PointerTagType rTag)
// �������������һ�����ݳɷ�Ϊval,����ΪlChild,�Һ���ΪrChild�Ľ��,���
//	־ΪlTag,�ұ�־ΪrTag�Ľ��
{	
	data = e;								// ���ݳɷ�ֵ
	leftChild = lChild;						// ����
	rightChild = rChild;					// �Һ���
	parent = pParent;						// ˫��
	leftTag = lTag;							// ���־
	rightTag = rTag;						// �ұ�־
}

#endif

