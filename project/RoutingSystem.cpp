#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
#include<string>
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
		if (p == NULL) {
			return 0;
		}
			
		else
		{
			p->bf = treeHeight(p->lchild) - treeHeight(p->rchild);
			return p->bf;
		}
	}

	void R_Rotate(Node*& p) {//右旋
		Node* lc = p->lchild;
		p->lchild = lc->rchild;
		lc->rchild = p;

		BalanceFactor(p);
		BalanceFactor(lc);//重新计算节点平衡因子

		p = lc;
		//调整新的头节点
	}

	void L_Rotate(Node*& p) {//左旋
		Node* rc = p->rchild;
		p->rchild = rc->lchild;
		rc->lchild = p;


		BalanceFactor(p);
		BalanceFactor(rc);//重新计算节点平衡因子

		p = rc;
	}

	Node* Rebalance(Node*& p) {//平衡
		int factor = 0;
	
		if(p!=nullptr)
			factor = p->bf;

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
		return p;

	}
	//AVL树已构成，接下来完成集合构建

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

//边集数组构建图（集合），完成第二题的图储存
class graphE {
	struct edge {
		int begin;//起点下标
		int end;//终点下标
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

		//需要解决edge在set中的排序问题，重载operator
		//就是重新定义比大小的方式
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

	set<vertex> V;//点集
	set<edge> E;//边集

	int numV;//点数
	int numE;//边数


public:
	graphE() {
		V.p = nullptr;
		E.p = nullptr;
		numV = 0;
		numE = 0;

	}//初始化函数

	void CreateGraph_Edge(graphE* g) {
		int i = 0, j = 0, k = 0;
		edge w{};
		vertex po{};
		
		printf("请输入顶点数和边数:\n");
		scanf("%d,%d", &g->numV, &g->numE);
		getchar();//获取回车符

		//点数组
		for (i = 1; i <= g->numV; i++) {
			po.point = i;
			g->V.emplace(g->V.p, po);
			
		}
	
		//邻边矩阵
		for (k = 1; k <= g->numE; k++) {
			printf("依次输入第%d条边<vi,vj>的下标i,j和权重w:\n",k);
			scanf("%d,%d,%d", &w.begin, &w.end, &w.weight);
			getchar();

			g->E.emplace(g->E.p, w);
			//因为无向图，所以一条边要调换头尾再储存一遍
			int temp = w.begin;
			w.begin = w.end;
			w.end = temp;
			g->E.emplace(g->E.p, w);

		}


	}

	//展示边集合专用,在下一个函数调用
	void showedge(setnode<edge>* root) {
		if (root != nullptr) {
			printf("(%d)->(%d)<%d>\n  ", root->value.begin, root->value.end, root->value.weight);
		}
		else
			return;

		showedge(root->lchild);
		showedge(root->rchild);
	}
	//展示点集合专用,在下一个函数调用
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

		//展示点集合
		setnode<vertex>* rootv = g->V.p;
		printf("点集合:\n[");
		showvertex(rootv);
		printf("]\n");

		//展示边集合
		setnode<edge>* roote = g->E.p;
		printf("边集合:\n[");
		showedge(roote);
		printf("]\n");
	}

	//两点最短路径
	int shortestway(int start, int des, graphE* g) {
	//计算两端点最短路径(迪杰斯特拉算法)
	//要不断寻找没有被访问过且离源点最近的点
		
		const int inf = 100000;//不可到达的距离！
		int n = g->numV;//图的顶点个数
		vector<int> dis(n, inf);//源点到其他点的最短距离，inf表示无法到达
		vector<bool> vis(n, false);//访问记录，false表示未访问
		vector<int> pre(n, -1);//记录这个点在加入时连接的点（需要重点理解）
		edge e{};
		

		dis[start] = 0;//源点到源点的距离为0

		for (int i = 0; i < n - 1; i++) {
			int node = -1;
			for (int j = 0; j < n; j++) {
				if (!vis[j] && (node == -1 || dis[j] < dis[node])) {
					node = j;
				}
			}
			 // 标记访问

			for (int j = 0; j < n; j++) {
				e.begin = node+1;
				e.end = j+1; //end和begin都是从1开始，函数中的点都是从0开始
				if (g->E.find(e) != nullptr) {
					if (dis[j] > dis[node] + g->E.find(e)->weight) {
						dis[j] = dis[node] + g->E.find(e)->weight;
						pre[j] = node;
					}
				}
			}

			vis[node] = true;

		}

		//打印最短路径
		printf("从%d到%d路线:\n",start+1,des+1);
		/*要反过来展示，从A到B的路径展示，前面需要以A为源点，一步步倒推到B，
		然后此时通过pre进行回溯
		展示出来就是正向的*/
		for (int i = des; i != start; i = pre[i]) {
			printf("%d -> ", i+1);
		}
		printf("%d,总距离%d\n", start+1,dis[des]);

		return dis[des];
	}
};

//第三题
typedef struct Place {
	char name[10];//储存地理位置的名称
	int station[10];//储存附近站点
	int length;//记录站点数量

	Place() { length = 0; }
}Place;

