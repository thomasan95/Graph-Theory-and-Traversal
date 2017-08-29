/*
 * DisjoinSet.cpp
 * Author: Thomas An
 * Date:   11-18-16
 *
 * Containers implementation files for Unionfind
 */
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <queue>
#include <map>
#include <set>
#include <string>

#include "DisjointSet.h"

using namespace std;
/* Method for parsing design file
*  Param:
*    in_filename: file to parse from
*  Return:
*     bool: true if succesfully parsed and false if not
*/
bool DisjointSet::loadFromFile(const char* in_filename) {
     // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
    //unordered_map<string, vector<string>> graph1;
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }
        istringstream ss( s );
        vector <string> record;
        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
    
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);
        //combines string to form a unique movie title key
        string movie_string = record[1] + "#@" + record[2];

        graph1[actor_name].push_back(movie_string);

        graph2[movie_string].push_back(actor_name);

        bool movie_dne = true;
        //Don't want duplicate  movies, so only add if movie DNE inside map.
        //AKA, if no key (year) maps to value (movie)
        if(year_to_movie[movie_year].find(movie_string) == 
        	year_to_movie[movie_year].end()) {
            year_to_movie[movie_year].insert(movie_string);
        }
    }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
    return true;
}
/* Method for initializing all the nodes
*  Initializes as in creates all nodes and maps to them
*/
void DisjointSet::initializeNodes() {
    auto graph1itr = graph1.begin();
    actor_to_node.clear();
    for(; graph1itr != graph1.end(); graph1itr++) {
        string actor_name = graph1itr->first;
        Node* n = new Node(actor_name);
        actor_to_node[actor_name] = n;
    }
}
/* Method for adding in movies year by year. It uses the map 
* year_to_movie to find a set of movies that has movies
* from that year
*  Param:
*    year: What year of movies to add in
*/
void DisjointSet::addMovies(int year) {
    set<string> movies = year_to_movie[year];
    auto movie_itr = movies.begin();
    //Slowly iterate through to find movies individually
    for(; movie_itr != movies.end(); movie_itr++) {
        string movie_name = *movie_itr;
        vector<string> cast = graph2[movie_name];
        if(cast.empty()) continue;
        string actor1 = cast[0];
        //For loop merges all the actors inside the cast to each other
        for(int i =0; i<cast.size(); i++) {
            if(i == 0) continue;
            string actor2 = cast[i];
            merge(actor1, actor2);
        }
    }
}
/* Method for traversal through the disjoint set and finding an actor
*  It calls itself recursively, going up the 'up-tree'
*  Param:
*    actor: actor to find
*  Return:
*    string: the name stored in sentinal node.
*/
string DisjointSet::find(string actor) {
    DisjointSet::Node* node = actor_to_node[actor];
    if (node->name != node->parent->name ) {
        node->parent->name = find(node->parent->name);
    }
    return node->parent->name;
}
/* Method for merging two disjoint sets
*  Param:
*    actor1: actor to merge
*	 actor2: actor to merge
*/
void DisjointSet::merge(string actor1, string actor2) {

    actor1 = find(actor1);
    actor2 = find(actor2);
    //create the node pointers
    Node* actorN1 = actor_to_node[actor1];
    Node* actorN2 = actor_to_node[actor2];
    //this is merging the small tree with the bigger one
    if(actorN1->rank > actorN2->rank) {
        actorN2->parent = actorN1;
        actorN1->rank = actorN1->rank + 1;
    } else {
        actorN1->parent = actorN2;
        if(actorN1->rank == actorN2->rank) {
            actorN2->rank = actorN2->rank + 1;
        }
    }
}
/* Method for deleting all nodes created on the heap
*/
void DisjointSet::deleteAll() {
    auto itr = actor_to_node.begin();
    for(; itr!=actor_to_node.end(); itr++) {
        Node* n = itr->second;
        delete(n);
    }
}
/* Destructor for class */
DisjointSet::~DisjointSet(){
	deleteAll();
}
