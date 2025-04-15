

typedef struct BSTnode {
	ElemType data;
	struct BSTnode* lchild, * rchild;

}*BStree, BSTnode;

int treeHeight(BStree p) {//���㵱ǰ�ڵ����߶�
	if (p == NULL) {
		return 0;
	}
	else if (treeHeight(p->lchild) > treeHeight(p->rchild))
		return treeHeight(p->lchild) + 1;
	else
		return treeHeight(p->rchild) + 1;

}

int BalanceFactor(BStree p) {//���㲢��ֵ��ǰ�ڵ�ƽ������
	if (p == NULL)
		return 0;
	else
		return treeHeight(p->lchild) - treeHeight(p->rchild);
}

void R_Rotate(BStree* p) {//����
	BStree lc = (*p)->lchild;
	(*p)->lchild = lc->rchild;
	lc->rchild = *p;

	BalanceFactor(*p);
	BalanceFactor(lc);//���¼���ڵ�ƽ������

	*p = lc;
	//�����µ�ͷ�ڵ�

}

void L_Rotate(BStree* p) {//����
	BStree rc = (*p)->rchild;
	(*p)->rchild = rc->lchild;
	rc->lchild = *p;


	BalanceFactor(*p);
	BalanceFactor(rc);//���¼���ڵ�ƽ������

	*p = rc;
}

BStree Rebalance(BStree* p) {//ƽ��
	int factor = treeHeight(*p);
	//�������������
	if (factor > 1 && BalanceFactor((*p)->lchild) >= 0)
		//LL,��Ҫ1������
		L_Rotate(p);
	else if (factor > 1 && BalanceFactor((*p)->lchild) < 0) {
		//LR,1������+1������
		L_Rotate(p);
		R_Rotate(p);
	}
	else if (factor < -1 && BalanceFactor((*p)->rchild) <= 0) {
		//RR,1������
		R_Rotate(p);
	}
	else if (factor < -1 && BalanceFactor((*p)->rchild)>0) {
		//RL,1������+1������
		R_Rotate(p);
		L_Rotate(p);
	}
	//�Ѿ�ƽ�⣬����Ҫ����
	return *p;

}

BStree treeInsert(BStree* p, ElemType value) {
	BStree newNode;
	BStree root = *p;

	if (root != NULL) {
		newNode = (BStree)malloc(sizeof(BSTnode));
		if (newNode == NULL)
			return NULL;

		newNode->data = value;
		newNode->lchild = newNode->rchild = NULL;

		*p = newNode;
	}

	else if (root->data == value) {
		return NULL;
	}
	else {
		if (root->data < value)
			treeInsert(&root->rchild, value);
		else
			treeInsert(&root->lchild, value);
	}

	Rebalance(&root);

	return newNode;
}

bool treeDelete(BStree* p, ElemType value) {
	BStree* byebye;
	BStree root = *p;

	if (root != NULL) {
		if (root->data = value) {
			if (root->rchild) {
				treeDelete(&(root->rchild), value);
			}
			else {
				byebye = &root;
				*p = (*byebye)->lchild;
				free(byebye);
			}
		}
		else {
			if (root->data < value) {
				treeDelete(&(root->rchild), value);
			}
			else
				treeDelete(&(root->lchild), value);
		}
		Rebalance(&root);
		return true;
	}

	else
		return false;
}

BStree find(BStree* p, ElemType T) {
	if (*p == nullptr)
		return false;

	BStree cur = *p;
	while (cur) {
		if (T < cur->data)
			cur = cur->lchild;
		else if (T > cur->data)
			cur = cur->rchild;
		else
			return cur;
	}

}

int size(BStree* p, int count) {
	if (*p != NULL) {
		if ((*p)->lchild == NULL && (*p)->rchild == NULL) {
			count++;
		}
		size(&((*p)->lchild), count);
		size(&((*p)->rchild), count);
	}
	return count;
}

void clear(BStree* p) {
	if (p == nullptr)
		return;

	clear(p->lchild);
	clear(p->rchild);
	free(p);
	p = nullptr;

}


#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
#include<vector>
#pragma warning(disable:4996)

using namespace std;
/*�Ұ�c���ԣ��Ұ����ݽṹ����ѧ�����в�������в����*/

