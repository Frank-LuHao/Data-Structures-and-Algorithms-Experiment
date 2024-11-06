#ifndef __SQ_BIN_TREE_NODE_H__
#define __SQ_BIN_TREE_NODE_H__

#ifndef __SQ_BIN_TREE_NODE_TAG_TYPE__
#define __SQ_BIN_TREE_NODE_TAG_TYPE__
enum SqBinTreeNodeTagType {EMPTY_NODE, USED_NODE};
#endif

// ˳��洢�����������ģ��
template <class ElemType>
struct SqBinTreeNode 
{
// ���ݳ�Ա:
	ElemType data;				// ���ݳɷ�
	SqBinTreeNodeTagType tag;	// ���ʹ�ñ�־

// ���캯��ģ��:
	SqBinTreeNode();			// �޲����Ĺ��캯��ģ��
	SqBinTreeNode(const ElemType &e, SqBinTreeNodeTagType tg = EMPTY_NODE);	
		// ��֪���ݳɷֺ�ʹ�ñ�־�����ṹ
};

// ˳��洢�����������ģ���ʵ�ֲ���
template<class ElemType>
SqBinTreeNode<ElemType>::SqBinTreeNode() 
// �������������ս��
{
   tag = EMPTY_NODE;			// ��һ���ս��
}

template<class ElemType>
SqBinTreeNode<ElemType>::SqBinTreeNode(const ElemType &e, SqBinTreeNodeTagType tg)
// �������������һ�����ݳɷ�Ϊitem��ʹ�ñ�־�ɷ�Ϊtg�Ľ��
{
   data = e;					// ���ݳɷ�
   tag = tg;					// ��־�ɷ�
}

#endif
