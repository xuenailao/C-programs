#include<stdio.h>
#include<stdlib.h>

typedef struct node {
	int data;
	node* pn;
	node* ppre;
}node;

void ListSort(int a[],int n) {
	int temp = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < i; j++)
			if (a[j] > a[i]) {
				temp = a[j];
				a[j] = a[i];
				a[i] = temp;
			}
	printf("顺序表排序从小到大为：");
	for (int i = 0; i < n; i++)
		printf("%d ", a[i]);
}

void LinkSort(int a[],int n){
	node* hey;
	hey = (node*)malloc(sizeof(node));
	hey->pn = NULL;
	for (int i = 0; i < n; i++) {
		node* p;
		p = (node*)malloc(sizeof(node));
		p->data = a[i];
		p->pn = hey->pn;
		hey->pn = p;
	
	}//将数组填入链表,带头结点
	
	node* p, *q, *pre=NULL;
	p = hey->pn->pn;
	hey->pn->pn = NULL;
	while (p) {
		q = p->pn;
		pre = hey;
		while (pre->pn != NULL && pre->pn->data < p->data)
			pre = pre->pn;
		p->pn = pre->pn;
		pre->pn = p;
		p = q;
	}

	printf("链表从小到大为：");
	while (pre->pn != NULL) {
		printf("%d ", pre->pn->data);
		pre = pre->pn;
	}
		
}
	

int main() {
	int a[] = {10,1,2,4,1,3,2,9,3,4,7,6 };
	ListSort(a, 12);
	LinkSort(a, 12);
	
}
