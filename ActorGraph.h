/*
 * ActorGraph.h
 * Author: Thomas An
 * Date:   11-18-16
 *
 * Containers header files for ActorGraph
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <limits>
#include <vector>
#include <unordered_map>
#include <list>
#include <queue>
#include <map>
#include <set>

using namespace std;

class Edge;
//Creating Node Class
class Node {

private:
    string name;
    vector<Edge*> edges;
    int distance;
    bool done;

public:
    Node* prev;
    Edge* preve;

    Node(string actorname): name(actorname), edges(), distance(-1), prev(NULL), preve(NULL),
    done(false) {
    }
    //set methods
    void addEdge(Edge* movie_edge);
    void setDistance(int distance);
    void markDone(bool);

    //getter methods
    vector<Edge*> getEdges();
    int getDistance() const;
    string getName();
    bool isDone();
};

//Creating edge class
class Edge {
private:

public:
    string name;
    int weight;
    Node* adjNode;
    int year;
    Edge(string moviename, Node* adjNode1, int weight_year): name(moviename),
    adjNode(adjNode1), weight(weight_year), year(0) {
    }

};
class ActorGraph {
protected:
    

private:
    //graph1 maps actors as keys and to a list of movies they've performed in
    unordered_map<string, vector<string>> graph1;
    //graph2 maps movies as keys and to a list of actors that are cast in them
    unordered_map<string, vector<string>> graph2; 

    unordered_map<string, int> movie_to_year;

    //make key actor, value movie
    unordered_map<string, Node*> actor_to_node;

    map<int, set<string>> year_to_movie;


public:
    ActorGraph(void);

    // Maybe add some more methods here
  
    /** times in input file and parses the code to get strings individually
     */
    bool loadFromFile(const char* in_filename);
    //Creates the graph, must be called after loadFromFile
    void createGraph();
    void addGraph(int year);
    void initializeGraph();
    //Does BFS across the nodes, and returns a vector pair with shortest path
    vector<pair<int, string>> BFSTraverse(string start, string end);
    bool BFSTraverseActor(string start, string end);

    vector<pair<int, string>> dijkstra(string start, string end);
    vector<int> getyearsascending();

    void deleteAll();

	~ActorGraph();
};

#endif // ACTORGRAPH_H