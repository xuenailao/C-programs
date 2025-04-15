#include<stdio.h>
#include<stdlib.h>
#include<stack>

typedef char BTDataType;
typedef struct BTnode {
	BTDataType data;
	BTnode* left;
	BTnode* right;
	int rtag;//是否为线索
	int ltag;
}BTnode;

//大学生哪有不封的，大学生哪有不疯的sdjaslkdjasjd nayoubufengde哈哈哈哈哈哈

//层次遍历方法很多，关键在于知道什么时候遍历到最后一项，以及对上一层节点地址的保留（左子树赋值后需转右子树）
// 有一种方法是在输入数组最后一位加入‘#’表示终止符
// 还可以用队列或者函数递归


//链式创建二叉树方法
void CreatBT(BTnode*& node, char* str, int n) {

	BTnode** theTree = (BTnode**)malloc(sizeof(BTnode*) * n);
	node = (BTnode*)malloc(sizeof(BTnode));
	//申请了n个 BTnode*节点的空间

	//此处传值、传引用的区别，*a指针传进来的是地址，实质还是传值，地址所对应的空间里的数据可以修改
	//但是地址a本身不能修改，函数内只是修改了同名的临时变量a，函数结束后会销毁。
	//如果是&a,则地址a可以修改，对应空间不会丢失，实质上是直接把对象传进来
	//如果要用指针并修改地址,**a or *&a。 前者依然不能修改a，但是*a这一层地址可以修改，在这层操作即可
	
	node->data = *str++;
	node->left = node->right = NULL;
	node->ltag = node->rtag = -1;//-1是子树，1是线索

	int front=0, rear = 0;
	theTree[0] = node;
	//为什么一定要用一个外来的node地址呢？
	//因为下面树遍历完了不能逆序，必须保留起点

	while (rear < n) {
		if (++rear == n)
			break;
		//这里是一个链表队列
		BTnode* tmp = (BTnode*)malloc(sizeof(BTnode));//tmp中转节点
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
		}//左子树结点入栈

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

BTnode* findnext(BTnode* BT) {//在中序线索树中找前序后继
	if (BT->ltag == -1 && BT->left!=NULL)
		return BT->left;//节点有左节点

	while (BT->rtag == 1 && BT->right!= NULL)
		BT = BT->right;
	return BT->right;
	}//检查节点的右线索
	//子树中序最后一个节点，也是前序的最后一个节点

BTnode* findpre(BTnode* BT) {//中序线索树找后序前驱
	if (BT-> rtag == -1 && BT->right != NULL) {
		return BT->right;
	}

	while (BT->ltag == 1 && BT->left != NULL)
		BT = BT->left;
	return BT->left;//子树中序最开始一个节点，是后序的第一个节点

	//中序：左->中->右
	//后序：左->右->中

}

void DestroyBT(BTnode*& BT) {
	if (&BT == NULL) {
		return;//非法树
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
	//free是收回分配给p的空间，p沦为野指针，再给NULL消除指针
	//如果不free，这一片空间就永远处于待分配但无法分配状态
}


int main() {
	char a[] = { 'a','b','c','d','e','f','g' };

	//a.生成链式二叉树
	BTnode* wow = NULL;
	CreatBT(wow,a, 7);

	//b.中序遍历输出
	Midsort(wow);

	//c.中序线索化
	Mid_Threadize(wow);

	//d.前序遍历的后继节点
	BTnode* hello = wow;
	printf("\n前序遍历的后继节点输出为：");
	for (int i = 0; i < 6; i++) {
		printf("%c ->", hello->data);
		hello = findnext(hello);
		printf("%c ", hello->data);
	}

	//e.后序遍历的前驱节点
	hello = wow;
	printf("\n后序遍历的前驱节点输出为：");
	for (int i = 0; i < 6; i++) {
		printf("%c ->", hello->data);
		hello = findpre(hello);
		printf("%c ", hello->data);
	}
	printf("\n即将被删除的节点：\n");
	DestroyBT(wow);
	return 0;
}