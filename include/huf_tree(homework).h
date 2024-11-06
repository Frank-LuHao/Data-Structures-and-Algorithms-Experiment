#if !defined(_HUF_TREE_H_)
#define _HUF_TREE_H_
#include "lk_binary_tree_base.h"
#include "huf_tree_node.h"
#include "huf_tree_node_ptr.h"
#include "min_priority_heap_queue.h"
#include "char_string.h"
#include "SqList.h"

// 哈夫曼树类模板
template  <class ElemType,class wtType>
class HufTree:public LkBinaryTreeBase<ElemType,HufTreeNode<ElemType,wtType>>
{
public:
    HufTree();						//构造函数
    HufTree(const HufTree < ElemType, wtType>& source);	//复制构造函数
    ~HufTree();//析构函数
    bool CreateHufTree(ElemType e[],wtType wt[],int nSize);	//由权重序列构造霍夫曼树
    bool Ecode(const ElemType e, CharString& strCode);	//编码
    bool Dcode(SqList <ElemType>& eLst, const CharString& strCode);//译码
    void Clear();						//销毁树，并清除对象分配内存
    virtual HufTreeNode<ElemType, wtType>* Parent(HufTreeNode<ElemType, wtType>* pCur) const ;// 返回双亲
    HufTree<ElemType, wtType>& operator=(const HufTree < ElemType, wtType>& source);	// 重载赋值运算符
private:
    virtual void LinkParentChild(HufTreeNode<ElemType, wtType>* pParent, HufTreeNode<ElemType, wtType>* pChild, bool bLeft);	//链接双亲节点和孩子节点
    HufTreeNode<ElemType, wtType>* m_pCurNode;		//当前解码路径节点
    CharString* m_pStrCodeTabel;				//编码（字符串）表
    ElemType* m_pElemTabel;	            //编码元素表，编码时查找该表，获得索引，通过所有在编码表中获得编码字符串
    int m_nElemCnts;					//编码的元素个数
};

template  <class ElemType,class wtType>
HufTree<ElemType, wtType>::HufTree()
{
    m_pCurNode = NULL;
    m_pStrCodeTabel = NULL;
    m_pElemTabel = NULL;
    m_nElemCnts = 0;
}

template  <class ElemType,class wtType>
HufTree<ElemType, wtType>::~HufTree()
{
    Clear();
}

template  <class ElemType,class wtType>
HufTree<ElemType, wtType>::HufTree(const HufTree < ElemType, wtType>& source)
{
    this->m_pRoot = CopyTreeAux(source.GetRoot());    
}

template  <class ElemType,class wtType>
bool HufTree<ElemType, wtType>::CreateHufTree(ElemType e[], wtType wt[], int nSize)
{
    if (!e || !wt || nSize <= 0)
        return false;
    Clear();				//清空树及编码表等内存
    int i;
    MinPriorityHeapQueue <HufTreeNodePtr<ElemType, wtType>> minHeap(nSize+8);	//最小化优先堆
    HufTreeNode<ElemType, wtType>** pLeafNodeTabel = new HufTreeNode<ElemType, wtType>* [nSize];//叶子节点表
    for (i = 0; i < nSize; i++)
    {
        HufTreeNodePtr<ElemType, wtType> temp;
        temp.ptr = new HufTreeNode<ElemType, wtType>;
        temp.ptr->data = e[i];
        temp.ptr->m_Weight = wt[i];
        pLeafNodeTabel[i] = temp.ptr;		//把叶子节点指针加入叶子节点表
        minHeap.InQueue(temp);		//叶子节点入队
    }
    for (i = 0; i < nSize - 1; i++)
    {//建立霍夫曼树
        HufTreeNodePtr<ElemType, wtType> r, r1, r2;
        minHeap.OutQueue(r1);		//出队r1
        minHeap.OutQueue(r2);		//出队r2
        r.ptr = new HufTreeNode<ElemType, wtType>(r1.ptr,r2.ptr);		//生成根节点r
        r.ptr->m_Weight = r1.ptr->m_Weight + r2.ptr->m_Weight;		//合并左、右子树的权重到节点r
        minHeap.InQueue(r);						//r入队
    }
    HufTreeNodePtr<ElemType, wtType> root;
    minHeap.OutQueue(root);		//最后出队的是根节点，因为权重最大
    this->m_pRoot = root.ptr;
    m_pCurNode = this->m_pRoot;	//译码时从根节点开始
    m_pStrCodeTabel = new CharString[nSize];//创建编码表，每个待编对象对应着一个编码字符串
    m_pElemTabel = new ElemType[nSize];	//创建字符索引表
    m_nElemCnts = nSize;
    for (i = 0; i < nSize; i++)
    {//遍历叶子节点，计算每个待编字符的编码
        HufTreeNode<ElemType, wtType>* pLeafNode = pLeafNodeTabel[i];//获取叶子节点
        CharString strCode;
        HufTreeNode<ElemType, wtType>* pNode = pLeafNode;  
        do{//自底向上回溯到根节点
            HufTreeNode<ElemType, wtType>* pParent = pNode->parent;
            if (pParent->leftChild == pNode)
                strCode.Insert(0, '0');	//是左孩子，编码为0
            else
                strCode.Insert(0, '1');	//右孩子，编码为1
            pNode = pParent;			//回溯
        } while (pNode !=this->m_pRoot);		//回溯到根节点结束
        m_pStrCodeTabel[i] = strCode;			//获得待编元素编码，以‘0’表示0，‘1’表示1
        m_pElemTabel[i] = e[i];			//存入元素表
    }
    delete []pLeafNodeTabel;				//销毁叶子节点表
}

