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
	bool Initialize(std::initializer_list<ElemType> ilist);//C++ 11��׼��֧�֣������б��ʼ��
#endif
	int GetRows() const;					// ���ؾ�������
	int GetCols() const;					// ���ؾ�������
	bool IsEmpty() const;//�����Ƿ��ǿվ���
	void PrintMatrix(const char* pMatrixName)const;//��ӡ����Ԫ��
	ElemType& operator()(int row, int col);	// ���غ��������
	Matrix<ElemType> CrossProduct(const Matrix<ElemType>& source)const;//����������
	Matrix<ElemType> DotProduct(const Matrix<ElemType>& source)const;//����������
	Matrix<ElemType> DataMult(const ElemType eLamda)const;//������������
	Matrix<ElemType> Sub(const Matrix<ElemType>& source)const;//���������
	Matrix<ElemType> Add(const Matrix<ElemType>& source)const;//���������
	Matrix<ElemType> Transpose() const;//����ת������
	Matrix<ElemType> Inverse()const;//�������
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
// ������������غ��������
{
	return *this->ElemAddress(row, col);
}
#if defined(_INITIALIZER_LIST_)
template <class ElemType>
//C++ 11��׼��֧�֣������������룬�����������֧��C++11��׼���������뱾�ӿ�
//���ܣ����ݳ�ʼ���б��ʼ������
bool Matrix<ElemType>::Initialize(std::initializer_list<ElemType> ilist)
{
	int size = ilist.size();
	int nTotal = GetRows() * GetCols();//��ȡ����ߴ�
	if (size > nTotal)
		return false;//�б�����������ھ���ߴ磬����ʧ��
	
	ElemType* pData = (ElemType*)ilist.begin();
	memcpy(this->m_pElemBase, pData, sizeof(ElemType) * size);//���б��е����ݿ��������
	memset(this->m_pElemBase + size, 0, sizeof(ElemType) * (nTotal - size));//ʣ�����������
	return true;
}
#endif
template <class ElemType>
//������������
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
//��ӡ����Ԫ��
void Matrix<ElemType>::PrintMatrix(const char* pMatrixName) const
{
	if (IsEmpty())
	{
		cout << "Empty matrix!" << endl;	// �վ���
		return;
	}
	int rows = GetRows();
	int cols = GetCols();
	int row, col;
	if (pMatrixName)
		cout << endl << pMatrixName << ':' << endl;//��ӡ��������
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
//�����п�
bool Matrix<ElemType>::IsEmpty() const
{
	if (GetCols() == 0 || GetRows() == 0)
		return true;
	return false;
}
template <class ElemType>
//����������
Matrix<ElemType> Matrix<ElemType>::CrossProduct(const Matrix<ElemType>& source) const
{
	if (GetCols() != source.GetRows())//�������ǰ������������ͺ�����������
	{
		Matrix<ElemType> mat(0, 0);
		return mat;//���ؿվ���
	}
	Matrix <ElemType> matrix(GetRows(), source.GetCols());

	int row, col;
	int cols = matrix.GetCols();
	int rows = matrix.GetRows();
	int k;
	ElemType* pDstElem = matrix.ElemAddress(0,0);
	int nSrcCols = source.GetCols();
	int nThisCols = GetCols();//ǰ��������
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
//����������
Matrix<ElemType> Matrix<ElemType>::DotProduct(const Matrix<ElemType>& source) const
{

	if (GetCols() != source.GetGetCols() || GetRows() != source.GetRows())
	{
		Matrix<ElemType> mat(0, 0);
		return mat;//���ؿվ���
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
//���������
Matrix<ElemType> Matrix<ElemType>::Sub(const Matrix<ElemType>& source) const
{

	if (GetCols() != source.GetGetCols() || GetRows() != source.GetRows())
	{
		Matrix<ElemType> mat(0, 0);
		return mat;//���ؿվ���
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
//���������
Matrix<ElemType> Matrix<ElemType>::Add(const Matrix<ElemType>& source) const
{

	if (GetCols() != source.GetCols() || GetRows() != source.GetRows())
	{
		Matrix<ElemType> mat(0, 0);
		return mat;//���ؿվ���
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
//����ת������
Matrix<ElemType> Matrix<ElemType>::Transpose() const
{
	int rows = GetRows();//Դ��������
	int cols = GetCols();//Դ��������
	Matrix <ElemType> matrix(cols, rows);//����ת�þ���
	int row, col;//���嵱ǰ�С��б���
	ElemType* pElem = this->ElemAddress(0, 0);//����Դ��Ԫ����ʼλ��
	ElemType* pDstElem;
	for (row = 0; row < rows; row++)
	{
		pDstElem = matrix.ElemAddress(0, row);//Դ������ж�Ӧ��ת�þ������
		for (col = 0; col < cols; col++, pElem++, pDstElem += rows)
			*pDstElem = *pElem;
	}
	return matrix;
}
template <class ElemType>
//�����������,����Gauss-Jordan (or reduced row) elimination method
Matrix<ElemType> Matrix<ElemType>::Inverse()const
{
	int rows = GetRows();
	int cols = GetCols();
	if (rows != cols)
	{//�����Ƿ����������
		Matrix<ElemType> mat(0, 0);
		return mat;//���ؿվ���
	}
	Matrix <ElemType> matrix = *this;//���ȸ���һ������

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
			return mat;//���ؿվ���
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
// ������ز���
template <class ElemType>
//�������
Matrix<ElemType> operator *(Matrix<ElemType> & const first, Matrix<ElemType> & const second)
{
	return first.CrossProduct(second);
}
template <class ElemType>
//��������(���ں�)
Matrix<ElemType> operator *(Matrix<ElemType>& const matrix, const ElemType  eLamda)
{
	return matrix.DataMult(eLamda);
}
template <class ElemType>
//��������(����ǰ)
Matrix<ElemType> operator *(const ElemType  eLamda,Matrix<ElemType>& const matrix)
{
	return matrix.DataMult(eLamda);
}
template <class ElemType>
//�������
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
	Matrix<ElemType> matInverse = second.Inverse();//���������
	return first.CrossProduct(matInverse);//������
}

#endif