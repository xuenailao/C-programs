
/*我爱c语言，我爱数据结构，大学生哪有不疯的哪有不疯的*/

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
//这里setnode加入parent指针可以避免后续函数内循环，更简洁

template<typename T>//加入比较原则
class set {
	typedef setnode<T> Node;

public:

	Node* p = nullptr;

	int treeHeight(Node* p) {//计算当前节点树高度
		if (p == NULL) {
			return 0;
		}
		else if (treeHeight(p->lchild) > treeHeight(p->rchild))
			return treeHeight(p->lchild) + 1;
		else
			return treeHeight(p->rchild) + 1;

	}

	int BalanceFactor(Node* p) {//计算并赋值当前节点平衡因子
		if (p == NULL)
			return 0;
		else
			return treeHeight(p->lchild) - treeHeight(p->rchild);
	}

	void R_Rotate(Node* p) {//右旋
		Node* lc = p->lchild;
		p->lchild = lc->rchild;
		lc->rchild = p;

		BalanceFactor(p);
		BalanceFactor(lc);//重新计算节点平衡因子

		p = lc;
		//调整新的头节点
	}

	void L_Rotate(Node* p) {//左旋
		Node* rc = p->rchild;
		p->rchild = rc->lchild;
		rc->lchild = p;


		BalanceFactor(p);
		BalanceFactor(rc);//重新计算节点平衡因子

		p = rc;
	}

	Node Rebalance(Node* p) {//平衡
		int factor = treeHeight(p);
		//分四种情况讨论
		if (factor > 1 && BalanceFactor(p->lchild) >= 0)
			//LL,需要1次左旋
			L_Rotate(p);
		else if (factor > 1 && BalanceFactor(p->lchild) < 0) {
			//LR,1次左旋+1次右旋
			L_Rotate(p);
			R_Rotate(p);
		}
		else if (factor < -1 && BalanceFactor(p->rchild) <= 0) {
			//RR,1次右旋
			R_Rotate(p);
		}
		else if (factor < -1 && BalanceFactor(p->rchild)>0) {
			//RL,1次右旋+1次左旋
			R_Rotate(p);
			L_Rotate(p);
		}
		//已经平衡，不需要操作
		return *p;

	}
	//AVL树已构成，接下来完成集合构建

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

//边集数组构建图（集合）
class graphE {
	struct edge {
		int begin;//起点下标
		int end;//终点下标
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

		//需要解决edge在set中的排序问题，重载operator
		//就是重新定义比大小的方式
	};

	set<int> V;//点集
	set<edge> E;//边集

	int numV;//点数
	int numE;//边数

public:
	void CreateGraph_Edge(graphE* g) {
		int i = 0, j = 0, k = 0;
		edge w{};

		printf("请输入顶点数和边数:\n");
		scanf("%d,%d", &g->numV, &g->numE);
		getchar();//获取回车符

		//点数组
		printf("请输入顶点信息:\n");
		for (i = 0; i < g->numV; i++) {
			scanf("%d", &k);
			g->V.emplace(g->V.p, k, mycmp<int>);
		}
		getchar();

		//邻边矩阵
		for (k = 0; k < g->numE; k++) {
			printf("依次输入边<vi,vj>的下标i,j和权重w:\n");
			scanf("%d,%d,%d", &w.begin, &w.end, &w.weight);
			getchar();

			g->E.emplace(g->E.p, w, mycmp<edge>);

		}


	}