typedef struct setnode {
	setnode* lchild;
	setnode* rchild;

	int value;
	int bf;

	setnode(const int& value) :
		lchild(nullptr),
		rchild(nullptr),
		value(value),
		bf(0) {}

}setnode;
//����setnode����parentָ����Ա������������ѭ���������

class set {
	typedef setnode Node;

public:
	Node* p = nullptr;

	int treeHeight(Node* p) {//���㵱ǰ�ڵ����߶�
		if (p == NULL) {
			return 0;
		}
		else if (treeHeight(p->lchild) > treeHeight(p->rchild))
			return treeHeight(p->lchild) + 1;
		else
			return treeHeight(p->rchild) + 1;

	}

	int BalanceFactor(Node* p) {//���㲢��ֵ��ǰ�ڵ�ƽ������
		if (p == NULL)
			return 0;
		else
			return treeHeight(p->lchild) - treeHeight(p->rchild);
	}

	void R_Rotate(Node*& p) {//����
		Node* lc = p->lchild;
		p->lchild = lc->rchild;
		lc->rchild = p;

		BalanceFactor(p);
		BalanceFactor(lc);//���¼���ڵ�ƽ������

		p = lc;
		//�����µ�ͷ�ڵ�
	}

	void L_Rotate(Node*& p) {//����
		Node* rc = p->rchild;
		p->rchild = rc->lchild;
		rc->lchild = p;


		BalanceFactor(p);
		BalanceFactor(rc);//���¼���ڵ�ƽ������

		p = rc;
	}

	Node Rebalance(Node* p) {//ƽ��
		int factor = treeHeight(p);
		//�������������
		if (factor > 1 && BalanceFactor(p->lchild) >= 0)
			//LL,��Ҫ1������
			L_Rotate(p);
		else if (factor > 1 && BalanceFactor(p->lchild) < 0) {
			//LR,1������+1������
			L_Rotate(p);
			R_Rotate(p);
		}
		else if (factor < -1 && BalanceFactor(p->rchild) <= 0) {
			//RR,1������
			R_Rotate(p);
		}
		else if (factor < -1 && BalanceFactor(p->rchild)>0) {
			//RL,1������+1������
			R_Rotate(p);
			L_Rotate(p);
		}
		//�Ѿ�ƽ�⣬����Ҫ����
		return *p;

	}

	Node* emplace(const int& v) {
		Node newNode;
		Node root = *p;

		if (root != nullptr) {
			newNode = (Node)malloc(sizeof(Node));
			if (newNode == nullptr)
				return nullptr;

			newNode->value = v;
			newNode->lchild = newNode->rchild = nullptr;

			*p = newNode;
		}

		else if (root->value == v) {
			return nullptr;
		}
		else {
			if (root->value < v)
				emplace(&root->rchild, v);
			else
				emplace(&root->lchild, v);
		}

		Rebalance(&root);

		return &newNode;
	}

	bool erase(Node*& p, const int value) {
		Node* byebye;
		Node* root = p;

		if (root != nullptr) {
			if (root->value = value) {
				if (root->rchild) {
					erase(root->rchild, value);
				}
				else {
					byebye = &root;
					p = byebye->lchild;
					free(byebye);
				}
			}
			else {
				if (root->value < value) {
					erase(root->rchild, value);
				}
				else
					erase(root->lchild, value);
			}
			Rebalance(&root);
			return true;
		}

		else
			return false;
	}

	int* find(const int& v) {
		if (p == nullptr)
			return NULL;

		Node* cur = p;
		while (cur) {
			if (v < cur->value)
				cur = cur->lchild;
			else if (v > cur->value)
				cur = cur->rchild;
			else
				return &(cur->value);
		}
	}

	int size(Node* p, int count) {
		if (*p != NULL) {
			if ((*p)->lchild == NULL && (*p)->rchild == NULL) {
				count++;
			}
			size(&((*p)->lchild), count);
			size(&((*p)->rchild), count);
		}
		return count;
	}

	bool empty() {
		if (size() == 0)
			return true;
		else
			return false;
	}

