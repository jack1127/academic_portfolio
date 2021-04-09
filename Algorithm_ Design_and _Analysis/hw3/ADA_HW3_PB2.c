#include <stdio.h>
#include <stdlib.h>
#define max_dis_sum 200000000000000
#define V 200000  
#define E 500000
long long int max_distance;
int max_danger;
int length; //len of minheap array
int heap[V];

struct Edge {
	int node1, node2, dis, dan, add;
};
 
struct node{     //edge in list
    int vertex;
    int dis;
    int dan;
    struct node* next;
};

struct Graph{
    int numVertices;
    struct node** adjLists;
};
 
struct node* createNode(int v, int dis, int dan){
    struct node* newNode =(struct node *)malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->dis = dis;
    newNode->dan = dan;
    newNode->next = NULL;
    return newNode;
}
 
struct Graph* createGraph(int vertices){
    struct Graph* graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->numVertices = vertices;
 
    graph->adjLists = (struct node **)malloc(vertices * sizeof(struct node ));
 
    int i;
    for (i = 0; i < vertices; i++)
        graph->adjLists[i] = NULL;
    return graph;
}
 
void addEdge(struct Graph* graph, int src, int dest, int dis, int dan){
    // Add edge from src to dest
    struct node* newNode = (struct node *)createNode(dest,dis,dan);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
 
    // Add edge from dest to src
    newNode = createNode(src, dis ,dan);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

int find(int x, int p[]) {
	return x == p[x] ? x : (p[x] = find(p[x],p));
}
void to_union(int x, int y,int p[]){
	p[find(x,p)] = find(y,p);
}
 
void merge(int low, int mid, int high, struct Edge e[]) {
    int leftIndex = low;
    int rightIndex = mid + 1;
    struct Edge tempArr[high - low + 1];
    int tempIndex = 0;
 	
    while (leftIndex <= mid && rightIndex <= high) {
        if (e[leftIndex].dan <= e[rightIndex].dan) {
            tempArr[tempIndex].dan = e[leftIndex].dan;
            tempArr[tempIndex].dis = e[leftIndex].dis;
            tempArr[tempIndex].node1 = e[leftIndex].node1;
            tempArr[tempIndex].node2 = e[leftIndex].node2;
            leftIndex++;
        }
        else {
            tempArr[tempIndex].dan = e[rightIndex].dan;
            tempArr[tempIndex].dis = e[rightIndex].dis; 
            tempArr[tempIndex].node1 = e[rightIndex].node1;
            tempArr[tempIndex].node2 = e[rightIndex].node2;
            rightIndex++;
        }
        tempIndex++;
    }
    if (leftIndex > mid) {
        while (rightIndex <= high) {
            tempArr[tempIndex].dan = e[rightIndex].dan;
            tempArr[tempIndex].dis = e[rightIndex].dis;
            tempArr[tempIndex].node1 = e[rightIndex].node1;
            tempArr[tempIndex].node2 = e[rightIndex].node2;
            rightIndex++;
            tempIndex++;
        }
    }
    else {
        while (leftIndex <= mid) {
           	tempArr[tempIndex].dan = e[leftIndex].dan;
           	tempArr[tempIndex].dis = e[leftIndex].dis;
            tempArr[tempIndex].node1 = e[leftIndex].node1;
            tempArr[tempIndex].node2 = e[leftIndex].node2;
            leftIndex++;
            tempIndex++;
        }
    }
    leftIndex = low;
    for (tempIndex=0; tempIndex< (high - low + 1); tempIndex++) {
        e[leftIndex].dan = tempArr[tempIndex].dan;
        e[leftIndex].dis = tempArr[tempIndex].dis;
        e[leftIndex].node1 = tempArr[tempIndex].node1;
        e[leftIndex].node2 = tempArr[tempIndex].node2;
        leftIndex++;
    }
}

void mergeSort(int low, int high, struct Edge e[]) {
    if (low < high) {
        int mid = (low + high) / 2;
 
        mergeSort(low, mid, e);
        mergeSort(mid + 1, high, e);
        merge(low, mid, high, e);
    }
}

void Kruskal(int N,int M, struct Edge e[], struct Graph* graph){
	int p[N];
    for (int i=0; i<N; i++) p[i] = i;
    mergeSort(0, M-1, e); // 依danger排序
 
    // 依序找出最小生成樹上的V-1條邊。
    int i, j;
    for (i = 0, j = 0; i < N-1 && j < M; ++i){
        // 產生環，則捨棄。直到產生橋。
        while (find(e[j].node1,p) == find(e[j].node2,p)) j++;
 
        // 產生橋，則以此邊連接兩棵MSS。
        to_union(e[j].node1, e[j].node2,p);
 		addEdge(graph, e[j].node1, e[j].node2, e[j].dis, e[j].dan);
 		e[j].add=1;
        j++;
    }
}

void find_danger(int N, int M, int s, int t, int danger, int parent, struct Graph* graph){
	int temp;
	struct node* ptr;
	if(s==t){
		max_danger = danger;
		return;
	}
	for(ptr = graph->adjLists[s]; ptr != NULL; ptr = ptr->next){
		if(ptr->vertex != parent){
			if(danger < ptr->dan){
				temp = ptr->dan;
			}
			else temp = danger;
			find_danger(N, M, ptr->vertex, t, temp, s, graph);
		}
	}
	return;
}

void rebuild(int M, struct Edge e[], struct Graph* graph){   //find all edge dan <= max_danger in e
	int i;
	for(i=0;i<M;i++){
		if(e[i].dan <= max_danger && e[i].add == 0){
			addEdge(graph, e[i].node1, e[i].node2, e[i].dis, e[i].dan);	
		}
	}
	return;
}

void minheap(int node, long long int d[]){
	// 用int heap[] 存在更新dis後進heap的vertex
	int left = 2*node;
	int right = 2*node+1;
	int smallest, temp;
	if(left <= length && d[heap[node]] < d[heap[left]]){
		smallest = left;
	}
	else{
		smallest = node;
	}
	if(right <= length && d[heap[node]] < d[heap[right]]){
		smallest = right;
	}
	if(smallest != node){
		temp = heap[smallest];
		heap[smallest] = heap[node];
		heap[node] = temp;
		minheap(smallest, d);
	}
	return;
}
void insert(int node, long long int d[]){
	int index = length;
	int temp;
	heap[length] = node;
	while(index > 1){
		if(d[heap[index]] <= d[heap[index/2]]){
			temp = heap[index];
			heap[index] = heap[index/2];
			heap[index/2] = temp;
		}
		index=index/2;
	}
}

int extract_min(long long int d[]){ 
    // Initialize min value 
    int min_index; 
  	min_index = heap[1];
  	heap[1] = heap[length];
    length--;
    minheap(1, d);
    return min_index;
}

void dijkstra(int source, int t, int N, int M, struct Graph* graph){
    long long int d[N]; //distance
	int visited[N],i;
	struct node* ptr;
    for (i=0; i<N; i++){
		visited[i] = 0;
 		d[i] = max_dis_sum;
    }
    d[source] = 0;
    heap[1]=source;     //heap 記index
 	length=1;

    for (int k=0; k<N; k++){   //
        int u;
        u = extract_min(d);
        if(visited[u]==1){
        	k--;
        }
        //printf("extract %d\n",u);
        visited[u]=1;
        for(ptr = graph->adjLists[u]; ptr != NULL; ptr = ptr->next){
			if(d[ptr->vertex] > d[u] + ptr->dis){
        		d[ptr->vertex] = d[u] + ptr->dis;
        		length++;
        		insert(ptr->vertex, d);
        		//printf("insert %d in dis  =%d\n",ptr->vertex, d[ptr->vertex]);
        	}
        }
        /*for(i=1; i<=length; i++){
        	printf("heap[%d] = %d/  ",i, heap[i]);
        }
        printf("\n");*/
    }
    max_distance=d[t];
}

int main(){
	int N,M,s,t,i,x,y;
	int distan,dang;
	scanf("%d%d%d%d",&N,&M,&s,&t);
	struct Edge e[M];
	struct Graph* graph = createGraph(N);
	for(i = 0; i < M; i++){
		scanf("%d%d%d%d", &x,&y,&distan,&dang);
		e[i].node1=x;
		e[i].node2=y;
		e[i].dis=distan;
		e[i].dan=dang;
		e[i].add=0;
	}
	Kruskal(N,M,e,graph);
	max_distance=0;
	max_danger=0;
	find_danger(N, M, s, t, 0, s,graph);
	rebuild(M, e, graph);
	dijkstra(s,t,N,M,graph);
	printf("%lld %d\n",max_distance,max_danger);
	return 0;
}