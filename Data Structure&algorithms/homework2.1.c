#include<stdio.h>
#include<stdlib.h>

typedef int SElemtype;
typedef struct SeqStack {
	SElemtype* data;
	int maxlen;
	int top;
}Seqstack;

int push(char* a, int top, char elem) {
	a[++top] = elem;
	return top;
}

int pop(char* a, int top) {
    if (top == -1) {
        printf("¿ÕÕ»");
        return -1;
    }
    printf("µ¯Õ»ÔªËØ£º%c\n", a[top]);
    top--;
    return top;
}