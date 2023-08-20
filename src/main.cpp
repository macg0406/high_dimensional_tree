/**
 * this is the test script for the project
 * 
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>

using namespace std;

#include "high_dimensional_tree.h"


float getRandomValue() {
    return (rand() % 10000 - 5000.0)/1000;
    // return rand() % 10000;
}

void testL2HighDimensionalTree() {
    // srand(time(NULL)); // initialize random seed
    srand(0);
    int dimension = 768;
    L2HighDimensionalTree tree(dimension);
    vector<vector<float> > data;
    vector<id_t> ids;
    for (int i=0;i<50000;i++) {
        vector<float> p;
        for (int d=0; d<dimension; d++) {
            p.push_back(getRandomValue());
        }
        data.push_back(p);
        ids.push_back(i);
    }
    clock_t start = clock();
    tree.construct(data, ids);
    clock_t finish_construct = clock();
    cout << "Construct time: " << (double)(finish_construct - start) / CLOCKS_PER_SEC << "s." << endl;
    vector<float> m;
    for (int d=0; d<dimension; d++) {
        m.push_back(getRandomValue());
    }
    vector<DistanceResult> result = tree.search(m, 8);
    clock_t finish_search = clock();
    std::cout<< "Searching for : (";
    for (int i=0;i<dimension;i++)
        cout << m[i] << ", ";
    cout << ")." << endl;
    for (int i=0; i<result.size(); i++) {
        size_t id = result[i].id;
        cout << "N:" << i << ", Id: "<< id << ", Distance:" << result[i].distance << ", value: (";
        // for (int i=0;i<dimension;i++) {
        //     cout << data[id][i] << ", ";
        // }
        cout << ")." << endl;
    }
    cout << "Construct time: " << (double)(finish_construct - start) / CLOCKS_PER_SEC << "s." << endl;
    cout << "Search time: " << (double)(finish_search - finish_construct) / CLOCKS_PER_SEC << "s." << endl;
}


int main(int argc, char const *argv[])
{
    testL2HighDimensionalTree();
    std::cout << "Hello World!" << std::endl;
    return 0;
}