	void clear() {
		if (*p == nullptr) {
			free(p);
			p = nullptr;
			return;
		}

		clear((*p)->lchild);
		clear((*p)->rchild);
		free(*p);
		*p = nullptr;

	}

};

//�߼����鹹��ͼ�����ϣ�
class graphE {
	struct edge {
		int begin;//����±�
		int end;//�յ��±�
		int weight;

		bool operator = (const edge* e) const {
			if ((this->begin == e->begin) &&
				(this->end == e->end)) {
				return true;
			}
		}

		bool operator > (const edge* e) const {
			if (this->begin > e->begin) {
				return true;
			}
			else if ((this->begin == e->begin) &&
				(this->end > e->end)) {
				return true;
			}
			else
				return false;
		}

		bool operator < (const edge* e) const {
			if (this->begin < e->begin) {
				return true;
			}
			else if ((this->begin == e->begin) &&
				(this->end < e->end)) {
				return true;
			}
			else
				return false;
		}

		//��Ҫ���edge��set�е��������⣬����operator
		//�������¶���ȴ�С�ķ�ʽ
	};

	set<int> V;//�㼯
	set<edge> E;//�߼�

	int numV;//����
	int numE;//����

public:

	void CreateGraph_Edge(graphE* g) {
		int i = 0, j = 0, k = 0;
		edge w{};

		printf("�����붥�����ͱ���:\n");
		scanf("%d,%d", &g->numV, &g->numE);
		getchar();//��ȡ�س���

		//������
		printf("�����붥����Ϣ:\n");
		for (i = 0; i < g->numV; i++) {
			scanf("%d", &k);
			g->V.emplace(k);
		}
		getchar();

		//�ڱ߾���
		for (k = 0; k < g->numE; k++) {
			printf("���������<vi,vj>���±�i,j��Ȩ��w:\n");
			scanf("%d,%d,%d", &w.begin, &w.end, &w.weight);
			getchar();

			g->E.emplace(w);

		}


	}

	//�������·��
	vector<int> shortestway(int start, int des, graphE* g) {
		//�������˵����·��(�Ͻ�˹�����㷨)
	//Ҫ����Ѱ��û�б����ʹ�����Դ������ĵ�

		const int inf = 10000;//���ɵ���ľ��룡
		int n = g->numV;//ͼ�Ķ������
		vector<int> dis(n, inf);//Դ�㵽���������̾��룬inf��ʾ�޷�����
		vector<int> vis(n, false);//���ʼ�¼��false��ʾδ����
		edge e{};
		e.begin = start;

		dis[start] = 0;//Դ�㵽Դ��ľ���Ϊ0

		for (int i = 0; i < n - 1; i++) {
			int node = -1;
			for (int j = 0; j < n; j++) {
				if (!vis[j] && (node == -1 || dis[j] < dis[node])) {
					node = j;
				}
				vis[j] = true;
			}
			for (int j = 0; j < n; j++) {
				e.end = j;
				dis[j] = min(dis[j], dis[node] + g->E.find(e)->weight);
			}

		}
		return dis;//��Ƿ���

	}
};

//�Ż��ѽṹ
class IndexHeap {
	struct Node {
		vector<int> dist;
		vector<int> reverse;
	};


public:
	int* index;
	Node* ass;
	int size;
	int capacity;

	void SwapIndex(IndexHeap* heap, int i) {
		heap->ass->reverse[heap->index[i]] = i;
	}

	IndexHeap* CreateHeap(int maxsize) {
		IndexHeap* heap = new IndexHeap;
		heap->size = 0;
		heap->capacity = maxsize;
		heap->index = new int[maxsize + 1]();
		heap->index[0] = 0;//�ڱ�
		return heap;

	}

	//���붥����Ϊv�Ķ���,����һ��index
	void InsertIndex(IndexHeap* heap, int v) {
		int x = heap->ass->dist[v];
		int child;
		for (child = ++heap->size;
			x < heap->ass->dist[heap->index[child / 2]];
			child /= 2) {
			heap->index[child] = heap->index[child / 2];
			SwapIndex(heap, child);
		}
		heap->index[child] = v;
		SwapIndex(heap, child);
	}

