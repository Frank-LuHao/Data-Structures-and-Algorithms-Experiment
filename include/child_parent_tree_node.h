#ifndef __CHILD_PARENT_TREE_NODE_H__
#define __CHILD_PARENT_TREE_NODE_H__

#include "SimpleLkList.h"				// ��������

// ����˫����ʾ�������ģ��
template <class ElemType>
struct ChildParentTreeNode 
{
// ���ݳ�Ա:
	ElemType data;					// ���ݳɷ�
	SimpleLkList<int> childLkList;		// ��������	
	int parent;						// ˫��λ�óɷ�

// ���캯��ģ��:
	ChildParentTreeNode();			// �޲����Ĺ��캯��ģ��
	ChildParentTreeNode(const ElemType &e, int pt = -1);// ��֪������ֵ��˫��λ�ý����ṹ
};

// ����˫����ʾ�������ģ���ʵ�ֲ���
template<class ElemType>
ChildParentTreeNode<ElemType>::ChildParentTreeNode()
// �������������˫�׳ɷ�Ϊ-1�Ľ��
{
	parent = -1;
}

template<class ElemType>
ChildParentTreeNode<ElemType>::ChildParentTreeNode(const ElemType &e, int pt)
// �������������һ�����ݳɷ�Ϊitem��˫����Ϊpt�Ľ��
{
	data = e;						// ����Ԫ��ֵ
	parent = pt;					// ˫��
}

#endif
