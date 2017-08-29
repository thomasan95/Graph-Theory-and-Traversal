/*
 * ExtensionGraph.cpp
 * Author: Thomas An
 * Date:   11-18-16
 *
 * Containers implementation files for ExtensionGraph
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
#include <stack>
#include "ExtensionGraph.h"

using namespace std;
/* loadfromFile method takes in an input file and parses by file into strings
*  Param:
*    in_filename: file to be parsed through
*  Return:
*     returns true is file correctly loaded, else returns false
*/
bool ExtensionGraph::loadFromFile(const char* in_filename) {
     // Initialize the file stream
    ifstream infile(in_filename);

    //unordered_map<string, vector<string>> graph1;
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
        // get the next line
        if (!getline( infile, s )) break;
        istringstream ss( s );
        vector <string> record;
        while (ss) {
            string next;
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, ' ' )) break;
            record.push_back( next );
        }
        if (record.size() != 2) {
            // we should have exactly 2 columns
            continue;
        }
        int user1 = stoi(record[0]);
		int user2 = stoi(record[1]);
		//cout << user1 << ' ' << user2 << endl;
        graph1[user1].push_back(user2);

    }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    /*
    auto itr = graph1.begin();
    for(; itr != graph1.end(); itr++) {
    	cout << itr->first << " storing: ";
    	for(int i = 0; i < itr->second.size(); i++) {
    		cout << itr->second[i] << ", ";
    	}
    	cout << endl;
    }
    */
    infile.close();
    return true;
}
/* loadfromFile method takes in an input file and parses by file into strings
*  Conceptually does the same as first loadfromfile but swaps columns
*  Param:
*    in_filename: file to be parsed through
*  Return:
*     returns true is file correctly loaded, else returns false
*/
bool ExtensionGraph::loadFromFileTrans(const char* in_filename) {
     // Initialize the file stream
    ifstream infile(in_filename);

    //unordered_map<string, vector<string>> graph1;
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
        // get the next line
        if (!getline( infile, s )) break;
        istringstream ss( s );
        vector <string> record;
        while (ss) {
            string next;
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, ' ' )) break;
            record.push_back( next );
        }
        if (record.size() != 2) {
            // we should have exactly 2 columns
            continue;
        }
        /* swapping the two columns and storing them in record */
        int user2 = stoi(record[0]);
		int user1 = stoi(record[1]);
        graph2[user1].push_back(user2);

    }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }

    infile.close();
    return true;
}
/* Creates the graph. Must be called after loadfromfile
*/
void ExtensionGraph::createGraph() {
	auto useritr = graph1.begin();
	int twitterid1 = 0;
	Node* user2id = NULL;
	for (; useritr != graph1.end(); useritr++) {
		twitterid1 = useritr->first;
		//cout << "graph1 contains " << twitterid1 << endl;
		auto usercheck = id_to_node.find(twitterid1);
		Node* user1id = NULL;

		if(usercheck == id_to_node.end()) {
			user1id = new Node(twitterid1);
			user1id->visited = false;

			id_to_node[twitterid1] = user1id;
			//cout << "pushing " << user1id->data << " onto vertices ";
			//cout << " is a new node " << endl;
			vertices.push(user1id->data);
		} else {
			user1id = id_to_node[twitterid1];
			//cout <<  "alr exists ";
			//cout << user1id->data << " with " << twitterid1 << endl;
		}
		//cout << user1id->data << " with retweets: ";

		vector<int> retweetids= useritr->second;
		for(int i = 0; i < retweetids.size(); i++) {
			//cout << "retweets size for " << user1id->data << " is " << retweetids.size() << endl; 
			int friendID = retweetids[i];
			//cout << friendID << endl;
			usercheck = id_to_node.find(friendID);
			if(usercheck == id_to_node.end()) {
				user2id = new Node(friendID);
				user2id->visited = false;
				id_to_node[friendID] = user2id;
				vertices.push(user2id->data);
			} else {
				user2id = id_to_node[friendID];
			}
			user1id->retweets.push_back(user2id);
			//cout << user2id->data << " onto user1id " << user1id->data << endl;;
		}
		//for (int j=0; j<user1id->retweets.size(); j++) {
			//cout << user1id->retweets[j]->data << " ";
		//}
		//cout << endl;

	}
}
/* Creates the graph. Must be called after loadfromfiletrans
*/
void ExtensionGraph::createGraphTrans() {
	auto useritr = graph2.begin();
	int twitterid1 = 0;
	Node* user2id = NULL;
	for (; useritr != graph2.end(); useritr++) {
		twitterid1 = useritr->first;
		auto usercheck = id_to_node2.find(twitterid1);
		Node* user1id;

		if(usercheck == id_to_node2.end()) {
			user1id = new Node(twitterid1);
			user1id->visited = false;

			id_to_node2[twitterid1] = user1id;
			vertices.push(user1id->data);
		} else {
			user1id = id_to_node2[twitterid1];
		}
		//cout << user1id->data << " with retweets: ";
		vector<int> retweetids= useritr->second;
		for(int i = 0; i < retweetids.size(); i++) {
			int friendID = retweetids[i];
			usercheck = id_to_node2.find(friendID);
			if(usercheck == id_to_node2.end()) {
				user2id = new Node(friendID);
				user2id->visited = false;
				id_to_node2[friendID] = user2id;
			} else {
				user2id = id_to_node2[friendID];
			}
			user1id->retweets.push_back(user2id);
			//cout << user2id->data << " ";
		}
		//cout << endl;
	}

}
/* Calls DFSsearch on the extensiongraph object's graph. File then 
*  calls DFSvisit on each unvisited node.
*  Return:
*     returns stack in which the nodes were finished, in reverse
*     order
*/
stack<int> ExtensionGraph::DFSsearch() {
	set<Node*> visited;
	stack<int> finished;
	int idtrack = 0;
	//cout << vertices.size() << "v size" << endl;
	while(!vertices.empty()) {
		//cout << vertices.size() << "v size" << endl;
		idtrack = vertices.top();
		vertices.pop();
		Node* v = id_to_node[idtrack];
		/*
		cout << v->data << " w retweets ";
		for (int i = 0; i < v->retweets.size(); i++) {
			cout << v->retweets[i]->data << ": ";
		}
		cout << endl;
		*/
		auto visitcheck = visited.find(v);
		//cout << "finding in visited " << v->data << endl;
		if(visitcheck == visited.end()) {
			//cout << " not found in visited, moving into visited " << v->data << endl;
			v->visited = true;
			//visited.insert(v);
			//cout << "calling DFSvisit  with " << v->data << " " << visited.size() << endl;
			DFSvisit(v, visited, finished);
		}
	}
	//stack<int> f = getfinished();
	//cout << "fin size" << f.size() << endl;
	return finished;
}
/* Calls DFSvisit on node passed in from DFSsearch
* Method works by recursively calling a visit on each of the nodes
* neighbors and only breaks out when there are no neighbors left unvisited
* Param:
*   v: Node that is currently being visited
*   visited: passed by reference, so modify visited from previous method. push on
*      nodes when visited.
*   finished: keeps track of which nodes have been done being searched
*/
void ExtensionGraph::DFSvisit(Node* v, set<Node*>& visited, stack<int>& finished) {
	visited.insert(v);
	//cout << v->data << endl;
	//cout << "recursion DFSvisit normal" << " data is " << v->data << endl;
	for(int i = 0; i < v->retweets.size(); i++) {
		Node* n = v->retweets[i];
		//cout << "using " << n->data << endl;
		if(!(n->visited)) {
			n->visited = true;
			//cout << "caling DFSVisit on " << n->data << endl;
			DFSvisit(n, visited, finished);
		}
	}
	//cout << "pushing onto finished " << v->data << endl;
	finished.push(v->data);
}
/* Calls DFSserch on the extensiongraph object's graph. File then 
*  calls DFSvisit on each unvisited node.
*  THIS METHOD IS FOR TRANSPOSED GRAPH
*  Return:
*     returns an unordered_map with the SCC's grouped as value, and
*     keyed by an iterating index
*/
unordered_map<int, vector<int>> ExtensionGraph::DFSsearch(stack<int> vertexes) {
	//cout << "something" << endl;
	set<Node*> visited1;
	auto set_false = visited1.begin();
	
	for(;set_false!=visited1.end(); set_false++) {
		Node* set = *set_false;
		set->visited = false;
		//cout << "setting false" << endl;
		for(int x = 0; x<set->retweets.size(); x++) {
			set->retweets[x]->visited = false;
		//	cout<<"setting retweets false" << endl;
		}
	}
	int idtrack = 0;
	int scctrack = 0;
	unordered_map<int,vector<int>> scc;
	while(!vertexes.empty()) {
		idtrack = vertexes.top();
		vertexes.pop();
		Node* v = id_to_node2[idtrack];
		auto visitcheck = visited1.find(v);
		if(visitcheck == visited1.end()) {
			v->visited = true;
			DFSvisit(v, visited1, scc, scctrack);
		}
		scctrack = scctrack+1;
	}
	return scc;
}
/* Calls DFSvisit on node passed in from DFSsearch
* Method works by recursively calling a visit on each of the nodes
* neighbors and only breaks out when there are no neighbors left unvisited
*
* THIS METHOD IS FOR TRANSPOSTED GRAPH
*
* Param:
*   v: Node that is currently being visited
*   visited1: passed by reference, so modify visited from previous method. push on
*      nodes when visited.
*   scc: keeps track of and maps the SCC's
*   scctrack: which index, or key, that is mapped to right now
*/
void ExtensionGraph::DFSvisit(Node* v, set<Node*>& visited1, 
	unordered_map<int, vector<int>>& scc, int scctrack) {
	visited1.insert(v);
	for(int i = 0; i < v->retweets.size(); i++) {
		Node* n = v->retweets[i];
		if(!(n->visited)) {
			n->visited = true;
			DFSvisit(n, visited1, scc, scctrack);
		}
	}
	scc[scctrack].push_back(v->data);
}
/* deletes all nodes, part of destructor */
void ExtensionGraph::deleteAll1() {
    auto itr = id_to_node.begin();
    for(; itr!=id_to_node.end(); itr++) {
        Node* n1 = itr->second;
        delete(n1);
    }
}
/* deletes all nodes, part of destructor */
void ExtensionGraph::deleteAll2() {
    auto itr2 = id_to_node2.begin();
    for(; itr2!=id_to_node2.end(); itr2++) {
        Node* n1 = itr2->second;
        /*vector<Node*> retweet = n1->retweets;
        for(int i = retweet.size()-1; i >= 0; i--) {
            Node* n2 = retweet[i];
            delete(n2);
        }*/
        delete(n1);
    }
}
/* destructor */
ExtensionGraph::~ExtensionGraph() {
	deleteAll1();
	deleteAll2();
}