	//ɾ����������С�ߵ�����
	int DeleteIndex(IndexHeap* heap) {
		int v = heap->index[1];
		int x = heap->ass->dist[heap->index[heap->size--]];
		int parent = 0, child = 0;

		for (parent = 1; parent * 2 <= heap->size; parent = child) {
			child = parent * 2;
			if (heap->ass->dist[heap->index[child + 1]] <
				heap->ass->dist[heap->index[child]] &&
				child != heap->size) {
				child++;
			}

			if (x > heap->ass->dist[heap->index[child + 1]]) {
				heap->index[parent] = heap->index[child];
				SwapIndex(heap, parent);
			}
			else
				break;
		}
		heap->index[parent] = heap->index[heap->size + 1];
		SwapIndex(heap, parent);
		return v;
	}

	//�޸�dist[v]������x����������
	void Fixup(IndexHeap* heap, int v, int x) {
		heap->ass->dist[v] = x;
		int child;

		//ʹ��reverse���ʶѶ�����v
		for (child = heap->ass->reverse[v];
			x < heap->ass->dist[heap->index[child / 2]];
			child /= 2) {
			heap->index[child] = heap->index[child / 2];
			SwapIndex(heap, child);
		}
		heap->index[child] = v;
		SwapIndex(heap, child);
	}

};

//�ڽӱ���ͼ�����飩
class graphL {

	struct Enode {//�߱�ڵ�
		int adjvex;//�ڽӵ���
		int weight;//Ȩ��
		Enode* next;
	};

	typedef struct Vnode {//�����ڵ�
		int vertex;//������
		Enode* firstedge;//�߱�ͷָ��
	}Vnode;

	int numV, numE;
	vector<Vnode> adjlist;

public:
	void CreateGraph_AL(graphL* g, IndexHeap* heap) {
		int i, j, k;
		Enode* e;

		printf("���붥�����ͱ���:\n");
		scanf("%d,%d", &g->numV, &g->numE);

		printf("���붥����Ϣ");
		for (i = 0; i < g->numV; i++) {
			scanf("%d", &g->adjlist[i].vertex);
			g->adjlist[i].firstedge = NULL;//�߱���Ϊ�ձ�
		}

		for (k = 0; k < g->numE; k++) {
			e = (Enode*)malloc(sizeof(Enode));//����ռ䴢��߱�ڵ�

			printf("�����Vi,Vj�Ķ������:\n");
			scanf("%d,%d", &i, &j);

			e->adjvex = j;
			e->next = g->adjlist[j].firstedge;
			g->adjlist[j].firstedge = e;

			e->adjvex = i;
			e->next = g->adjlist[i].firstedge;
			g->adjlist[i].firstedge = e;

		}
	}

	//prim+�ѣ��������У�
	int prim_optim(graphL* g, IndexHeap* heap) {

		vector<bool> added(false, g->numE);//�Ƿ������������
		int count = 0;//ȷ������С����������
		int sum = 0;
		added[0] = true;//����Ѿ�����
		heap->ass->dist[0] = 0;//�������������

		while (count < heap->size) {
			int minDist = heap->ass->dist[1];
			int minIndex = heap->DeleteIndex(heap);

			if (minIndex == 0) {
				break;
			}

			added[minIndex] = true;
			count++;
			sum += minDist;
			printf("���붥��%d,�߳�:%d\n", minIndex + 1, minDist);

			//����dist����
			Enode* e = g->adjlist[minIndex].firstedge->next;
			while (e != NULL) {
				int index = e->adjvex;
				if (added[index] == false &&
					heap->ass->dist[index] > e->weight) {
					heap->Fixup(heap, index, e->weight);
				}
				e = e->next;//��һ���ڵ�

			}
		}
		return sum;//��С��������·����
	}


};

int main() {
	//��һ��ʡ��
	//�ڶ���
	//���ñ߼����鱣��
	graphE* ge{};
	ge->CreateGraph_Edge(ge);



	//������
	//step1:�������Ϣ��ѡȡ�ص㸽��������ţ��������̵�·����
	//1�ţ���һ¥��
}


/*�Ұ�c���ԣ��Ұ����ݽṹ����ѧ�����в�������в����*/

template <typename T>
int mycmp(const T& a, const T& b)
{
	if (a > b)
		return 1;
	if (a == b)
		return 0;
	if (a < b)
		return -1;
}


