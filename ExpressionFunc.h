
#ifndef __EXPRESSION_FUNC_H__
#define __EXPRESSION_FUNC_H__			1

//���ʽ����
#define MAX_EXPRESSION_LENGTH		256

//����������
#define ITEM_TYPE_NONE			0 //��
#define ITEM_TYPE_OPERATOR		1 //������
#define ITEM_TYPE_OPERAND		2 //������

//����������
#define OPERATOR_TYPE_NONE		0 //��
#define OPERATOR_TYPE_PLUS		1 //��
#define OPERATOR_TYPE_MINUS		2 //��
#define OPERATOR_TYPE_MULTIPLY	3 //��
#define OPERATOR_TYPE_DIVIDE	4 //��
#define OPERATOR_TYPE_NEGATIVE	5 //��
#define OPERATOR_TYPE_LEFTP		6 //������
#define OPERATOR_TYPE_RIGHTP	7 //������
#define OPERATOR_TYPE_END		8 //����

//��С����
#define SAMLL_NUMBER			0.000000001

/////////////////////////////

//���ʽ
struct SExpression
{
	//���ʽ�ַ���
	char			m_expStr[MAX_EXPRESSION_LENGTH];
	//ֵ
	double			m_value;


	//�����ǿ��ܵ������ֶ�

	//���ʽ����
//	int				m_HZtop;
	//���ʽ��ǰλ��
	int				m_curPos;

	//��������ջ
	int				m_operatorStack[MAX_EXPRESSION_LENGTH];
	int				m_operatorTop;
	//��������ջ
	double			m_operandStack[MAX_EXPRESSION_LENGTH];
	int				m_operandTop;
};


/*��׺���ʽ��ṹ��*/
struct HZExpression
{
	char exp_operator; //�����
	double exp_operand; //��ֵ
	int exp_item_type;  //���ͱ�� 
};


/*
���ܣ�
	��ʼ���������ʽ
������
	pExp���������ʽ
���أ�
	0����ȷ
	����������
*/
int ExpressionInit(struct SExpression *pExp);

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
int ExpressionEvaluate(struct SExpression *pExp, struct HZExpression *hzexp,int n);

/*ת����׺���ʽΪ��׺���ʽ*/
int ConvertExp(struct SExpression *pExp, struct HZExpression *hzexp, int expStrlen);

/*�����������ȼ�*/
int getPrioraty(char m_opreator);

/*����ֵ*/
double getValue(double a, double b, char c);
#endif

