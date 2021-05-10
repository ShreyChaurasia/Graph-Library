#ifndef DIRECTEDGRAPH_H
#define DIRECTEDGRAPH_H
#include "graph.h"


class DirectedGraph : public Graph{
private:
	// not directly used by the user but used by isCyclic method.
	// so made private
	bool checkCycle(int node, vector <int> &localVisited, vector <int> &globalVisited);
	// not directly used by the user but used in inDegree calculation by isTree()
	vector <int> inDegreeCalculation();
	// not directly used by the user but used by isTree
	bool isTreeHelper(int root, vector <int> &visited);
	// method used to produce the transpose of the graph
	vector <vector <pair <int, int> > > doTransposeGraph();
	// method to give the order of the vertices in which they should get traversed
	stack <int> doOrderToProcess();
	// method to help doOrderProcess method
	void doOrderProcessHelper(int currNode, vector <int> &visited, stack <int> &orderToProcess);
	// method used by getNumberOfStronglyConnectedComponents
	void getNumOfStronglyConnectedComponentsHelper(int currNode, vector <int> &visited, vector <vector <pair <int, int> > > &transposedGraph);
public:
	DirectedGraph(int number) : Graph(number){

	}
	// overridden the addEdge pure virtual method in base class Graph
	virtual void addEdge(int x, int y, int w = 1){
		adjacencyList[x].push_back({y, w});
		numOfEdges++;
	}

	// gives the number of strongly connected components using kosaraju's algorithm
	virtual int getNumOfStronglyConnectedComponents();
    // overriden the isCyclic pure virtual method in base class Graph
    virtual bool isCyclic();
    // overriden the isTree pure virtual method in base class Graph
    virtual bool isTree();
    // override the isBipartite method in base class Graph because we have to
    // change the directed to undirected one and then apply the same algorithm.
    bool isBipartite(){
    	cout<<"No implementation is provided for Directed Graphs"<<"\n";
    	return 0;
    }
};


vector <vector <pair <int,int> > > DirectedGraph :: doTransposeGraph(){
	vector <vector <pair <int, int> > > transposedGraph(numOfNodes + 1);
	for(int node = 1; node <= numOfNodes; ++node){
		for(pair <int, int> temp : adjacencyList[node]){
			int currNode = temp.first;
			int weight = temp.second;
			transposedGraph[currNode].push_back({node, weight});
		}
	}
	return transposedGraph; 
}


void DirectedGraph :: doOrderProcessHelper(int currNode, vector <int> &visited, stack <int> &orderToProcess){
	visited[currNode] = 1;
	for(pair <int, int> temp : adjacencyList[currNode]){
		int neighbour = temp.first;
		if(!visited[neighbour]){
			doOrderProcessHelper(neighbour, visited, orderToProcess);
		}
	}
	orderToProcess.push(currNode);
}

stack <int> DirectedGraph :: doOrderToProcess(){
	stack <int> orderToProcess;
	vector <int> visited(numOfNodes + 1, 0);
	for(int currNode = 1; currNode <= numOfNodes; ++currNode){
		if(!visited[currNode]){
			doOrderProcessHelper(currNode, visited, orderToProcess);
		}
	}
	return orderToProcess;
}


void DirectedGraph :: getNumOfStronglyConnectedComponentsHelper(int source, vector <int> &visited, vector <vector <pair <int, int> > > &transposedGraph){
	visited[source] = 1;
	for(int i = 0; i < transposedGraph[source].size(); ++i){
		pair <int, int> temp = transposedGraph[source][i];
		int currNode = temp.first;
		if(!visited[currNode]){
			getNumOfStronglyConnectedComponentsHelper(currNode, visited, transposedGraph);
	    }
	}
}

int DirectedGraph :: getNumOfStronglyConnectedComponents(){
	vector <vector <pair <int, int> > > transposedGraph = doTransposeGraph();
	stack <int> orderToProcess = doOrderToProcess();
	vector <int> visited(numOfNodes + 1, 0);
	int numOfStronglyConnectedComponents = 0;

	while(!orderToProcess.empty()){
		int currNode = orderToProcess.top();
		orderToProcess.pop();
		if(!visited[currNode]){
			numOfStronglyConnectedComponents++;
			getNumOfStronglyConnectedComponentsHelper(currNode, visited, transposedGraph);
		}
	}
	return numOfStronglyConnectedComponents;
}

vector <int> DirectedGraph :: inDegreeCalculation(){
	vector <int> inDegree(numOfNodes + 1, 0);
	for(int currNode = 1; currNode <= numOfNodes; ++currNode){
		for(pair <int,int> neighbour : adjacencyList[currNode]){
			inDegree[neighbour.first]++;
		}
	}
	return inDegree;
}

// checks if starting from the root you get any visited node
// this is basically cycle checking
bool DirectedGraph :: isTreeHelper(int source, vector <int> &visited){
	visited[source] = 1;
	for(pair <int, int> temp : adjacencyList[source]){
		int currNode = temp.first;
		if(visited[currNode]){
			return true;
		}
		else if(!visited[currNode]){
			if(isTreeHelper(currNode, visited)){
				return true;
			}
		}
	}
	return false; 
}

bool DirectedGraph :: isTree(){
	vector <int> inDegree = inDegreeCalculation();
	int inDegreeZeroCount = 0;
	int root = -1;
	for(int currNode = 1; currNode <= numOfNodes; ++currNode){
		if(!inDegree[currNode]){
			inDegreeZeroCount++;
			root = currNode;
		}
	}
	// zero represents that there is a cycle
	// greater than 1 represents that there is a node other than root which cannot be reached so
	// it is not connected so in both the cases the directed graph is not a tree 
	if(inDegreeZeroCount == 0 || inDegreeZeroCount > 1){
		return false;
	}
	vector <int> visited(numOfNodes + 1, 0);
	if(isTreeHelper(root, visited)){
		return false;
	}
	// checking if all the nodes are visited
	// again connected condition is checked
	for(int currNode = 1; currNode <= numOfNodes; ++currNode){
		if(!visited[currNode]){
			return false;
		}
	}
	return true;
}


bool DirectedGraph :: checkCycle(int source, vector <int> &localVisited, vector <int> &globalVisited){
	localVisited[source] = globalVisited[source] = 1;
	for(pair <int, int> temp : adjacencyList[source]){
		int currNode = temp.first;
		if(localVisited[currNode]){
			return true;
		}
		else if(!globalVisited[currNode]){
			if(checkCycle(currNode, localVisited, globalVisited)){
				return true;
			}
		}
	}
	localVisited[source] = 0;
	return false;
}

bool DirectedGraph :: isCyclic(){
	vector <int> localVisited(numOfNodes + 1, 0);
	vector <int> globalVisited(numOfNodes + 1, 0);
     
    for(int node = 1; node <= numOfNodes; ++node){
    	if(!globalVisited[node]){
    		if(checkCycle(node, localVisited, globalVisited)){
    			return true;
    		}
    	}
    }
    return false;
}

#endif