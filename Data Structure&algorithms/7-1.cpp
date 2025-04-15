#include<stdio.h>

void sift(int* a, int start, int end) {
	//��������±��1��ʼ����Ϊ0x2=0���޷�����
	//�ѳ������һ��һ����һ������������,���һ�����һ����Ҷ�ӣ��ұ߿���û��

	//���ﲻ��ֻ�Ƚ϶ѵ����ң���ͼ��֪
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
	for (i = n / 2; i >= 1; --i)//i<=n/2���Ǹ��ڵ�
		sift(a, i, n);
	//sift����:ʹ���ϲ�ڵ��������һ��

	for (i = n; i >= 2; --i) {
		tmp = a[1];
		a[1] = a[i];
		a[i] = tmp; // ���Ѿ���������ڵ������󲻲���
		sift(a, 1, i - 1);
		//ǰ��Ŀ�ʼ����
	}

}

int main() {

	int b[] = { 100,8,12,11,9,10,13,7,6,5,4,3,2,1,0 };
	HeapSort(b, 14);
	int i;
	for (i = 0; i < 14; ++i)
		printf("%d ", b[i + 1]);
}
