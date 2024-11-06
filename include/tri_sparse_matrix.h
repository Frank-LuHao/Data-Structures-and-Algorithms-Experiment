#if !defined(_TRI_SPARSE_MATRIX_H_)
#define _TRI_SPARSE_MATRIX_H_
#include <memory.h>
#include "triple.h"
#include "SqList.h"
#include "matrix.h"
// 稀疏矩阵三元组顺序表类模板
template<class ElemType>
class TriSparseMatrix : public SqList< Triple<ElemType>>
{
public:
	TriSparseMatrix(int nRows,int nCols,int nSize = 32);
	TriSparseMatrix(const TriSparseMatrix<ElemType>& source);	// 复制构造函数模板
	TriSparseMatrix(const Matrix<ElemType>& source);	// 由普通矩阵复制构造函数模板
	~TriSparseMatrix();
	int GetRows() const;								// 返回稀疏矩阵行数
	int GetCols() const;								// 返回稀疏矩阵列数
	int GetNum() const;									// 返回稀疏矩阵非零元个数
	bool SetElem(int row, int col, const ElemType& v);		// 设置指定位置的元素值
	bool GetElem(int row, int col, ElemType& v);			// 求指定位置的元素值
	Matrix<ElemType> ToMatrix() const;//返回转换的普通矩阵
	TriSparseMatrix<ElemType>& operator =(const TriSparseMatrix<ElemType>& source);// 重载赋值运算符
	TriSparseMatrix<ElemType>& operator =(const Matrix<ElemType>& source);// 重载由普通矩阵赋值的运算符
	TriSparseMatrix<ElemType> SimpleTranspose() const;//简单方法求稀疏矩阵的转置矩阵
	TriSparseMatrix<ElemType> FastTranspose() const;//快速转置稀疏矩阵，时间复杂度为O(n)
private:
	int Locate(int row, int col, bool& bFind);//定位指定行、列元素在稀疏矩阵中的存储位置
	int m_nRows;//矩阵列数
	int m_nCols;//矩阵行数

};
template<class ElemType>
TriSparseMatrix<ElemType>::TriSparseMatrix(int nRows, int nCols, int nSize)
	:SqList< Triple<ElemType>>(nSize)
{
	m_nRows = nRows;
	m_nCols = nCols;
}
template<class ElemType>
TriSparseMatrix<ElemType>::~TriSparseMatrix()
{
}
template<class ElemType>
int TriSparseMatrix<ElemType>::GetRows() const
// 返回稀疏矩阵行数
{
	return m_nRows;
}
template<class ElemType>
// 返回稀疏矩阵列数
int TriSparseMatrix<ElemType>::GetCols() const
{
	return m_nCols;
}
template<class ElemType>
int TriSparseMatrix<ElemType>::GetNum() const
// 返回稀疏矩阵非零元个数
{
	return this->Length();
}
template<class ElemType>
bool TriSparseMatrix<ElemType>::SetElem(int row, int col, const ElemType& v)
// 设置指定位置的元素值
{
	if (row < 0 || col < 0 || row >= m_nRows || col >= m_nCols)
		return false;
	bool bFind = false;
	int nLocation = Locate(row, col, bFind);//定位在稀疏矩阵中的存储位置
	if (nLocation < 0)
		return false;//定位失败，返回错误
	if (!bFind)
	{//在nLocation位置插入元素
		if (v != 0)//值为0的元素不需要插入
		{
			Triple<ElemType> eTriNode(row, col, v);
			this->Insert(nLocation, eTriNode);
		}
	}
	else
	{
		if (v == 0)
		{//新值为0，在顺序表中删除该元素
			this->Delete(nLocation);
		}
		else
			this->m_pElemData[nLocation].value = v;
	}
	return true;
}
template<class ElemType>
bool TriSparseMatrix<ElemType>::GetElem(int row, int col, ElemType& v)
// 求指定位置的元素值
{
	if (row < 0 || col < 0 || row >= m_nRows || col >= m_nCols)
		return false;
	bool bFind = false;
	int nLocation = Locate(row, col, bFind);//定位在稀疏矩阵中的存储位置
	if (nLocation < 0)
		return false;//定位失败，返回错误
	if (!bFind)
	{//没有找到，表明是0元素
		v = 0;
	}
	else
		v = this->m_pElemData[nLocation].value;
	return true;

}

