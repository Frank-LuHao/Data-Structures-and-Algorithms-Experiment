#if !defined(__TRIPLE_H__)
#define __TRIPLE_H__
typedef union
{
	unsigned int key;//�ȶ�ʱ�Ĺؼ���
#if defined(IS_BIG_ENDIAN)
	//���ģʽ
	struct
	{
		unsigned short row;//�к�
		unsigned short col;//�к�
	}pos;
#else
	//С��ģʽ
	struct
	{
		unsigned short col;//�к�
		unsigned short row;//�к�
	}pos;
#endif 
}ElemPos;
// ��Ԫ����ģ��
template<class ElemType>
struct Triple
{
	// ���ݳ�Ա:
	ElemPos ePos;						// ����Ԫ�ص����±������±�
	ElemType value;						// ����Ԫ�ص�ֵ

	// ���캯��ģ��:
	Triple();							// �޲����Ĺ��캯��
	Triple(int r, int c, const ElemType& v);	// ��֪���ݳ�Ա������Ԫ��
};

// ��Ԫ����ģ���ʵ�ֲ���
template<class ElemType>
Triple<ElemType>::Triple()
// ����������������Ԫ��
{
}

template<class ElemType>
Triple<ElemType>::Triple(int r, int c, const ElemType& v)
// �������������֪�����ݳ�Ա������Ԫ��
{
	ePos.pos.col = c; //�кŸ�ֵ
	ePos.pos.row = r; //�кŸ�ֵ
	value = v;		// ����Ԫ��ֵ
}

#endif