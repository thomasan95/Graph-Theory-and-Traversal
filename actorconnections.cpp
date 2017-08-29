/*
 * actorconnections.cpp
 * Author: Thomas An
 * Date:   11-17-16
 *
 * contains main method for ActorGraph, taking in arguments and
 * outputting a file with when two actors became connected
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <string.h>

#include "ActorGraph.h"
#include "DisjointSet.h"

using namespace std;
/** argv[1] is movies file
*   argv[2] is actor pairs file
*   argv[3] is outfile file
*   argv[4] is bfs or union find boolean
*/
int main(int argc, char* argv[]) {
    if(argc != 5) {
        cout << "Need 4 arguments! Given " << argc-1 << endl;
        return -1;
    }
    //false means use bfs
    bool bfs_or_uf = false;
    if(strncmp(argv[4], "ufind", 5) == 0) {
        bfs_or_uf = true;
    }

    ActorGraph actorgraph;
    actorgraph.loadFromFile(argv[1]);

    DisjointSet disjointset;
    disjointset.loadFromFile(argv[1]);

    //initializing input and outfit streams
    ifstream in(argv[2]);
    ofstream out(argv[3]);
    bool have_header = false;
    out << "Actor1" << '\t' << "Actor2" << '\t' << "Year" << endl;
    while (in) {
        vector<pair<int, string>> path;
        string s;

        if (!getline( in, s )) break;
        
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
        if(bfs_or_uf) {
            disjointset.initializeNodes();
        } else {
            actorgraph.initializeGraph();
        }
        vector<int> years = actorgraph.getyearsascending();

        bool connected = false;
        int year = 0;
        
        /* 
        * loop will keep adding in movies year by year until a connection
        * between the actors are made. For example, all movies form 1991
        * will be added and then BFS will be used to find a connection,
        * if theres no connection then movies from 1992 will be added
        * then repeat.
        * Once a connection has been found, breaks from all loops and goes to print 
        */

        while(!connected) {
            for(int i = 0; i<years.size(); i++) {
                year = years[i];
                if(bfs_or_uf) {
                    disjointset.addMovies(year);
                    if(disjointset.find(actor_name1) == disjointset.find(actor_name2)) {
                        connected = true;
                    }
                } else {
                    actorgraph.addGraph(year);
                    connected = actorgraph.BFSTraverseActor(actor_name2, actor_name1);
                }
                if(connected) break;
            }
            if(connected) break;
        }
        if(!connected) {
            out << actor_name1 << '\t' << actor_name2 << '\t' << "9999" << endl;
        } else {
            out << actor_name1 << '\t' << actor_name2 << '\t' << year << endl;
        }
    }

    if (!in.eof()) {
        cerr << "Failed to read " << argv[3] << "!\n";
        return false;
    }
    in.close();
    out.close();
    return 1;
}
