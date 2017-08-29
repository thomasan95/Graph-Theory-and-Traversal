/*
 * DisjointSet.h
 * Author: Thomas An
 * Date:   11-18-16
 *
 * Containers header files for Unionfind
 */

#ifndef DISJOINTSET_H
#define DISJOINTSET_H

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <unordered_map>
#include <limits>
#include <set>

using namespace std;

class DisjointSet {
public:
    class Node {
    public:
        string name;
        int rank;
        int index;
        Node* parent;

        Node(string actor_name): name(actor_name), rank(0), 
            index(0), parent(this) {}
    };
    bool loadFromFile(const char* in_filename);

    vector<Node*> nodes;
    //find and merge methods
    string find(string actor);
    void merge(string actor1, string actor2); 
    void initializeNodes();
    void addMovies(int year);
    void deleteAll();
    ~DisjointSet();

private:
    /**
    * graph1 maps actors as keys and to a list of movies they've performed in
    * graph2 maps movies as keys and to a list of actors that are cast in them
    */
    unordered_map<string, vector<string>> graph1;
    unordered_map<string, vector<string>> graph2; 

    //make key actor, value movie
    unordered_map<string, Node*> actor_to_node;
    map<int, set<string>> year_to_movie;

};

#endif // DISJOINTSET_H