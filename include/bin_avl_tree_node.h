#if !defined(_BIN_AVL_TREE_NODE_H_)
#define _BIN_AVL_TREE_NODE_H_

#define LH 1								// ���
#define EH 0								// �ȸ�
#define RH -1								// �Ҹ�

// ����ƽ���������ģ��
template <class ElemType>
struct BinAVLTreeNode
{
// ���ݳ�Ա:
	ElemType data;							// ���ݳɷ�
	int bf;									// ����ƽ������
	BinAVLTreeNode<ElemType> *leftChild;	// ����ָ��ɷ�
	BinAVLTreeNode<ElemType> *rightChild;	// �Һ���ָ��ɷ�

// ���캯��ģ��:
	BinAVLTreeNode();						// �޲����Ĺ��캯��ģ�� 
	BinAVLTreeNode(const ElemType &e,		// ��֪����Ԫ��ֵ,ƽ�����Ӻ�ָ�����Һ��ӵ�ָ�빹��һ�����
		int bFactor = 0,
		BinAVLTreeNode<ElemType> *lChild = NULL, 
		BinAVLTreeNode<ElemType> *rChild = NULL);
};

// ����ƽ���������ģ���ʵ�ֲ���
template <class ElemType>
BinAVLTreeNode<ElemType>::BinAVLTreeNode()
// �������������һ��Ҷ���
{
	bf = 0;							// ƽ������
	leftChild = rightChild = NULL;	// Ҷ������Һ���Ϊ��
}

template <class ElemType>
BinAVLTreeNode<ElemType>::BinAVLTreeNode(const ElemType &e, int bFactor, 
								   BinAVLTreeNode<ElemType> *lChild, 
								   BinAVLTreeNode<ElemType> *rChild)

// �������������һ�����ݳɷ�Ϊval,ƽ������ΪbFactor,����ΪlChild,�Һ���ΪrChild�Ľ��
{	
	data = e;						// ����Ԫ��ֵ
	bf = bFactor;					// ƽ������
	leftChild = lChild;				// ����
	rightChild = rChild;			// �Һ���
}

#endif

