#if !defined(_ARRAY_H_)
#define _ARRAY_H_
#include <memory.h>
#include <stdarg.h>
template<class ElemType>
class Array
{
public:
	Array(int dim, ...);
	~Array();
	Array(const Array<ElemType>& source);	// 拷贝构造函数
	ElemType& operator()(int sub0, ...);	// 重载函数运算符
	ElemType* ElemAddress(int sub0, ...)const;	//求元素的地址
	Array<ElemType>& operator =(const Array<ElemType>& source); // 重载赋值运算符
protected:
	int Locate(int sub0, va_list& va) const;// 求元素在顺序存储中的位置
	ElemType* m_pElemBase;							// 数组元素基址
	int m_nDim;								// 数组维数
	int* m_pnBounds;							// 数组各维长度
	int* m_pnConstants;							// 数组映象函数常量
};
template<class ElemType>
Array<ElemType>::Array(int dim, ...)
{
	if (dim <= 0)
		throw std::runtime_error("Array dim error!");//检查维度参数是否正确，小于1维抛出错误
	m_nDim = dim;		// 数组维数为dim
	m_pnBounds = new int[dim];	// 分配数组各维长度存储空间
	va_list va;		// 变长参数变量
	int elemTotalNum = 1;	// 元素总数
	int tempPos;		// 临时变量
	va_start(va, dim);	// 初始化变长参数变量va，用于存储变长参数信息，dim为省略号左侧最右边的参数标识符
	for (tempPos = 0; tempPos < dim; tempPos++)
	{	// 为各维长度赋值并计算数组总元素个数
		m_pnBounds[tempPos] = va_arg(va, int);	// 取出变长参数作为各维长度
		elemTotalNum = elemTotalNum * m_pnBounds[tempPos];	// 统计数组总元素个数
	}
	va_end(va);					// 结束变长参数的引用
	m_pElemBase = new ElemType[elemTotalNum];	// 分配数组元素空间
	m_pnConstants = new int[dim];			// 分配数组映象函数常量
	m_pnConstants[dim - 1] = 1;
	for (tempPos = dim - 2; tempPos >= 0; --tempPos)
	{
		m_pnConstants[tempPos] = m_pnBounds[tempPos + 1] * m_pnConstants[tempPos + 1];// 计算数组映象函数常量
	}
}

template<class ElemType>
Array<ElemType>::~Array()
{
	if (m_pElemBase != NULL) delete[]m_pElemBase;		// 释放数组元素空间
	if (m_pnBounds != NULL) delete[]m_pnBounds;	// 释放各维长度空间
	if (m_pnConstants != NULL) delete[]m_pnConstants;		// 释放映象函数常量空间
}
template<class ElemType>
Array<ElemType>::Array(const Array<ElemType>& source)
{
	*this = source;//因为重载了'=',可以直接赋值
}
template <class ElemType>
ElemType& Array<ElemType>::operator()(int sub0, ...)
// 操作结果：重载函数运算符
{
	va_list va;								// 变长参数变量
	va_start(va, sub0);						// 初始化变长参数变量va，用于存储变长参数信息，sub0为第1维的下标，sub0为省略号左侧最右边的参数标识符
	int position = Locate(sub0, va);		// 元素在顺序存储中的位置
	va_end(va);								// 结束变长参数变量的引用
	return *(m_pElemBase + position);				// 返回数组元素
}
template <class ElemType>
ElemType* Array<ElemType>::ElemAddress(int sub0, ...)const
//求元素的地址
{
	va_list va;								// 变长参数变量
	va_start(va, sub0);						// 初始化变长参数变量va，用于存储变长参数信息，sub0为第1维的下标，sub0为省略号左侧最右边的参数标识符
	int position = Locate(sub0, va);		// 元素在顺序存储中的位置
	if (position < 0)
		return NULL;
	return m_pElemBase + position;
}
template <class ElemType>
// 操作结果：求元素在顺序存储中的位置，sub0为第1维下标，va为第2至第dim维下标的可变参数
int Array<ElemType>::Locate(int sub0, va_list& va) const
{
	if (sub0 < 0 || sub0 >= m_pnBounds[0])
	{
		throw std::runtime_error("out of bound!");//参数检查错误，抛出错误
		return -1;
	}
	int position = m_pnConstants[0] * sub0;		// 初始化元素在顺序存储中的位置	
	for (int tempPos = 1; tempPos < m_nDim; tempPos++)
	{
		int sub = va_arg(va, int);			// 取出第tempPos+1维数组元素下标
		if (sub < 0 || sub >= m_pnBounds[tempPos])
		{
			throw std::runtime_error("out of bound!");	//参数检查错误，抛出错误
			return -1;
		}
		position += m_pnConstants[tempPos] * sub;	// 累加乘积求元素在顺序存储中的位置
	}
	return position;					// 返回元素在顺序存储中的位置
}

template <class ElemType>
Array<ElemType>& Array<ElemType>::operator =(const Array<ElemType>& source)
// 操作结果：将数组source赋值给当前数组——重载赋值运算符
{
	if (&source != this)
	{
		if (m_pElemBase != NULL) delete[]m_pElemBase;	// 释放数组元素空间
		if (m_pnBounds != NULL) delete[]m_pnBounds;// 释放各维长度空间
		if (m_pnConstants != NULL) delete[]m_pnConstants;		// 释放映象函数常量空间

		m_nDim = source.m_nDim;					// 数组维数

		int elemTotalNum = 1;	
		// 元素总数
		int tempPos;
		for (tempPos = 0; tempPos < m_nDim; tempPos++)
		{	// 统计数组总元素个数
			elemTotalNum = elemTotalNum * source.m_pnBounds[tempPos];		// 计算数组总元素个数
		}
		m_pElemBase = new ElemType[elemTotalNum];	// 为数组元素分配存储空间
		memcpy(m_pElemBase, source.m_pElemBase, sizeof(ElemType) * elemTotalNum);// 复制数组元素
		m_pnBounds = new int[m_nDim];				// 为数组各维长度分配存储空间
		m_pnConstants = new int[m_nDim];			// 为数组映象函数常量分配存储空间
		memcpy(m_pnBounds, source.m_pnBounds, sizeof(int) * m_nDim);//复制各维长度	
		memcpy(m_pnConstants, source.m_pnConstants, sizeof(int) * m_nDim);//复制函数常量
	}
	return *this;
}

#endif