#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <limits>
#include <stack>
using namespace std;
#define maxN 100000
#define maxM 500000

long long int max_len;
int endless;

struct node{     //edge in list
    int vertex;
    int dis;
    struct node* next;
};

struct Graph{
    int numVertices;
    struct node** adjLists;
};
 
struct node* createNode(int v, int dis){
    struct node* newNode =(struct node *)malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->dis = dis;
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
 
void addEdge(struct Graph* graph, int src, int dest, int dis){
    // Add edge from src to dest
    struct node* newNode = (struct node *)createNode(dest,dis);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
}

void merge(int l, int m, int r, int jumpsong[], int jumplen[]){ 
    int i, j, k;
    int n1 = m - l + 1; 
    int n2 =  r - m;   
    int Lsong[n1], Rsong[n2];     /* create temp arrays */
    int Llen[n1], Rlen[n2];
    
    for (i = 0; i < n1; i++){
        Lsong[i] = jumpsong[l + i];
        Llen[i] = jumplen[l + i]; 
    }

    for (j = 0; j < n2; j++){
        Rsong[j] = jumpsong[m + 1+ j];
        Rlen[j] = jumplen[m + 1+ j];
    }
  
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 
    while (i < n1 && j < n2){
        if (Lsong[i] < Rsong[j]){ 
            jumpsong[k] = Lsong[i];
            jumplen[k] = Llen[i]; 
            i++;
        }
        else if(Lsong[i] > Rsong[j]){ 
            jumpsong[k] = Rsong[j]; 
            jumplen[k] = Rlen[j];
            j++; 
        }
        else{
            if(Llen[i] < Rlen[j]){
                jumpsong[k] = Lsong[i];
                jumplen[k] = Llen[i]; 
                i++;
            }
            else{
                jumpsong[k] = Rsong[j]; 
                jumplen[k] = Rlen[j];
                j++;
            }
        }
        k++; 
    } 
  
    while (i < n1){  /* Copy the remaining elements of L[], if there are any */
        jumpsong[k] = Lsong[i];
        jumplen[k] = Llen[i];  
        i++; 
        k++; 
    } 
  
    while (j < n2){  /* Copy the remaining elements of R[], if there are any */
        jumpsong[k] = Rsong[j]; 
        jumplen[k] = Rlen[j];
        j++; 
        k++; 
    } 
} 
  
/* l is for left index and r is right index of the 
   sub-array of arr to be sorted */
void mergeSort(int l, int r, int jumpsong[], int jumplen[]){ 
    if (l < r){ 
        int m = (l+r)/2; 
        // Sort first and second halves 
        mergeSort(l, m, jumpsong, jumplen); 
        mergeSort(m+1, r, jumpsong, jumplen); 
        merge(l, m, r, jumpsong, jumplen); 
    }
}

int deletesame(int nosamesong[], int nosamelen[], int amount, int jumpsong[], int jumplen[], int segment[], int indegree[]){
    int i,count = 0, lastsong, lastlen;
    nosamesong[0] = jumpsong[0];
    nosamelen[0] = jumplen[0];
    lastsong = jumpsong[0];
    lastlen = jumplen[0];
    indegree[0]=0;
    segment[0]=-1;
    int segmentidx=0;
    for(i=1; i<=amount; i++){
        if( (lastsong != jumpsong[i]) || (lastlen != jumplen[i])){
            if(lastsong != jumpsong[i]){
                segmentidx++;
                segment[segmentidx]=count;
            }
            count++;
            nosamesong[count] = jumpsong[i];
            nosamelen[count] = jumplen[i];
            lastsong=jumpsong[i];
            lastlen=jumplen[i];
            indegree[count]=0;
        }
    }
    segmentidx++;
    segment[segmentidx]=count;
    return count;
}

bool isCyclicUtil(struct Graph* graph, int v, bool visited[], bool *recStack) { 
    if(visited[v] == false) { 
        visited[v] = true; 
        recStack[v] = true; 
        struct node* current = graph->adjLists[v];
        while(current) {
            if ( !visited[current->vertex] && isCyclicUtil(graph, current->vertex, visited, recStack)) 
                return true; 
            else if (recStack[current->vertex]) 
                return true; 
            current = current->next;
        }
    } 
    recStack[v] = false; 
    return false; 
} 
  
bool isCyclic(struct Graph* graph, int node_amount) { 
    bool *visited = new bool[node_amount+1]; 
    bool *recStack = new bool[node_amount+1]; 
    for(int i = 0; i <= node_amount; i++) { 
        visited[i] = false; 
        recStack[i] = false; 
    }
    for(int i = 0; i <= node_amount ; i++) 
        if (isCyclicUtil(graph, i, visited, recStack)) 
            return true; 
    return false; 
}

void topological_ordering(struct Graph* graph, int size, int ref[], int Q[]){
    // 宣告一個queue來記錄已經沒有被任何邊連向的點
    int qcount=-1, qpop=-1;
    struct node* ptr;
    //printf("ref = ");
    for (int i=0; i<=size; i++){    // 開始找出一個合理的排列順序
        Q[i]=0;
        if (ref[i] == 0){
            qcount++;
            Q[qcount]=i;
        }
        //printf("%d ",ref[i]);
    }//printf("\n");
    for (int i=0; i<=size; i++){    // 尋找沒有被任何邊連向的點
        qpop++;
        int s = Q[qpop];
        ref[s] = -1;                // 設為已找過（刪去s點)
        //printf("%d -> ",s);             // 印出合理的排列順序的第i點
 
        // 更新ref的值（刪去由s點連出去的邊）
        ptr = graph->adjLists[s];
        while(ptr != NULL){
            if(ref[ptr->vertex]>0){
                ref[ptr->vertex]--;
                if (ref[ptr->vertex]==0){  // 記錄已經沒有被任何邊連向的點
                    qcount++;
                    Q[qcount]=ptr->vertex;
                }
            }
            ptr=ptr->next;
        }
        //printf("qcount = %d, qpop = %d\n",qcount,qpop);
    }
}

void relax(int u, int v, int dis, long long int distance[]){
    if(distance[v] < distance[u] + dis){
        distance[v] = distance[u] + dis;
        if(max_len < distance[v]){
            max_len = distance[v];
        }
    }
}

void lawler(struct Graph* graph, int node_amount, int ref[], int segment[], int N){
    struct node* ptr;
    int i,s, Q[node_amount+1];
    long long int distance[node_amount+1];
    topological_ordering(graph, node_amount, ref, Q);
    //initialize
    for(i=0; i<=node_amount; i++){
        distance[i]=0;
    }

    for(i=0; i<=node_amount; i++){
        s = Q[i];
        for(ptr = graph->adjLists[s]; ptr != NULL; ptr = ptr->next){
            //printf("%d to %d   ",s,ptr->vertex);
            relax(s, ptr->vertex, ptr->dis, distance);
            //printf("distance[%d]=%d\n",ptr->vertex,distance[ptr->vertex]);
        }
    }
}


// Directed Graph
int main(){
    int T,N,M,i,j,k,s1,t1,s2,t2;
    int jumpnode_count;
    scanf("%d",&T);
    for(k=0; k<T; k++){
        scanf("%d%d",&N,&M);
        struct Graph* graph = createGraph(2*N+2*M);
        int length;
        int jumpsong[2*N+2*M];      //song idx
        int jumplen[2*N+2*M];       // length from start point
        int jumppair1[2][M+1], jumppair2[2][M+1];     // 1 to 2 , 0=song num, 1=len
        int nosamesong[2*N+2*M];
        int nosamelen[2*N+2*M];
        int node_amount;
        int segment[N+1];   //記end的idx
        int indegree[2*N+2*M+1];
        jumpnode_count=-1;
        for(i=1; i<=N; i++){
            scanf("%d",&length);
            jumpnode_count++;
            jumpsong[jumpnode_count]=i;
            jumplen[jumpnode_count]=0;
            jumpnode_count++;
            jumpsong[jumpnode_count]=i;
            jumplen[jumpnode_count]=length;
        }   
        for(i=1; i<=M; i++){
            scanf("%d%d%d%d",&s1,&t1,&s2,&t2);
            jumpnode_count++;
            jumpsong[jumpnode_count]=s1;
            jumplen[jumpnode_count]=t1;
            jumppair1[0][i]=s1;
            jumppair1[1][i]=t1;
            jumpnode_count++;
            jumpsong[jumpnode_count]=s2;
            jumplen[jumpnode_count]=t2;
            jumppair2[0][i]=s2;
            jumppair2[1][i]=t2;
        }
        /*for(i=0; i<=jumpnode_count; i++){
            printf("before sort [%d] = %d %d\n",i,jumpsong[i], jumplen[i]);
        }*/
        mergeSort(0, jumpnode_count, jumpsong, jumplen);
        node_amount = deletesame(nosamesong, nosamelen, jumpnode_count, jumpsong, jumplen, segment, indegree);
        /*for(i=0; i<=node_amount; i++){
            printf("after delete %d = %d %d\n",i,nosamesong[i], nosamelen[i]);
        }*/
        for(i=1; i<=N; i++){   // link nodes without jump
            for(j=segment[i-1]+1; j<segment[i]; j++){
                addEdge(graph, j, j+1, nosamelen[j+1]-nosamelen[j]);
                //printf("%d to %d with dis =%d \n", j, j+1, nosamelen[j+1]-nosamelen[j]);
                indegree[j+1]++; 
            }
        }
        for(i=1; i<=M; i++){  // link jump nodes
            int src,dest;
            for(j=segment[jumppair1[0][i]-1]+1; j<=segment[jumppair1[0][i]]; j++){
                if(jumppair1[1][i]==nosamelen[j]){
                    src=j;
                    break;
                }
            }
            for(j=segment[jumppair2[0][i]-1]+1; j<=segment[jumppair2[0][i]]; j++){
                if(jumppair2[1][i]==nosamelen[j]){
                    dest=j;
                    break;
                }
            }
            addEdge(graph, src, dest, 1);
            indegree[dest]++;
        }
  
        /*struct node* ptr; //check graph
        for(i=0; i<=node_amount; i++){
            ptr = graph->adjLists[i];
            printf("adj : %d =>  ",i);
            while(ptr != NULL){
                printf("%d, ",ptr->vertex);
                ptr = ptr->next;
            }
            printf("\n");
        }*/

        max_len=0;
        //print answer
        if(isCyclic(graph, node_amount)){
            printf("LoveLive!\n");
        }
        else{
            lawler(graph, node_amount, indegree,segment,N);
            printf("%lld\n",max_len);
        }
    }
    return 0;
}