#include<stdio.h>
#include<math.h>

//�ݹ�ǰ��
void re_presort(int n, char* data, char*result) {
	//data������洢
	//����+���������������� => ��ͼ
	//��������ͷ��->��ǰ��ͷ����
	int tmp;
	if (n > 0) {
		tmp = n / 2;//ͷ
		*result = data[tmp];
		re_presort(n / 2, data, result + 1);
		re_presort(n / 2, data + tmp + 1, result + tmp + 1);
	}

}

//ѭ��ǰ��
void cir_presort(int n, char* data, char* result) {
	int d = log(n + 1) / log(2);//���Ĳ���
	int flag, num, q, distance;

	for (int i = 0; i < d;i++) {    //��㴦��
		num = pow(2, i);//ÿ������
		q = pow(2, d - i) - 1;
		distance= (2 * (n / 2 - n / (2 * num)));
		flag = 0;

		result[i] = data[n/(2*num)];//����

;		for (int k = 1; k < num; k++) {

			while (result[i + flag + k * q] != 0) {
				flag++;
			}//�ѱ���䣬��ͼ��֪������ἷռ��һ��ĳ�ڵ��λ��
		
			printf("%d,%d\n", i+flag + k * q, n / (2 * num) + k * distance / (num - 1));//��֤����
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
	printf("�ݹ�ǰ������\n");
	for (int i = 0; i < 15; i++) {
		printf("%c", b[i]);

	}
	printf("\n");
	printf("ѭ��ǰ������\n");
	for (int i = 0; i < 15; i++) {
		printf("%c", c[i]);

	}
}