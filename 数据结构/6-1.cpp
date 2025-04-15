#include<stdio.h>
#include<math.h>

//递归前序
void re_presort(int n, char* data, char*result) {
	//data按中序存储
	//中序+完美二叉树的条件 => 树图
	//（中序）左头右->（前序）头左右
	int tmp;
	if (n > 0) {
		tmp = n / 2;//头
		*result = data[tmp];
		re_presort(n / 2, data, result + 1);
		re_presort(n / 2, data + tmp + 1, result + tmp + 1);
	}

}

//循环前序
void cir_presort(int n, char* data, char* result) {
	int d = log(n + 1) / log(2);//树的层数
	int flag, num, q, distance;

	for (int i = 0; i < d;i++) {    //逐层处理
		num = pow(2, i);//每层结点数
		q = pow(2, d - i) - 1;
		distance= (2 * (n / 2 - n / (2 * num)));
		flag = 0;

		result[i] = data[n/(2*num)];//行首

;		for (int k = 1; k < num; k++) {

			while (result[i + flag + k * q] != 0) {
				flag++;
			}//已被填充，画图就知道这里会挤占上一层某节点的位置
		
			printf("%d,%d\n", i+flag + k * q, n / (2 * num) + k * distance / (num - 1));//验证代码
			result[i + flag + k * q] = data[n / (2 * num) + k * distance / (num - 1)];
		}
		
	}
}

int main() {
	char a[] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o'};
	char b[15] = { 0 };
	re_presort(15, a, b);
	char c[15] = { 0 };
	cir_presort(15, a, c);
	printf("递归前序结果：\n");
	for (int i = 0; i < 15; i++) {
		printf("%c", b[i]);

	}
	printf("\n");
	printf("循环前序结果：\n");
	for (int i = 0; i < 15; i++) {
		printf("%c", c[i]);

	}
}