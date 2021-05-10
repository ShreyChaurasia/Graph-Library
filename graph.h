#ifndef graph_h
#define graph_h

#include<vector>
#include<queue>
#include<stack>
#include<iostream>
using namespace std;

class Graph{
private:
	// made private because it is helping the dfs traversal but is not directly used by the
	// user.
	void dfsHelper(int, vector <int> &nodesDiscoverOrder, vector <int> &visited);
	// not used by user directly but used by function numOfConnectedComponents
	void numOfConnectedComponentsHelper(int source, vector <int> &visited);
	// not used by user directly but used by isBipartite function
	bool isBipartiteHelper(int source, vector <int> &visited, vector <int> &color);
public:
    // constructor to initialize the number of nodes and adjacencyList of a graph
    // since it is same for both directed and undirected so it is defined in base class.
    Graph(int number){
    	numOfNodes = number;
    	numOfEdges = 0;
    	// initialzing the adjacency list of size number + 1
    	adjacencyList = vector <vector <pair <int,int> > >(number + 1);
    }

	// virtual function is used to add an edge from x--y of weight w
	// since we need to add edges in different ways in directed and undirected graph
	// so this is made virtual
	virtual void addEdge(int x, int y, int w) = 0;
    // method for finding cycles is different in directed and undirected graph so
    // it is defined as a virtual function
	virtual bool isCyclic() = 0;
	// method to return the number of nodes in a graph
	int getNumberOfNodes(){
		return numOfNodes;
	}
	// gives the number of current edges in a graph
	int getNumberOfEdges(){
		return numOfEdges;
	}
	// this method gives the number of connected components 
	int getNumOfConnectedComponents();
    // tells if a graph is bipartite
    bool isBipartite();
    // this function checks if a graph is a tree or not
    // different for directed and undirected graph so
    // defined as a pure virtual method
    virtual bool isTree() = 0;
	// this method is used to traverse any graph in bfs fashion
	// same for every graph type so defined in base class
	vector <int> breadthFirstTraversal(int source);
	// this method is used to traverse any graph in dfs fashion
	// same for every graph type so defined in base class
	vector <int> depthFirstTraversal(int source);
	// this method gives the number of strongly connected components
	// different for directed and undirected graphs so defined as a pure virtual method
	virtual int getNumOfStronglyConnectedComponents() = 0;
protected:
	// current number of edges in a graph
	int numOfEdges;
	// number of nodes in a graph
	int numOfNodes;
	// pair will store the vertex number and weight
	vector <vector <pair <int,int> > > adjacencyList;
};


bool Graph :: isBipartiteHelper(int source, vector <int> &visited, vector <int> &color){
	visited[source] = 1;

	for(pair <int, int> temp : adjacencyList[source]){
		int currNode = temp.first;
		if(!visited[currNode]){
			color[currNode] = (color[source] + 1) % 2;
			if(!isBipartiteHelper(currNode, visited, color)){
				return false;
			}
		}
		else if(visited[currNode]){
			if(color[currNode] == color[source]){
				return false;
			}
		}
	}
	return true;
}


bool Graph :: isBipartite(){
	vector <int> color(numOfNodes + 1, -1);
	vector <int> visited(numOfNodes + 1, 0);

	for(int node = 1; node <= numOfNodes; ++node){
		if(!visited[node]){
			color[node] = 0;
			if(!isBipartiteHelper(node, visited, color)){
				return false;
			}
		}
	}
	return true;
}


void Graph :: numOfConnectedComponentsHelper(int source, vector <int> &visited){
	visited[source] = 1;
	for(pair <int, int> temp : adjacencyList[source]){
		int currNode = temp.first;
		if(!visited[currNode]){
			numOfConnectedComponentsHelper(currNode, visited);
		}
	}
}


int Graph::getNumOfConnectedComponents(){
	int countConnectedComponents = 0;
	vector <int> visited(numOfNodes + 1, 0);
	for(int node = 1; node <= numOfNodes; ++node){
		if(!visited[node]){
			countConnectedComponents++;
			numOfConnectedComponentsHelper(node, visited);
		}
	}
	return countConnectedComponents;
}

// assumed that the graph is connected
vector <int> Graph::breadthFirstTraversal(int source){
	vector <int> nodesDiscoverOrder;
	vector <int> visited(numOfNodes + 1, 0);
	queue <int> bfsTraverser;
    
    bfsTraverser.push(source);
    visited[source] = 1;

    while(!bfsTraverser.empty()){
    	int currNode = bfsTraverser.front();
    	bfsTraverser.pop();
    	nodesDiscoverOrder.push_back(currNode);
        
        for(pair <int,int> temp : adjacencyList[currNode]){
        	int neighbour = temp.first;

        	if(!visited[neighbour]){
        		bfsTraverser.push(neighbour);
        		visited[neighbour] = 1;
        	}
        }
    }

    return nodesDiscoverOrder;
}


void Graph :: dfsHelper(int source, vector <int> &nodesDiscoverOrder, vector <int> &visited){
	
	visited[source] = 1;
	
	nodesDiscoverOrder.push_back(source);

	for(pair <int,int> temp : adjacencyList[source]){
		int currNode = temp.first;
		if(!visited[currNode]){
			dfsHelper(currNode, nodesDiscoverOrder, visited);
		}
	}

}
 
// assumed that the graph is connected
vector <int> Graph::depthFirstTraversal(int source){
	vector <int> nodesDiscoverOrder;
	vector <int> visited(numOfNodes + 1, 0);
	dfsHelper(source, nodesDiscoverOrder, visited);
	return nodesDiscoverOrder;
}



#endif