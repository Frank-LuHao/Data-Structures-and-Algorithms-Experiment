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
	bool InfixToPostfix();//中缀表达式转换为后缀表达式
	bool CalcuPostfix(ElemType& eValue);//计算后缀表达式的值
	virtual Calculator& operator = (const char* pstrInfix); // 重载赋值运算符
	bool PrintPostfix(); // 定义输出后缀表达式的函数
	bool CheckInfix();//检查中缀表达式是否合法

private:
	CDynamicArray <char> m_arryInfix;   //中缀表达式字符动态数组
	CDynamicArray <char> m_arryPostfix; //后缀表达式字符动态数组
	int IspPriority(char chOperator);   //获取操作码栈内优先级
	int IcpPriority(char chOperator);   //获取操作码栈外优先级
	bool IsOperator(char ch);           //是否字符是操作码

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
	m_arryInfix[idx] = 0; //末尾添加0
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

	int nPostfixIdx = 0, nInfixIdx = 0;//字符在后缀表达式和中缀表达式中的位置
	SqStack <char>stackOperator;
	stackOperator.Push('=');//首先入栈运算符'='
	char chInfix = m_arryInfix[nInfixIdx];
	while (chInfix != '\0')
	{
		bool bBreak = false;
		if ((chInfix >= '0' && chInfix <= '9') || chInfix == '.')
		{//是数字
			m_arryPostfix[nPostfixIdx++] = chInfix;//进入后缀表达式
		}
		else if (IsOperator(chInfix))
		{//是运算符
			m_arryPostfix[nPostfixIdx++] = ' ';//后缀表达式操作数之间添加一个空格
			char chStackTop;
			if (!stackOperator.Top(chStackTop))
				break;//获取栈顶错误，结束循环

			if ((chInfix == '+' || chInfix == '-') && ( nInfixIdx == 0 || m_arryInfix[nInfixIdx - 1] == '(')) {
				//单目加减号，将0加入后缀表达式
				m_arryPostfix[nPostfixIdx++] = '0';
				m_arryPostfix[nPostfixIdx++] = ' ';

			} 

			while (IspPriority(chStackTop) >= IcpPriority(chInfix))
			{//栈内运算符的优先级大于栈外运算符的优先级

				if (chStackTop == '=')
				{//栈顶元素是'='号，结束出栈循环
					bBreak = true;
					break;
				}
				else if (chStackTop != '(')
				{//栈顶元素不是'('，栈顶元素进入后缀表达式
					m_arryPostfix[nPostfixIdx++] = chStackTop;//进入后缀表达式
					m_arryPostfix[nPostfixIdx++] = ' ';//添加一个空格
				}
				if (chInfix == ')' && chStackTop == '(') {
					stackOperator.Pop();//栈顶元素出栈
					break;
				}
				else {
					stackOperator.Pop();//栈顶元素出栈
				}
				if (!stackOperator.Top(chStackTop))
				{
					bBreak = true;
					break;//出栈后栈为空，结束循环
				}

			}
			if (!bBreak && chInfix != ')') 
				stackOperator.Push(chInfix);//运算符入栈
		}
		else
		{
			m_arryPostfix[nPostfixIdx++] = ' ';//后缀表达式操作数之间添加一个空格
		}
		if (bBreak)
			break;
		nInfixIdx++;
		chInfix = m_arryInfix[nInfixIdx];//获取下一个中缀表达式字符
	}
	if (m_arryPostfix.GetValidDataCount() < 1)
		return false;

	m_arryPostfix[m_arryPostfix.GetValidDataCount()] = 0;//末尾添加0
	return true;

}
template <class ElemType>
bool Calculator<ElemType>::CalcuPostfix(ElemType& eValue)
{
	if (m_arryPostfix.GetValidDataCount() < 1)
		return false;
	eValue = 0;
	const type_info& tElemType = typeid(ElemType);//获取数据类型
	const char* pszTypeName = tElemType.name();//取得类型名称
	SqStack <ElemType> stackOprand;
	int nPosixfixIdx = 0;
	char chPostfix = m_arryPostfix[nPosixfixIdx];
	bool bSuc = true; //是否成功
	while (chPostfix != '\0')
	{
		if ((chPostfix >= '0' && chPostfix <= '9') || chPostfix == '.')
		{
			char* pStop = NULL;
			//转换为双精度浮点数
			ElemType eNum;
			if (strcmp(pszTypeName, "double") == 0)
			{//double 类型
				eNum = strtod(&m_arryPostfix[nPosixfixIdx], &pStop);
				nPosixfixIdx += pStop - &m_arryPostfix[nPosixfixIdx];//字符位置调整到数值后
			}
			else if (strcmp(pszTypeName, "float") == 0)
			{// float 类型
				eNum = strtof(&m_arryPostfix[nPosixfixIdx], &pStop);
				nPosixfixIdx += pStop - &m_arryPostfix[nPosixfixIdx];//字符位置调整到数值后
			}
			else
			{//其它为int类型
				eNum = strtol(&m_arryPostfix[nPosixfixIdx], &pStop, 10);
				nPosixfixIdx += pStop - &m_arryPostfix[nPosixfixIdx];//字符位置调整到数值后
			}
			stackOprand.Push(eNum);//入栈

		}
		else if (IsOperator(chPostfix))
		{//是运算符
			ElemType eNum1, eNum2;
			if (!stackOprand.Pop(eNum1) || !stackOprand.Pop(eNum2))//出栈两个操作数
			{
				bSuc = false;
				break;
			}
			ElemType eNum = 0;
			switch (chPostfix)
			{//根据运算符计算
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
			stackOprand.Push(eNum);//计算结果入栈
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