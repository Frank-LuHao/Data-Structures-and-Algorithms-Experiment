#if !defined(_DYNAMIC_ARRAY_)
#define  _DYNAMIC_ARRAY_
#include <vector>
#include <memory.h>
template <class elemType>  class CDynamicArray
{
public:
	CDynamicArray();//??????
#if defined(_INITIALIZER_LIST_)
	CDynamicArray(std::initializer_list<elemType> ilist);//???????§Ò?????,CXX11 ?????
#endif
	CDynamicArray(const CDynamicArray& other);//??????????
	~CDynamicArray();//????????
	int SetBufferSize(int nSize);//???????????????§³
	int GetValidDataCount();//????????§¹????????
	int  AppendData(elemType tData);//???????????¦Â
	int SaveData(elemType* pTData, int nDataLen);//???????????
	int InsertData(int index, elemType tData);//????????
	bool RemoveData(int index, elemType &tData);//???????
	const CDynamicArray& operator = (const CDynamicArray& other);//???? ??=???????
	const CDynamicArray& operator += (const CDynamicArray& other);//???? ??+=???????
	elemType& operator [] (int index);//???? ??[]???????

private:
	elemType* m_pData;//????????????
	int m_nDataLen;//????›¥????§¹????????
	int m_nBufferLen;//???????????§³
};

//?????????????
template <class elemType> 
CDynamicArray<elemType>::CDynamicArray()
{
	m_pData = new elemType[4];//??????????›¥4?????

	m_nBufferLen = 4;
	m_nDataLen = 0;
	memset(m_pData, 0, sizeof(elemType) * m_nBufferLen);
}
#if defined(_INITIALIZER_LIST_)
//C++ 11?????????????????????????????C++11????????????????
template <class elemType> 
CDynamicArray<elemType>::CDynamicArray(std::initializer_list<elemType> ilist)
{
	//elemType tData = ilist.begin();
	int size = ilist.size();
	m_nBufferLen = size + 4;
	m_pData = new elemType[m_nBufferLen];
	m_nDataLen = size;
	elemType* pData = (elemType * )ilist.begin();
	memcpy(m_pData, pData, sizeof(elemType) * size);
	
	memset(m_pData+ m_nDataLen, 0, sizeof(elemType) * (m_nBufferLen-m_nDataLen));//????????§¹????????????????

	
}
#endif
template <class elemType>
CDynamicArray<elemType>::~CDynamicArray()
{
	if (m_pData)
	{
		delete[]m_pData;
		m_pData = NULL;
	}
	m_nDataLen = 0;
	m_nBufferLen = 0;
}
template <class elemType> 
CDynamicArray<elemType>::CDynamicArray(const CDynamicArray<elemType>& other)
{
	int nDataLen = other.m_nDataLen;
	m_nBufferLen = other.m_nBufferLen;
	m_pData = new elemType[m_nBufferLen];//???¨??
	m_nDataLen = 0;//??§¹????????0
	elemType* pOtherBuf = other.m_pData;
	if (nDataLen > 0)
	{//?????????????§¹?????????0
		SaveData(pOtherBuf, nDataLen);//????????
	}
}

