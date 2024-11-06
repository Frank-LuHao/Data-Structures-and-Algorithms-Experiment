#if !defined(_CALCULATOR_H_)
#define _CALCULATOR_H_
#include <stdlib.h>
#include "DynamicArray.h"
#include "sq_stack.h"
template <class ElemType>
class Calculator
{
public:
	Calculator();
	Calculator(const char* pstrInfix);
	~Calculator();
	const char* GetInfix();
	const char* GetPostFix();
	bool InfixToPostfix();//��׺���ʽת��Ϊ��׺���ʽ
	bool CalcuPostfix(ElemType& eValue);//�����׺���ʽ��ֵ
	virtual Calculator& operator = (const char* pstrInfix); // ���ظ�ֵ�����
	bool PrintPostfix(); // ���������׺���ʽ�ĺ���
	bool CheckInfix();//�����׺���ʽ�Ƿ�Ϸ�

private:
	CDynamicArray <char> m_arryInfix;   //��׺���ʽ�ַ���̬����
	CDynamicArray <char> m_arryPostfix; //��׺���ʽ�ַ���̬����
	int IspPriority(char chOperator);   //��ȡ������ջ�����ȼ�
	int IcpPriority(char chOperator);   //��ȡ������ջ�����ȼ�
	bool IsOperator(char ch);           //�Ƿ��ַ��ǲ�����

};
template <class ElemType>
Calculator<ElemType>::Calculator()
{

}
template <class ElemType>
Calculator<ElemType>::Calculator(const char* pstrInfix)
{
	*this = pstrInfix;
}
template <class ElemType>
Calculator<ElemType>& Calculator<ElemType>::operator = (const char* pstrInfix)
{
	int idx = 0;
	while (pstrInfix[idx] != '\0')
	{
		m_arryInfix[idx] = pstrInfix[idx];
		idx++;
	}
	m_arryInfix[idx] = 0; //ĩβ���0
	return *this;
}
template <class ElemType>
Calculator<ElemType>::~Calculator()
{
}
template <class ElemType>
bool Calculator<ElemType>::IsOperator(char ch)
{
	if (ch == '+' || ch == '-' || ch == '*' ||
		ch == '/' || ch == '=' || ch == '(' ||
		ch == ')' || ch == '%' || ch == '^')
		return true;
	return false;
}
template <class ElemType>
int Calculator<ElemType>::IspPriority(char chOperator)
{
	int nPriority = -1;
	switch (chOperator)
	{
	case '=':nPriority = 0;
		break;
	case '(':nPriority = 1;
		break;
	case '*':
	case '/':
	case '%':
		nPriority = 5;
		break;
	case '+':
	case '-':
		nPriority = 3;
		break;
	case ')':
		nPriority = 8;
		break;
	case '^':
		nPriority = 7;
		break;
	default:
		nPriority = -1;
		break;
	}
	return nPriority;
}
template <class ElemType>
int Calculator<ElemType>::IcpPriority(char chOperator)
{
	int nPriority = -1;
	switch (chOperator)
	{
	case '=':nPriority = 0;
		break;
	case '(':nPriority = 8;
		break;
	case '*':
	case '/':
	case '%':
		nPriority = 4;
		break;
	case '+':
	case '-':
		nPriority = 2;
		break;
	case ')':
		nPriority = 1;
		break;
	case '^':
		nPriority = 6;
		break;
	default:
		nPriority = -1;
		break;
	}
	return nPriority;
}
template <class ElemType>
const char* Calculator<ElemType>::GetInfix()
{
	return &m_arryInfix[0];
}
template <class ElemType>
const char* Calculator<ElemType>::GetPostFix()
{
	return &m_arryPostfix[0];
}
template <class ElemType>
bool Calculator<ElemType>::InfixToPostfix()
{

	int nPostfixIdx = 0, nInfixIdx = 0;//�ַ��ں�׺���ʽ����׺���ʽ�е�λ��
	SqStack <char>stackOperator;
	stackOperator.Push('=');//������ջ�����'='
	char chInfix = m_arryInfix[nInfixIdx];
	while (chInfix != '\0')
	{
		bool bBreak = false;
		if ((chInfix >= '0' && chInfix <= '9') || chInfix == '.')
		{//������
			m_arryPostfix[nPostfixIdx++] = chInfix;//�����׺���ʽ
		}
		else if (IsOperator(chInfix))
		{//�������
			m_arryPostfix[nPostfixIdx++] = ' ';//��׺���ʽ������֮�����һ���ո�
			char chStackTop;
			if (!stackOperator.Top(chStackTop))
				break;//��ȡջ�����󣬽���ѭ��

			if ((chInfix == '+' || chInfix == '-') && ( nInfixIdx == 0 || m_arryInfix[nInfixIdx - 1] == '(')) {
				//��Ŀ�Ӽ��ţ���0�����׺���ʽ
				m_arryPostfix[nPostfixIdx++] = '0';
				m_arryPostfix[nPostfixIdx++] = ' ';

			} 

			while (IspPriority(chStackTop) >= IcpPriority(chInfix))
			{//ջ������������ȼ�����ջ������������ȼ�

				if (chStackTop == '=')
				{//ջ��Ԫ����'='�ţ�������ջѭ��
					bBreak = true;
					break;
				}
				else if (chStackTop != '(')
				{//ջ��Ԫ�ز���'('��ջ��Ԫ�ؽ����׺���ʽ
					m_arryPostfix[nPostfixIdx++] = chStackTop;//�����׺���ʽ
					m_arryPostfix[nPostfixIdx++] = ' ';//���һ���ո�
				}
				if (chInfix == ')' && chStackTop == '(') {
					stackOperator.Pop();//ջ��Ԫ�س�ջ
					break;
				}
				else {
					stackOperator.Pop();//ջ��Ԫ�س�ջ
				}
				if (!stackOperator.Top(chStackTop))
				{
					bBreak = true;
					break;//��ջ��ջΪ�գ�����ѭ��
				}

			}
			if (!bBreak && chInfix != ')') 
				stackOperator.Push(chInfix);//�������ջ
		}
		else
		{
			m_arryPostfix[nPostfixIdx++] = ' ';//��׺���ʽ������֮�����һ���ո�
		}
		if (bBreak)
			break;
		nInfixIdx++;
		chInfix = m_arryInfix[nInfixIdx];//��ȡ��һ����׺���ʽ�ַ�
	}
	if (m_arryPostfix.GetValidDataCount() < 1)
		return false;

	m_arryPostfix[m_arryPostfix.GetValidDataCount()] = 0;//ĩβ���0
	return true;

}
template <class ElemType>
bool Calculator<ElemType>::CalcuPostfix(ElemType& eValue)
{
	if (m_arryPostfix.GetValidDataCount() < 1)
		return false;
	eValue = 0;
	const type_info& tElemType = typeid(ElemType);//��ȡ��������
	const char* pszTypeName = tElemType.name();//ȡ����������
	SqStack <ElemType> stackOprand;
	int nPosixfixIdx = 0;
	char chPostfix = m_arryPostfix[nPosixfixIdx];
	bool bSuc = true; //�Ƿ�ɹ�
	while (chPostfix != '\0')
	{
		if ((chPostfix >= '0' && chPostfix <= '9') || chPostfix == '.')
		{
			char* pStop = NULL;
			//ת��Ϊ˫���ȸ�����
			ElemType eNum;
			if (strcmp(pszTypeName, "double") == 0)
			{//double ����
				eNum = strtod(&m_arryPostfix[nPosixfixIdx], &pStop);
				nPosixfixIdx += pStop - &m_arryPostfix[nPosixfixIdx];//�ַ�λ�õ�������ֵ��
			}
			else if (strcmp(pszTypeName, "float") == 0)
			{// float ����
				eNum = strtof(&m_arryPostfix[nPosixfixIdx], &pStop);
				nPosixfixIdx += pStop - &m_arryPostfix[nPosixfixIdx];//�ַ�λ�õ�������ֵ��
			}
			else
			{//����Ϊint����
				eNum = strtol(&m_arryPostfix[nPosixfixIdx], &pStop, 10);
				nPosixfixIdx += pStop - &m_arryPostfix[nPosixfixIdx];//�ַ�λ�õ�������ֵ��
			}
			stackOprand.Push(eNum);//��ջ

		}
		else if (IsOperator(chPostfix))
		{//�������
			ElemType eNum1, eNum2;
			if (!stackOprand.Pop(eNum1) || !stackOprand.Pop(eNum2))//��ջ����������
			{
				bSuc = false;
				break;
			}
			ElemType eNum = 0;
			switch (chPostfix)
			{//�������������
			case '+':eNum = eNum2 + eNum1;
				break;
			case '-':eNum = eNum2 - eNum1;
				break;
			case '*':eNum = eNum2 * eNum1;
				break;
			case '/':eNum = eNum2 / eNum1;
				break;
			case '%':eNum = (int)(eNum2) % (int)(eNum1);
				break;
			case '^':eNum = pow(eNum2, eNum1);
				break;

			}
			stackOprand.Push(eNum);//��������ջ
			nPosixfixIdx++;
		}
		else
			nPosixfixIdx++;
		chPostfix = m_arryPostfix[nPosixfixIdx];
	}
	if (!stackOprand.Pop(eValue))
		return false;
	return bSuc;
}
template <class ElemType>
bool Calculator<ElemType>::PrintPostfix()
{
	int nPosixfixIdx = 0;
	char chPostfix = m_arryPostfix[nPosixfixIdx];
	while (chPostfix != '\0')
	{
		printf("%c", chPostfix);
		nPosixfixIdx++;
		chPostfix = m_arryPostfix[nPosixfixIdx];
	}
	printf("\n");
	return true;
}
template <class ElemType>
bool Calculator<ElemType>::CheckInfix()
{
	int nInfixIdx = 0;
	char chInfix = m_arryInfix[nInfixIdx];
	SqStack<char> s;
	while (chInfix != '\0')
	{
		if (!(IsOperator(chInfix)) && !(chInfix >= '0' && chInfix <= '9') && chInfix != '.')
		{
			return false;
		}
		else if (chInfix == '(')
		{
			s.Push('(');
		}
		else
		{
			if (chInfix == ')')
			{
				if (s.IsEmpty())
				{
					return false;
				}
				else
				{
					s.Pop();
				}
			}
		}
		nInfixIdx++;
		chInfix = m_arryInfix[nInfixIdx];
	}
	if (!s.IsEmpty() || m_arryInfix[nInfixIdx-1] != '=')
	{
		return false;
	}
	else {
		return true;
	}
}
#endif