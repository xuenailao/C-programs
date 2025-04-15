#include<stdio.h>

void buildHeap(int* a, int now, int end) {
	if (now < 1)
		return;
	int i = now, lnum = 2 * now, rnum = 2 * now + 1;
	if (lnum <= end && a[lnum] > a[i]) {
		i = lnum;
	}
	if (rnum <= end && a[rnum] > a[i]) {
		i = rnum;
	}
	if (i != now) {
		int tmp = a[now];
		a[now] = a[i];
		a[i] = tmp;
		buildHeap(a, i, end);
	}

}

void heapSort(int* a, int n) {
	for (int i = n / 2; i > 0; --i)
		buildHeap(a, i, n);

	for (int i = n; i >= 2; --i) {
		int tmp = a[1];
		a[1] = a[i];
		a[i] = tmp;
		buildHeap(a, 1, i - 1);
	}
}

int main() {
	int wow[] = { 1000,6,8,7,9,0,1,3,2,4,5 };
	heapSort(wow, 10);
	for (int i = 1; i < 11; i++)
		printf("%d ", wow[i]);
}