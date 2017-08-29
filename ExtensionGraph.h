/*
 * ExtensionGraph.h
 * Author: Thomas An
 * Date:   11-18-16
 *
 * Containers header files for ExtensionGraph
 */

#ifndef EXTENSIONGRAPH_H
#define EXTENSIONGRAPH_H

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <unordered_map>
#include <limits>
#include <set>
#include <stack>

using namespace std;

class ExtensionGraph {
public:
    class Node {
    public:
    	int data;
    	bool visited;
    	vector<Node*> retweets;
    	Node(int number): data(number), visited(false), retweets() {
    	}
    };
    /* Create normal graph */
    bool loadFromFile(const char* in_filename);
    /* Create the transposed graph */
	bool loadFromFileTrans(const char* in_filename);

    /* Algorithm for  calling DFSsearch.
    */ 
    stack<int> DFSsearch();
    /* Same concept as previous DFSsearch except this one has different 
    *  return type and argument. Used on the transposed graph
    */
    unordered_map<int, vector<int>> DFSsearch(stack<int> vertices);

    void DFSvisit(Node* v, set<Node*>& visited, stack<int>& finished);
    void DFSvisit(Node* v, set<Node*>& visited, unordered_map<int, vector<int>>& scc, int track);
    /* creates either normal graph or transposed graph*/
    void createGraph();
    void createGraphTrans();

    void deleteAll1();
    void deleteAll2();

private:
	//userid retweet to id
    map<int, vector<int>> graph1;
    map<int, vector<int>> graph2;

    map<int, Node*> id_to_node;
    map<int, Node*> id_to_node2;
    stack<int> vertices;

public:
	~ExtensionGraph();
};

#endif // EXTENSIONGRAPH_H