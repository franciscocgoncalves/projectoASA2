#include <cstdio>
#include <vector>
#include <stack>
#include <list>
const int INF = 1000 * 1000 * 1000;

struct Set{
	int ID;
	Set *parent;
	int rank;
    
	Set(int id): ID(id), parent(this), rank(0){}
};

Set* MakeSet(int ID){
	return new Set(ID);
}

Set* FindSet(Set* x){
	if(x->parent != x)
		x->parent = FindSet(x->parent);
	return x->parent;
}

void Union(Set* x, Set* y){
	Set *xRoot = FindSet(x);
	Set *yRoot = FindSet(y);
    
	if(xRoot == yRoot)
		return;
	if(xRoot->rank < yRoot->rank)
		xRoot->parent = yRoot;
	else{
		yRoot->parent = xRoot;
		if (xRoot->rank == yRoot->rank)
			xRoot->rank = xRoot->rank + 1;
	}
}

struct Node {
	std::vector<Node*> adjacencyNodes;
	int nodeNum;
	bool discovered, critical;
	Node() :
    nodeNum(-1), discovered(false), critical(false){
	}
};

struct Graph {
	std::vector<Node> nodes;
	Graph(int size) :
    nodes(size) {
		for (int i = 0; i < size; i++) {
			nodes[i].nodeNum = i;
		}
	}
    
	Node* getNode(int pos) {
		return &nodes[pos];
	}
};

std::vector<std::vector<int> > readCriticalPoints(Graph& graph, int numberOfProblems){
    int scanf_err, numberOfCriticalPoints, criticalPoint;
    std::vector<std::vector<int> > criticalPoints(numberOfProblems);
    
    for(int i = 0; i < numberOfProblems; i++){
        scanf_err = scanf("%d", &numberOfCriticalPoints);
        (void) scanf_err;
        
        for(int j = 0; j < numberOfCriticalPoints; j++){
            scanf_err = scanf("%d", &criticalPoint);
            (void) scanf_err;
            criticalPoints[i].push_back(criticalPoint);
        }
    }
    return criticalPoints;
}

void readGraph(Graph& graph, int numberOfEdges) {
	int node, adjacencyNode, scanferr;
    
	for (int i = 0; i < numberOfEdges; i++) {
		scanferr = scanf("%d %d", &node, &adjacencyNode);
		(void) scanferr;
        
		graph.getNode(node)->adjacencyNodes.push_back(graph.getNode(adjacencyNode));
		graph.getNode(adjacencyNode)->adjacencyNodes.push_back(graph.getNode(node));        
	}
}

int bfs(Graph& graph, int numberOfNodes, int numberOfCriticalNodes, int startNode){
	std::list<Node*> queue;
	Node *node;
	std::vector<Set*> sets(numberOfNodes);
	int cut = 0, minCut = INF;
    
    queue.push_back(graph.getNode(startNode));
    
    for(int i = 0; i < numberOfNodes; i++){
    	sets[i] = MakeSet(i);
    }
    
    while(!queue.empty()){
        node = queue.front();
        queue.pop_front();
        
        if(!node->discovered){
	        node->discovered = true;
            
		    if(node->critical){
                numberOfCriticalNodes--;
               	if(numberOfCriticalNodes == 0)
	    			continue;
			}            
			Union(FindSet(sets[startNode]), FindSet(sets[node->nodeNum]));
            
			for(unsigned int i = 0; i < node->adjacencyNodes.size(); i++ ){
				if(node->adjacencyNodes[i] != node){
					if(FindSet(sets[node->adjacencyNodes[i]->nodeNum]) != FindSet(sets[startNode])){
						cut++;
					}
					else
						cut--;
					if(node->adjacencyNodes[i]->discovered == false)
						queue.push_back(node->adjacencyNodes[i]);
				}
			}			
			minCut = std::min(minCut, cut);            
		}
    }
    if(numberOfCriticalNodes == 0)
    	return minCut;
    return 0;
}

int main() {
	int numberOfNodes, numberOfEdges, numberOfProblems, cut;
	int scanf_err = scanf("%d %d", &numberOfNodes, &numberOfEdges);
	(void) scanf_err;
    
	Graph graph(numberOfNodes);
    
	readGraph(graph, numberOfEdges);
    
   	scanf_err = scanf("%d", &numberOfProblems);
    (void) scanf_err;
    
    std::vector<std::vector<int> > criticalPoints = readCriticalPoints(graph, numberOfProblems);
    
    int min_cut = INF;
	for(unsigned int i = 0; i < criticalPoints.size(); i++){
		for(unsigned int j = 0; j < criticalPoints[i].size(); j++){
			graph.getNode(criticalPoints[i][j])->critical = true;
		}		
		for(unsigned int j = 0; j < criticalPoints[i].size(); j++){
			cut = (int)bfs(graph, numberOfNodes, (int)criticalPoints[i].size(), criticalPoints[i][j]);
			if(!cut){
				min_cut = 0;
				break;
			}
			else{ 
				min_cut = std::min(min_cut, cut);
                for(unsigned int j = 0; j < graph.nodes.size(); j++){
                    graph.getNode(j)->discovered = false;
                }
            }
		}
		printf("%d\n",min_cut);

		for(unsigned int j = 0; j < graph.nodes.size(); j++){
            graph.getNode(j)->discovered = false;
            graph.getNode(j)->critical = false;
        }
		min_cut = INF;
	}
}