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
	node* mov = link;//ע���������˫ָ�룬���ܸı�link��ֵ��������ݺ�������ԭ������ı������linkֵ

	for (i = 1; i < pos; i++)
		mov = mov->pn;
	(mov->ppre)->pn = (mov->pn)->ppre;//ֱ�ӽ�ǰһ���ڵ�ͺ�һ���ڵ�����
	
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

