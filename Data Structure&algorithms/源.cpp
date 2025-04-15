
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

template<typename T>
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

template<typename T>//����Ƚ�ԭ��
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
	//AVL���ѹ��ɣ���������ɼ��Ϲ���

	template<typename Compare>
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

		else if (comp(root->value, v) == 0) {
			return nullptr;
		}
		else {
			if (comp(root->value, v) == -1)
				emplace(root->rchild, v, comp);
			else
				emplace(root->lchild, v, comp);
		}

		Rebalance(root);

		return newNode;
	}

	template<class T, typename Compare>
	bool erase(Node*& p, const T& value, Compare comp) {
		Node* byebye;
		Node* root = p;

		if (root != nullptr) {
			if (comp(root->value, value) == 0) {
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
				if (comp(root->value, value) == -1) {
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

	template<class T, typename Compare>
	T* find(const T& v, Compare comp) {
		if (p == nullptr)
			return NULL;

		Node* cur = p;
		while (cur) {
			if (comp(cur->value, v) == 1)
				cur = cur->lchild;
			else if (comp(cur->value, v) == -1)
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
			g->V.emplace(g->V.p, k, mycmp<int>);
		}
		getchar();

		//�ڱ߾���
		for (k = 0; k < g->numE; k++) {
			printf("���������<vi,vj>���±�i,j��Ȩ��w:\n");
			scanf("%d,%d,%d", &w.begin, &w.end, &w.weight);
			getchar();

			g->E.emplace(g->E.p, w, mycmp<edge>);

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
				dis[j] = min(dis[j], dis[node] + g->E.find(e, mycmp<edge>)->weight);
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


#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
#include<vector>
#pragma warning(disable:4996)
using namespace std;
/*�Ұ�c���ԣ��Ұ����ݽṹ����ѧ�����в�������в����*/


template<typename T>
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
//����setnode����parentָ����Ա������������ѭ��������࣬���Գ��ԸĽ�

template<typename T>//����Ƚ�ԭ��
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
	//AVL���ѹ��ɣ���������ɼ��Ϲ���

	Node* emplace(Node*& p, const T& v) {
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

	bool erase(Node*& p, const T& value) {
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

	T* find(const T& v) {
		if (p == nullptr)
			return NULL;

		Node* cur = p;
		while (cur) {
			if (cur->value > v)
				cur = cur->lchild;
			else if (cur->value < v)
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

//�߼����鹹��ͼ�����ϣ�����ɵڶ����ͼ����
//��set<T>��ʵ�����У���edge��vertex����ʵ�����ֱ��������������أ�
//����Ȼ����û��������������Ǻ�������˳�����⣿

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

	struct vertex {
		int point;

		bool operator > (const vertex*& p1) const {
			return this->point > p1->point;
		}
	};

	set<vertex> V;//�㼯
	set<edge> E;//�߼�

	int numV;//����
	int numE;//����

public:
	void CreateGraph_Edge(graphE* g) {
		int i = 0, j = 0, k = 0;
		edge w{};
		vertex po{};

		printf("�����붥�����ͱ���:\n");
		scanf("%d,%d", &g->numV, &g->numE);
		getchar();//��ȡ�س���

		//������
		printf("�����붥����Ϣ:\n");
		for (i = 0; i < g->numV; i++) {
			scanf("%d", &po.point);
			g->V.emplace(g->V.p, po);
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
				dis[j] = min(dis[j], dis[node] + g->E.find(e)->weight);
			}

		}
		return dis;//��Ƿ���

	}
};



/*�����ǵ������������*/
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



//������
	// ���������ָ���ص�
	//step1:�������Ϣ��ѡȡ�ص㸽��������ţ��������̵�·����
	/* 1�ţ���һ¥��10;
	   2�ţ��̶�¥��17��18
	   3�ţ�����¥��27
	   4�ţ�����¥��5��9
	   5�ţ���ѧԺ��19��20
	   6�ţ������ѧԺ��12��13��14
	   7�ţ�������ѧԺ��1
	   8�ţ���ѧѧԺ��6,7,8,28
	   9�ţ�����ѧԺ,25
	   10�ţ�����ѧԺ��22
	   11�ţ�����ѧԺ��11��19
	   12�ţ�������¥��10��11
	   13�ţ���ͼ��ݣ�29
	   14�ţ���ͼ��ݣ�5
	   15�ţ�׿�������ݣ�7��12*/
	   //���������ʽ
	   /*��һ¥
   1
   10
   �̶�¥
   2
   17
   18
   ����¥
   1
   27
   ����¥
   2
   5
   9
   ��ѧԺ
   2
   19
   20
   �����ѧԺ
   3
   12
   13
   14
   ������ѧԺ
   1
   1
   ��ѧѧԺ
   4
   6
   7
   8
   28
   ����ѧԺ
   1
   25
   ����ѧԺ
   1
   22
   ����ѧԺ
   2
   11
   19
   ������¥
   2
   10
   11
   ��ͼ���
   1
   29
   ��ͼ���
   1
   5
   ׿��������
   2
   7
   12
   */

   //step2:�洢�ڵ���Ϣ
Place place[15];
for (int i = 0; i < 15; i++) {
	printf("������%d�ŵص�:\n", i + 1);
	scanf("%s", place[i].name);
	getchar();
	printf("������õص��ٽ���վ�����:\n");
	int m = 0;
	scanf("%d", &m);

	for (int j = 0; j < m; j++) {
		printf("������վ��:\n");
		if (scanf("%d", &place[i].station[j]) == 1) {
			place[i].length++;
			getchar();
		}
		else
			break;
	}
}


//step3:Ѱ·
pathplanning(place, ge);

//������

graphL* gl = new graphL(30, 38);
gl->CreateGraph_AL(gl);
gl->ShowGraph_AL(gl);

IndexHeap* heap = new IndexHeap(30);
gl->prim_optim(gl, heap, 10);
}