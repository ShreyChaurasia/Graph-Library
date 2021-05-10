#ifndef UNDIRECTEDGRAPH_H
#define UNDIRECTEDGRAPH_H

#include "graph.h"


class UndirectedGraph : public Graph{
private:
	// not directly used by the user but used by isCyclic method.
	// so made private
	bool checkCycle(int source, vector <int> &visited, int parent);
public:
	// constructor to initialize the number of nodes in Graph class
	// as it is same for both directed and undirected
	UndirectedGraph(int number) : Graph(number){

	}
	// overridden the addEdge pure virtual method in base class Graph
	virtual void addEdge(int x, int y, int w = 1){
		adjacencyList[x].push_back({y, w});
		adjacencyList[y].push_back({x, w});
		numOfEdges++;
	}
	// overriden the isCyclic pure virtual method in base class Graph
	virtual bool isCyclic();
	// overriden the isTree pure virtual method in base class Graph
	virtual bool isTree();
	// overriden the getNumOfStronglyConnectedComponents in base class Graph
	virtual int getNumOfStronglyConnectedComponents();
};

// strongly connected is same as connected in undirected graphs
int UndirectedGraph :: getNumOfStronglyConnectedComponents(){
	return Graph :: getNumOfConnectedComponents();
}

bool UndirectedGraph :: isTree(){
	if((numOfEdges + 1) != numOfNodes){
		return false;
	}
	int count = Graph :: getNumOfConnectedComponents();
	if(count > 1){
		return false;
	}
	return true;
}


bool UndirectedGraph :: checkCycle(int source, vector <int> &visited, int parent){
	visited[source] = 1;

	for(pair <int,int> temp : adjacencyList[source]){
		int currNode = temp.first;
		if(!visited[currNode]){
			if(checkCycle(currNode, visited, source)){
				return true;
			}
		}
		else if(visited[currNode]){
			if(currNode != parent){
				return true;
			}
		}
	}
	return false;
}

bool UndirectedGraph :: isCyclic(){
	vector <int> visited(numOfNodes + 1, 0);
	int parent = 0;
	for(int node = 1; node <= numOfNodes; ++node){
		if(!visited[node]){
			if(checkCycle(node, visited, parent)){
				return true;
			}
		}
	}
	return false;
}

#endif 