	//两点最短路径
	vector<int> shortestway(int start, int des, graphE* g) {
		//计算两端点最短路径(迪杰斯特拉算法)
	//要不断寻找没有被访问过且离源点最近的点

		const int inf = 10000;//不可到达的距离！
		int n = g->numV;//图的顶点个数
		vector<int> dis(n, inf);//源点到其他点的最短距离，inf表示无法到达
		vector<int> vis(n, false);//访问记录，false表示未访问
		edge e{};
		e.begin = start;

		dis[start] = 0;//源点到源点的距离为0

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
		return dis;//标记访问

	}
};

//优化堆结构
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
		heap->index[0] = 0;//哨兵
		return heap;

	}

	//插入顶点编号为v的顶点,插入一个index
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

	//删除并返回最小边的索引
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

	//修改dist[v]减少至x，并调整堆
	void Fixup(IndexHeap* heap, int v, int x) {
		heap->ass->dist[v] = x;
		int child;

		//使用reverse访问堆而不是v
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

//邻接表构建图（数组）
class graphL {

	struct Enode {//边表节点
		int adjvex;//邻接点域；
		int weight;//权重
		Enode* next;
	};

	typedef struct Vnode {//顶点表节点
		int vertex;//顶点域
		Enode* firstedge;//边表头指针
	}Vnode;

	int numV, numE;
	vector<Vnode> adjlist;

public:
	void CreateGraph_AL(graphL* g, IndexHeap* heap) {
		int i, j, k;
		Enode* e;

		printf("输入顶点数和边数:\n");
		scanf("%d,%d", &g->numV, &g->numE);

		printf("输入顶点信息");
		for (i = 0; i < g->numV; i++) {
			scanf("%d", &g->adjlist[i].vertex);
			g->adjlist[i].firstedge = NULL;//边表置为空表
		}

		for (k = 0; k < g->numE; k++) {
			e = (Enode*)malloc(sizeof(Enode));//申请空间储存边表节点

			printf("输入边Vi,Vj的顶点序号:\n");
			scanf("%d,%d", &i, &j);

			e->adjvex = j;
			e->next = g->adjlist[j].firstedge;
			g->adjlist[j].firstedge = e;

			e->adjvex = i;
			e->next = g->adjlist[i].firstedge;
			g->adjlist[i].firstedge = e;

		}
	}

	//prim+堆（优先序列）
	int prim_optim(graphL* g, IndexHeap* heap) {

		vector<bool> added(false, g->numE);//是否加入生成树中
		int count = 0;//确定了最小距离点的数量
		int sum = 0;
		added[0] = true;//起点已经访问
		heap->ass->dist[0] = 0;//起点在生成树中

		while (count < heap->size) {
			int minDist = heap->ass->dist[1];
			int minIndex = heap->DeleteIndex(heap);

			if (minIndex == 0) {
				break;
			}

			added[minIndex] = true;
			count++;
			sum += minDist;
			printf("加入顶点%d,边长:%d\n", minIndex + 1, minDist);

			//更新dist数组
			Enode* e = g->adjlist[minIndex].firstedge->next;
			while (e != NULL) {
				int index = e->adjvex;
				if (added[index] == false &&
					heap->ass->dist[index] > e->weight) {
					heap->Fixup(heap, index, e->weight);
				}
				e = e->next;//下一个节点

			}
		}
		return sum;//最小生成树的路径和
	}


};


int main() {
	//第一题省略
	//第二题
	//采用边集数组保存
	//graphE* ge{};
	//ge->CreateGraph_Edge(ge);



	//第三题
	//step1:输入点信息，选取地点附近几个编号，并输出最短的路径。
	//1号：教一楼，
}


#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
#include<vector>
#pragma warning(disable:4996)
using namespace std;
/*我爱c语言，我爱数据结构，大学生哪有不疯的哪有不疯的*/


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
//这里setnode加入parent指针可以避免后续函数内循环，更简洁，可以尝试改进

template<typename T>//加入比较原则
class set {
	typedef setnode<T> Node;

public:

	Node* p = nullptr;

	int treeHeight(Node* p) {//计算当前节点树高度
		if (p == NULL) {
			return 0;
		}
		else if (treeHeight(p->lchild) > treeHeight(p->rchild))
			return treeHeight(p->lchild) + 1;
		else
			return treeHeight(p->rchild) + 1;

	}

	int BalanceFactor(Node* p) {//计算并赋值当前节点平衡因子
		if (p == NULL)
			return 0;
		else
			return treeHeight(p->lchild) - treeHeight(p->rchild);
	}

	void R_Rotate(Node* p) {//右旋
		Node* lc = p->lchild;
		p->lchild = lc->rchild;
		lc->rchild = p;

		BalanceFactor(p);
		BalanceFactor(lc);//重新计算节点平衡因子

		p = lc;
		//调整新的头节点
	}

	void L_Rotate(Node* p) {//左旋
		Node* rc = p->rchild;
		p->rchild = rc->lchild;
		rc->lchild = p;


		BalanceFactor(p);
		BalanceFactor(rc);//重新计算节点平衡因子

		p = rc;
	}

	Node Rebalance(Node* p) {//平衡
		int factor = treeHeight(p);
		//分四种情况讨论
		if (factor > 1 && BalanceFactor(p->lchild) >= 0)
			//LL,需要1次左旋
			L_Rotate(p);
		else if (factor > 1 && BalanceFactor(p->lchild) < 0) {
			//LR,1次左旋+1次右旋
			L_Rotate(p);
			R_Rotate(p);
		}
		else if (factor < -1 && BalanceFactor(p->rchild) <= 0) {
			//RR,1次右旋
			R_Rotate(p);
		}
		else if (factor < -1 && BalanceFactor(p->rchild)>0) {
			//RL,1次右旋+1次左旋
			R_Rotate(p);
			L_Rotate(p);
		}
		//已经平衡，不需要操作
		return *p;

	}
	//AVL树已构成，接下来完成集合构建

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

//边集数组构建图（集合），完成第二题的图储存
//在set<T>的实例化中，有edge和vertex两个实例，分别进行了运算符重载，
//但仍然报错没有运算符，可能是函数编译顺序问题？

class graphE {
	struct edge {
		int begin;//起点下标
		int end;//终点下标
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

		//需要解决edge在set中的排序问题，重载operator
		//就是重新定义比大小的方式
	};

	struct vertex {
		int point;

		bool operator > (const vertex*& p1) const {
			return this->point > p1->point;
		}
	};

	set<vertex> V;//点集
	set<edge> E;//边集

	int numV;//点数
	int numE;//边数

public:
	void CreateGraph_Edge(graphE* g) {
		int i = 0, j = 0, k = 0;
		edge w{};
		vertex po{};

		printf("请输入顶点数和边数:\n");
		scanf("%d,%d", &g->numV, &g->numE);
		getchar();//获取回车符

		//点数组
		printf("请输入顶点信息:\n");
		for (i = 0; i < g->numV; i++) {
			scanf("%d", &po.point);
			g->V.emplace(g->V.p, po);
		}
		getchar();

		//邻边矩阵
		for (k = 0; k < g->numE; k++) {
			printf("依次输入边<vi,vj>的下标i,j和权重w:\n");
			scanf("%d,%d,%d", &w.begin, &w.end, &w.weight);
			getchar();

			g->E.emplace(g->E.p, w);

		}


	}

	//两点最短路径
	vector<int> shortestway(int start, int des, graphE* g) {
		//计算两端点最短路径(迪杰斯特拉算法)
	//要不断寻找没有被访问过且离源点最近的点

		const int inf = 10000;//不可到达的距离！
		int n = g->numV;//图的顶点个数
		vector<int> dis(n, inf);//源点到其他点的最短距离，inf表示无法到达
		vector<int> vis(n, false);//访问记录，false表示未访问
		edge e{};
		e.begin = start;

		dis[start] = 0;//源点到源点的距离为0

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
		return dis;//标记访问

	}
};



/*以下是第三、四题代码*/
//优化堆结构
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
		heap->index[0] = 0;//哨兵
		return heap;

	}

	//插入顶点编号为v的顶点,插入一个index
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

	//删除并返回最小边的索引
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

	//修改dist[v]减少至x，并调整堆
	void Fixup(IndexHeap* heap, int v, int x) {
		heap->ass->dist[v] = x;
		int child;

		//使用reverse访问堆而不是v
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

//邻接表构建图（数组）
class graphL {

	struct Enode {//边表节点
		int adjvex;//邻接点域；
		int weight;//权重
		Enode* next;
	};

	typedef struct Vnode {//顶点表节点
		int vertex;//顶点域
		Enode* firstedge;//边表头指针
	}Vnode;

	int numV, numE;
	vector<Vnode> adjlist;

public:
	void CreateGraph_AL(graphL* g, IndexHeap* heap) {
		int i, j, k;
		Enode* e;

		printf("输入顶点数和边数:\n");
		scanf("%d,%d", &g->numV, &g->numE);

		printf("输入顶点信息");
		for (i = 0; i < g->numV; i++) {
			scanf("%d", &g->adjlist[i].vertex);
			g->adjlist[i].firstedge = NULL;//边表置为空表
		}

		for (k = 0; k < g->numE; k++) {
			e = (Enode*)malloc(sizeof(Enode));//申请空间储存边表节点

			printf("输入边Vi,Vj的顶点序号:\n");
			scanf("%d,%d", &i, &j);

			e->adjvex = j;
			e->next = g->adjlist[j].firstedge;
			g->adjlist[j].firstedge = e;

			e->adjvex = i;
			e->next = g->adjlist[i].firstedge;
			g->adjlist[i].firstedge = e;

		}
	}

	//prim+堆（优先序列）
	int prim_optim(graphL* g, IndexHeap* heap) {

		vector<bool> added(false, g->numE);//是否加入生成树中
		int count = 0;//确定了最小距离点的数量
		int sum = 0;
		added[0] = true;//起点已经访问
		heap->ass->dist[0] = 0;//起点在生成树中

		while (count < heap->size) {
			int minDist = heap->ass->dist[1];
			int minIndex = heap->DeleteIndex(heap);

			if (minIndex == 0) {
				break;
			}

			added[minIndex] = true;
			count++;
			sum += minDist;
			printf("加入顶点%d,边长:%d\n", minIndex + 1, minDist);

			//更新dist数组
			Enode* e = g->adjlist[minIndex].firstedge->next;
			while (e != NULL) {
				int index = e->adjvex;
				if (added[index] == false &&
					heap->ass->dist[index] > e->weight) {
					heap->Fixup(heap, index, e->weight);
				}
				e = e->next;//下一个节点

			}
		}
		return sum;//最小生成树的路径和
	}


};


int main() {
	//第一题省略
	//第二题
	//采用边集数组保存
	//graphE* ge{};
	//ge->CreateGraph_Edge(ge);



	//第三题
	//step1:输入点信息，选取地点附近几个编号，并输出最短的路径。
	//1号：教一楼，
}



//第三题
	// 搭公交车到达指定地点
	//step1:输入点信息，选取地点附近几个编号，并输出最短的路径。
	/* 1号：教一楼，10;
	   2号：教二楼，17，18
	   3号：教三楼，27
	   4号：教四楼，5，9
	   5号：理学院，19，20
	   6号：计算机学院，12，13，14
	   7号：生命科学院，1
	   8号：化学学院，6,7,8,28
	   9号：物理学院,25
	   10号：经管学院，22
	   11号：艺术学院，11，19
	   12号：行政大楼，10，11
	   13号：老图书馆，29
	   14号：新图书馆，5
	   15号：卓尔体育馆，7，12*/
	   //数据输入格式
	   /*教一楼
   1
   10
   教二楼
   2
   17
   18
   教三楼
   1
   27
   教四楼
   2
   5
   9
   理学院
   2
   19
   20
   计算机学院
   3
   12
   13
   14
   生命科学院
   1
   1
   化学学院
   4
   6
   7
   8
   28
   物理学院
   1
   25
   经管学院
   1
   22
   艺术学院
   2
   11
   19
   行政大楼
   2
   10
   11
   老图书馆
   1
   29
   新图书馆
   1
   5
   卓尔体育馆
   2
   7
   12
   */

   //step2:存储节点信息
Place place[15];
for (int i = 0; i < 15; i++) {
	printf("请输入%d号地点:\n", i + 1);
	scanf("%s", place[i].name);
	getchar();
	printf("请输入该地点临近的站点个数:\n");
	int m = 0;
	scanf("%d", &m);

	for (int j = 0; j < m; j++) {
		printf("请输入站点:\n");
		if (scanf("%d", &place[i].station[j]) == 1) {
			place[i].length++;
			getchar();
		}
		else
			break;
	}
}


//step3:寻路
pathplanning(place, ge);

//第四题

graphL* gl = new graphL(30, 38);
gl->CreateGraph_AL(gl);
gl->ShowGraph_AL(gl);

IndexHeap* heap = new IndexHeap(30);
gl->prim_optim(gl, heap, 10);
}