template<class T>
class setnode {
public:
	setnode<T>* lchild;
	setnode<T>* rchild;

	T value;
	int bf;

	setnode() {
		this->lchild = nullptr;
		this->rchild = nullptr;
		this->bf = 0;
	}

};
//����setnode����parentָ����Ա������������ѭ���������

template<class T, typename Compare>//����Ƚ�ԭ��
class set {
	typedef setnode<T> Node;

public:

	Node* p = nullptr;

	int treeHeight(Node* p) {//���㵱ǰ�ڵ����߶�
		if (p == NULL) {
			return 0;
		}
		else if (treeHeight(p->lchild) > treeHeight(p->rchild))
			return treeHeight(p->lchild) + 1;
		else
			return treeHeight(p->rchild) + 1;

	}

	int BalanceFactor(Node* p) {//���㲢��ֵ��ǰ�ڵ�ƽ������
		if (p == NULL)
			return 0;
		else
			return treeHeight(p->lchild) - treeHeight(p->rchild);
	}

	void R_Rotate(Node* p) {//����
		Node* lc = p->lchild;
		p->lchild = lc->rchild;
		lc->rchild = p;

		BalanceFactor(p);
		BalanceFactor(lc);//���¼���ڵ�ƽ������

		p = lc;
		//�����µ�ͷ�ڵ�
	}

	void L_Rotate(Node* p) {//����
		Node* rc = p->rchild;
		p->rchild = rc->lchild;
		rc->lchild = p;


		BalanceFactor(p);
		BalanceFactor(rc);//���¼���ڵ�ƽ������

		p = rc;
	}

	Node Rebalance(Node* p) {//ƽ��
		int factor = treeHeight(p);
		//�������������
		if (factor > 1 && BalanceFactor(p->lchild) >= 0)
			//LL,��Ҫ1������
			L_Rotate(p);
		else if (factor > 1 && BalanceFactor(p->lchild) < 0) {
			//LR,1������+1������
			L_Rotate(p);
			R_Rotate(p);
		}
		else if (factor < -1 && BalanceFactor(p->rchild) <= 0) {
			//RR,1������
			R_Rotate(p);
		}
		else if (factor < -1 && BalanceFactor(p->rchild)>0) {
			//RL,1������+1������
			R_Rotate(p);
			L_Rotate(p);
		}
		//�Ѿ�ƽ�⣬����Ҫ����
		return *p;

	}

	Node* emplace(Node*& p, const T& v, Compare comp) {
		Node* newNode;
		Node* root = p;

		if (root != nullptr) {
			newNode = (Node*)malloc(sizeof(Node));
			if (newNode == nullptr)
				return nullptr;

			newNode->value = v;
			newNode->lchild = newNode->rchild = nullptr;

			p = newNode;
		}

		else if (root->value == v) {
			return nullptr;
		}
		else {
			if (root->value < v)
				emplace(root->rchild, v);
			else
				emplace(root->lchild, v);
		}

		Rebalance(root);

		return newNode;
	}

	bool erase(Node*& p, const T& value��Compare comp) {
		Node* byebye;
		Node* root = p;

		if (root != nullptr) {
			if (root->value == value) {
				if (root->rchild) {
					erase(root->rchild, value);
				}
				else {
					byebye = &root;
					p = byebye->lchild;
					free(byebye);
				}
			}
			else {
				if (root->value < value) {
					erase(root->rchild, value);
				}
				else
					erase(root->lchild, value);
			}
			Rebalance(&root);
			return true;
		}

		else
			return false;
	}

	template<typename Compare>
	T* find(const T& v, Compare comp) {
		if (p == nullptr)
			return NULL;

		Node* cur = p;
		while (cur) {
			if (comp(cur->value, v))
				cur = cur->lchild;
			else if (v > cur->value)
				cur = cur->rchild;
			else
				return &(cur->value);
		}
	}

	int size(Node* p, int count) {
		if (*p != NULL) {
			if ((*p)->lchild == NULL && (*p)->rchild == NULL) {
				count++;
			}
			size(&((*p)->lchild), count);
			size(&((*p)->rchild), count);
		}
		return count;
	}

