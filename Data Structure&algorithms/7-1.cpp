#include<stdio.h>

void sift(int* a, int start, int end) {
	//堆数组的下标从1开始，因为0x2=0，无法下推
	//堆除了最后一层一定是一个完整二叉树,最后一层左边一定有叶子，右边可能没有

	//这里不能只比较堆的左右，画图可知
	int i = start, j = 2 * start;
	int tmp = a[start];
	while (j <= end) {
		if (j<end && a[j]<a[j+1]) {
			++j;
		}
		if (tmp < a[j]) {
			a[i] = a[j];
			i = j;
			j = 2 * i;
		}
		else
			break;
	}
	a[i] = tmp;
}

void HeapSort(int* a, int n) {
	int i, tmp;
	for (i = n / 2; i >= 1; --i)//i<=n/2才是根节点
		sift(a, i, n);
	//sift函数:使得上层节点均大于下一层

	for (i = n; i >= 2; --i) {
		tmp = a[1];
		a[1] = a[i];
		a[i] = tmp; // 将已经排完的最大节点放在最后不参与
		sift(a, 1, i - 1);
		//前面的开始调整
	}

}

int main() {

	int b[] = { 100,8,12,11,9,10,13,7,6,5,4,3,2,1,0 };
	HeapSort(b, 14);
	int i;
	for (i = 0; i < 14; ++i)
		printf("%d ", b[i + 1]);
}
