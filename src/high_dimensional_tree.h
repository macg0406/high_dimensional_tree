/*
high dimensional tree is a binay tree that each node has a vector of data
and a vector of children
*/
#ifndef HIGH_DIMENSIONAL_TREE_H
#define HIGH_DIMENSIONAL_TREE_H


#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>

using namespace std;
#include "knn_search_result.h"
#include "high_dimensional_area.h"


struct NDVector
{
    float *data;
    distance_t len;
};



class HighDimensionalNode {
public:
    HighDimensionalNode *left;
    HighDimensionalNode *right;
    float *data;
    id_t id;
    int sub_count; // number of nodes in the subtree
    // int depth; // depth of the node in the tree
    HighDimensionalNode(vector<float>& data, id_t id);
    HighDimensionalNode(float *data, int dimension, id_t id);
    HighDimensionalNode();
    ~HighDimensionalNode();
};


class HighDimensionalTree {
public:
    HighDimensionalTree();
    HighDimensionalTree(int dimension);
    ~HighDimensionalTree();
    void insert(vector<float> data, id_t id);
    void construct(vector<vector<float> >& data, vector<id_t>& ids);
    void print();
    void print(const HighDimensionalNode *node);
    vector<DistanceResult> search(vector<float>& data, int k);
    vector<DistanceResult> search(vector<float>& data, distance_t radius);
    vector<DistanceResult> search(vector<float>& data, int k, distance_t radius);

protected:
    int dimension;
    HighDimensionalNode *root;
    void insert_node(HighDimensionalNode *node, HighDimensionalNode *new_node);
    void delete_tree(HighDimensionalNode *node);
    bool is_left_closer(const HighDimensionalNode *node, const vector<float>& data) const;
    bool is_left_closer(const HighDimensionalNode *node, const HighDimensionalNode *newNode) const;
    distance_t minDistance(const HighDimensionalNode *node, const vector<float>& new_node, vector<float>& projectionPoint) const;
    void _search(
        const HighDimensionalNode *node,
        const vector<float> &data,
        const vector<float> &dist_point,
        SearchResult &result,
        HighDimensionalArea &searchArea
    ) const;
    void _searchWithoutCondition(const HighDimensionalNode *node, const vector<float> &dist_point, SearchResult &result) const;
    virtual distance_t distance(const float *data1, const float *data2) const;
    distance_t distance(const vector<float>& data1, const vector<float>& data2) const;
    distance_t distance(const float *data1, const vector<float>& data2) const;
    distance_t distance(const HighDimensionalNode *node1, const HighDimensionalNode *node2) const;
    distance_t distance(const HighDimensionalNode *node, const vector<float>& data) const;
    void addToSearchArea(const HighDimensionalNode *node, HighDimensionalArea &searchArea, bool leftDirection) const;
};


class L2HighDimensionalTree : public HighDimensionalTree {
public:
    L2HighDimensionalTree();
    L2HighDimensionalTree(int dimension);
    ~L2HighDimensionalTree();

protected:
    distance_t distance(const float *data1, const float *data2) const;
};


class L1HighDimensionalTree : public HighDimensionalTree {
public:
    L1HighDimensionalTree();
    L1HighDimensionalTree(int dimension);
    ~L1HighDimensionalTree();

protected:
    distance_t distance(const float *data1, const float *data2) const;
};


class LInfHighDimensionalTree : public HighDimensionalTree {
public:
    LInfHighDimensionalTree();
    LInfHighDimensionalTree(int dimension);
    ~LInfHighDimensionalTree();

protected:
    distance_t distance(const float *data1, const float *data2) const;
};


class CosineHighDimensionalTree : public HighDimensionalTree {
public:
    CosineHighDimensionalTree();
    CosineHighDimensionalTree(int dimension);
    ~CosineHighDimensionalTree();

protected:
    distance_t distance(const float *data1, const float *data2) const;
};


#endif // HIGH_DIMENSIONAL_TREE_H