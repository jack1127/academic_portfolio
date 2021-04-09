#include <stdio.h>
#include <stdlib.h>

// Define maximum number of vertices in the graph
#define MAXN 2*2*2*2*2*2*2*2*2*2*2*2*2*2*2*2*2*2

// A data structure to store adjacency list nodes of the graph
struct Node {
	int dest;
	int delete_count;
	int indegree;
	int indegree_keep;
	int next;
};

struct Node node[MAXN];
int clean[MAXN];
long long int init_count;

// Directed Graph Implementation in C
int main(void)
{
	int N,Q,i,j,x,temp,rnum,remove,ans;
	// input array containing edges of the graph (as per above diagram)
	// (x, y) pair in the array represents an edge from x to y
	scanf("%d",&N);
	for(i=0; i<=N; i++){    //build edge
		node[i].delete_count=0;
		node[i].indegree=0;
		node[i].indegree_keep=0;
	}
	for(i=1; i<=N; i++){    //build edge
		scanf("%d",&x);
		node[i].next=x;
		node[x].indegree++;
		node[x].indegree_keep++;
	}
	node[0].next=0;
	node[0].indegree++;
	node[0].indegree_keep++;
	
	for(i=1; i<=N; i++){       //find destination
		if(node[i].indegree == 0){ //only find dest of node whose indegree =0
			if(node[i].next==i){
				node[i].dest=i;
				node[i].delete_count=1;
			}
			else{
				temp=node[i].next;
				node[i].delete_count=1;
				while(node[temp].indegree == 1){
					if(temp==node[temp].next){
						break;
					}
					temp=node[temp].next;
					node[i].delete_count++;
				}
				node[i].dest=temp;
			}
		}
	}

	scanf("%d",&Q);
	for(i=0; i<Q; i++){
		ans=0;
		scanf("%d",&rnum);
		for(j=0; j<rnum; j++){
			scanf("%d",&remove);
			ans+=node[remove].delete_count;
			node[node[remove].dest].indegree--;
			clean[init_count]=node[remove].dest;
			init_count++;
			while(node[node[remove].dest].indegree == 0){
				remove=node[remove].dest;
				if(node[remove].next==remove){ //find dest of the nodes whose indegree is reduced to 0
					node[remove].dest=remove;
					node[remove].delete_count=1;
				}
				else{
					temp=node[remove].next;
					node[remove].delete_count=1;
					while(node[temp].indegree == 1){
						if(temp==node[temp].next){
							break;
						}
						temp=node[temp].next;
						node[remove].delete_count++;
					}
					node[remove].dest=temp;
				}
				//printf("remove = %d in while\n",remove);
				ans+=node[remove].delete_count;
				node[node[remove].dest].indegree--;
				clean[init_count]=node[remove].dest;
				init_count++;
			}
			//printf("total delete_count = %d\n",ans);
		}
		printf("%d\n",ans); //print answer
		for(j=0; j<init_count; j++){
			node[clean[j]].indegree=node[clean[j]].indegree_keep;
		}
		init_count=0;
	}

	return 0;
}