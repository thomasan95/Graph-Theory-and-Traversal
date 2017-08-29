/*
 * pathfinder.cpp
 * Author: Thomas An
 * Date:   11-17-16
 *
 * contains main method for ActorGraph, taking in arguments and
 * outputting a file with list of shortest paths
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>


#include "ActorGraph.h"
using namespace std;

int main(int argc, char* argv[]) {
    if(argc != 5) {
        cout << "Need 4 arguments! Given " << argc-1 << endl;
        return -1;
    }
    string movie_casts_f = argv[1];
    string u_or_w = argv[2];
    string actor_pair_f = argv[3];
    string outFile = argv[4];
    bool weighted_edges = false;
    if(u_or_w == "w") {
        weighted_edges = true;
    }

    ActorGraph actorgraph;
    actorgraph.loadFromFile(argv[1]);
    actorgraph.createGraph();

    //initializing input and outfit streams
    ifstream in(argv[3]);
    ofstream out(argv[4]);
    //creating header for file
    out << "(actor)--[movie#@year]-->(actor)--..." << endl;
    bool have_header = false;
    //takes in the input file and parses through it getting start actor and 
    //end actor
    while (in) {
        vector<pair<int, string>> path;
        string s;
        //cout << s << endl;
        // get the next line
        //cout << "hi" << endl;
        if (!getline( in, s )) break;
        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }
        //cout << s << endl;

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;

            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;
            //cout << next << endl;
            record.push_back( next );
        }
        //cout << "record size " << record.size() << endl;
        if (record.size() != 2) {
            // we should have exactly 2 columns
            continue;
        }

        string actor_name1(record[0]);
        string actor_name2(record[1]);
        //cout << "hi" << endl;
        if (weighted_edges) {
            path = actorgraph.dijkstra(actor_name1, actor_name2);
        } else {
            path = actorgraph.BFSTraverse(actor_name1, actor_name2);
        }
        //The for loop and if statements handle the printing. the last print
        //is outside because the formating for the last print is slightly different
        for(int j = 0; j < path.size()-1; j++) {
            if(path[j].first == 1) {
                out << "(" << path[j].second << ")" << "--";
            } else {
                out << "[" << path[j].second << "]" << "-->";
            }
        }
        if(path[path.size()-1].first == 1) {
            out << "(" << path[path.size()-1].second << ")";
        } else {
            out << "[" << path[path.size()-1].second << "]";
        }
        out << endl;

    }

    if (!in.eof()) {
        cerr << "Failed to read " << argv[3] << "!\n";
        return false;
    }
    in.close();
    out.close();
    //cout << "ending" << endl;
    return 1;
}