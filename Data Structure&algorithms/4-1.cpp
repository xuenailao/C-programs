#include<stdio.h>
#include<stdlib.h>

typedef struct node {
	int data;
	node *pn;
	node *ppre;
}node;

int Insert(node *link, int pos, node* pele) {
	int i = 0;
	node *mov;
	mov = link;
	for (i = 1; i < pos-1; i++)
		mov = mov->pn;
	pele->pn = mov->pn;
	pele->ppre = mov;
	mov->pn = pele;

}

int Delete(node *link, int pos) {
	int i = 0;
	node* mov = link;//注意这里必须双指针，才能改变link的值，否则根据函数调参原理，不会改变输入的link值

	for (i = 1; i < pos; i++)
		mov = mov->pn;
	(mov->ppre)->pn = (mov->pn)->ppre;//直接将前一个节点和后一个节点连接
	
}

bool IsEmpty(node *link) {
	if (link->pn == NULL && link->ppre == NULL)
		return true;
	else
		return false;
}

bool FindValue(node *link, int value) {
	node* mov = link;
	while (mov->pn != NULL) {
		if (mov->data == value)
			return true;
		else 
			return false;
		mov = mov->pn;
	}
}

void DispList(node* link) {
	while (link->pn != NULL) {
		printf("%d", link->data);
		link = link->pn;
	}
}

void DeleteList(node* link) {
	node* p = link;
	while (p != NULL) {
		link = link->pn;
		free(p);
		p = link;
		
	 }
}