	bool empty() {
		if (size() == 0)
			return true;
		else
			return false;
	}

	void clear() {
		if (*p == nullptr) {
			free(p);
			p = nullptr;
			return;
		}

		clear((*p)->lchild);
		clear((*p)->rchild);
		free(*p);
		*p = nullptr;

	}

};

//�߼����鹹��ͼ�����ϣ�
class graphE {
	struct edge {
		int begin;//����±�
		int end;//�յ��±�
		int weight;

		bool operator = (const edge* e) const {
			if ((this->begin == e->begin) &&
				(this->end == e->end)) {
				return true;
			}
		}

		bool operator > (const edge* e) const {
			if (this->begin > e->begin) {
				return true;
			}
			else if ((this->begin == e->begin) &&
				(this->end > e->end)) {
				return true;
			}
			else
				return false;
		}

		bool operator < (const edge* e) const {
			if (this->begin < e->begin) {
				return true;
			}
			else if ((this->begin == e->begin) &&
				(this->end < e->end)) {
				return true;
			}
			else
				return false;
		}

		//��Ҫ���edge��set�е��������⣬����operator
		//�������¶���ȴ�С�ķ�ʽ
	};

	set<int> V;//�㼯
	set<edge> E;//�߼�

	int numV;//����
	int numE;//����

public:
	void CreateGraph_Edge(graphE* g) {
		int i = 0, j = 0, k = 0;
		edge w{};

		printf("�����붥�����ͱ���:\n");
		scanf("%d,%d", &g->numV, &g->numE);
		getchar();//��ȡ�س���

		//������
		printf("�����붥����Ϣ:\n");
		for (i = 0; i < g->numV; i++) {
			scanf("%d", &k);
			g->V.emplace(g->V.p, k);
		}
		getchar();

		//�ڱ߾���
		for (k = 0; k < g->numE; k++) {
			printf("���������<vi,vj>���±�i,j��Ȩ��w:\n");
			scanf("%d,%d,%d", &w.begin, &w.end, &w.weight);
			getchar();

			g->E.emplace(g->E.p, w);

		}


	}

	//�������·��
	vector<int> shortestway(int start, int des, graphE* g) {
		//�������˵����·��(�Ͻ�˹�����㷨)
	//Ҫ����Ѱ��û�б����ʹ�����Դ������ĵ�

		const int inf = 10000;//���ɵ���ľ��룡
		int n = g->numV;//ͼ�Ķ������
		vector<int> dis(n, inf);//Դ�㵽���������̾��룬inf��ʾ�޷�����
		vector<int> vis(n, false);//���ʼ�¼��false��ʾδ����
		edge e{};
		e.begin = start;

		dis[start] = 0;//Դ�㵽Դ��ľ���Ϊ0

		for (int i = 0; i < n - 1; i++) {
			int node = -1;
			for (int j = 0; j < n; j++) {
				if (!vis[j] && (node == -1 || dis[j] < dis[node])) {
					node = j;
				}
				vis[j] = true;
			}
			for (int j = 0; j < n; j++) {
				e.end = j;
				dis[j] = min(dis[j], dis[node] + g->E.find(e, myCompare<edge>)->weight);
			}

		}
		return dis;//��Ƿ���

	}
};

//�Ż��ѽṹ
class IndexHeap {
	struct Node {
		vector<int> dist;
		vector<int> reverse;
	};


public:
	int* index;
	Node* ass;
	int size;
	int capacity;

	void SwapIndex(IndexHeap* heap, int i) {
		heap->ass->reverse[heap->index[i]] = i;
	}

	IndexHeap* CreateHeap(int maxsize) {
		IndexHeap* heap = new IndexHeap;
		heap->size = 0;
		heap->capacity = maxsize;
		heap->index = new int[maxsize + 1]();
		heap->index[0] = 0;//�ڱ�
		return heap;

	}

	//���붥����Ϊv�Ķ���,����һ��index
	void InsertIndex(IndexHeap* heap, int v) {
		int x = heap->ass->dist[v];
		int child;
		for (child = ++heap->size;
			x < heap->ass->dist[heap->index[child / 2]];
			child /= 2) {
			heap->index[child] = heap->index[child / 2];
			SwapIndex(heap, child);
		}
		heap->index[child] = v;
		SwapIndex(heap, child);
	}

