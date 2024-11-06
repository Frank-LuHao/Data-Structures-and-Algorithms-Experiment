#if !defined(_HUF_TREE_H_)
#define _HUF_TREE_H_
#include "lk_binary_tree_base.h"
#include "huf_tree_node.h"
#include "huf_tree_node_ptr.h"
#include "min_priority_heap_queue.h"
#include "char_string.h"
#include "SqList.h"

// ����������ģ��
template  <class ElemType,class wtType>
class HufTree:public LkBinaryTreeBase<ElemType,HufTreeNode<ElemType,wtType>>
{
public:
    HufTree();						//���캯��
    HufTree(const HufTree < ElemType, wtType>& source);	//���ƹ��캯��
    ~HufTree();//��������
    bool CreateHufTree(ElemType e[],wtType wt[],int nSize);	//��Ȩ�����й����������
    bool Ecode(const ElemType e, CharString& strCode);	//����
    bool Dcode(SqList <ElemType>& eLst, const CharString& strCode);//����
    void Clear();						//���������������������ڴ�
    virtual HufTreeNode<ElemType, wtType>* Parent(HufTreeNode<ElemType, wtType>* pCur) const ;// ����˫��
    HufTree<ElemType, wtType>& operator=(const HufTree < ElemType, wtType>& source);	// ���ظ�ֵ�����
private:
    virtual void LinkParentChild(HufTreeNode<ElemType, wtType>* pParent, HufTreeNode<ElemType, wtType>* pChild, bool bLeft);	//����˫�׽ڵ�ͺ��ӽڵ�
    HufTreeNode<ElemType, wtType>* m_pCurNode;		//��ǰ����·���ڵ�
    CharString* m_pStrCodeTabel;				//���루�ַ�������
    ElemType* m_pElemTabel;	            //����Ԫ�ر�����ʱ���Ҹñ����������ͨ�������ڱ�����л�ñ����ַ���
    int m_nElemCnts;					//�����Ԫ�ظ���
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
    Clear();				//��������������ڴ�
    int i;
    MinPriorityHeapQueue <HufTreeNodePtr<ElemType, wtType>> minHeap(nSize+8);	//��С�����ȶ�
    HufTreeNode<ElemType, wtType>** pLeafNodeTabel = new HufTreeNode<ElemType, wtType>* [nSize];//Ҷ�ӽڵ��
    for (i = 0; i < nSize; i++)
    {
        HufTreeNodePtr<ElemType, wtType> temp;
        temp.ptr = new HufTreeNode<ElemType, wtType>;
        temp.ptr->data = e[i];
        temp.ptr->m_Weight = wt[i];
        pLeafNodeTabel[i] = temp.ptr;		//��Ҷ�ӽڵ�ָ�����Ҷ�ӽڵ��
        minHeap.InQueue(temp);		//Ҷ�ӽڵ����
    }
    for (i = 0; i < nSize - 1; i++)
    {//������������
        HufTreeNodePtr<ElemType, wtType> r, r1, r2;
        minHeap.OutQueue(r1);		//����r1
        minHeap.OutQueue(r2);		//����r2
        r.ptr = new HufTreeNode<ElemType, wtType>(r1.ptr,r2.ptr);		//���ɸ��ڵ�r
        r.ptr->m_Weight = r1.ptr->m_Weight + r2.ptr->m_Weight;		//�ϲ�����������Ȩ�ص��ڵ�r
        minHeap.InQueue(r);						//r���
    }
    HufTreeNodePtr<ElemType, wtType> root;
    minHeap.OutQueue(root);		//�����ӵ��Ǹ��ڵ㣬��ΪȨ�����
    this->m_pRoot = root.ptr;
    m_pCurNode = this->m_pRoot;	//����ʱ�Ӹ��ڵ㿪ʼ
    m_pStrCodeTabel = new CharString[nSize];//���������ÿ����������Ӧ��һ�������ַ���
    m_pElemTabel = new ElemType[nSize];	//�����ַ�������
    m_nElemCnts = nSize;
    for (i = 0; i < nSize; i++)
    {//����Ҷ�ӽڵ㣬����ÿ�������ַ��ı���
        HufTreeNode<ElemType, wtType>* pLeafNode = pLeafNodeTabel[i];//��ȡҶ�ӽڵ�
        CharString strCode;
        HufTreeNode<ElemType, wtType>* pNode = pLeafNode;  
        do{//�Ե����ϻ��ݵ����ڵ�
            HufTreeNode<ElemType, wtType>* pParent = pNode->parent;
            if (pParent->leftChild == pNode)
                strCode.Insert(0, '0');	//�����ӣ�����Ϊ0
            else
                strCode.Insert(0, '1');	//�Һ��ӣ�����Ϊ1
            pNode = pParent;			//����
        } while (pNode !=this->m_pRoot);		//���ݵ����ڵ����
        m_pStrCodeTabel[i] = strCode;			//��ô���Ԫ�ر��룬�ԡ�0����ʾ0����1����ʾ1
        m_pElemTabel[i] = e[i];			//����Ԫ�ر�
    }
    delete []pLeafNodeTabel;				//����Ҷ�ӽڵ��
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
    const char* pCodeStr = strCode.ToCStr();		//ת��ΪC�����ַ���
    int nLen = strCode.Length();
    int i;
    for (i = 0; i < nLen; i++)
    {
        if (pCodeStr[i] == '0')
            m_pCurNode = m_pCurNode->leftChild;	//���֧
        else
            m_pCurNode = m_pCurNode->rightChild;	//�ҷ�֧
        if (m_pCurNode->IsLeaf())
        {//��Ҷ�ӽڵ�
            eLst.AddTail(m_pCurNode->data);		//�õ�����
            m_pCurNode = this->m_pRoot;		//��ǰ����ڵ�ع鵽���ڵ�
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