#if !defined(_SQ_LIST_H_)
#define  _SQ_LIST_H_
#include "ListBase.h"
template <class ElemType> 
class SqList: public ListBase <ElemType>
{
public:
	SqList(int nBufferSize = 16);//���캯��
	SqList(const SqList& other);//�������캯��
	~SqList();//��������
	int Length() const;//��ȡ���Ա���Ԫ�ظ���
	bool IsEmpty() const;//�ж����Ա��Ƿ�Ϊ��
	void Clear();//������Ա�
	bool GetElem(int position, ElemType& e) const;//��ȡָ��λ��Ԫ��
	bool SetElem(int position, const ElemType e);//����ָ��λ��Ԫ��
	bool Delete(int position,ElemType& e);//ɾ��ָ��λ��Ԫ��
	bool Insert(int position,const ElemType e);//��ָ��λ�ò���Ԫ��
	void Traverse(bool (*visit)(const ElemType&))const;//�������Ա�
	int AddTail(const ElemType e) ;	//������ݵ����Ա�ĩβ
	virtual SqList& operator = (const SqList<ElemType>& source); // ���ظ�ֵ�����
protected:
	int SaveData(ElemType* pTData, int nDataLen);//�ڲ��洢���ݵ�˽�к���
	int SetBufferSize(int nSize);//�ڲ�������������С��˽�к���
	ElemType* m_pElemData;//�ڲ����ݻ�����
	int m_nDataLen;//�ڲ��洢����Ч������������
	int m_nBufferLen;//�ڲ���������С
};

//˳���ʵ�ִ���
template <class ElemType> 
SqList<ElemType>::SqList(int nBufferSize)
{
	m_pElemData = new ElemType[nBufferSize];//��ʼ��ʱ�ܹ��洢4��Ԫ��

	m_nBufferLen = nBufferSize;
	m_nDataLen = 0;
	//memset(m_pElemData, 0, sizeof(ElemType) * m_nBufferLen);//����Ƕ��󣬸þ��Ѷ������㣬����
}

template <class ElemType> 
SqList<ElemType>::~SqList()
{
	if (m_pElemData)
	{
		delete[]m_pElemData;
		m_pElemData = NULL;
	}
	m_nDataLen = 0;
	m_nBufferLen = 0;
}
template <class ElemType>
SqList<ElemType>& SqList<ElemType>::operator = (const SqList<ElemType>& source)
{
	Clear();//�������
	int nDataLen = source.m_nDataLen;
	ElemType* pSrcBuf = source.m_pElemData;
	if (nDataLen > 0)
	{//�������Ķ�����Ч���ݳ��Ȳ�Ϊ0
		SaveData(pSrcBuf, nDataLen);//��������
	}
	return *this;
}
template <class ElemType>
SqList<ElemType>::SqList(const SqList<ElemType>& other)
{
	int nDataLen = other.m_nDataLen;
	m_nBufferLen = other.m_nBufferLen;
	m_pElemData = new ElemType[m_nBufferLen];//���仺��
	m_nDataLen = 0;//��Ч���ݳ���Ϊ0
	ElemType* pOtherBuf = other.m_pElemData;
	if (nDataLen > 0)
	{//�������Ķ�����Ч���ݳ��Ȳ�Ϊ0
		SaveData(pOtherBuf, nDataLen);//��������
	}
}

template <class ElemType> 
int SqList<ElemType>::SetBufferSize(int nSize)
{

	if (nSize <= m_nBufferLen)
		return m_nBufferLen;
	ElemType* pData = new ElemType[nSize];//���·��仺����
	memcpy(pData, m_pElemData, sizeof(ElemType) * m_nDataLen);//�Ѿɻ����������ݿ������»�������
	memset(pData + m_nDataLen, 0, (m_nBufferLen - m_nDataLen) * sizeof(ElemType));//�����еĿ�����������
	delete[] m_pElemData;//�ͷžɵĻ�����
	m_pElemData = pData;
	m_nBufferLen = nSize;
	return m_nBufferLen;//���ػ������ߴ�
}
template <class ElemType> 
int SqList<ElemType>::Length() const
{
	return m_nDataLen;
}
template <class ElemType> 
void SqList<ElemType>::Clear()
{
	m_nDataLen = 0;
}
template <class ElemType>
bool SqList<ElemType>::IsEmpty()const
{
	if (m_nDataLen <= 0)
		return true;
	return false;
}

