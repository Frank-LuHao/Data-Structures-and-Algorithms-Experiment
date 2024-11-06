#if !defined(_TRI_SPARSE_MATRIX_H_)
#define _TRI_SPARSE_MATRIX_H_
#include <memory.h>
#include "triple.h"
#include "SqList.h"
#include "matrix.h"
// ϡ�������Ԫ��˳�����ģ��
template<class ElemType>
class TriSparseMatrix : public SqList< Triple<ElemType>>
{
public:
	TriSparseMatrix(int nRows,int nCols,int nSize = 32);
	TriSparseMatrix(const TriSparseMatrix<ElemType>& source);	// ���ƹ��캯��ģ��
	TriSparseMatrix(const Matrix<ElemType>& source);	// ����ͨ�����ƹ��캯��ģ��
	~TriSparseMatrix();
	int GetRows() const;								// ����ϡ���������
	int GetCols() const;								// ����ϡ���������
	int GetNum() const;									// ����ϡ��������Ԫ����
	bool SetElem(int row, int col, const ElemType& v);		// ����ָ��λ�õ�Ԫ��ֵ
	bool GetElem(int row, int col, ElemType& v);			// ��ָ��λ�õ�Ԫ��ֵ
	Matrix<ElemType> ToMatrix() const;//����ת������ͨ����
	TriSparseMatrix<ElemType>& operator =(const TriSparseMatrix<ElemType>& source);// ���ظ�ֵ�����
	TriSparseMatrix<ElemType>& operator =(const Matrix<ElemType>& source);// ��������ͨ����ֵ�������
	TriSparseMatrix<ElemType> SimpleTranspose() const;//�򵥷�����ϡ������ת�þ���
	TriSparseMatrix<ElemType> FastTranspose() const;//����ת��ϡ�����ʱ�临�Ӷ�ΪO(n)
private:
	int Locate(int row, int col, bool& bFind);//��λָ���С���Ԫ����ϡ������еĴ洢λ��
	int m_nRows;//��������
	int m_nCols;//��������

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
// ����ϡ���������
{
	return m_nRows;
}
template<class ElemType>
// ����ϡ���������
int TriSparseMatrix<ElemType>::GetCols() const
{
	return m_nCols;
}
template<class ElemType>
int TriSparseMatrix<ElemType>::GetNum() const
// ����ϡ��������Ԫ����
{
	return this->Length();
}
template<class ElemType>
bool TriSparseMatrix<ElemType>::SetElem(int row, int col, const ElemType& v)
// ����ָ��λ�õ�Ԫ��ֵ
{
	if (row < 0 || col < 0 || row >= m_nRows || col >= m_nCols)
		return false;
	bool bFind = false;
	int nLocation = Locate(row, col, bFind);//��λ��ϡ������еĴ洢λ��
	if (nLocation < 0)
		return false;//��λʧ�ܣ����ش���
	if (!bFind)
	{//��nLocationλ�ò���Ԫ��
		if (v != 0)//ֵΪ0��Ԫ�ز���Ҫ����
		{
			Triple<ElemType> eTriNode(row, col, v);
			this->Insert(nLocation, eTriNode);
		}
	}
	else
	{
		if (v == 0)
		{//��ֵΪ0����˳�����ɾ����Ԫ��
			this->Delete(nLocation);
		}
		else
			this->m_pElemData[nLocation].value = v;
	}
	return true;
}
template<class ElemType>
bool TriSparseMatrix<ElemType>::GetElem(int row, int col, ElemType& v)
// ��ָ��λ�õ�Ԫ��ֵ
{
	if (row < 0 || col < 0 || row >= m_nRows || col >= m_nCols)
		return false;
	bool bFind = false;
	int nLocation = Locate(row, col, bFind);//��λ��ϡ������еĴ洢λ��
	if (nLocation < 0)
		return false;//��λʧ�ܣ����ش���
	if (!bFind)
	{//û���ҵ���������0Ԫ��
		v = 0;
	}
	else
		v = this->m_pElemData[nLocation].value;
	return true;

}

template<class ElemType>
// ���ƹ��캯��ģ��
TriSparseMatrix<ElemType>::TriSparseMatrix(const TriSparseMatrix<ElemType>& source)
{
	*this = source;//��Ϊ������'='������ֱ�Ӹ�ֵ
}
template<class ElemType>
//����ͨ�����ƹ��캯��
TriSparseMatrix<ElemType>::TriSparseMatrix(const Matrix<ElemType>& source)
{
	*this = source;//��Ϊ������'='������ֱ�Ӹ�ֵ
}
template<class ElemType>
//���ظ�ֵ�����
TriSparseMatrix<ElemType>& TriSparseMatrix<ElemType>::operator =(const TriSparseMatrix<ElemType>& source)
{
	int nSrcLen = source.Length();//��ȡԴ���ݳ���
	this->Clear();//���ԭ��������
	this->SaveData(source.m_pElemData, nSrcLen);//���ø���ӿڴ�������
	m_nCols = source.m_nCols;
	m_nRows = source.m_nRows;
	return *this;
}
template<class ElemType>
//��������ͨ����ֵ�����
TriSparseMatrix<ElemType>& TriSparseMatrix<ElemType>::operator =(const Matrix<ElemType>& source)
{
	this->Clear();//��������ڲ�����
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
//����ת������ͨ����
Matrix<ElemType> TriSparseMatrix<ElemType>::ToMatrix() const
{
	int rows = GetRows();
	int cols = GetCols();
	Matrix <ElemType> matrix(rows,cols);
	ElemType* pDstElem = matrix.ElemAddress(0, 0);
	memset(pDstElem, 0, sizeof(ElemType) * rows * cols);//����ռ�
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
//��λָ���С���Ԫ����ϡ������еĴ洢λ��
//�����۰���ҷ�����ʱ�临�Ӷ���O(n)��ΪO(log(n))
//�����λʧ�ܷ���-1,(û�в鵽��һ����λʧ��)
//�۰���ҷ���ǰ������������
int TriSparseMatrix<ElemType>::Locate(int row, int col, bool& bFind)
{
	bFind = false;
	if (row < 0 || col < 0 || row >= m_nRows || col >= m_nCols)
		return -1;
	int start = 0, end = GetNum() - 1;//����������ʼ����ֹλ��

	ElemPos ePos;//���嶨λλ�ñ���
	ePos.pos.row = row;
	ePos.pos.col = col;
	//����ȷ�������� [start,end]����
	if (ePos.key<this->m_pElemData[0].ePos.key )
		return 0;
	if (ePos.key>this->m_pElemData[end].ePos.key )
		return end+1;
	int mid;
	int nLocation = -1;//��λλ�ã���ֵΪ-1
	while (end - start > 1)
	{
		mid = (start + end) >> 1;//�����м�λ��
		if (ePos.key < this->m_pElemData[mid].ePos.key)
			end = mid;//��λλ�����м�λ��ǰ
		else if (ePos.key > this->m_pElemData[mid].ePos.key)
			start = mid; // ��λλ�����м�λ�ú�
		else
		{
			nLocation = mid;
			bFind = true;
			break;
		}
	}
	if (!bFind)
	{//ѭ����ɣ���û���ҵ�ʱ����ʣ����λ��û�бȶ�
		if (ePos.key == this->m_pElemData[start].ePos.key)
		{//����ʼλ�ñȶԳɹ����ҵ�
			bFind = true;
			nLocation = start;
		}
		else if (ePos.key == this->m_pElemData[end].ePos.key)
		{//����ֹλ�ñȶԳɹ����ҵ�
			bFind = true;
			nLocation = end;
		}
		else
			nLocation = start + 1; //û���ҵ����ݣ������Ҫ�������ݣ���Ӧ���ڸ�λ�ò���
	}
	return nLocation;
}
template<class ElemType>
//�򵥷�����ϡ������ת�þ���
TriSparseMatrix<ElemType> TriSparseMatrix<ElemType>::SimpleTranspose() const
{
	TriSparseMatrix<ElemType> matrix(GetCols(), GetRows(), GetNum() + 8);
	int nTriCounts = GetNum();
	int pos;
	Triple<ElemType> triNode;
	for (pos = 0; pos < nTriCounts; pos++)
	{
		triNode = this->m_pElemData[pos];
		matrix.SetElem(triNode.ePos.pos.col, triNode.ePos.pos.row, triNode.value);//��������
	}
	return matrix;
}
template<class ElemType>
//����ת��ϡ�����ʱ�临�Ӷ�ΪO(n)
TriSparseMatrix<ElemType> TriSparseMatrix<ElemType>::FastTranspose() const
{
	int cols = GetCols();
	int rows = GetRows();
	TriSparseMatrix<ElemType> matrix(cols, rows, GetNum() + 8);
	int nTriCounts = GetNum();

	matrix.m_nDataLen = nTriCounts;
	int* pnColNums = new int[cols];//�洢ÿһ��Ԫ�ظ���
	int* pnColPos = new int[cols]; //ÿһ��Ԫ��Ԫ�����ȴ洢λ��
	memset(pnColNums, 0, sizeof(int) * cols);//��0ͳ������
	//����Ĵ���ͳ��ÿһ��Ԫ�ظ���
	int pos;
	Triple<ElemType>* pSrcNode = this->m_pElemData;
	for (pos = 0; pos < nTriCounts; pos++, pSrcNode++)
	{
		pnColNums[pSrcNode->ePos.pos.col]++;//��Ӧ�е���Ŀ��1
	}
	int sum = 0;
	//����Ĵ������ÿһ���׸�Ԫ�ش洢λ��
	int col;
	for (col = 0; col < cols; col++)
	{
		pnColPos[col] = sum;//ÿһ�еĴ洢��ʼλ����ǰ�����Ԫ����Ŀ���ܺ�
		sum += pnColNums[col];//�ۼ��Ѽ������Ԫ����Ŀ
	}
	//����Ĵ���ʵ��ת��
	Triple<ElemType>* pDstElemData = matrix.m_pElemData;
	pSrcNode = this->m_pElemData;
	
	for (pos = 0; pos < nTriCounts; pos++, pSrcNode++)
	{
		col = pSrcNode->ePos.pos.col;
		int dstPos = pnColPos[col];//pnColPos[col]��ʼʱ���׸���Ԫ�صĴ洢λ�ã�ÿ��ѭ��pnColPos[col]��ֵ��1
		pDstElemData[dstPos].value = pSrcNode->value;
		pDstElemData[dstPos].ePos.pos.row = col;
		pDstElemData[dstPos].ePos.pos.col = pSrcNode->ePos.pos.row;
		pnColPos[col]++;//col�洢��λ��ָ����һ��λ��
	}

	delete []pnColNums;
	delete []pnColPos;
	return matrix;

}
#endif