	//ɾ����������С�ߵ�����
	int DeleteIndex(IndexHeap* heap) {
		int v = heap->index[1];
		int x = heap->ass->dist[heap->index[heap->size--]];
		int parent = 0, child = 0;

		for (parent = 1; parent * 2 <= heap->size; parent = child) {
			child = parent * 2;
			if (heap->ass->dist[heap->index[child + 1]] <
				heap->ass->dist[heap->index[child]] &&
				child != heap->size) {
				child++;
			}

			if (x > heap->ass->dist[heap->index[child + 1]]) {
				heap->index[parent] = heap->index[child];
				SwapIndex(heap, parent);
			}
			else
				break;
		}
		heap->index[parent] = heap->index[heap->size + 1];
		SwapIndex(heap, parent);
		return v;
	}

	//�޸�dist[v]������x����������
	void Fixup(IndexHeap* heap, int v, int x) {
		heap->ass->dist[v] = x;
		int child;

		//ʹ��reverse���ʶѶ�����v
		for (child = heap->ass->reverse[v];
			x < heap->ass->dist[heap->index[child / 2]];
			child /= 2) {
			heap->index[child] = heap->index[child / 2];
			SwapIndex(heap, child);
		}
		heap->index[child] = v;
		SwapIndex(heap, child);
	}

};

//�ڽӱ���ͼ�����飩
class graphL {

	struct Enode {//�߱�ڵ�
		int adjvex;//�ڽӵ���
		int weight;//Ȩ��
		Enode* next;
	};

	typedef struct Vnode {//�����ڵ�
		int vertex;//������
		Enode* firstedge;//�߱�ͷָ��
	}Vnode;

	int numV, numE;
	vector<Vnode> adjlist;

public:
	void CreateGraph_AL(graphL* g, IndexHeap* heap) {
		int i, j, k;
		Enode* e;

		printf("���붥�����ͱ���:\n");
		scanf("%d,%d", &g->numV, &g->numE);

		printf("���붥����Ϣ");
		for (i = 0; i < g->numV; i++) {
			scanf("%d", &g->adjlist[i].vertex);
			g->adjlist[i].firstedge = NULL;//�߱���Ϊ�ձ�
		}

		for (k = 0; k < g->numE; k++) {
			e = (Enode*)malloc(sizeof(Enode));//����ռ䴢��߱�ڵ�

			printf("�����Vi,Vj�Ķ������:\n");
			scanf("%d,%d", &i, &j);

			e->adjvex = j;
			e->next = g->adjlist[j].firstedge;
			g->adjlist[j].firstedge = e;

			e->adjvex = i;
			e->next = g->adjlist[i].firstedge;
			g->adjlist[i].firstedge = e;

		}
	}

	//prim+�ѣ��������У�
	int prim_optim(graphL* g, IndexHeap* heap) {

		vector<bool> added(false, g->numE);//�Ƿ������������
		int count = 0;//ȷ������С����������
		int sum = 0;
		added[0] = true;//����Ѿ�����
		heap->ass->dist[0] = 0;//�������������

		while (count < heap->size) {
			int minDist = heap->ass->dist[1];
			int minIndex = heap->DeleteIndex(heap);

			if (minIndex == 0) {
				break;
			}

			added[minIndex] = true;
			count++;
			sum += minDist;
			printf("���붥��%d,�߳�:%d\n", minIndex + 1, minDist);

			//����dist����
			Enode* e = g->adjlist[minIndex].firstedge->next;
			while (e != NULL) {
				int index = e->adjvex;
				if (added[index] == false &&
					heap->ass->dist[index] > e->weight) {
					heap->Fixup(heap, index, e->weight);
				}
				e = e->next;//��һ���ڵ�

			}
		}
		return sum;//��С��������·����
	}


};

int main() {
	//��һ��ʡ��
	//�ڶ���
	//���ñ߼����鱣��
	//graphE* ge{};
	//ge->CreateGraph_Edge(ge);



	//������
	//step1:�������Ϣ��ѡȡ�ص㸽��������ţ��������̵�·����
	//1�ţ���һ¥��
}