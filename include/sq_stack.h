#if !defined(_SQ_STACK_H_)
#define  _SQ_STACK_H_
#include "SqList.h"

#define DEFAULT_SIZE 32
template <class ElemType>
class SqStack:private SqList <ElemType>
{
public:
	SqStack(int nBufferSize = DEFAULT_SIZE);//构造函数
	SqStack(const SqStack<ElemType>& source);//拷贝构造函数
	~SqStack();//析构函数
//重载的接口
	int Length() const;//获取顺序栈中元素个数
	bool IsEmpty() const;//判断顺序栈是否为空
	void Clear();//清空顺序栈
	void Traverse(bool (*visit)(const ElemType& e))const;//遍历顺序栈
//个性化接口
	bool Push(const ElemType& e);	// 入栈
	bool Top(ElemType& e) const;		// 返回栈顶
	bool Pop(ElemType& e);	// 出栈
	bool Pop();			// 出栈
};

//顺序表实现代码
template <class ElemType> 
SqStack<ElemType>::SqStack(int nBufferSize)
	:SqList<ElemType>(nBufferSize)
{
	
}
template <class ElemType>
SqStack<ElemType>::SqStack(const SqStack<ElemType>& source)
{
	*this = source;//基类中重载了'='，调用赋值运算，构造对象
}
template <class ElemType> 
SqStack<ElemType>::~SqStack()
{
}

template <class ElemType> 
int SqStack<ElemType>::Length() const
{
	return SqList<ElemType>::Length();//调用父类的方法
}
template <class ElemType> 
void SqStack<ElemType>::Clear()
{
	return SqList<ElemType>::Clear();//调用父类的方法
}
template <class ElemType>
bool SqStack<ElemType>::IsEmpty()const
{
	return SqList<ElemType>::IsEmpty();//调用父类的方法
}

template <class ElemType>
void SqStack<ElemType>::Traverse(bool (*visit)(const ElemType& e)) const
{
	return SqList<ElemType>::Traverse(visit);//调用父类的方法
}
template <class ElemType>
bool SqStack<ElemType>::Push(const ElemType& e)
{
	this->AddTail(e);////调用父类添加到表末尾的方法
	return true;
}
template <class ElemType>
bool SqStack<ElemType>::Top(ElemType& e) const
{
	int nCounts = Length();
	if (nCounts <= 0)
		return false;
	return this->GetElem(nCounts - 1, e);//调用父类的方法，获取栈顶元素值
}
template <class ElemType>
bool SqStack<ElemType>::Pop(ElemType& e)
{
	int nCounts = Length();
	if (nCounts <= 0)
		return false;
	return this->Delete(nCounts - 1, e);//调用父类的方法，获取并删除栈顶元素值
}
template <class ElemType>
bool SqStack<ElemType>::Pop()
{
	int nCounts = Length();
	if (nCounts <= 0)
		return false;
	ElemType e;
	return this->Delete(nCounts - 1, e);//调用父类的方法，删除栈顶元素值
}
#endif