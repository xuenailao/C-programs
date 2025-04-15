#include <stdio.h>

/*判断栈是否为空*/
int isEmpty(int top)
{
	return top == -1 ? top : 0;
 } 
 
 /*入栈*/ 
 void push(char a[],char x, int *ptop)
 {
 	(*ptop)++;
 	a[*ptop]=x;
 }
 
 /*出栈操作*/ 
 void pop(int *ptop)
 {
 	(*ptop)--;
 }
 
 /*取出顶部数据*/
 
 void getTop(char a[],int itop,char *p)
 {
 	*p = a[itop];
 }
 
 
 void PostExp(char *exp,char *pexp)
 {
 	char stack[100];    /*储存运算符的栈*/ 
 	int top = -1;
 	char tmp;
 	while (1)
 	{
 		if (*exp>='0'&&*exp<='9')  /*当输入的表达式字符为数字的时候*/
 			*pexp++ = *exp++;
 		if (*exp=='(')              /*当输入的表达式字符为（的时候，压栈*/
 		push(stack,*exp,&top);
 		if(*exp == ')')             /*当输入的表达式字符为）的时候,回溯栈内的前一个符号*/
 		{
 			getTop(stack,top ,&tmp);   
 			pop(&top);
 		
 			while(tmp!='('){     /*如果前一个符号不是（，将弹出的算符追加进后缀表达式*/ 
 				*pexp++=tmp;
 				getTop(stack,top,&tmp);
 				pop(&top);
			}
		}
		if(*exp=='+'||*exp=='-')       /*当输入的表达式字符为 + 或者 - 的时候，不是（就出栈*/
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
		if(*exp == '*'||*exp=='/')   /*当输入的表达式字符为* 或者 / 时，先检查前面的是不是还有*和/，若有就先弹出*/ 
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
 
 /*将中缀表达式表达为后缀表达式*/
 
 
 /*计算后缀表达式*/ 
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

 
 /*主函数*/
void main()
{
	char exp[100],pexp[100]={0};
	scanf("%s",&exp);
	PostExp(exp,pexp);
	printf(*pexp);
	printf("\n %d \n",calc(pexp));
 } 
 
