#ifndef __PARENT_TREE_NODE_H__
#define __PARENT_TREE_NODE_H__

// ˫�׷���ʾ�������ģ�� 
template <class ElemType>
struct ParentTreeNode 
{
// ���ݳ�Ա:
	ElemType data;									// ���ݳɷ�
	int parent;										// ˫��λ�óɷ�

// ���캯��ģ��:
	ParentTreeNode();								// �޲����Ĺ��캯��ģ��
	ParentTreeNode(const ElemType &e, int pt = -1);	// ��֪���ݳɷ�ֵ��˫��λ�ý������
};

// ˫����ʾ�������ģ���ʵ�ֲ���
template<class ElemType>
ParentTreeNode<ElemType>::ParentTreeNode()
// �������������˫�׳ɷ�Ϊ-1�Ľ��
{
	parent = -1;
}

template<class ElemType>
ParentTreeNode<ElemType>::ParentTreeNode(const ElemType &e, int pt)
// �������������һ�����ݳɷ�Ϊitem��˫�׳ɷ�Ϊpt�Ľ��
{
	data = e;
	parent = pt;
}

#endif
