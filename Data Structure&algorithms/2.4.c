#include <stdio.h>

/*�ж�ջ�Ƿ�Ϊ��*/
int isEmpty(int top)
{
	return top == -1 ? top : 0;
 } 
 
 /*��ջ*/ 
 void push(char a[],char x, int *ptop)
 {
 	(*ptop)++;
 	a[*ptop]=x;
 }
 
 /*��ջ����*/ 
 void pop(int *ptop)
 {
 	(*ptop)--;
 }
 
 /*ȡ����������*/
 
 void getTop(char a[],int itop,char *p)
 {
 	*p = a[itop];
 }
 
 
 void PostExp(char *exp,char *pexp)
 {
 	char stack[100];    /*�����������ջ*/ 
 	int top = -1;
 	char tmp;
 	while (1)
 	{
 		if (*exp>='0'&&*exp<='9')  /*������ı��ʽ�ַ�Ϊ���ֵ�ʱ��*/
 			*pexp++ = *exp++;
 		if (*exp=='(')              /*������ı��ʽ�ַ�Ϊ����ʱ��ѹջ*/
 		push(stack,*exp,&top);
 		if(*exp == ')')             /*������ı��ʽ�ַ�Ϊ����ʱ��,����ջ�ڵ�ǰһ������*/
 		{
 			getTop(stack,top ,&tmp);   
 			pop(&top);
 		
 			while(tmp!='('){     /*���ǰһ�����Ų��ǣ��������������׷�ӽ���׺���ʽ*/ 
 				*pexp++=tmp;
 				getTop(stack,top,&tmp);
 				pop(&top);
			}
		}
		if(*exp=='+'||*exp=='-')       /*������ı��ʽ�ַ�Ϊ + ���� - ��ʱ�򣬲��ǣ��ͳ�ջ*/
		{
			if(!isEmpty(top))
			{
				getTop(stack,top,&tmp);
				while (tmp!='(')
				{
					*pexp ++= tmp;
					pop(&top);
					if(!isEmpty(top))
					{
						getTop(stack,top,&tmp);
					}else
					break;
				}
			}
			push(stack,*exp,&top);
		}
		if(*exp == '*'||*exp=='/')   /*������ı��ʽ�ַ�Ϊ* ���� / ʱ���ȼ��ǰ����ǲ��ǻ���*��/�����о��ȵ���*/ 
		{
			if(!isEmpty(top))
			{
				getTop(stack,top,&tmp);
				while(tmp == '*'||tmp=='/'){
					*pexp++=tmp;
					pop(&top);
					if(!isEmpty(top))
					{
						getTop(stack,top,&tmp);
					 } else
					 break;
				}
			}
			push(stack,*exp,&top);
		}
		if (!*exp)
			break;

		exp++;
	 }
	 while (!isEmpty(top))
	{
		getTop(stack, top, pexp);
		pexp++;
		pop(&top);
	}
	*pexp = 0;
  } 
 
 /*����׺���ʽ���Ϊ��׺���ʽ*/
 
 
 /*�����׺���ʽ*/ 
 int calculate(char a,char c,char b)
{
	if (c == '+')
		return a + b;
	if (c == '-')
		return b - a;
	if (c == '/')
		return b / a;
	if (c == '*')
		return a * b;
}

int calc(char* pexp)
{
	char stack[100] = { 0 };
	int top = -1;
	while (*pexp)
	{
		if (*pexp >= '0' && *pexp <= '9')
			push(stack, *pexp - '0', &top);
		else
		{
			char tmp1, tmp2;
			getTop(stack, top, &tmp1);
			pop(&top);
			getTop(stack, top, &tmp2);
			pop(&top);
			push(stack,calculate(tmp1,*pexp,tmp2), &top);
		}
		pexp++;
	}
	char v;
	getTop(stack, top, &v);
	return v;
}

 
 /*������*/
void main()
{
	char exp[100],pexp[100]={0};
	scanf("%s",&exp);
	PostExp(exp,pexp);
	printf(*pexp);
	printf("\n %d \n",calc(pexp));
 } 
 
