
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ExpressionFunc.h"
//#include "pch.h"
const char			*g_validChars = "0123456789.+-*/()";

 
int main()
{
	struct SExpression	exp;
	struct HZExpression hzexp[256] = {};
	int					ret, ii, jj, nn, mm;

	while (1 == 1)
	{
		//������ʽ���磺
		// 20.6-5*(3.5-1.0)
		// (3+2)*5-10.8
		// (((-3.8))) 
		// (((3.2)))
		// (((-3+5))) 
		// -3+5.7
		// -3
		// -(3+7.6)*6
		// (3.5+2.5)*(3.5-2.5)
		// 3*(-5-8)
		// (-5-8)*3
		// (2+3*3-1+4)

		//scanf()��������ʽ���пո�
		ExpressionInit(&exp);
		memset(hzexp, 0x00, sizeof(struct HZExpression));
		printf("expression = ");
		scanf("%s", exp.m_expStr);
		//�ض�
		exp.m_expStr[MAX_EXPRESSION_LENGTH - 2] = 0x00;

		//��������?
		nn = strlen(exp.m_expStr);
		if (nn < 1)
			break;
		if (exp.m_expStr[0] == '#')
			break;

		//���Ƿ��ַ�
		mm = strlen(g_validChars);
		for (ii = 0; ii < nn; ii++)
		{
			for (jj = 0; jj < mm; jj++)
			{
				if (exp.m_expStr[ii] == g_validChars[jj])
					break;
			}
			if (jj >= mm)
			{
				printf("illegal char in expression\n");
				break;
			}
		}
		if (ii < nn)
			continue;


		int L=ConvertExp(&exp, hzexp, nn); // L ��ֵ�������������    ConvertExpת�����ʽ

	
	
		//���ʽ��ֵ
		ret = ExpressionEvaluate(&exp,hzexp,L);
		if (ret != 100)
			printf("error = %d\n", ret);
		else
			printf("result = %lf\n", exp.m_value);
	} //1==1

	return 0;
}

///////////////////////////

