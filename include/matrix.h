#if !defined(_MATRIX_H_)
#define _MATRIX_H_
#include "array.h"
using namespace std;
#define SWAP(a,b,tmp)       \
    (tmp = a,a = b,b = tmp)

template<class ElemType>
class Matrix:public Array<ElemType>
{
public:
	Matrix(int rows,int cols);
	~Matrix();
#if defined(_INITIALIZER_LIST_)
	bool Initialize(std::initializer_list<ElemType> ilist);//C++ 11标准才支持，根据列表初始化
#endif
	int GetRows() const;					// 返回矩阵行数
	int GetCols() const;					// 返回矩阵列数
	bool IsEmpty() const;//返回是否是空矩阵
	void PrintMatrix(const char* pMatrixName)const;//打印矩阵元素
	ElemType& operator()(int row, int col);	// 重载函数运算符
	Matrix<ElemType> CrossProduct(const Matrix<ElemType>& source)const;//矩阵叉积运算
	Matrix<ElemType> DotProduct(const Matrix<ElemType>& source)const;//矩阵点积运算
	Matrix<ElemType> DataMult(const ElemType eLamda)const;//矩阵数乘运算
	Matrix<ElemType> Sub(const Matrix<ElemType>& source)const;//矩阵减运算
	Matrix<ElemType> Add(const Matrix<ElemType>& source)const;//矩阵加运算
	Matrix<ElemType> Transpose() const;//矩阵转置运算
	Matrix<ElemType> Inverse()const;//求逆矩阵
};
template<class ElemType>
Matrix<ElemType>::Matrix(int rows, int cols)
	:Array<ElemType>(2,rows,cols)
{
}
template<class ElemType>
Matrix<ElemType>::~Matrix()
{
}
template<class ElemType>
int Matrix<ElemType>::GetRows() const
{
	return this->m_pnBounds[0];
}
template<class ElemType>
int Matrix<ElemType>::GetCols() const
{
	return this->m_pnBounds[1];
}
template <class ElemType>
ElemType& Matrix<ElemType>::operator()(int row, int col)
// 操作结果：重载函数运算符
{
	return *this->ElemAddress(row, col);
}
#if defined(_INITIALIZER_LIST_)
template <class ElemType>
//C++ 11标准才支持，采用条件编译，如果编译器不支持C++11标准，将不编译本接口
//功能：根据初始化列表初始化矩阵
bool Matrix<ElemType>::Initialize(std::initializer_list<ElemType> ilist)
{
	int size = ilist.size();
	int nTotal = GetRows() * GetCols();//获取矩阵尺寸
	if (size > nTotal)
		return false;//列表的数据量大于矩阵尺寸，返回失败
	
	ElemType* pData = (ElemType*)ilist.begin();
	memcpy(this->m_pElemBase, pData, sizeof(ElemType) * size);//把列表中的数据拷贝入矩阵
	memset(this->m_pElemBase + size, 0, sizeof(ElemType) * (nTotal - size));//剩余的数据清零
	return true;
}
#endif
template <class ElemType>
//矩阵数乘运算
Matrix<ElemType> Matrix<ElemType>::DataMult(const ElemType eLamda) const
{
	Matrix<ElemType> matrix = *this;
	int rows = GetRows();
	int cols = GetCols();
	int row, col;
	ElemType* pElem = this->ElemAddress(0, 0);
	ElemType* pDstElem = matrix.ElemAddress(0, 0);
	for (row = 0; row < rows; row++)
	{
		for (col = 0; col < cols; col++, pElem++, pDstElem++)
		{
			*pDstElem = *pElem * eLamda;
		}
	}
	return matrix;
}
template <class ElemType>
//打印矩阵元素
void Matrix<ElemType>::PrintMatrix(const char* pMatrixName) const
{
	if (IsEmpty())
	{
		cout << "Empty matrix!" << endl;	// 空矩阵
		return;
	}
	int rows = GetRows();
	int cols = GetCols();
	int row, col;
	if (pMatrixName)
		cout << endl << pMatrixName << ':' << endl;//打印矩阵名称
	else
		cout << endl;
	ElemType* pElem = this->ElemAddress(0, 0);
	for (row = 0; row < rows; row++)
	{
		for (col = 0; col < cols; col++, pElem++)
		{
			if (col < cols - 1)
				cout << *pElem << ",\t";
			else
				cout << *pElem << endl;
		}
	}
}
template <class ElemType>
//矩阵判空
bool Matrix<ElemType>::IsEmpty() const
{
	if (GetCols() == 0 || GetRows() == 0)
		return true;
	return false;
}
template <class ElemType>
//矩阵叉积运算
Matrix<ElemType> Matrix<ElemType>::CrossProduct(const Matrix<ElemType>& source) const
{
	if (GetCols() != source.GetRows())//差积运算前矩阵列数必须和后矩阵行数相等
	{
		Matrix<ElemType> mat(0, 0);
		return mat;//返回空矩阵
	}
	Matrix <ElemType> matrix(GetRows(), source.GetCols());

	int row, col;
	int cols = matrix.GetCols();
	int rows = matrix.GetRows();
	int k;
	ElemType* pDstElem = matrix.ElemAddress(0,0);
	int nSrcCols = source.GetCols();
	int nThisCols = GetCols();//前矩阵列数
	ElemType* pElem, * pSrcElem;
	for (row = 0; row < rows; row++)
		for (col = 0; col < cols; col++, pDstElem++)
		{
			
			ElemType e = 0;
			pElem = this->ElemAddress(row, 0);
			pSrcElem = source.ElemAddress(0, col);
			for (k = 0; k < nThisCols; k++, pElem++, pSrcElem += nSrcCols)
			{
				e += *pElem * *pSrcElem;
			}
			*pDstElem = e;

		}

	return matrix;
}
template <class ElemType>
//矩阵点积运算
Matrix<ElemType> Matrix<ElemType>::DotProduct(const Matrix<ElemType>& source) const
{

	if (GetCols() != source.GetGetCols() || GetRows() != source.GetRows())
	{
		Matrix<ElemType> mat(0, 0);
		return mat;//返回空矩阵
	}
	int rows = GetRows();
	int cols = GetCols();
	Matrix <ElemType> matrix(rows, cols);
	int row, col;
	ElemType* pElem = this->ElemAddress(0, 0);
	ElemType* pSrcElem = source.ElemAddrss(0, 0);
	ElemType* pDstElem = matrix.ElemAddress(0, 0);
	for(row = 0;row<rows;row++)
		for (col = 0; col < cols; col++,pElem++,pSrcElem++,pDstElem++)
		{
			*pDstElem = *pSrcElem * *pElem;
		}
	return matrix;
}
template <class ElemType>
//矩阵减运算
Matrix<ElemType> Matrix<ElemType>::Sub(const Matrix<ElemType>& source) const
{

	if (GetCols() != source.GetGetCols() || GetRows() != source.GetRows())
	{
		Matrix<ElemType> mat(0, 0);
		return mat;//返回空矩阵
	}
	int rows = GetRows();
	int cols = GetCols();
	Matrix <ElemType> matrix(rows, cols);
	int row, col;
	ElemType* pElem = this->ElemAddress(0, 0);
	ElemType* pSrcElem = source.ElemAddrss(0, 0);
	ElemType* pDstElem = matrix.ElemAddress(0, 0);
	for (row = 0; row < rows; row++)
		for (col = 0; col < cols; col++,pElem++, pSrcElem++, pDstElem++)
		{
			*pDstElem = *pSrcElem - *pElem;
		}
	return matrix;
}
template <class ElemType>
//矩阵加运算
Matrix<ElemType> Matrix<ElemType>::Add(const Matrix<ElemType>& source) const
{

	if (GetCols() != source.GetCols() || GetRows() != source.GetRows())
	{
		Matrix<ElemType> mat(0, 0);
		return mat;//返回空矩阵
	}
	int rows = GetRows();
	int cols = GetCols();
	Matrix <ElemType> matrix(rows, cols);
	int row, col;
	ElemType* pElem = this->ElemAddress(0, 0);
	ElemType* pSrcElem = source.ElemAddress(0, 0);
	ElemType* pDstElem = matrix.ElemAddress(0, 0);
	for (row = 0; row < rows; row++)
		for (col = 0; col < cols; col++, pElem++, pSrcElem++, pDstElem++)
		{
			*pDstElem = *pSrcElem + *pElem;
		}
	return matrix;
}
template <class ElemType>
//矩阵转置运算
Matrix<ElemType> Matrix<ElemType>::Transpose() const
{
	int rows = GetRows();//源矩阵行数
	int cols = GetCols();//源矩阵列数
	Matrix <ElemType> matrix(cols, rows);//定义转置矩阵
	int row, col;//定义当前行、列变量
	ElemType* pElem = this->ElemAddress(0, 0);//定义源矩元素起始位置
	ElemType* pDstElem;
	for (row = 0; row < rows; row++)
	{
		pDstElem = matrix.ElemAddress(0, row);//源矩阵的行对应着转置矩阵的列
		for (col = 0; col < cols; col++, pElem++, pDstElem += rows)
			*pDstElem = *pElem;
	}
	return matrix;
}
template <class ElemType>
//求逆矩阵运算,采用Gauss-Jordan (or reduced row) elimination method
Matrix<ElemType> Matrix<ElemType>::Inverse()const
{
	int rows = GetRows();
	int cols = GetCols();
	if (rows != cols)
	{//必须是方阵才能求逆
		Matrix<ElemType> mat(0, 0);
		return mat;//返回空矩阵
	}
	Matrix <ElemType> matrix = *this;//首先复制一个矩阵

	int n = rows;
	int* is = new int [n];
	int* js = new int[n];
	ElemType** m = new   ElemType*[n];
	int i, j, k, r;
	for (i = 0; i < n; i++)
	{
		is[i] = i;
		js[i] = i;
		m[i] = matrix.ElemAddress(i, 0);
	}
	ElemType   fDet = 1.0;
	ElemType   f = 1.0;
	ElemType fMax = 0.0;
	ElemType tmp;
	for (k = 0; k < n; k++)
	{
		
		fMax = 0.0;
		for (i = k; i < n; i++)
		{
			for (j = k; j < n; j++)
			{

				f = m[i][j];
				if (f < 0)
					f = -f;
				if (f > fMax)
				{
					fMax = f;
					is[k] = i;
					js[k] = j;
				}
			}
		}
		if (fMax <0)
			fMax = -fMax;
		if (fMax < 0.0000001)
		{
			delete []is;
			delete []js;
			delete []m;
			Matrix<ElemType> mat(0, 0);
			return mat;//返回空矩阵
		}
		if (is[k] != k)
		{
			f = -f;

			for (r = 0; r < n; r++)
			{
				SWAP(m[k][r], m[is[k]][r], tmp);
			}

		}
		if (js[k] != k)
		{
			f = -f;
			for (r = 0; r < n; r++)
			{
				SWAP(m[r][k], m[r][js[k]], tmp);
			}
		}
		fDet *= m[k][k];
		m[k][k] = 1.0 / m[k][k];
		for (j = 0; j < n; j++)
		{
			if (j != k)
				m[k][j] *= m[k][k];
		}
		for (i = 0; i < n; i++)
		{
			if (i != k)
				for (j = 0; j < n; j++)
					if (j != k)
						m[i][j] = m[i][j] - m[i][k] * m[k][j];
		}
		for (i = 0; i < n; i++)
		{
			if (i != k)
				m[i][k] *= -m[k][k];
		}
	}
	for (k = n - 1; k >= 0; k--)
	{
		if (js[k] != k)
		{
			for (r = 0; r < n; r++)
			{
				SWAP(m[k][r], m[js[k]][r], tmp);
			}
		}
		if (is[k] != k)
		{

			for (r = 0; r < n; r++)
			{

				SWAP(m[r][k], m[r][is[k]], tmp);
			}
		}
	}
	delete []is;
	delete []js;
	delete []m;
	return matrix;

}
// 矩阵相关操作
template <class ElemType>
//差积运算
Matrix<ElemType> operator *(Matrix<ElemType> & const first, Matrix<ElemType> & const second)
{
	return first.CrossProduct(second);
}
template <class ElemType>
//数乘运算(数在后)
Matrix<ElemType> operator *(Matrix<ElemType>& const matrix, const ElemType  eLamda)
{
	return matrix.DataMult(eLamda);
}
template <class ElemType>
//数乘运算(数在前)
Matrix<ElemType> operator *(const ElemType  eLamda,Matrix<ElemType>& const matrix)
{
	return matrix.DataMult(eLamda);
}
template <class ElemType>
//点积运算
Matrix<ElemType> operator ^ (Matrix<ElemType>&const first, Matrix<ElemType>&const second)
{
	return first.DotProduct(second);
}
template <class ElemType>
Matrix<ElemType> operator -( Matrix<ElemType>& const first, Matrix<ElemType>&  const second)
{
	return first.Sub(second);
}
template <class ElemType>
Matrix<ElemType> operator +(Matrix<ElemType>& const first,  Matrix<ElemType>& const second)
{
	return first.Add(second);
}
template <class ElemType>
Matrix<ElemType> operator /( Matrix<ElemType>&const first,  Matrix<ElemType>& const second)
{
	Matrix<ElemType> matInverse = second.Inverse();//先求逆矩阵
	return first.CrossProduct(matInverse);//再求点积
}

#endif