template  <class ElemType,class wtType>
bool HufTree<ElemType, wtType>::Ecode(const ElemType e, CharString& strCode)
{
    int i;
    for (i = 0; i < m_nElemCnts; i++)
    {
        if (m_pElemTabel[i] == e)
        {
            strCode = m_pStrCodeTabel[i];
            return true;
        }
    }
    return false;
}

template  <class ElemType,class wtType>
bool HufTree<ElemType, wtType>::Dcode(SqList <ElemType>& eLst, const CharString& strCode)
{
    if (!this->m_pRoot || !m_pCurNode)
       return false;
    const char* pCodeStr = strCode.ToCStr();		//转换为C风格的字符串
    int nLen = strCode.Length();
    int i;
    for (i = 0; i < nLen; i++)
    {
        if (pCodeStr[i] == '0')
            m_pCurNode = m_pCurNode->leftChild;	//左分支
        else
            m_pCurNode = m_pCurNode->rightChild;	//右分支
        if (m_pCurNode->IsLeaf())
        {//是叶子节点
            eLst.AddTail(m_pCurNode->data);		//得到译码
            m_pCurNode = this->m_pRoot;		//当前译码节点回归到根节点
        }
    }
    return true;
}

template  <class ElemType,class wtType>
void HufTree<ElemType, wtType>::Clear()
{
    if (m_pStrCodeTabel)
        delete []m_pStrCodeTabel;
    if (m_pElemTabel)
        delete []m_pElemTabel;
    m_pStrCodeTabel = NULL;
    m_pElemTabel = NULL;
    m_nElemCnts = 0;
    LkBinaryTreeBase<ElemType, HufTreeNode<ElemType, wtType>>::Clear();
}

template <class ElemType,class wtType>
HufTreeNode<ElemType, wtType>* HufTree<ElemType, wtType>::Parent(HufTreeNode<ElemType, wtType>* pCur) const
{
    return pCur->parent;
}

template  <class ElemType,class wtType>
void HufTree<ElemType, wtType>::LinkParentChild(HufTreeNode<ElemType, wtType>* pParent, HufTreeNode<ElemType, wtType>* pChild, bool bLeft)
{
    if (bLeft)
        pParent->leftChild = pChild;
    else
        pParent->rightChild = pChild;
    if (pChild)
        pChild->parent = pParent;
}

template  <class ElemType,class wtType>
HufTree<ElemType, wtType>& HufTree<ElemType, wtType>::operator=(const HufTree < ElemType, wtType>& source)
{
    if (this == &source)
        return *this;
    Clear();
    this->m_pRoot = CopyTreeAux(source.GetRoot());
    return *this;
}

#endif