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
	Array(const Array<ElemType>& source);	// �������캯��
	ElemType& operator()(int sub0, ...);	// ���غ��������
	ElemType* ElemAddress(int sub0, ...)const;	//��Ԫ�صĵ�ַ
	Array<ElemType>& operator =(const Array<ElemType>& source); // ���ظ�ֵ�����
protected:
	int Locate(int sub0, va_list& va) const;// ��Ԫ����˳��洢�е�λ��
	ElemType* m_pElemBase;							// ����Ԫ�ػ�ַ
	int m_nDim;								// ����ά��
	int* m_pnBounds;							// �����ά����
	int* m_pnConstants;							// ����ӳ��������
};
template<class ElemType>
Array<ElemType>::Array(int dim, ...)
{
	if (dim <= 0)
		throw std::runtime_error("Array dim error!");//���ά�Ȳ����Ƿ���ȷ��С��1ά�׳�����
	m_nDim = dim;		// ����ά��Ϊdim
	m_pnBounds = new int[dim];	// ���������ά���ȴ洢�ռ�
	va_list va;		// �䳤��������
	int elemTotalNum = 1;	// Ԫ������
	int tempPos;		// ��ʱ����
	va_start(va, dim);	// ��ʼ���䳤��������va�����ڴ洢�䳤������Ϣ��dimΪʡ�Ժ�������ұߵĲ�����ʶ��
	for (tempPos = 0; tempPos < dim; tempPos++)
	{	// Ϊ��ά���ȸ�ֵ������������Ԫ�ظ���
		m_pnBounds[tempPos] = va_arg(va, int);	// ȡ���䳤������Ϊ��ά����
		elemTotalNum = elemTotalNum * m_pnBounds[tempPos];	// ͳ��������Ԫ�ظ���
	}
	va_end(va);					// �����䳤����������
	m_pElemBase = new ElemType[elemTotalNum];	// ��������Ԫ�ؿռ�
	m_pnConstants = new int[dim];			// ��������ӳ��������
	m_pnConstants[dim - 1] = 1;
	for (tempPos = dim - 2; tempPos >= 0; --tempPos)
	{
		m_pnConstants[tempPos] = m_pnBounds[tempPos + 1] * m_pnConstants[tempPos + 1];// ��������ӳ��������
	}
}

template<class ElemType>
Array<ElemType>::~Array()
{
	if (m_pElemBase != NULL) delete[]m_pElemBase;		// �ͷ�����Ԫ�ؿռ�
	if (m_pnBounds != NULL) delete[]m_pnBounds;	// �ͷŸ�ά���ȿռ�
	if (m_pnConstants != NULL) delete[]m_pnConstants;		// �ͷ�ӳ���������ռ�
}
template<class ElemType>
Array<ElemType>::Array(const Array<ElemType>& source)
{
	*this = source;//��Ϊ������'=',����ֱ�Ӹ�ֵ
}
template <class ElemType>
ElemType& Array<ElemType>::operator()(int sub0, ...)
// ������������غ��������
{
	va_list va;								// �䳤��������
	va_start(va, sub0);						// ��ʼ���䳤��������va�����ڴ洢�䳤������Ϣ��sub0Ϊ��1ά���±꣬sub0Ϊʡ�Ժ�������ұߵĲ�����ʶ��
	int position = Locate(sub0, va);		// Ԫ����˳��洢�е�λ��
	va_end(va);								// �����䳤��������������
	return *(m_pElemBase + position);				// ��������Ԫ��
}
template <class ElemType>
ElemType* Array<ElemType>::ElemAddress(int sub0, ...)const
//��Ԫ�صĵ�ַ
{
	va_list va;								// �䳤��������
	va_start(va, sub0);						// ��ʼ���䳤��������va�����ڴ洢�䳤������Ϣ��sub0Ϊ��1ά���±꣬sub0Ϊʡ�Ժ�������ұߵĲ�����ʶ��
	int position = Locate(sub0, va);		// Ԫ����˳��洢�е�λ��
	if (position < 0)
		return NULL;
	return m_pElemBase + position;
}
template <class ElemType>
// �����������Ԫ����˳��洢�е�λ�ã�sub0Ϊ��1ά�±꣬vaΪ��2����dimά�±�Ŀɱ����
int Array<ElemType>::Locate(int sub0, va_list& va) const
{
	if (sub0 < 0 || sub0 >= m_pnBounds[0])
	{
		throw std::runtime_error("out of bound!");//�����������׳�����
		return -1;
	}
	int position = m_pnConstants[0] * sub0;		// ��ʼ��Ԫ����˳��洢�е�λ��	
	for (int tempPos = 1; tempPos < m_nDim; tempPos++)
	{
		int sub = va_arg(va, int);			// ȡ����tempPos+1ά����Ԫ���±�
		if (sub < 0 || sub >= m_pnBounds[tempPos])
		{
			throw std::runtime_error("out of bound!");	//�����������׳�����
			return -1;
		}
		position += m_pnConstants[tempPos] * sub;	// �ۼӳ˻���Ԫ����˳��洢�е�λ��
	}
	return position;					// ����Ԫ����˳��洢�е�λ��
}

template <class ElemType>
Array<ElemType>& Array<ElemType>::operator =(const Array<ElemType>& source)
// ���������������source��ֵ����ǰ���顪�����ظ�ֵ�����
{
	if (&source != this)
	{
		if (m_pElemBase != NULL) delete[]m_pElemBase;	// �ͷ�����Ԫ�ؿռ�
		if (m_pnBounds != NULL) delete[]m_pnBounds;// �ͷŸ�ά���ȿռ�
		if (m_pnConstants != NULL) delete[]m_pnConstants;		// �ͷ�ӳ���������ռ�

		m_nDim = source.m_nDim;					// ����ά��

		int elemTotalNum = 1;	
		// Ԫ������
		int tempPos;
		for (tempPos = 0; tempPos < m_nDim; tempPos++)
		{	// ͳ��������Ԫ�ظ���
			elemTotalNum = elemTotalNum * source.m_pnBounds[tempPos];		// ����������Ԫ�ظ���
		}
		m_pElemBase = new ElemType[elemTotalNum];	// Ϊ����Ԫ�ط���洢�ռ�
		memcpy(m_pElemBase, source.m_pElemBase, sizeof(ElemType) * elemTotalNum);// ��������Ԫ��
		m_pnBounds = new int[m_nDim];				// Ϊ�����ά���ȷ���洢�ռ�
		m_pnConstants = new int[m_nDim];			// Ϊ����ӳ������������洢�ռ�
		memcpy(m_pnBounds, source.m_pnBounds, sizeof(int) * m_nDim);//���Ƹ�ά����	
		memcpy(m_pnConstants, source.m_pnConstants, sizeof(int) * m_nDim);//���ƺ�������
	}
	return *this;
}

#endif