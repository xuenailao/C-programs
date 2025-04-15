#include<stdio.h>
#include<stdlib.h>

//ÿ�λ�׼ֵ��λ��������
int each_sort(int* a, int n, int start, int end) {
	int base = a[start];
	int end0 = end;
	int start0 = start;
	int temp = 0;

	if (start > end) {
		printf("��������ȷ���˽�β");
		exit(0);
	}
		

	while (start0 < end0) {
		if (base > a[end0]) {
			while (start0 < end0) {
				if (base < a[start0]) {
					break;
				}
				else
					start0++;
			}

			if (start0 < end0) {
				temp = a[start0];
				a[start0] = a[end0];
				a[end0] = temp;
				end0--;
				start0++;
			}

		}
		else
			end0--;
	}
	a[start] = a[start0];
	a[start0] = base;//��׼ֵ������ȷ�ĵط�

	printf("\n%dԪ����λʱ������Ϊ��\n", base);
	for (int i = 0; i < n; i++)
		printf("%d ", a[i]);

	return start0;
}


//�ǵݹ飬����ջ
void QuickSortNonRec(int *a,int n) {
	
	typedef struct node {
		int r;
		int l;
	}node;
	typedef struct stack {
		node data[10];
		int top;
	}stack;

	//����malloc�ռ�
	stack* s;
	s = (stack*)malloc(sizeof(stack));
	if (s) {
		s->top = -1;
	}//��ʼ��
	else
		exit(0);

	int start = 0, end = n - 1, record = 0;
	while (1) {
		record = each_sort(a, n, start, end);
		printf("nonRec");
		
		if (start < record - 1) {
			s->top++;
			s->data[s->top].l = start;
			s->data[s->top].r = record - 1;
		}
		
		if (end > record + 1) {
			s->top++;
			s->data[s->top].l = record + 1;
			s->data[s->top].r = end;

		}
		if (s->top != -1) {
			start = s->data[s->top].l;
			end = s->data[s->top].r;
			s->top--;
		}
		else
			break;
		
	}
	
	free(s);
}
	

//�ݹ����
void QuickSort(int* a, int n,int start,int end) {
	//ʵ���Ͼ��Ǹ�����each_sort�Ĵ���
	if (start >= end)
		return;

	int record;
	record = each_sort(a, n, start, end);
	printf("rec");
	QuickSort(a, n, start, record - 1);
	QuickSort(a, n, record + 1, end);
	
}

int main(){
	int a[10] = { 6,8,7,9,0,1,3,2,4,5 };
	int b[10] = { 6,8,7,9,0,1,3,2,4,5 };
	QuickSort(a, 10, 0, 9);//�ݹ�
	QuickSortNonRec(b, 10);

}