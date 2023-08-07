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


template <typename T>
struct NDVector
{
    T *data;
    distance_t len;
};


template <typename T>
class HighDimensionalNode {
public:
    HighDimensionalNode<T> *left;
    HighDimensionalNode<T> *right;
    T *data;
    id_t id;
    int sub_count; // number of nodes in the subtree
    // int depth; // depth of the node in the tree
    HighDimensionalNode(vector<T>& data, id_t id);
    HighDimensionalNode(T *data, int dimension, id_t id);
    HighDimensionalNode();
    ~HighDimensionalNode();
};

template <typename T>
class HighDimensionalTree {
public:
    HighDimensionalTree();
    ~HighDimensionalTree();
    void insert(vector<T> data, id_t id);
    void construct(vector<vector<T>> data, vector<id_t>& ids);
    void print();
    void print(HighDimensionalNode<T> *node);
    vector<DistanceResult> search(vector<T>& data, int k);
    vector<DistanceResult> search(vector<T>& data, distance_t radius);
    vector<DistanceResult> search(vector<T>& data, int k, distance_t radius);

protected:
    int dimension;
    HighDimensionalNode<T> *root;
    void insert_node(HighDimensionalNode<T> *node, HighDimensionalNode<T> *new_node);
    void delete_tree(HighDimensionalNode<T> *node);
    bool is_left_closer(const HighDimensionalNode<T> *node, const vector<T>& data) const;
    distance_t minDistance(HighDimensionalNode<T> *node, HighDimensionalNode<T> *new_node, vector<T>& projectionPoint);
    void _search(const HighDimensionalNode<T> *node, const vector<T>& data, const vector<T>& dist_point, SearchResult& result) const;
    virtual distance_t distance(const T *data1, const T *data2) const;
    distance_t distance(const vector<T>& data1, const vector<T>& data2) const;
    distance_t distance(const T *data1, const vector<T>& data2) const;
    distance_t distance(const HighDimensionalNode<T> *node1, const HighDimensionalNode<T> *node2) const;
    distance_t distance(const HighDimensionalNode<T> *node, const vector<T>& data) const;

};

template <typename T>
class L2HighDimensionalTree : public HighDimensionalTree<T> {
public:
    L2HighDimensionalTree();
    ~L2HighDimensionalTree();

protected:
    distance_t distance(const T *data1, const T *data2) const;
};

template <typename T>
class L1HighDimensionalTree : public HighDimensionalTree<T> {
public:
    L1HighDimensionalTree();
    ~L1HighDimensionalTree();

protected:
    distance_t distance(const T *data1, const T *data2) const;
};

template <typename T>
class LInfHighDimensionalTree : public HighDimensionalTree<T> {
public:
    LInfHighDimensionalTree();
    ~LInfHighDimensionalTree();

protected:
    distance_t distance(const T *data1, const T *data2) const;
};

template <typename T>
class CosineHighDimensionalTree : public HighDimensionalTree<T> {
public:
    CosineHighDimensionalTree();
    ~CosineHighDimensionalTree();

protected:
    distance_t distance(const T *data1, const T *data2) const;
};


#endif // HIGH_DIMENSIONAL_TREE_H