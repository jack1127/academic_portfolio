#include <iostream>
#include <algorithm>
#include <bitset>       
using namespace std;
	int visited[200002] = {};
	int rec[400002] = {};
	int cnt;
	bitset<20000> bsans;
	bitset<20000> bs[200001];
	int myvector[200002];
	float child,parent;

int max(int a, int b) {
    if (a > b) return a;
    else return b;
}
struct node{
    int vertex;
    struct node* next;
};

struct Graph{
    int numVertices;
    struct node** adjLists; // we need int** to store a two dimensional array. Similary, we need struct node** to store an array of Linked lists
};

struct node* createNode(int v){
    struct node* newNode = (struct node *)malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}
struct Graph* createGraph(int vertices){
    struct Graph* graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->numVertices = vertices;
    graph->adjLists = (struct node **)malloc(vertices * sizeof(struct node*));

    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
    }
    return graph;
}
void addEdge(struct Graph* graph, int src, int dest){
    // Add edge from src to dest
    struct node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
}

void DFS2(struct Graph* graph, int vertex, int checknum) {
        struct node* temp = graph->adjLists[vertex];
        visited[vertex] = 1;
       	//printf("Visited %d \n", vertex);
    	while(temp!=NULL){
       	   	int connectedVertex = temp->vertex;
       		if(visited[connectedVertex] == 0) {
           	   	DFS2(graph, connectedVertex, checknum);
      		}
          	bs[vertex] = bs[vertex] | bs[connectedVertex];
           	temp = temp->next;
       	}
       	return;
}
void DFS(struct Graph* graph, int checknum, int N) {
    for(int i=0; i<N; i++){
        if(visited[i] == 0){
        	struct node* temp = graph->adjLists[i];
        	visited[i] = 1;
       		//printf("Visited %d \n", vertex);
    		while(temp!=NULL){
        	   	int connectedVertex = temp->vertex;
          		if(visited[connectedVertex] == 0) {
       	    	   	DFS2(graph, connectedVertex, checknum);
       	   		}
       	   		bs[i] = bs[i] | bs[connectedVertex];
       	   		temp = temp->next;
   			}    		
        }
    }	
    return;
}

void DFS3(struct Graph* graph, int vertex, int who) {  
    if (who == 1) {
        parent+=1;
        visited[vertex] = 1;
        rec[cnt] = vertex;
    	cnt++;
    }
    else {
        if (visited[vertex] == 0){
            parent+=1;
            rec[cnt] = vertex;
    		cnt++;
        }
        else
            child+=1;
        visited[vertex] = 2;
    }
   	struct node* temp = graph->adjLists[vertex];
    while (temp) {
        if (visited[temp->vertex] == 0)
            DFS3(graph, temp->vertex, who);
        if (who == 2 && visited[temp->vertex] == 1)
            DFS3(graph, temp->vertex, who);
        temp = temp->next; 
    }
    return;
}

int main(){
	int N,M,Q,i,j,x,y,checknum;
	scanf("%d%d%d",&N,&M,&Q);
	if(N>=20000){
		checknum=20000;
		for(j=0; j<N; j++) myvector[j]=j;
		random_shuffle ( myvector, myvector+N );
		for(j=0; j<N; j++){
			if(myvector[j] < checknum){
       			bs[j][myvector[j]]=1;
       		}
		}
	}
	else{
		checknum=N;
		for(i=0; i<N;i++) bs[i][i]=1;
	}
	struct Graph* graph = createGraph(N);
	for(i=0; i<M; i++){
		scanf("%d%d",&x,&y);
		addEdge(graph,x-1,y-1);
	}
	DFS(graph,checknum,N);
	for(i=0; i<N;i++) visited[i]=0;
	for(i=0; i<Q; i++){
		scanf("%d%d",&x,&y);
		if(max(bs[x-1].count(), bs[y-1].count()) < 180){
			child=0,parent=0;
			DFS3(graph,x-1,1);
			DFS3(graph,y-1,2);
			for (int j = 0; j < cnt; j++)
                visited[rec[j]] = 0;
            cnt = 0;
		}
		else{
			bsans = bs[x-1] | bs[y-1];
			parent = bsans.count();
			bsans = bs[x-1] & bs[y-1];
			child = bsans.count();
		}
		printf("%f\n",child/parent);
	}
	/*for(i=0; i<N; i++){
		for(j=N-1; j>=0; j--){
			cout << bs[i][j];
		}
		printf("\n");
	}*/
	return 0;
}