template <class elemType> 
int CDynamicArray<elemType>::SetBufferSize(int nSize)
{
	//????????????
	if (nSize <= m_nDataLen)
		return m_nBufferLen;
	elemType* pData = new elemType[nSize];//???¡¤??¨????
	if (!pData)
	{
		return m_nBufferLen;
	}
	if (m_pData != pData)
	{
		memcpy(pData, m_pData, sizeof(elemType) * m_nDataLen);//????????????????????????????
		memset(pData + m_nDataLen, 0, (m_nBufferLen - m_nDataLen) * sizeof(elemType));//?????§Ö????????????
		delete[] m_pData;//??????????
	}
	m_pData = pData;
	m_nBufferLen = nSize;
	return m_nBufferLen;
}
template <class elemType> 
int CDynamicArray<elemType>::GetValidDataCount()
{
	return m_nDataLen;
}
template <class elemType> 
int CDynamicArray<elemType>::AppendData(elemType tData)
{
	//???????????????????
	if (m_nDataLen >= m_nBufferLen)
	{
		int nNewBufLen = m_nBufferLen;
		if (nNewBufLen < 64)
			nNewBufLen += 32;
		else
			nNewBufLen += nNewBufLen >> 1;

		if (!SetBufferSize(nNewBufLen))
			return -1;

	}
	int nIdx = m_nDataLen;
	m_pData[m_nDataLen++] = tData;
	return nIdx;
}
template <class elemType> 
int CDynamicArray<elemType>::SaveData(elemType* pTData, int nDataLen)
{
	//????????????????????????
	if (!pTData || nDataLen <= 0)
		return -1;

	if (m_nDataLen + nDataLen >= m_nBufferLen)
	{
		int nNewBufLen = m_nDataLen + nDataLen;
		if (nNewBufLen < 64)
			nNewBufLen += 32;
		else
			nNewBufLen += nNewBufLen >> 1;
		if (!SetBufferSize(nNewBufLen))
			return -1;
	}
	int index = m_nDataLen;
	memcpy(m_pData + index, pTData, nDataLen * sizeof(elemType));
	m_nDataLen += nDataLen;

	return index;
}
template <class elemType> 
int CDynamicArray<elemType>::InsertData(int index, elemType tData)
{
	//???????¦Ë??????
	if (index < 0 || index >= m_nBufferLen)
		return -1;
	//??üv????
	if (m_nDataLen >= m_nBufferLen)
	{
		int nNewBufLen = m_nBufferLen;
		if (nNewBufLen < 64)
			nNewBufLen += 32;
		else
			nNewBufLen += nNewBufLen >> 1;

		if (!SetBufferSize(nNewBufLen))
			return -1;
	}
	if (index <= m_nDataLen - 1)
	{//??????????
		memmove(m_pData + index + 1, m_pData + index, sizeof(elemType) * (m_nDataLen - index));
		m_pData[index] = tData;
		m_nDataLen++;
	}
	else
	{//?????¦Â
		index = m_nDataLen;
		m_pData[index] = tData;
		m_nDataLen++;
	}
	return index;
}
template <class elemType> 
bool CDynamicArray<elemType>::RemoveData(int index, elemType & tData)
{
	if (index < 0 || index >= m_nDataLen)
		return false;

	tData = m_pData[index];//?????????????tData
	
	if (index < m_nDataLen - 1) //???????????????????????????????????
		memmove(m_pData + index, m_pData + index + 1, (m_nDataLen - index - 1) * sizeof(elemType));//????????????????index????¦Ë??
	m_nDataLen--;
	return true;
}
template <class elemType> 
const CDynamicArray<elemType>& CDynamicArray<elemType>::operator = (const CDynamicArray<elemType>& other)
{
	m_nDataLen = 0;//????????
	elemType* pDataBuf = other.m_pData;
	int nDataLen = other.m_nDataLen;
	if (nDataLen > 0)
	{
		SaveData(pDataBuf, nDataLen);
	}
	return *this;
}
template <class elemType> 
const CDynamicArray<elemType>& CDynamicArray<elemType>::operator += (const CDynamicArray<elemType>& other)
{

	elemType* pDataBuf = other.m_pData;
	int nDataLen = other.m_nDataLen;
	if (nDataLen > 0)
	{
		SaveData(pDataBuf, nDataLen);
	}
	return *this;
}
template <class elemType> 
elemType& CDynamicArray<elemType>::operator [] (int index)
{
	if (index < 0)
	{
		throw std::runtime_error("out of bound!");//?????
	}
	if (index >= m_nBufferLen)
	{
		int nNewBufLen = index + 1;
		if (nNewBufLen < 64)
			nNewBufLen += 32;
		else
			nNewBufLen += nNewBufLen >> 1;
		SetBufferSize(nNewBufLen);
	}
	if (index >= m_nDataLen)
		m_nDataLen = index + 1;
	return m_pData[index];
}
#endif