
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "ExpressionFunc.h"
//#include "pch.h"
////////////////////////

//���ʽ����
struct SExpItem
{
	int				m_itemType;
	int				m_operatorType;
	double			m_itemValue;
};

////////////////////////


/*
���ܣ�
	��ʼ���������ʽ
������
	pExp���������ʽ
���أ�
	0����ȷ
	����������
*/
int ExpressionInit(struct SExpression *pExp)
{
	if (pExp == NULL)
		return 200;

	memset(pExp, 0x00, sizeof(struct SExpression));
	pExp->m_value = 0.0;
	return 0;
}

/*��׺���ʽת��׺���ʽ
ת��������Ҫ�õ�ջ������������£�

1��������������������Ǿ�ֱ�ӽ��������

2����������������������ǽ�����뵽ջ�У�����������ʱ����Ҳ�������ջ�С�

3���������һ�������ţ���ջԪ�ص������������Ĳ��������ֱ������������Ϊֹ��ע�⣬������ֻ�������������

4����������κ������Ĳ��������磨��+���� ��*�������������ȣ���ջ�е���Ԫ��ֱ���������ָ������ȼ���Ԫ��(����ջΪ��)Ϊֹ����������ЩԪ�غ󣬲Ž������Ĳ�����ѹ�뵽ջ�С���һ����Ҫע�⣬ֻ��������" ) "����������ǲŵ���" ( "������������Ƕ����ᵯ��" ( "��

5��������Ƕ����������ĩβ����ջ������Ԫ�����ε�����
*/
int ConvertExp(struct SExpression *pExp, struct HZExpression *hzexp, int expStrlen)
{
	int HZtop = 0;  //��׺���ʽ��ǰλ��
	int pex = 0;    //ͳ�����ֳ���
	int j = 0;
	for (int i = 0; i < expStrlen; i++)
	{
		
		char temp[256] = {};
		//�����ȡpExp->m_expStr�е����֣��ݴ浽temp�У���ֵ���ȡ����������׺���ʽ������
		if (pExp->m_expStr[i] >= '0'&&pExp->m_expStr[i] <= '9' || pExp->m_expStr[i] == '.')
		{
			pex++;
			
			//��ĳһ���ֵĺ�һλ��"+-*/�����ͽ�β'\0' "���е�һ������������ֵ������һλ
			if (pExp->m_expStr[i + 1] == '+' || pExp->m_expStr[i + 1] == '-' || pExp->m_expStr[i + 1] == '*' || pExp->m_expStr[i + 1] == '/' || pExp->m_expStr[i + 1] == '(' || pExp->m_expStr[i + 1] == ')' || pExp->m_expStr[i + 1] == '\0')
			{
				for (int k = i - pex + 1; k <= i; k++)
				{
					temp[j++] = pExp->m_expStr[k];
				}
				pExp->m_operandStack[pExp->m_operandTop++] = atof(temp);
				hzexp[HZtop].exp_operand = atof(temp);
				hzexp[HZtop].exp_operator = '\0';
				hzexp[HZtop++].exp_item_type = ITEM_TYPE_OPERAND;
				j = 0;
				pex = 0;
			}

		}

		else if (pExp->m_expStr[i] == '+' || pExp->m_expStr[i] == '-' || pExp->m_expStr[i] == '*' || pExp->m_expStr[i] == '/' || pExp->m_expStr[i] == '(')
		{
			//���ʽ���и���ʱ���ڸ���ǰ��0��������ת��Ϊ�������ʽ
			if ((pExp->m_expStr[i] == '-' &&i<1)||(pExp->m_expStr[i] == '-' &&pExp->m_expStr[i-1] == '('))
			{
				hzexp[HZtop].exp_item_type = ITEM_TYPE_OPERAND;
				hzexp[HZtop].exp_operand = 0;
				hzexp[HZtop++].exp_operator = '\0';
			}
			if (pExp->m_operatorTop == 0)
			{
				pExp->m_operatorStack[pExp->m_operatorTop++] = pExp->m_expStr[i];
			}
			else {
				//�жϵ�ǰ�������ջ�е����ȼ��������ȼ��ߵĵ���
				for (int a = pExp->m_operatorTop - 1; a >= 0; a--)
				{
					if (getPrioraty(pExp->m_expStr[i]) <= getPrioraty(pExp->m_operatorStack[a]) && pExp->m_operatorStack[a] != '(')
					{
						//pExp->m_operandStack[pExp->m_operandTop++] = pExp->m_operatorStack[--pExp->m_operatorTop];
						hzexp[HZtop].exp_operator = pExp->m_operatorStack[--pExp->m_operatorTop];
						hzexp[HZtop].exp_operand = 0x00;
						hzexp[HZtop++].exp_item_type = ITEM_TYPE_OPERATOR;
					}
					else
						break;
				}
				pExp->m_operatorStack[pExp->m_operatorTop++] = pExp->m_expStr[i]; //��ǰ�������ջ
			}
		}
		//����������ʱ���������ջ��ֱ��'('
		else if (pExp->m_expStr[i] == ')'&&pExp->m_operatorTop > 0)
		{
			while (pExp->m_operatorStack[--pExp->m_operatorTop] != '(')
			{
				hzexp[HZtop].exp_operator = pExp->m_operatorStack[pExp->m_operatorTop];
				hzexp[HZtop].exp_operand = 0x00;
				hzexp[HZtop++].exp_item_type = ITEM_TYPE_OPERATOR;
			}
			
		}

		//�����ַ������һλʱ��������ȫ����ջ
		if (i == expStrlen - 1)
		{
			while (pExp->m_operatorTop > 0)
			{
				hzexp[HZtop].exp_operator = pExp->m_operatorStack[--pExp->m_operatorTop];
				hzexp[HZtop].exp_operand = 0x00;
				hzexp[HZtop++].exp_item_type = ITEM_TYPE_OPERATOR;
			}
		}
	}
	return HZtop; 
}

