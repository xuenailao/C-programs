#include<stdio.h>
#include<math.h>
void nthPassShellSort(int a[],int len) {
	//Hibbard增量序列
	int layb = 1, k = 2, flag = 0, time = 0;
	while (len / layb != 0) {
		layb = pow(2, ++k - 1)-1;
	}
	k--;

	for (k; k > 1; k--) {
		layb = pow(2, k - 1) - 1;
		for (int i = 0; i < len; i++) {
			for (int j = i; j +layb< len; j += layb) {
				if (a[i] > a[j + layb]) {
					int temp;
					temp = a[i];
					a[i] = a[j + layb];
					a[j + layb] = temp;

				}
			}
		}
		time++;
		printf("\n第%d次（增量为%d）的排序结果为(k=%d)：\n",time, layb, k);
	
		for (int i = 0; i < len; i++)
			printf("%d ", a[i]);
		printf("\n");
	}
}

int main() {
	int a[] = { 12,34325,213,4,5,335,23,1,21,2,2312,23,35,3,4543,23123,333 };
	nthPassShellSort(a,17);
	
}