//路线规划函数
void pathplanning(Place* place, graphE* ge) {
	int A = 0, B = 0;
	int p1 = 0, p2 = 0;
	printf("请输入起点和终点的序号:\n");
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
	printf("最短路径如下:\n");
	ge->shortestway(p1, p2, ge);

}


/*以下是第四题代码*/
//优化堆结构
class IndexHeap {
	struct Node {
		int* dist;//顶点到生成树的最小距离
		int* pos;//顶点在堆中的位置
	};

public:
	int* index;//堆节点对应的顶点编号
	Node* ass;
	int size;
	int count;

	IndexHeap(int maxsize) {
		int inf = 100000;//无法到达
		ass = new Node;
		count = 0;
		size = maxsize;
		index = new int[maxsize + 1]();
		index[0] = 0;//哨兵
		ass->dist = new int[maxsize+1]();
		ass->pos = new int[maxsize+1]();
		for (int i = 0; i <= size; i++) {
			ass->dist[i] = inf;
		}


	}

	void SwapIndex(IndexHeap* heap, int i) {
		heap->ass->pos[heap->index[i]] = i;
	}//i是顶点在堆中的位置，index[i]是顶点编号

	//插入顶点编号为v的顶点,插入一个index
	void InsertIndex(IndexHeap* heap, int v) {
		int x = heap->ass->dist[v];
		int child;
		for (child = ++heap->count;
			x < heap->ass->dist[heap->index[child / 2]] &&child>0;
			child /= 2) {
			heap->index[child] = heap->index[child / 2];
			SwapIndex(heap, child);//child是v顶点在堆中的位置
		}
		heap->index[child] = v;
		SwapIndex(heap, child);
	}

	//删除并返回最小边的索引
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

	//修改dist[v]减少至x，并调整堆
	void Fixup(IndexHeap* heap, int v, int x) {
		heap->ass->dist[v] = x;
		int child;

		//使用pos访问堆而不是v

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

//邻接表构建图（数组）
class graphL {
	typedef struct Enode {//边表节点
		int adjvex;//邻接点域；
		int weight;//权重
		Enode* next;
	}Enode;

	typedef struct Vnode {//顶点表节点
		int vertex;//顶点域
		Enode* firstedge;//边表头指针
	}Vnode;

public:
	int numV, numE;
	vector<Vnode> adjlist;

	//初始化函数
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
			printf("输入边<vi,vj>的顶点序号和权重:\n");
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
			
			//这里一定要用malloc重新分配，
			//不然上面一个的地址和下面的地址会连起来
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
			printf("顶点%d的邻接表:\n%d->", i+1,i+1);
			while (e->next != nullptr) {
				printf("%d->", e->adjvex);
				e = e->next;
			}
			printf("%d\n", e->adjvex);
			
		}
	}

	//prim+堆（优先序列）
	int prim_optim(graphL* gl, IndexHeap* heap, int start) {
	
		int sum = 0;
		heap->ass->dist[start] = 0;

		//将gl图中的距离导入heap中
		Enode* e = new Enode;
		e = gl->adjlist[start-1].firstedge;//找到直接和出发点连接的点

		while (e != nullptr) {
			heap->ass->dist[e->adjvex] = e->weight;
			e = e->next;//先加入所有直接连接的点
		}

		for (int i = 1; i <= gl->numV; i++) {
			heap->InsertIndex(heap, i);
		}

	

		printf("路径如下:\n");
		printf("%d", start);
		while (heap->count!=1) {//起点也在树里面，下标为0

			//此段代码展示了堆的节点，需要时可启用，查看堆节点的变化
			//for (int i = 1; i <= gl->numV; i++) {
				//if(heap->ass->dist[i]!=100000)
				//printf("%d,%d,%d\n", i, heap->ass->pos[i], heap->ass->dist[i]);
			//}
			
			int minIndex= heap->DeleteIndex(heap);
			int minDist = heap->ass->dist[minIndex];
			heap->ass->dist[minIndex] = 0;//该点加入生成树
			sum += minDist;
			

			//更新dist数组
			Enode* e = new Enode;
			e = gl->adjlist[minIndex-1].firstedge;

			while (e != nullptr) {
				int index = e->adjvex;
				if (heap->ass->dist[index] != 0 && heap->ass->dist[index] > e->weight){
					heap->Fixup(heap, index, e->weight);
					//这里的dist有prim到生成树的距离，和迪杰斯特拉求到源点距离的区别
					//prim里，只需要比较新的点对已有生成树中某一点的距离，和到生成树的距离
					//迪杰斯特拉里，需要比较新的点到已有点集合中某一点的距离+某点到源点的距离，
					//和某点直接到源点的距离
				}
				e = e->next;//下一个节点

			}
			

			printf("->%d(%d)", minIndex, minDist);
		}
		printf("\n总路径长度%d", sum);
		return sum;//最小生成树的路径和
	}
};






int main() {
	//第一题省略

	//第二题
	//采用边集数组保存
	//需要输入的图数据如下
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
	//展示地图
	ge->ShowGraph_Edge(ge);
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
