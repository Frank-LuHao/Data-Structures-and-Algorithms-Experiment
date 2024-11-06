#if !defined(__NODE_H__)
#define __NODE_H__

// ����������ģ��
template <class ElemType>
class Node 
{

public:
// ���캯��ģ��:
	Node();						// �޲����Ĺ��캯��ģ��
	Node(const ElemType &e, Node<ElemType> *link = nullptr);	// ��֪����Ԫ��ֵ��ָ�뽨�����
	~Node();//��������
	// ���ݳ�Ա:
	ElemType m_TData;				// ���ݳɷ�
	Node<ElemType>* m_pNext;		// ָ��ɷ�

	virtual void ClearLink(); //�������״̬
};

// �����ģ���ʵ�ֲ���
template<class ElemType>
//�����������������
Node<ElemType>::~Node()
{

}

template<class ElemType>
Node<ElemType>::Node()
// �������������ָ��ɷ�Ϊ�յĽ��
{
	m_pNext = nullptr;
}

template<class ElemType>
Node<ElemType>::Node(const ElemType &e, Node<ElemType> *link)
// �������������һ�����ݳɷ�Ϊe��ָ��ɷ�Ϊlink�Ľ��
{
	m_TData = e;
	m_pNext = link;
}
template<class ElemType>
//����������������״̬
void Node<ElemType>::ClearLink()
{
	m_pNext = nullptr;
}
// ˫��������ģ��
template <class ElemType>
class DbNode 
{

public:
	// ���캯��ģ��:
	DbNode();						// �޲����Ĺ��캯��ģ��
	DbNode(const ElemType& e, DbNode<ElemType>* prelink = nullptr, DbNode<ElemType>* nextlink = nullptr);	// ��֪����Ԫ��ֵ��ָ�뽨�����
	~DbNode();//��������
	//m_TData,m_pNext�����ݼ̳���Node
	// ���ݳ�Ա:
	DbNode<ElemType>* m_pPre;		// ָ����ǰ�Ľڵ��ָ��ɷ�
	DbNode<ElemType>* m_pNext;		//ָ���ڵ��ָ��ɷ�
	ElemType m_TData;				// ���ݳɷ�
	virtual void ClearLink(); //�������״̬

};
template<class ElemType>
DbNode<ElemType>::DbNode()
// �������������ָ��ɷ�Ϊ�յĽ��
{
	m_pPre = nullptr;
	m_pNext = nullptr;
}
template<class ElemType>
DbNode<ElemType>::DbNode(const ElemType& e, DbNode<ElemType>* prelink , DbNode<ElemType>* nextlink )
// �������������ָ��ɷ�Ϊ�յĽ��
{
	this->m_TData = e;
	m_pPre = prelink;
	m_pNext = nextlink;

}

template<class ElemType>
DbNode<ElemType>::~DbNode()
// �����������������
{
}

template<class ElemType>
//����������������״̬
void DbNode<ElemType>::ClearLink()
{
	m_pNext = nullptr;
	m_pPre = nullptr;
}
#endif
