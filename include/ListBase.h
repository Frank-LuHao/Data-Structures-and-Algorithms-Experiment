#if !defined(_LIST_BASE_H_)
#define  _LIST_BASE_H_
#include <memory.h>
template <class ElemType>  class ListBase
{
public:
	virtual int Length() const = 0;//��ȡ���Ա���Ԫ�ظ���
	virtual bool IsEmpty() const = 0;//�ж����Ա��Ƿ�Ϊ��
	virtual void Clear() = 0;//������Ա�
	virtual bool GetElem(int position, ElemType& e) const =0;//��ȡָ��λ��Ԫ��
	virtual bool SetElem(int position, const ElemType e) = 0;//����ָ��λ��Ԫ��
	virtual bool Delete(int position,ElemType& e) = 0;//ɾ��ָ��λ��Ԫ��
	virtual bool Insert(int position, const ElemType e) = 0;//��ָ��λ�ò���Ԫ��
	virtual void Traverse(bool (*visit)(const ElemType&))const = 0;//�������Ա�
	virtual int AddTail(const ElemType e) = 0;	//������ݵ����Ա�ĩβ

};
#endif