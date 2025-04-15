#include<stdio.h>
int IsEmpty(int top)
{
	return top == -1 ? top : 0;
}
void push(char a[], char x, int* ptop)
{
	(*ptop)++;
	a[*ptop] = x;
}
void pop(int* ptop)
{
	(*ptop)--;
}
void GetTop(char a[], int itop, char* p)
{
	*p = a[itop];
}

void Postexp(char* exp, char* pexp) {
	char stack[100];//ÖÐ×ªÕ»
	int top = -1;
	char tmp= '0';
	while (1) {
		if (*exp >= '0' && *exp <= '9') 
			*pexp++ = *exp++;
		if (*exp == '(')
			push(stack, *exp, &top);

		if (*exp == ')') {
			GetTop(stack, top, &tmp);
			pop(&top);
			while (tmp != '(') {
				GetTop(stack, top, &tmp);
				*pexp++ = tmp;
				pop(&top);
			}	
		}
		
		if (*exp == '+' || *exp == '-') {
			GetTop(stack, top, &tmp);
			while (tmp != '(') {
				*pexp++ = tmp;
				pop(&top);
				if (!IsEmpty(top)) {
					GetTop(stack, top, &tmp);
				}
				else
					break;
			}
			push(stack, *exp, &top);
		}
			

		if (*exp == '*' || *exp == '/') {
			if (!IsEmpty(top)) {
				GetTop(stack, top, &tmp);
				while (tmp == '*' || tmp == '/') {
					*pexp++ = tmp;
					pop(&top);
					if ((!IsEmpty(top)))
					{
						GetTop(stack, top, &tmp);
					}
					else
						break;
				}
			}
			push(stack, *exp, &top);
		}
		if (!*exp)
			break;
		exp++;
	}
	while (IsEmpty(top)) {
		GetTop(stack, top, pexp);
		pexp++;
		pop(&top);
	}
	*pexp=0;
}

int calc(char* pexp) {
	char stack[100] = { 0 };
	int top = -1;
	while (*pexp) {
		if (*pexp >= '0' && *pexp <= '9')
			push(stack, *pexp - '0', &top);
		if (*pexp == '+' || *pexp == '-' || *pexp == '*' || *pexp == '/') {
			char tmp1 = '0';
			char tmp2 = '0';
			GetTop(stack, top, &tmp1);
			pop(&top);
			GetTop(stack, top, &tmp2);
			pop(&top);
			switch (*pexp)
			{
			case '+': push(stack, tmp1 + tmp2, &top); break;
			case '-': push(stack, tmp2 - tmp1, &top); break;
			case '*': push(stack, tmp2 * tmp1, &top); break;
			case '/': push(stack, tmp2 / tmp1, &top); break;
			}
		}
		char result;
		GetTop(stack, top, &result);
		return result;

	}
}

int main()
{
	char exp[] = "3*(5-2)+7", pexp[100] = { 0 };
	Postexp(exp, pexp);
	printf(pexp);
	printf("\n %d \n", calc(pexp));
}
