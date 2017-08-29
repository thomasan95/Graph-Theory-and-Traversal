/*
 * ActorGraph.cpp
 * Author: Thomas An
 * Date:   11-17-16
 *
 * ActorGraph contains implementation for ActorGraph.h
 */ 
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <deque>
#include <map>
#include <set>

#define CURR_YEAR 2015

#include "ActorGraph.h"

using namespace std;

/* setter method for distance
*  Param:
*    distance: edge to set distance
*/
void Node::setDistance(int distance1) {
    distance = distance1;
}
/* setter method for edges
*  Param:
*    movie_edge: edge to add
*/
void Node::addEdge(Edge* movie_edge) {
    edges.push_back(movie_edge);
}
/* setter method for done
*  Param:
*    don: to set
*/
void Node::markDone(bool don) {
    done = don;
}
/* getter method for edges
*  Return:
*    done: to show done or not
*/
bool Node::isDone() {
    return done;
}
//return method for distance
int Node::getDistance() const{
    return distance;
}
//return method for edges
vector<Edge*> Node::getEdges() {
    return edges;
}
//return method for names
string Node::getName() {
    return name;
}

ActorGraph::ActorGraph(void) {}
/* loadfromFile method takes in an input file and parses by file into strings
*  Param:
*    in_filename: file to be parsed through
*    use_weighted_edges: use weights on edges or not (checkpoint uses unweighted)
*  Return:
*     returns true is file correctly loaded, else returns false
*/
bool ActorGraph::loadFromFile(const char* in_filename) {
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
        string movie_string = record[1] + "#@" + record[2];

        graph1[actor_name].push_back(movie_string);
        graph2[movie_string].push_back(actor_name);

        movie_to_year[movie_string] = movie_year;
        bool movie_dne = true;
        if(year_to_movie[movie_year].find(movie_string) == 
            year_to_movie[movie_year].end()) {
            year_to_movie[movie_year].insert(movie_string);
            //cout << "inserting movie: " << movie_string << endl;
        }
    }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
    return true;
}
/** Create graph must be called after loadFromFile.
* Creates nodes from maps created in loadfromFile, and connects everything
* Will also get a mapping of actor to corresponding node
*/
void ActorGraph::createGraph() {
    //initialize variables needed
    string actor_name = "";
    string movie = "";

    auto iter = graph1.begin();

    int year = 0;
    /*Node* nodepoint;
    * using iterator, to loop through graph1
    * iter->first getting actor name
    * iter->second holding list of movies
    */
    for(; iter != graph1.end(); iter++) {

        actor_name = iter->first;
        Node* n = NULL;
        auto actor_check = actor_to_node.find(actor_name);
        if (actor_check == actor_to_node.end()) {
            n = new Node(actor_name);
            actor_to_node[actor_name] = n;
        } else {
            n = actor_to_node[actor_name];
        }
        //loop goes through movie vector, grabbing movies individually
        for(int i = 0; i < iter->second.size(); i++) {
            //sets movie string to movie at that index
            movie = iter->second[i];

            //grabs movie year;
            year = movie_to_year[movie];
            //loop uses movie stores, and access's vector of actors from graph2
            for (int k = 0; k < graph2[movie].size(); k++) {
                //grabs actor at 'k' index
                actor_name = graph2[movie][k];
                Node* temp = NULL;
                //check if the node already exists, if not then create new node
                //and point temp to that node
                if(actor_name == iter->first) {
                    continue;
                }
                if (actor_to_node.find(actor_name) == actor_to_node.end()) {
                    temp = new Node(actor_name);
                    //mapping actor to this nap in map 'actor_to_node'    
                    actor_to_node[actor_name] = temp;              
                } else {
                    //shows node 'found' or already exists, so just 
                    //sets temp to point to node
                    temp = actor_to_node[actor_name];
                }
                //Node* adjNode = &n2;
                //Creates edge pointer with string name, temp(adjNode), and year(weight)
                int edge_weight = 1 + (CURR_YEAR - year);
                Edge* e = new Edge(movie, temp, edge_weight);

                n->addEdge(e);
            }
        }
    }
}
/**
* Method used for actorconnections file. Initializes
* All nodes without any edges. Must be called after loadfromfile
* and must be called before addGraph(int year) can be used 
*/
void ActorGraph::initializeGraph() {

    auto graph1itr = graph1.begin();
    actor_to_node.clear();
    for(; graph1itr != graph1.end(); graph1itr++) {
        string actor_name = graph1itr->first;
        //cout << actor_name << endl;
        Node* n = new Node(actor_name);
        actor_to_node[actor_name] = n;
    }
}
/**
* Graph adding only edges that pertain to that year
* Param:
*   year: year that specifies which movies, or edges, to add
*/
void ActorGraph::addGraph(int year) {
    set<string> movies = year_to_movie[year];
    auto movies_itr = movies.begin();
    for(; movies_itr != movies.end(); movies_itr++) {

        string movie_name = *movies_itr;

        //cout << movie_name << endl;
        vector<string> cast = graph2[movie_name];
        for(int i=0; i<cast.size(); i++) {
            string actor_name = cast[i];
            Node* cast_node1 = actor_to_node[actor_name];
            for(int j=0; j<cast.size(); j++) {
                if(j == i) continue;
                string actor_name2 = cast[j];
                
                Node* cast_node2 = actor_to_node[actor_name2];
                Edge* e = new Edge(movie_name, cast_node2, year);
                cast_node1->addEdge(e);
            }
        }
    }
}
/* Method for traversal through the Graph. must be called after createGraph
*  Param:
*    start: Starting actor name
*    end: Ending actor name
*  Return:
*     returns a vector, with the indexes in order with actor to movie to actor
*     to movie to actor to movie and so on. The integer value in pair represents
*     whether the corresponding string is an actor or a movie.
*     1 for actor 2 for movie
*/
vector<pair<int, string>> ActorGraph::BFSTraverse(string start, string end) {

    queue<Node*> toExplore;
    auto itr = actor_to_node.find(start);
    Node* curr = 0;
    if(itr != actor_to_node.end()) {
        curr = itr->second;
        toExplore.push(curr);
    } else {
        cerr << "Actor does not exist" << endl;
        return {};
    }
    for (auto node : actor_to_node) {
        node.second->setDistance(-1);
        node.second->preve = NULL;
        node.second->prev = NULL;
    }
    //Node* curr = actor_to_node[start];
    while(!toExplore.empty()) {

        Node* next = toExplore.front();
        toExplore.pop();
        //next->markVisited();
        //grabs edges coming from current node
        vector<Edge*> edges = next->getEdges();
        //creates iterator to go through vector of edges
        //auto itr = edges.begin();
        //for loop iterating through edge vector
        for (int i = 0; i < edges.size(); i++) {
            Node* adjacent = edges[i]->adjNode;
            //setting a few node since -1 represents infinity or unvisited
            if(adjacent->getDistance() == -1) {
                adjacent->setDistance(next->getDistance() + 1);
                toExplore.push(adjacent);
                adjacent->prev = next;
                adjacent->preve = edges[i];
            }
            //setting new shortest path 
            else if((next->getDistance() + 1) < adjacent->getDistance()) {
                adjacent->setDistance(next->getDistance() + 1);
                adjacent->prev = next;
                adjacent->preve = edges[i];
            }
        }
    }
    auto itr2 = actor_to_node.find(end);
    Node* last;
    if(itr2 != actor_to_node.end()) {
        last = itr2->second;
    }
    vector<pair<int,string>> path;
    auto itr3 = path.begin();
    pair<int,string> toinsert;
    //Uses iterator to push to front, to get a forward order
    //while going backwards through the nodes
    while(last != curr) {
        toinsert = make_pair(1,last->getName());
        path.insert(itr3, toinsert);
        toinsert = make_pair(2,last->preve->name);
        itr3 = path.begin();
        path.insert(itr3, toinsert);
        last = last->prev;
        itr3 = path.begin();
    }
    toinsert = make_pair(1, last->getName());
    path.insert(itr3, toinsert);

    return path;

}
/* Method for traversal through the Graph. finding shortest fath
*  Param:
*    start: Starting actor name
*    end: Ending actor name
*  Return:
*     returns a vector, with the indexes in order with actor to movie to actor
*     to movie to actor to movie and so on. The integer value in pair represents
*     whether the corresponding string is an actor or a movie.
*     1 for actor 2 for movie
*/
vector<pair<int, string>> ActorGraph::dijkstra(string start, string end) {
    //priority_queue<Node*, std::vector<Node*>, std::greater<Node*> > pq;
    //using vector to act as priority queue
    vector<Node*> pq;
    auto itr = actor_to_node.find(start);
    Node* curr = 0;
    if(itr != actor_to_node.end()) {
        curr = itr->second;
        pq.push_back(curr);
    } else {
        cerr << "Actor does not exist" << endl;
        return {};
    }
    for (auto node : actor_to_node) {
        node.second->setDistance(-1);
        node.second->markDone(false);
        node.second->preve = NULL;
    }
    //initializing distance
    curr->setDistance(0);

    while (!pq.empty()) {

        int nodemin = pq[0]->getDistance();
        int nodeminindex = 0;

        Node* n = 0;
        //For loop  used to find the node inside the queue with the lowest weight
        for (int j = 0; j < pq.size(); j++) {
            if(pq[j]->getDistance() != -1 && pq[j]->getDistance() < nodemin) {
                nodemin = pq[j]->getDistance();
                nodeminindex = j;
            }
        }
        n = pq[nodeminindex];
        //when a node is popped, we know that its the shortest path to that node
        n->markDone(true);
        pq.erase(pq.begin() + nodeminindex);

        vector<Edge*> edges = n->getEdges(); 
        //For loop for calculating distance and everything and setting and
        //pushing onto the queue
        for (int i = 0; i < edges.size(); i++) {
            Node* next = edges[i]->adjNode;

            if(next->getDistance() == -1) {
                next->setDistance(n->getDistance() + edges[i]->weight);
                pq.push_back(next);
                next->prev = n;
                next->preve = edges[i];
            } else {
                int n_dist = n->getDistance() + edges[i]->weight;
                if (n_dist < next->getDistance()) {
                    next->setDistance(n_dist);
                    next->prev = n;
                    next->preve = edges[i];
                }
            }
        }
    }
    auto itr2 = actor_to_node.find(end);
    Node* last = 0;
    if(itr2 != actor_to_node.end()) {
        last = itr2->second;
    }

    vector<pair<int,string>> path;
    pair<int,string> toinsert;

    if (!last->isDone()) {
        cout << last->getName() << " not done" << endl;
    }
    while(last != curr) {
        toinsert = make_pair(1,last->getName());
        cout << last->getName() << endl;
        path.insert(path.begin(), toinsert);
        toinsert = make_pair(2,last->preve->name);
        cout << last->preve->name << endl;
        path.insert(path.begin(), toinsert);
        last = last->prev;
    }
    toinsert = make_pair(1, last->getName());
    path.insert(path.begin(), toinsert);
    cout << last->getName() << endl << endl;
    return path;
}
/* Method for traversal through the Graph. must be called after createGraph
*  Param:
*    start: Starting actor name
*    end: Ending actor name
*  Return:
*     true is there is a connection between start and end
*     false is there isnt a connection between start and end
*/
bool ActorGraph::BFSTraverseActor(string start, string end) {
    queue<Node*> toExplore;

    auto actor_itr = actor_to_node.begin();
    //For every call on BFSTraverseActor, resets previous pointers
    for(; actor_itr!=actor_to_node.end(); actor_itr++) {
        actor_itr->second->prev = NULL;
    }
    actor_itr = actor_to_node.find(start);
    Node* curr = NULL;

    if(actor_itr != actor_to_node.end()) {
        curr = actor_itr->second;
        //cout << "BFS Starting with " << actor_itr->first << endl;
        toExplore.push(curr);
    }

    while(!toExplore.empty()) {

        Node* n = toExplore.front();
        toExplore.pop();

        vector<Edge*> edges = n->getEdges();
        //for loop iterating through edge vector
        for (int i = 0; i < edges.size(); i++) {
            Node* next = edges[i]->adjNode;
            //setting a few node since -1 represents infinity or unvisited
            if(next->getDistance() == -1) {
                next->setDistance(n->getDistance() + 1);
                toExplore.push(next);
                next->prev = n;
            }
            else if((n->getDistance() + 1) < next->getDistance()) {
                next->setDistance(n->getDistance() + 1);
                next->prev = n;
            }
        }
    }

    actor_itr = actor_to_node.find(end);
    Node* last;
    if(actor_itr != actor_to_node.end()) {
        last = actor_itr->second;
    }

    //while going backwards through the nodes
    while(last != curr) {
        if(last->prev == NULL) {
            return false;
        }
        //cout << last->getName() << " -> " << last->prev->getName() << endl;
        last = last->prev;
    }
    if (last == curr) {
        return true;
    } else return false;
}

void ActorGraph::deleteAll() {
    auto itr = actor_to_node.begin();
    for(; itr!=actor_to_node.end(); itr++) {
        Node* n = itr->second;
        vector<Edge*> edges = n->getEdges();
        for(int i = edges.size()-1; i >= 0; i--) {
            Edge* e = edges[i];
            delete(e);
        }
        delete(n);
    }
}
/* Method for grabbing list of years
*  Return:
*     returns a vector, with years in which movies were made
*     in ascending order
*/
vector<int> ActorGraph::getyearsascending() {
    auto years_itr = year_to_movie.begin();
    vector<int> years;
    for(; years_itr != year_to_movie.end(); years_itr++) {
        years.push_back(years_itr->first);
    }
    return years;
}

ActorGraph::~ActorGraph() {
    deleteAll();
}

