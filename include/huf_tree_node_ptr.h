#if !defined(HUF_TREE_NODE_PTR_H)
#define HUF_TREE_NODE_PTR_H
#include "huf_tree_node.h"
// 哈夫曼树结点指针类模板
template <class ElemType, class wtType>
struct HufTreeNodePtr
{
    HufTreeNode<ElemType, wtType>* ptr;
    //重载比较运算符
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