template<class ElemType>
// 复制构造函数模板
TriSparseMatrix<ElemType>::TriSparseMatrix(const TriSparseMatrix<ElemType>& source)
{
	*this = source;//因为重载了'='，可以直接赋值
}
template<class ElemType>
//由普通矩阵复制构造函数
TriSparseMatrix<ElemType>::TriSparseMatrix(const Matrix<ElemType>& source)
{
	*this = source;//因为重载了'='，可以直接赋值
}
template<class ElemType>
//重载赋值运算符
TriSparseMatrix<ElemType>& TriSparseMatrix<ElemType>::operator =(const TriSparseMatrix<ElemType>& source)
{
	int nSrcLen = source.Length();//获取源数据长度
	this->Clear();//清空原来的数据
	this->SaveData(source.m_pElemData, nSrcLen);//调用父类接口存入数据
	m_nCols = source.m_nCols;
	m_nRows = source.m_nRows;
	return *this;
}
template<class ElemType>
//重载由普通矩阵赋值运算符
TriSparseMatrix<ElemType>& TriSparseMatrix<ElemType>::operator =(const Matrix<ElemType>& source)
{
	this->Clear();//首先清除内部缓冲
	m_nRows = source.GetRows();
	m_nCols = source.GetCols();
	ElemType* pSrcElem = source.ElemAddress(0, 0);
	int row, col;
	Triple<ElemType> triNode;
	for(row = 0;row<m_nRows;row++)
		for (col = 0; col < m_nCols; col++,pSrcElem++)
		{
			if (*pSrcElem != 0)
			{
				triNode.value = *pSrcElem;
				triNode.ePos.pos.row = row;
				triNode.ePos.pos.col = col;
				this->AddTail(triNode);
			}

		}
	return *this;
}
template<class ElemType>
//返回转换的普通矩阵
Matrix<ElemType> TriSparseMatrix<ElemType>::ToMatrix() const
{
	int rows = GetRows();
	int cols = GetCols();
	Matrix <ElemType> matrix(rows,cols);
	ElemType* pDstElem = matrix.ElemAddress(0, 0);
	memset(pDstElem, 0, sizeof(ElemType) * rows * cols);//清零空间
	int pos;
	int nElemCounts = GetNum();
	Triple<ElemType>* pNode = this->m_pElemData;
	for (pos = 0; pos < nElemCounts; pos++, pNode++)
	{
		ElemType* p = pDstElem + pNode->ePos.pos.row * rows + pNode->ePos.pos.col;
		*p = pNode->value;
	}
	return matrix;
}
template<class ElemType>
//定位指定行、列元素在稀疏矩阵中的存储位置
//采用折半查找法，把时间复杂度由O(n)降为O(log(n))
//如果定位失败返回-1,(没有查到不一定定位失败)
//折半查找法的前提是数据有序
int TriSparseMatrix<ElemType>::Locate(int row, int col, bool& bFind)
{
	bFind = false;
	if (row < 0 || col < 0 || row >= m_nRows || col >= m_nCols)
		return -1;
	int start = 0, end = GetNum() - 1;//定义搜索起始、终止位置

	ElemPos ePos;//定义定位位置变量
	ePos.pos.row = row;
	ePos.pos.col = col;
	//首先确保数据在 [start,end]区间
	if (ePos.key<this->m_pElemData[0].ePos.key )
		return 0;
	if (ePos.key>this->m_pElemData[end].ePos.key )
		return end+1;
	int mid;
	int nLocation = -1;//定位位置，初值为-1
	while (end - start > 1)
	{
		mid = (start + end) >> 1;//计算中间位置
		if (ePos.key < this->m_pElemData[mid].ePos.key)
			end = mid;//定位位置在中间位置前
		else if (ePos.key > this->m_pElemData[mid].ePos.key)
			start = mid; // 定位位置在中间位置后
		else
		{
			nLocation = mid;
			bFind = true;
			break;
		}
	}
	if (!bFind)
	{//循环完成，且没有找到时，还剩两个位置没有比对
		if (ePos.key == this->m_pElemData[start].ePos.key)
		{//和起始位置比对成功，找到
			bFind = true;
			nLocation = start;
		}
		else if (ePos.key == this->m_pElemData[end].ePos.key)
		{//和终止位置比对成功，找到
			bFind = true;
			nLocation = end;
		}
		else
			nLocation = start + 1; //没有找到数据，但如果要插入数据，就应该在该位置插入
	}
	return nLocation;
}
template<class ElemType>
//简单方法求稀疏矩阵的转置矩阵
TriSparseMatrix<ElemType> TriSparseMatrix<ElemType>::SimpleTranspose() const
{
	TriSparseMatrix<ElemType> matrix(GetCols(), GetRows(), GetNum() + 8);
	int nTriCounts = GetNum();
	int pos;
	Triple<ElemType> triNode;
	for (pos = 0; pos < nTriCounts; pos++)
	{
		triNode = this->m_pElemData[pos];
		matrix.SetElem(triNode.ePos.pos.col, triNode.ePos.pos.row, triNode.value);//互换行列
	}
	return matrix;
}
template<class ElemType>
//快速转置稀疏矩阵，时间复杂度为O(n)
TriSparseMatrix<ElemType> TriSparseMatrix<ElemType>::FastTranspose() const
{
	int cols = GetCols();
	int rows = GetRows();
	TriSparseMatrix<ElemType> matrix(cols, rows, GetNum() + 8);
	int nTriCounts = GetNum();

	matrix.m_nDataLen = nTriCounts;
	int* pnColNums = new int[cols];//存储每一列元素个数
	int* pnColPos = new int[cols]; //每一列元素元素首先存储位置
	memset(pnColNums, 0, sizeof(int) * cols);//清0统计数组
	//下面的代码统计每一列元素个数
	int pos;
	Triple<ElemType>* pSrcNode = this->m_pElemData;
	for (pos = 0; pos < nTriCounts; pos++, pSrcNode++)
	{
		pnColNums[pSrcNode->ePos.pos.col]++;//对应列的数目加1
	}
	int sum = 0;
	//下面的代码计算每一列首个元素存储位置
	int col;
	for (col = 0; col < cols; col++)
	{
		pnColPos[col] = sum;//每一列的存储起始位置是前面的列元素数目的总和
		sum += pnColNums[col];//累加已计算的列元素数目
	}
	//下面的代码实现转置
	Triple<ElemType>* pDstElemData = matrix.m_pElemData;
	pSrcNode = this->m_pElemData;
	
	for (pos = 0; pos < nTriCounts; pos++, pSrcNode++)
	{
		col = pSrcNode->ePos.pos.col;
		int dstPos = pnColPos[col];//pnColPos[col]初始时是首个列元素的存储位置，每次循环pnColPos[col]的值加1
		pDstElemData[dstPos].value = pSrcNode->value;
		pDstElemData[dstPos].ePos.pos.row = col;
		pDstElemData[dstPos].ePos.pos.col = pSrcNode->ePos.pos.row;
		pnColPos[col]++;//col存储的位置指向下一个位置
	}

	delete []pnColNums;
	delete []pnColPos;
	return matrix;

}
#endif