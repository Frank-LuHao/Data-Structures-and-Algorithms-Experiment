#if !defined(_SQ_STACK_H_)
#define  _SQ_STACK_H_
#include "SqList.h"

#define DEFAULT_SIZE 32
template <class ElemType>
class SqStack:private SqList <ElemType>
{
public:
	SqStack(int nBufferSize = DEFAULT_SIZE);//���캯��
	SqStack(const SqStack<ElemType>& source);//�������캯��
	~SqStack();//��������
//���صĽӿ�
	int Length() const;//��ȡ˳��ջ��Ԫ�ظ���
	bool IsEmpty() const;//�ж�˳��ջ�Ƿ�Ϊ��
	void Clear();//���˳��ջ
	void Traverse(bool (*visit)(const ElemType& e))const;//����˳��ջ
//���Ի��ӿ�
	bool Push(const ElemType& e);	// ��ջ
	bool Top(ElemType& e) const;		// ����ջ��
	bool Pop(ElemType& e);	// ��ջ
	bool Pop();			// ��ջ
};

//˳���ʵ�ִ���
template <class ElemType> 
SqStack<ElemType>::SqStack(int nBufferSize)
	:SqList<ElemType>(nBufferSize)
{
	
}
template <class ElemType>
SqStack<ElemType>::SqStack(const SqStack<ElemType>& source)
{
	*this = source;//������������'='�����ø�ֵ���㣬�������
}
template <class ElemType> 
SqStack<ElemType>::~SqStack()
{
}

template <class ElemType> 
int SqStack<ElemType>::Length() const
{
	return SqList<ElemType>::Length();//���ø���ķ���
}
template <class ElemType> 
void SqStack<ElemType>::Clear()
{
	return SqList<ElemType>::Clear();//���ø���ķ���
}
template <class ElemType>
bool SqStack<ElemType>::IsEmpty()const
{
	return SqList<ElemType>::IsEmpty();//���ø���ķ���
}

template <class ElemType>
void SqStack<ElemType>::Traverse(bool (*visit)(const ElemType& e)) const
{
	return SqList<ElemType>::Traverse(visit);//���ø���ķ���
}
template <class ElemType>
bool SqStack<ElemType>::Push(const ElemType& e)
{
	this->AddTail(e);////���ø�����ӵ���ĩβ�ķ���
	return true;
}
template <class ElemType>
bool SqStack<ElemType>::Top(ElemType& e) const
{
	int nCounts = Length();
	if (nCounts <= 0)
		return false;
	return this->GetElem(nCounts - 1, e);//���ø���ķ�������ȡջ��Ԫ��ֵ
}
template <class ElemType>
bool SqStack<ElemType>::Pop(ElemType& e)
{
	int nCounts = Length();
	if (nCounts <= 0)
		return false;
	return this->Delete(nCounts - 1, e);//���ø���ķ�������ȡ��ɾ��ջ��Ԫ��ֵ
}
template <class ElemType>
bool SqStack<ElemType>::Pop()
{
	int nCounts = Length();
	if (nCounts <= 0)
		return false;
	ElemType e;
	return this->Delete(nCounts - 1, e);//���ø���ķ�����ɾ��ջ��Ԫ��ֵ
}
#endif