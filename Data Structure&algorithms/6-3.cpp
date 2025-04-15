#include<stdio.h>
#include<stdlib.h>
#include<stack>

typedef char BTDataType;
typedef struct BTnode {
	BTDataType data;
	BTnode* left;
	BTnode* right;
	int rtag;//�Ƿ�Ϊ����
	int ltag;
}BTnode;

//��ѧ�����в���ģ���ѧ�����в����sdjaslkdjasjd nayoubufengde������������

//��α��������ܶ࣬�ؼ�����֪��ʲôʱ����������һ��Լ�����һ��ڵ��ַ�ı�������������ֵ����ת��������
// ��һ�ַ������������������һλ���롮#����ʾ��ֹ��
// �������ö��л��ߺ����ݹ�


//��ʽ��������������
void CreatBT(BTnode*& node, char* str, int n) {

	BTnode** theTree = (BTnode**)malloc(sizeof(BTnode*) * n);
	node = (BTnode*)malloc(sizeof(BTnode));
	//������n�� BTnode*�ڵ�Ŀռ�

	//�˴���ֵ�������õ�����*aָ�봫�������ǵ�ַ��ʵ�ʻ��Ǵ�ֵ����ַ����Ӧ�Ŀռ�������ݿ����޸�
	//���ǵ�ַa�������޸ģ�������ֻ���޸���ͬ������ʱ����a����������������١�
	//�����&a,���ַa�����޸ģ���Ӧ�ռ䲻�ᶪʧ��ʵ������ֱ�ӰѶ��󴫽���
	//���Ҫ��ָ�벢�޸ĵ�ַ,**a or *&a�� ǰ����Ȼ�����޸�a������*a��һ���ַ�����޸ģ�������������
	
	node->data = *str++;
	node->left = node->right = NULL;
	node->ltag = node->rtag = -1;//-1��������1������

	int front=0, rear = 0;
	theTree[0] = node;
	//Ϊʲôһ��Ҫ��һ��������node��ַ�أ�
	//��Ϊ�������������˲������򣬱��뱣�����

	while (rear < n) {
		if (++rear == n)
			break;
		//������һ���������
		BTnode* tmp = (BTnode*)malloc(sizeof(BTnode));//tmp��ת�ڵ�
		tmp->data = *str++;
		tmp->left = tmp->right = NULL;
		tmp->ltag = tmp->rtag = -1;
		theTree[front]->left = tmp;
		theTree[rear] = theTree[front]->left;

		if (++rear == n)
			break;

		tmp = (BTnode*)malloc(sizeof(BTnode));
		tmp->data = *str++;
		tmp->left = tmp->right = NULL;
		tmp->ltag = tmp->rtag = -1;

		theTree[front]->right = tmp;
		theTree[rear] = theTree[front]->right;

		front++;

	}

}

void Midsort(BTnode* BT) {
	if (BT!=NULL) {
		if(BT->left!=NULL)
			Midsort(BT->left);

		printf("%c ", BT->data);

		if(BT->right!=NULL)
			Midsort(BT->right);
	}
}

void Mid_Threadize(BTnode* BT) {
	BTnode* pre;
	pre = NULL;
	std::stack<BTnode*>s;

	while (1) {
		while (BT!=NULL) {
			s.push(BT);
			BT=BT->left;
		}//�����������ջ

		if (s.empty()) {
			return;
		}

		else{
			BT = s.top();
			s.pop();
	
			if (pre!=NULL) {
				if (pre->right == NULL) {
					pre->right = BT;
					pre->rtag = 1;
				}
					
				if (BT->left == NULL) {
					BT->left = pre;
					BT->ltag = 1;
				}
					
			}
		}
		pre = BT;
		BT = BT->right;
	}
}

BTnode* findnext(BTnode* BT) {//����������������ǰ����
	if (BT->ltag == -1 && BT->left!=NULL)
		return BT->left;//�ڵ�����ڵ�

	while (BT->rtag == 1 && BT->right!= NULL)
		BT = BT->right;
	return BT->right;
	}//���ڵ��������
	//�����������һ���ڵ㣬Ҳ��ǰ������һ���ڵ�

BTnode* findpre(BTnode* BT) {//�����������Һ���ǰ��
	if (BT-> rtag == -1 && BT->right != NULL) {
		return BT->right;
	}

	while (BT->ltag == 1 && BT->left != NULL)
		BT = BT->left;
	return BT->left;//���������ʼһ���ڵ㣬�Ǻ���ĵ�һ���ڵ�

	//������->��->��
	//������->��->��

}

void DestroyBT(BTnode*& BT) {
	if (&BT == NULL) {
		return;//�Ƿ���
	}
	if (BT != NULL) {
		printf("%c ", BT->data);
		if(BT->ltag==-1)
			DestroyBT(BT->left);
		if (BT->rtag == -1)
			DestroyBT(BT->right);

		free(BT);
		BT = NULL;
	}
	//free���ջط����p�Ŀռ䣬p��ΪҰָ�룬�ٸ�NULL����ָ��
	//�����free����һƬ�ռ����Զ���ڴ����䵫�޷�����״̬
}


int main() {
	char a[] = { 'a','b','c','d','e','f','g' };

	//a.������ʽ������
	BTnode* wow = NULL;
	CreatBT(wow,a, 7);

	//b.����������
	Midsort(wow);

	//c.����������
	Mid_Threadize(wow);

	//d.ǰ������ĺ�̽ڵ�
	BTnode* hello = wow;
	printf("\nǰ������ĺ�̽ڵ����Ϊ��");
	for (int i = 0; i < 6; i++) {
		printf("%c ->", hello->data);
		hello = findnext(hello);
		printf("%c ", hello->data);
	}

	//e.���������ǰ���ڵ�
	hello = wow;
	printf("\n���������ǰ���ڵ����Ϊ��");
	for (int i = 0; i < 6; i++) {
		printf("%c ->", hello->data);
		hello = findpre(hello);
		printf("%c ", hello->data);
	}
	printf("\n������ɾ���Ľڵ㣺\n");
	DestroyBT(wow);
	return 0;
}