int getPrioraty(char m_opreator)
{
	switch (m_opreator)
	{
	case '+':
		return 1;
	case '-':
		return 2;
	case '*':
		return 3;
	case '/':
		return 4;
	case '(':
		return 5;
	case ')':
		return 6;
	default:
		return 0;

	}

}

/*�������ֵ*/
double getValue(double a, double b, char c)
{
	switch (c)
	{
	case '+':
		return a + b;
	case '-':
		return a - b;
	case '*':
		return a * b;
	case '/':
		return a / b;
	default:
		break;
	}
}

/*
���ܣ�
	�����������ʽ��ֵ
������
	pExp���������ʽ
		�������ʽ�ɲ����������������
		����������-����+����-����*����/��������(��������)
		��������0--9��С������ɵ�������С��
���أ�
	0����ȷ���������ʽ��ֵ��pExp->m_value��
	����������
*/
int ExpressionEvaluate(struct SExpression *pExp, struct HZExpression *hzexp, int HZtop)
{
	int p=HZtop;
	
		//��ȡһ�����ʽ��

		//�õ����ǲ�����������������

		//�õ����ǲ��������������������������������


	if(p==1)				//ֻ��һ�������ʱ��ֱ�����
		pExp->m_value = hzexp[0].exp_operand;
	else {
		for (int i = 0; i < p; i++)
		{
			if (hzexp[i].exp_item_type == ITEM_TYPE_OPERATOR) //�����������ȡ��ǰ��λ���м���
			{
				hzexp[i - 2].exp_operand = getValue(hzexp[i - 2].exp_operand, hzexp[i - 1].exp_operand, hzexp[i].exp_operator);
				hzexp[i - 2].exp_operator = '\0';
				hzexp[i - 2].exp_item_type = ITEM_TYPE_OPERAND;
				for (int s = i; s < HZtop - 1; s++)
				{
					hzexp[s - 1].exp_item_type = hzexp[s + 1].exp_item_type;
					hzexp[s - 1].exp_operand = hzexp[s + 1].exp_operand;
					hzexp[s - 1].exp_operator = hzexp[s + 1].exp_operator;

				}
				HZtop = HZtop - 2;        
				
				i = i - 2;         //��׺���ʽλ�ú�����λ
				if (HZtop == 1)
				{
					pExp->m_value = hzexp[--HZtop].exp_operand;
					break;
				}

			}
		}
	}

	

	return 100;
}


//////////////////////////

