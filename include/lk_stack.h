#if !defined(_LK_STACK_H_)
#define  _LK_STACK_H_
#include "SimpleLkList.h"

template <class ElemType>
class LinkStack :private SimpleLkList <ElemType>
{
public:
	LinkStack();//���캯��
	LinkStack(const LinkStack<ElemType>& source);//�������캯��
	~LinkStack();//��������
	//���صĽӿ�
	int Length() const;//��ȡ��ʽջ��Ԫ�ظ���
	bool IsEmpty() const;//�ж���ʽջ�Ƿ�Ϊ��
	void Clear();//�����ʽջ
	void Traverse(bool (*visit)(const ElemType& e))const;//������ʽջ
	//���Ի��ӿ�
	bool Push(const ElemType& e);	// ��ջ
	bool Top(ElemType& e) const;		// ����ջ��
	bool Pop(ElemType& e);	// ��ջ
	bool Pop();			// ��ջ
};

//˳���ʵ�ִ���
template <class ElemType>
LinkStack<ElemType>::LinkStack()
{

}
template <class ElemType>
LinkStack<ElemType>::LinkStack(const LinkStack<ElemType>& source)
{
	*this = source;//������������'='�����ø�ֵ���㣬�������
}
template <class ElemType>
LinkStack<ElemType>::~LinkStack()
{
}

template <class ElemType>
int LinkStack<ElemType>::Length() const
{
	return SimpleLkList<ElemType>::Length();//���ø���ķ���
}
template <class ElemType>
void LinkStack<ElemType>::Clear()
{
	return SimpleLkList<ElemType>::Clear();//���ø���ķ���
}
template <class ElemType>
bool LinkStack<ElemType>::IsEmpty()const
{
	return SimpleLkList<ElemType>::IsEmpty();//���ø���ķ���
}

template <class ElemType>
void LinkStack<ElemType>::Traverse(bool (*visit)(const ElemType& e)) const
{
	return SimpleLkList<ElemType>::Traverse(visit);//���ø���ķ���
}
template <class ElemType>
bool LinkStack<ElemType>::Push(const ElemType& e)
{
	this->Insert(0,e);////���ø���ķ���������ͷ�巨��������
	return true;
}
template <class ElemType>
bool LinkStack<ElemType>::Top(ElemType& e) const
{
	if (this->IsEmpty())
		return false;//����Ϊ��ʱ������false
	return this->GetElem(0, e);//���ø���ķ�������ȡջ��Ԫ��ֵ
}
template <class ElemType>
bool LinkStack<ElemType>::Pop(ElemType& e)
{
	if (this->IsEmpty())
		return false;//����Ϊ��ʱ������false
	return this->Delete(0, e);//���ø���ķ�������ȡ��ɾ��ջ��Ԫ��ֵ
}
template <class ElemType>
bool LinkStack<ElemType>::Pop()
{
	if (this->IsEmpty())
		return false;//����Ϊ��ʱ������false
	ElemType e;
	return this->Delete(0, e);//���ø���ķ�����ɾ��ջ��Ԫ��ֵ
}
#endif