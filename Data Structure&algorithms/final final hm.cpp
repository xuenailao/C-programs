#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
#include<string>
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
		if (p == NULL) {
			return 0;
		}
			
		else
		{
			p->bf = treeHeight(p->lchild) - treeHeight(p->rchild);
			return p->bf;
		}
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

	Node* Rebalance(Node*& p) {//ƽ��
		int factor = 0;
	
		if(p!=nullptr)
			factor = p->bf;

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
		return p;

	}
	//AVL���ѹ��ɣ���������ɼ��Ϲ���

	Node* emplace(Node*& p, const T& v) {
		Node* root = p;

		if (root == nullptr) {
			Node* newNode = new Node;
			if (newNode == nullptr)
				return nullptr;

 			newNode->value = v;

			root = newNode;
			if (p == nullptr) {
				p = newNode;
			}

			return newNode;
		}
		
		else {
			
			if (root->value == v) {
				return nullptr;
			}
			else {
				if (root->value < v)
					emplace(root->rchild, v);
				else
					emplace(root->lchild, v);
			}
			
		}
		Rebalance(p);
		return nullptr;
	}

	bool erase(Node*& p, const T& value) {
		Node* byebye;
		Node* root = p;

		if (root != nullptr) {
			if (root->value==value) {
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
				if (root->value<value) {
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
			return nullptr;

		Node* cur = p;
		while (cur) {
			if (cur->value>v)
				cur = cur->lchild;
			else if (cur->value<v)
				cur = cur->rchild;
			else
				return &(cur->value);
		}
		return nullptr;
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
class graphE {
	struct edge {
		int begin;//����±�
		int end;//�յ��±�
		int weight;

		edge() { begin = 0; end = 0; weight = 0; }

		bool operator == (const edge &e) const {
			if ((this->begin == e.begin) &&
				(this->end == e.end)) {
				return true;
			}
			else
				return false;
		}

		bool operator > (const edge &e) const {
			if (this->begin > e.begin) {
				return true;
			}
			else if ((this->begin == e.begin) &&
				(this->end > e.end)) {
				return true;
			}
			else
				return false;
		}

		bool operator < (const edge& e) const {

			if (this->begin < e.begin) {
				return true;
			}
			else if ((this->begin == e.begin) &&
				(this->end < e.end)) {
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

		vertex() { point = 0; };
		

		bool operator > (const vertex & p1) const {
			return point > p1.point;
		}

		bool operator < (const vertex& p1) const {
			return point < p1.point;
		}

		bool operator == (const vertex& p1) const {
			return point == p1.point;
		}
	};

	set<vertex> V;//�㼯
	set<edge> E;//�߼�

	int numV;//����
	int numE;//����


public:
	graphE() {
		V.p = nullptr;
		E.p = nullptr;
		numV = 0;
		numE = 0;

	}//��ʼ������

	void CreateGraph_Edge(graphE* g) {
		int i = 0, j = 0, k = 0;
		edge w{};
		vertex po{};
		
		printf("�����붥�����ͱ���:\n");
		scanf("%d,%d", &g->numV, &g->numE);
		getchar();//��ȡ�س���

		//������
		for (i = 1; i <= g->numV; i++) {
			po.point = i;
			g->V.emplace(g->V.p, po);
			
		}
	
		//�ڱ߾���
		for (k = 1; k <= g->numE; k++) {
			printf("���������%d����<vi,vj>���±�i,j��Ȩ��w:\n",k);
			scanf("%d,%d,%d", &w.begin, &w.end, &w.weight);
			getchar();

			g->E.emplace(g->E.p, w);
			//��Ϊ����ͼ������һ����Ҫ����ͷβ�ٴ���һ��
			int temp = w.begin;
			w.begin = w.end;
			w.end = temp;
			g->E.emplace(g->E.p, w);

		}


	}

	//չʾ�߼���ר��,����һ����������
	void showedge(setnode<edge>* root) {
		if (root != nullptr) {
			printf("(%d)->(%d)<%d>\n  ", root->value.begin, root->value.end, root->value.weight);
		}
		else
			return;

		showedge(root->lchild);
		showedge(root->rchild);
	}
	//չʾ�㼯��ר��,����һ����������
	void showvertex(setnode<vertex>* root) {
		if (root != nullptr) {
			printf("%d ",root->value.point);
		}
		else
			return;
		showvertex(root->lchild);
		showvertex(root->rchild);
	}

	void ShowGraph_Edge(graphE* g) {

		//չʾ�㼯��
		setnode<vertex>* rootv = g->V.p;
		printf("�㼯��:\n[");
		showvertex(rootv);
		printf("]\n");

		//չʾ�߼���
		setnode<edge>* roote = g->E.p;
		printf("�߼���:\n[");
		showedge(roote);
		printf("]\n");
	}

	//�������·��
	int shortestway(int start, int des, graphE* g) {
	//�������˵����·��(�Ͻ�˹�����㷨)
	//Ҫ����Ѱ��û�б����ʹ�����Դ������ĵ�
		
		const int inf = 100000;//���ɵ���ľ��룡
		int n = g->numV;//ͼ�Ķ������
		vector<int> dis(n, inf);//Դ�㵽���������̾��룬inf��ʾ�޷�����
		vector<bool> vis(n, false);//���ʼ�¼��false��ʾδ����
		vector<int> pre(n, -1);//��¼������ڼ���ʱ���ӵĵ㣨��Ҫ�ص���⣩
		edge e{};
		

		dis[start] = 0;//Դ�㵽Դ��ľ���Ϊ0

		for (int i = 0; i < n - 1; i++) {
			int node = -1;
			for (int j = 0; j < n; j++) {
				if (!vis[j] && (node == -1 || dis[j] < dis[node])) {
					node = j;
				}
			}
			 // ��Ƿ���

			for (int j = 0; j < n; j++) {
				e.begin = node+1;
				e.end = j+1; //end��begin���Ǵ�1��ʼ�������еĵ㶼�Ǵ�0��ʼ
				if (g->E.find(e) != nullptr) {
					if (dis[j] > dis[node] + g->E.find(e)->weight) {
						dis[j] = dis[node] + g->E.find(e)->weight;
						pre[j] = node;
					}
				}
			}

			vis[node] = true;

		}

		//��ӡ���·��
		printf("��%d��%d·��:\n",start+1,des+1);
		/*Ҫ������չʾ����A��B��·��չʾ��ǰ����Ҫ��AΪԴ�㣬һ�������Ƶ�B��
		Ȼ���ʱͨ��pre���л���
		չʾ�������������*/
		for (int i = des; i != start; i = pre[i]) {
			printf("%d -> ", i+1);
		}
		printf("%d,�ܾ���%d\n", start+1,dis[des]);

		return dis[des];
	}
};

//������
typedef struct Place {
	char name[10];//�������λ�õ�����
	int station[10];//���渽��վ��
	int length;//��¼վ������

	Place() { length = 0; }
}Place;

//·�߹滮����
void pathplanning(Place* place, graphE* ge) {
	int A = 0, B = 0;
	int p1 = 0, p2 = 0;
	printf("�����������յ�����:\n");
	scanf("%d,%d", &A, &B);
	int shortdis = 100000;
	for (int i = 0; i < place[A-1].length; i++) {
		for (int j = 0; j < place[B-1].length; j++) {
			int d = ge->shortestway(place[A-1].station[i]-1, place[B-1].station[j]-1, ge);
			if (shortdis > d) {
				shortdis = d;
				p1 = place[A - 1].station[i] - 1;
				p2 = place[B - 1].station[j] - 1;

			}
		}
	}
	printf("���·������:\n");
	ge->shortestway(p1, p2, ge);

}


/*�����ǵ��������*/
//�Ż��ѽṹ
class IndexHeap {
	struct Node {
		int* dist;//���㵽����������С����
		int* pos;//�����ڶ��е�λ��
	};

public:
	int* index;//�ѽڵ��Ӧ�Ķ�����
	Node* ass;
	int size;
	int count;

	IndexHeap(int maxsize) {
		int inf = 100000;//�޷�����
		ass = new Node;
		count = 0;
		size = maxsize;
		index = new int[maxsize + 1]();
		index[0] = 0;//�ڱ�
		ass->dist = new int[maxsize+1]();
		ass->pos = new int[maxsize+1]();
		for (int i = 0; i <= size; i++) {
			ass->dist[i] = inf;
		}


	}

	void SwapIndex(IndexHeap* heap, int i) {
		heap->ass->pos[heap->index[i]] = i;
	}//i�Ƕ����ڶ��е�λ�ã�index[i]�Ƕ�����

	//���붥����Ϊv�Ķ���,����һ��index
	void InsertIndex(IndexHeap* heap, int v) {
		int x = heap->ass->dist[v];
		int child;
		for (child = ++heap->count;
			x < heap->ass->dist[heap->index[child / 2]] &&child>0;
			child /= 2) {
			heap->index[child] = heap->index[child / 2];
			SwapIndex(heap, child);//child��v�����ڶ��е�λ��
		}
		heap->index[child] = v;
		SwapIndex(heap, child);
	}

	//ɾ����������С�ߵ�����
	int DeleteIndex(IndexHeap* heap) {
		int v = heap->index[1];
		int x = heap->ass->dist[heap->index[heap->count--]];
		int parent, child ;

		for (parent = 1; parent * 2 <= heap->count ; parent = child) {
			child = parent * 2;
			if (heap->ass->dist[heap->index[child + 1]] <
				heap->ass->dist[heap->index[child]] &&
				child != heap->count) {
				child++;
			}

			if (x > heap->ass->dist[heap->index[child]]) {
				heap->index[parent] = heap->index[child];
				SwapIndex(heap, parent);
			}
			else
				break;
		}
		heap->index[parent] = heap->index[heap->count + 1];
		SwapIndex(heap, parent);
		return v;
	}

	//�޸�dist[v]������x����������
	void Fixup(IndexHeap* heap, int v, int x) {
		heap->ass->dist[v] = x;
		int child;

		//ʹ��pos���ʶѶ�����v

		for (child = heap->ass->pos[v];
			x < heap->ass->dist[heap->index[child / 2]] &&child>0;
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
	typedef struct Enode {//�߱�ڵ�
		int adjvex;//�ڽӵ���
		int weight;//Ȩ��
		Enode* next;
	}Enode;

	typedef struct Vnode {//�����ڵ�
		int vertex;//������
		Enode* firstedge;//�߱�ͷָ��
	}Vnode;

public:
	int numV, numE;
	vector<Vnode> adjlist;

	//��ʼ������
	graphL(int V,int E) {
		numV = V;
		numE = E;
		adjlist.resize(numV);
		for (int i = 0; i < numV; i++) {
			adjlist[i].vertex = i+1;
			adjlist[i].firstedge = nullptr;
		}
	}

	void CreateGraph_AL(graphL* gl) {
		Enode* e{};
		int v1, v2, w;;
		for (int k = 0; k < gl->numE; k++) {
			printf("�����<vi,vj>�Ķ�����ź�Ȩ��:\n");
			scanf("%d,%d,%d", &v1, &v2, &w);
			getchar();

			e = (Enode*)malloc(sizeof(Enode));
			if (e == nullptr) {
				exit(0);
			}
			e->adjvex = v2;
			e->weight = w;
			e->next = gl->adjlist[v1 - 1].firstedge;
			gl->adjlist[v1 - 1].firstedge = e;
			
			//����һ��Ҫ��malloc���·��䣬
			//��Ȼ����һ���ĵ�ַ������ĵ�ַ��������
			e = (Enode*)malloc(sizeof(Enode));
			if (e == nullptr) {
				exit(0);
			}
			e->adjvex = v1;
			e->weight = w;
			e->next = gl->adjlist[v2 - 1].firstedge;
			gl->adjlist[v2 - 1].firstedge = e;
		}
	}

	void ShowGraph_AL(graphL* gl) {
		for (int i = 0; i < gl->numV; i++) {
			Enode* e = (Enode*)malloc(sizeof(Enode));
			if (e == nullptr) {
				exit(0);
			}
			e = gl->adjlist[i].firstedge;
			printf("����%d���ڽӱ�:\n%d->", i+1,i+1);
			while (e->next != nullptr) {
				printf("%d->", e->adjvex);
				e = e->next;
			}
			printf("%d\n", e->adjvex);
			
		}
	}

	//prim+�ѣ��������У�
	int prim_optim(graphL* gl, IndexHeap* heap, int start) {
	
		int sum = 0;
		heap->ass->dist[start] = 0;

		//��glͼ�еľ��뵼��heap��
		Enode* e = new Enode;
		e = gl->adjlist[start-1].firstedge;//�ҵ�ֱ�Ӻͳ��������ӵĵ�

		while (e != nullptr) {
			heap->ass->dist[e->adjvex] = e->weight;
			e = e->next;//�ȼ�������ֱ�����ӵĵ�
		}

		for (int i = 1; i <= gl->numV; i++) {
			heap->InsertIndex(heap, i);
		}

	

		printf("·������:\n");
		printf("%d", start);
		while (heap->count!=1) {//���Ҳ�������棬�±�Ϊ0

			//�˶δ���չʾ�˶ѵĽڵ㣬��Ҫʱ�����ã��鿴�ѽڵ�ı仯
			//for (int i = 1; i <= gl->numV; i++) {
				//if(heap->ass->dist[i]!=100000)
				//printf("%d,%d,%d\n", i, heap->ass->pos[i], heap->ass->dist[i]);
			//}
			
			int minIndex= heap->DeleteIndex(heap);
			int minDist = heap->ass->dist[minIndex];
			heap->ass->dist[minIndex] = 0;//�õ����������
			sum += minDist;
			

			//����dist����
			Enode* e = new Enode;
			e = gl->adjlist[minIndex-1].firstedge;

			while (e != nullptr) {
				int index = e->adjvex;
				if (heap->ass->dist[index] != 0 && heap->ass->dist[index] > e->weight){
					heap->Fixup(heap, index, e->weight);
					//�����dist��prim���������ľ��룬�͵Ͻ�˹������Դ����������
					//prim�ֻ��Ҫ�Ƚ��µĵ��������������ĳһ��ľ��룬�͵��������ľ���
					//�Ͻ�˹�������Ҫ�Ƚ��µĵ㵽���е㼯����ĳһ��ľ���+ĳ�㵽Դ��ľ��룬
					//��ĳ��ֱ�ӵ�Դ��ľ���
				}
				e = e->next;//��һ���ڵ�

			}
			

			printf("->%d(%d)", minIndex, minDist);
		}
		printf("\n��·������%d", sum);
		return sum;//��С��������·����
	}
};






int main() {
	//��һ��ʡ��

	//�ڶ���
	//���ñ߼����鱣��
	//��Ҫ�����ͼ��������
	/*
	30,38
1,2,120
14,15,186
30,33,192
12,7,252
7,6,195
27,28,268
27,17,308
28,8,53
8,6,132
6,1,206
3,4,127
4,5,270
8,9,233
5,9,134
9,17,77
9,26,96
9,30,303
17,18,124
18,29,390
18,19,410
30,10,175
26,10,290
26,25,322
25,24,459
19,11,219
11,24,137
10,11,115
19,20,86
24,23,521
22,23,320
22,21,252
21,20,320
20,16,460
16,15,580
13,28,288
12,13,70
2,4,173
2,3,133*/
	graphE* ge = new graphE;
	ge->CreateGraph_Edge(ge);
	//չʾ��ͼ
	ge->ShowGraph_Edge(ge);
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