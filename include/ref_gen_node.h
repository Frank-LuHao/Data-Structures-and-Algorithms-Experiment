#ifndef __GEN_NODE_H__
#define __GEN_NODE_H__

#ifndef __REF_GEN_LIST_NODE_TYPE__
#define __REF_GEN_LIST_NODE_TYPE__
typedef enum {_HEAD, _ATOM,_LIST}RefGenListNodeType;
#endif

// �����������������ģ��
template<class ElemType>
class RefGenListNode
{
public:
// ���ݳ�Ա:

	RefGenListNodeType m_tag;				
		// ��־�ɷ�,HEAD(0):ͷ���, ATOM(1):ԭ�ӽ��, LIST(2):����
	RefGenListNode<ElemType> *m_pNext;	// ָ��ͬһ���е���һ�����ָ��ɷ�
	union
	{
		int m_nRef;						// tag=HEAD,ͷ���,���������
		ElemType m_TData;					// tag=ATOM,���ԭ�ӽ������ݳɷ�
		RefGenListNode<ElemType> *m_pSubLink;	// tag=LISK,���ָ���ӱ��ָ��ɷ�
	}; 

// ���캯��ģ��:
	RefGenListNode(RefGenListNodeType tg = _HEAD, RefGenListNode<ElemType> *next = NULL);
	// �ɱ�־tg��ָ��next�������������������
	void ClearLink();
};

// �����������������ģ���ʵ�ֲ���
template<class ElemType>
RefGenListNode<ElemType>::RefGenListNode(RefGenListNodeType tg, RefGenListNode<ElemType> *next)
// ����������ɱ�־tg��ָ��next�������������������
{
	m_tag = tg;							// ��־
	m_pNext = next;					// ���
}

template<class ElemType>
void RefGenListNode<ElemType>::ClearLink()
{
	m_pNext = NULL;
	m_nRef = 1;//ͷ�ڵ�������Ϊ1
}
#endif
