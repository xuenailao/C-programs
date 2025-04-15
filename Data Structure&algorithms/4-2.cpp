#include<stdio.h>
#include<stdlib.h>
typedef struct node {
	int data;
	node* pn;
}node;

typedef struct Queue {
	int size;
	node* front;
	node* rear;
}Queue;

void enQueue(Queue *queue, int newdata) {
	node* hey;
	hey=(node*)malloc(sizeof(node));
	if (hey) {
		exit(0);
	}
	else {
		hey->data = newdata;
		hey->pn = NULL;
	}
	

	if (queue->size == 0)
		queue->front = queue->rear = hey;
	else
		queue->rear = queue->rear->pn = hey;

	queue->size++;

}

int deQueue(Queue* queue,int output) {
	if (queue->size == 0) {
		printf("╤сапря©у");
		exit(0);
	}
	output = queue->front->data;
	queue->front = queue->front->pn;
	queue->size--;
	return output;
}

int GetHead(Queue* queue, int output) {
	if (queue->size != 0)
		return queue->front->data;
	else
		exit(0);
}

bool IsEmpty(Queue* queue) {
	if (queue->size == 0)
		return true;
	else
		return false;
}

void DelQueue(Queue* queue) {

	node* p = queue->front;

	while (queue->front != NULL) {
		queue->front = queue->front->pn;
		free(p);
		p = queue->front;
	}

	queue->size = 0;
	queue->front = queue->rear = NULL;
}