#if !defined(HUF_TREE_NODE_PTR_H)
#define HUF_TREE_NODE_PTR_H
#include "huf_tree_node.h"
// �����������ָ����ģ��
template <class ElemType, class wtType>
struct HufTreeNodePtr
{
    HufTreeNode<ElemType, wtType>* ptr;
    //���رȽ������
    bool operator < (const HufTreeNodePtr<ElemType, wtType>& right) const
    {
        return ptr->m_Weight < right.ptr->m_Weight;
    }
    bool operator > (const HufTreeNodePtr<ElemType, wtType>& right) const
    {
        return ptr->m_Weight > right.ptr->m_Weight;
    }
    bool operator <= (const HufTreeNodePtr<ElemType, wtType>& right) const
	{
		return ptr->m_Weight <= right.ptr->m_Weight;
	}
    bool operator >= (const HufTreeNodePtr<ElemType, wtType>& right) const
    {
        return ptr->m_Weight >= right.ptr->m_Weight;
    }
    bool operator == (const HufTreeNodePtr<ElemType, wtType>& right) const
    {
        return ptr->m_Weight == right.ptr->m_Weight;
    }
};
#endif