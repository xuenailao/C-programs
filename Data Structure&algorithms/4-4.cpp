#include<stdio.h>

//√∞≈›∑®
void BubbleSort(int a[],int n) {
	int i = 0, j = 0, temp = 0;
	for(i=0;i<n-1;i++)
		for(j=0;j<n-1;j++)
			if (a[j] > a[j + 1]) {
				temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
			}
	printf("√∞≈›≈≈–Ú∑®£∫");
	for (i = 0; i < n; i++)
		printf("%d ", a[i]);
}

// ·≈≈–Ú∑®
void CombSort(int a[], int n) {
	const double shrink = 1.3;
	int i = 0, delta = n, noswap = 0,temp=0;
	while (!noswap) {
		for (noswap = 1, i = 0; i + delta < n; i++)
			if (a[i] > a[i + delta]) {
				temp = a[i];
				a[i] = a[i + delta];
				a[i + delta] = temp;
				noswap = 0;
			}
		if (delta > 1) {
			delta /= shrink;
			noswap = 0;
		}
	}
	printf(" ·≈≈–Ú∑®£∫");
	for(i=0;i<n;i++)
		printf("%d ", a[i]);
}

int main() {
	int a[] = { 123,23,4,51,23,45,532,535,444,67 };
	BubbleSort(a, 10);
	CombSort(a, 10);
}