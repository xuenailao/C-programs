#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void std_presort(int n, int* data, int* result) {
	int* p= (int*)malloc(n*sizeof(*p));
	int length = 0;
	if (!p) {
		exit(0);
	}
	//malloc���ص����ڴ�ռ���׵�ַ����ַָ��ռ�Ϊvoid�ͣ�����ͬ�������͵ĵ�ַ�ֽڲ�һ��
	//ǰ������ţ�int*����ǿ������ת��,һ��Ҫ������
	// c����ת����c++����ת
	//����ַָ��Ŀռ�Ϊint���ͣ����ȡ��ַʱ��int��Ӧ�ķ�ʽ��ȡ��
	//û�иı������ַ�������Ǹ���ϵͳ��δ��������ַ
	//����int *p���������int�Ƕ�ָ��ָ��Ŀռ����͹涨��*�Ǹ��ݵ�ַ�ҵ�����ռ䣬p�Ǵ洢�ĵ�ַ��
	// ��Ȼp�����е�ַ��&p��֪��p�������������ݲ�ͬ�������洢���ǵ�ַ���������������*
	//���Զ���*����Ӧ�ú�int����p�󶨣�����ν����Ӱ�������ײ����С�

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

	printf("ǰ����������\n");
	for (int i = 0; i < 15; i++) {
		printf("%d ", b[i]);

	}

}