template <class ElemType> 
int SqList<ElemType>::SaveData(ElemType* pTData, int nDataLen)
{
	//����һ������׷���׵�ַ������
	if (!pTData || nDataLen <= 0)
		return -1;

	if (m_nDataLen + nDataLen >= m_nBufferLen)
	{
		if (!SetBufferSize((m_nDataLen + nDataLen)*2))
			return -1;
	}
	int position = m_nDataLen;
	memcpy(m_pElemData + position, pTData, nDataLen * sizeof(ElemType));
	m_nDataLen += nDataLen;

	return position;
}
template <class ElemType> 
bool SqList<ElemType>::Insert(int position, const ElemType eData)
{
	//���ز���λ������
	if (position < 0 )
		return false;
	//��黺����
	if (m_nDataLen >= m_nBufferLen)
	{
		if (!SetBufferSize(m_nDataLen*2))//����������1��
			return false;
	}
	if (position <= m_nDataLen - 1)
	{//����������
		// ��ѭ�����鱻memmove���,�ٶȸ���,��������������Ϊû������'='�Ķ���
		for (int i = m_nDataLen-1; i >=position; i--)
		{
			m_pElemData[i+1] = m_pElemData[i];
		}
		//memmove(m_pElemData + position + 1, m_pElemData + position, sizeof(ElemType) * (m_nDataLen - position));
		m_pElemData[position] = eData;
		m_nDataLen++;
	}
	else
	{//׷�ӵ�ĩβ
		position = m_nDataLen;
		m_pElemData[position] = eData;
		m_nDataLen++;
	}
	return true;
}
template <class ElemType>
int SqList<ElemType>::AddTail(const ElemType e)
{
	//��黺����
	if (m_nDataLen >= m_nBufferLen)
	{
		if (!SetBufferSize(m_nDataLen * 2))//����������1��
			return -1;
	}
	int position = m_nDataLen;//׷�ӵ�ĩβ
	m_pElemData[position] = e; 
	m_nDataLen++;
	return position;
}

template <class ElemType> 
bool SqList<ElemType>::Delete(int position,ElemType& eData)
{
	if (position < 0 || position >= m_nDataLen)
		return false;

	eData = m_pElemData[position];//ɾ�������ݸ�ֵ��tData
	
	if (position < m_nDataLen - 1) //��������һ�����ݣ�����Ҫ�ƶ������������
	{
		//��ѭ�����鱻memmove���,�ٶȸ���,��������û������'='�����ݶ���
		for (int i = position + 1; i < m_nDataLen; i++)
		{
			m_pElemData[i - 1] = m_pElemData[i];
		}
		//memmove(m_pElemData + position, m_pElemData + position + 1, (m_nDataLen - position - 1) * sizeof(ElemType));//�������������ǰ��
	}
	m_nDataLen--;
	return true;
}
template <class ElemType>
bool SqList<ElemType>::GetElem(int position, ElemType& e) const
{
	if (position < 0 || position >= m_nDataLen)
		return false;
	e = m_pElemData[position];
	return true;
}
template <class ElemType> 
bool SqList<ElemType>::SetElem(int position, const ElemType e)
{
	if (position < 0)
		return false;
	if (position >= m_nBufferLen)
	{
		if (!SetBufferSize(position * 2))//����������1��
			return false;
	}
	m_pElemData[position] = e;
	if (position >= m_nDataLen)
		m_nDataLen = position + 1;
	return true;
}
template <class ElemType>
void SqList<ElemType>::Traverse(bool (*visit)(const ElemType&)) const
{
	if (!visit)
		return;
	int position;
	for (position = 0; position < m_nDataLen; position++)
	{
		if(!(*visit)(m_pElemData[position]))
			break;//����ָ����ʺ�������Ϊfalse�˳�ѭ��
	}
}
#endif