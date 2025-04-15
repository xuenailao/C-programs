#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void std_presort(int n, int* data, int* result) {
	int* p= (int*)malloc(n*sizeof(*p));
	int length = 0;
	if (!p) {
		exit(0);
	}
	//malloc返回的是内存空间的首地址，地址指向空间为void型，但不同数据类型的地址字节不一样
	//前面的括号（int*）是强制类型转换,一定要加括号
	// c不用转，但c++必须转
	//即地址指向的空间为int类型，请读取地址时按int相应的方式读取。
	//没有改变这个地址本身，而是告诉系统如何处理这个地址
	//对于int *p这个声明，int是对指针指向的空间类型规定，*是根据地址找到这个空间，p是存储的地址，
	// 当然p本身有地址，&p可知，p和其他类型数据不同就是它存储的是地址，因此有特殊运算*
	//所以对于*到底应该和int还是p绑定，无所谓，不影响计算机底层运行。

	int k = 0;
	int left, right;

	int i = 0;
	while (1) {
		result[k++] = data[i];
		if (i == n - 1) {
			break;
		}

		left = 2 * i + 1;
		right = left + 1;

		if (right < n ) {
			p++;
			*p = right;
			length++;

			i = left;
		}
		else {
			if (length > 0) {
				i = *p;
				p--;
				length--;
			}
			
		}
	}
	free(p);
}

int main() {
	int a[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14 };
	int b[15] = { 0 };
	std_presort(15, a, b);

	printf("前序遍历结果：\n");
	for (int i = 0; i < 15; i++) {
		printf("%d ", b[i]);

	}

}