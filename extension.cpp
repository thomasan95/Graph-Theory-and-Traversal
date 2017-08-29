/*
 * extension.cpp
 * Author: Thomas An
 * Date:   11-17-16
 *
 * Returns SCC of Twitter Data: 
 * Problem solved is how much a single tweet could travel
 * or how many people could possibly view it through retweets
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <stack>

#include "ExtensionGraph.h"

using namespace std;
/** argv[1] is twitter data to analyze
*/
int main(int argc, char* argv[]) {

    /*Create 2 ExtensionGraph objects.
    * One for normal Graph the other for transposed*/
    ExtensionGraph extensiongraph;
    extensiongraph.loadFromFile(argv[1]);
    extensiongraph.createGraph();
    stack<int> finished1 = extensiongraph.DFSsearch();

    ExtensionGraph extensiongraph2;
    extensiongraph2.loadFromFileTrans(argv[1]);
    extensiongraph2.createGraphTrans();

    unordered_map<int, vector<int>> scc = extensiongraph2.DFSsearch(finished1);
    int max = 0;
    auto itr = scc.begin();
    for (; itr != scc.end(); itr++) {
    	int temp = itr->second.size();
    	if(temp > max) {
    		max = temp;
    	}
    }
   	cout << "The largest SCC of the DATA is: " << max << endl;

    return 1;
}
