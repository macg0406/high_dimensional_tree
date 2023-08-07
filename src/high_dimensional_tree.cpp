/*
This is the implementation file for the high_dimensional_tree class.
*/

#include "high_dimensional_tree.h"


template <typename T>
HighDimensionalNode<T>::HighDimensionalNode(vector<T>& data, id_t id) {
    this->data = new T[data.size()];
    for (int i = 0; i < data.size(); i++) {
        this->data[i] = data[i];
    }
    this->left = NULL;
    this->right = NULL;
    this->sub_count = 1;
    // this->depth = 1;
    this->id = id;
}

template <typename T>
HighDimensionalNode<T>::HighDimensionalNode(T *data, int dimension, id_t id) {
    this->data = new T[dimension];
    for (int i = 0; i < dimension; i++) {
        this->data[i] = data[i];
    }
    this->left = NULL;
    this->right = NULL;
    this->sub_count = 1;
    // this->depth = 1;
    this->id = id;
}

template <typename T>
HighDimensionalNode<T>::HighDimensionalNode() {
    this->data = NULL;
    this->left = NULL;
    this->right = NULL;
    // this->depth = 1;
    this->sub_count = 1;
    this->id = 0;
}

template <typename T>
HighDimensionalNode<T>::~HighDimensionalNode() {
    if (this->data != NULL) {
        delete [] this->data;
    }
}

template <typename T>
HighDimensionalTree<T>::HighDimensionalTree() {
    this->root = NULL;
    this->dimension = 0;
}


template <typename T>
void HighDimensionalTree<T>::delete_tree(HighDimensionalNode<T> *node) {
    if (node == NULL) {
        return;
    }
    this->delete_tree(node->left);
    this->delete_tree(node->right);
    delete node;
}

template <typename T>
HighDimensionalTree<T>::~HighDimensionalTree() {
    if (this->root != NULL) {
        this->delete_tree(this->root);
    }
}

template <typename T>
bool HighDimensionalTree<T>::is_left_closer(const HighDimensionalNode<T> *node, const vector<T>& data) const {
    distance_t distance_left = this->distance(node->left, new_node);
    distance_t distance_right = this->distance(node->right, new_node);
    if (distance_left < distance_right)
        return true;
    return false;
}

template <typename T>
void HighDimensionalTree<T>::insert_node(HighDimensionalNode<T> *node, HighDimensionalNode<T> *new_node) {
    node->sub_count++;
    if (node->left == NULL) {
        node->left = new_node;
        // node->depth++;
        return;
    }
    if (node->right == NULL) {
        node->right = new_node;
        return;
    }
    if this->left_is_closer(node, new_node) {
        this->insert_node(node->left, new_node);
        // if (node->left->depth >= node->depth) {
        //     node->depth = node->left->depth + 1;
        // }
    } else {
        this->insert_node(node->right, new_node);
        // if (node->right->depth >= node->depth) {
        //     node->depth = node->right->depth + 1;
        // }
    }
}

template <typename T>
void HighDimensionalTree<T>::insert(vector<T> data, id_t id) {
    HighDimensionalNode<T> *new_node = new HighDimensionalNode<T>(data, id);
    if (this->root == NULL) {
        this->root = new_node;
        this->dimension = data.size();
        return;
    }
    HighDimensionalNode<T> *node = this->root;
    while (node != NULL) {
        node->sub_count++;
        if (node->left == NULL) {
            node->left = new_node;
            return;
        }
        if (node->right == NULL) {
            node->right = new_node;
            return;
        }
        if this->left_is_closer(node, new_node) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
}

template <typename T>
void HighDimensionalTree<T>::construct(vector<vector<T>> data, vector<id_t>& ids) {
    for (int i = 0; i < data.size(); i++) {
        this->insert(data[i], ids[i]);
    }
}

template <typename T>
void HighDimensionalTree<T>::print() {
    this->print(this->root);
}

template <typename T>
void HighDimensionalTree<T>::print(HighDimensionalNode<T> *node) {
    if (node == NULL) {
        return;
    }
    cout << "data: ";
    for (int i = 0; i < node->data->size(); i++) {
        cout << (*node->data)[i] << " ";
    }
    cout << endl;
    cout << "sub_count: " << node->sub_count << endl;
    cout << "left: ";
    if (node->left != NULL) {
        cout << "data: ";
        for (int i = 0; i < node->left->data->size(); i++) {
            cout << (*node->left->data)[i] << " ";
        }
        cout << endl;
        cout << "sub_count: " << node->left->sub_count << endl;
    } else {
        cout << "NULL" << endl;
    }
    cout << "right: ";
    if (node->right != NULL) {
        cout << "data: ";
        for (int i = 0; i < node->right->data->size(); i++) {
            cout << (*node->right->data)[i] << " ";
        }
        cout << endl;
        cout << "sub_count: " << node->right->sub_count << endl;
    } else {
        cout << "NULL" << endl;
    }
    cout << endl;
    this->print(node->left);
    this->print(node->right);
}


template <typename T>
void HighDimensionalTree<T>::_search(const HighDimensionalNode<T> *node, const vector<T>& data, const vector<T>& dist_point, SearchResult& result) const {
    distance_t left_distance;
    distance_t right_distance
    if (curr_node == NULL) {
        return;
    }
    if (curr_node->left == NULL) {
        return;
    } else {
        left_distance = this->distance(curr_node->left->data, dist_point);
        result.add(curr_node->left->id, left_distance);
    }
    if (curr_node->right == NULL) {
        return;
    } else {
        right_distance = this->distance(curr_node->right->data, dist_point);
        result.add(curr_node->right->id, right_distance);
    }

    if (this->is_left_closer(curr_node, data)) {
        this->_search(curr_node->left, data, dist_point, result);
        vector<T> projectionPoint(this->dimension);
        distance_t minDistance = this->minDistance(curr_node, dist_point, projectionPoint);
        if (minDistance < result.get_max_distance()) {
            this->_search(curr_node->right, projectionPoint, dist_point, result);
        }
    } else {
        this->_search(curr_node->right, data, dist_point, result);
        vector<T> projectionPoint(this->dimension);
        distance_t minDistance = this->minDistance(curr_node, dist_point, projectionPoint);
        if (minDistance < result.get_max_distance()) {
            this->_search(curr_node->left, projectionPoint, dist_point, result);
        }
    }
}

template <typename T>
distance_t HighDimensionalTree<T>::minDistance(HighDimensionalNode<T> *node, HighDimensionalNode<T> *new_node, vector<T>& projectionPoint) {
    distance_t distance = 0.0, dotProduct = 0.0, baseLen = 0.0;
    for (int i = 0; i < this->dimension; i++) {
        dotProduct += (node->left->data[i] + node->right->data[i] - new_node->data[i]) * (node->right->data[i] - node->left->data[i]);
        baseLen += pow((node->right->data[i] - node->left->data[i], 2);
    }
    base_len = sqrt(baseLen);
    distance = dotProduct / baseLen;
    for (int i = 0; i < this->dimension; i++) {
        projectionPoint[i] = new_node->data[i] + (node->right->data[i] - node->left->data[i]) * distance / baseLen;
    }
    return abs(distance);
}

template <typename T>
vector<DistanceResult> HighDimensionalTree<T>::search(vector<T>& data, int k) {
    vector<DistanceResult> answer;
    TopkSearchResult result(k);
    if (this->root == NULL) return answer;
    this->_search(this->root, data, data, result);
    result.toList(answer)
    return answer;
}

template <typename T>
vector<DistanceResult> HighDimensionalTree<T>::search(vector<T>& data, distance_t radius){
    vector<DistanceResult> answer;
    RadiusSearchResult result(radius);
    if (this->root == NULL) return answer;
    this->_search(this->root, data, data, result);
    result.toList(answer)
    return answer;
}

template <typename T>
vector<DistanceResult> HighDimensionalTree<T>::search(vector<T>& data, int k, distance_t radius) {
    vector<DistanceResult> answer;
    SearchResult result;
    if (this->root == NULL) return answer;
    this->_search(this->root, data, data, result);
    result.toList(answer)
    return answer;
}

template <typename T>
distance_t HighDimensionalTree<T>::distance(const T *data1, const T *data2) const {
    distance_t result = 0;
    for (int i = 0; i < this->dimension; i++) {
        result += pow(data1[i] - data2[i], 2);
    }
    return sqrt(result);
}

template <typename T>
distance_t HighDimensionalTree<T>::distance(const T *data1, const vector<T>& data2) const {
    return this->distance(data1, data2.data());
}

template <typename T>
distance_t HighDimensionalTree<T>::distance(const vector<T>& data1, const vector<T>& data2) const {
    return this->distance(data1.data(), data2.data());
}

template <typename T>
distance_t HighDimensionalTree<T>::distance(const HighDimensionalNode<T> *node1, const HighDimensionalNode<T> *node2) const {
    return this->distance(*node1->data, *node2->data);
}

template <typename T>
distance_t HighDimensionalTree<T>::distance(const HighDimensionalNode<T> *node, const vector<T>& data) const {
    return this->distance(*node->data, data);
}

template <typename T>
L2HighDimensionalTree<T>::L2HighDimensionalTree() : HighDimensionalTree<T>() {
}

template <typename T>
L2HighDimensionalTree<T>::~L2HighDimensionalTree() {
}

template <typename T>
distance_t L2HighDimensionalTree<T>::distance(const T *data1, const T *data2) const {
    distance_t result = 0;
    for (int i = 0; i < this->dimension; i++) {
        result += pow(data1[i] - data2[i], 2);
    }
    return sqrt(result);
}

template <typename T>
L1HighDimensionalTree<T>::L1HighDimensionalTree() : HighDimensionalTree<T>() {
}

template <typename T>
L1HighDimensionalTree<T>::~L1HighDimensionalTree() {
}

template <typename T>
distance_t L1HighDimensionalTree<T>::distance(const T *data1, const T *data2) const {
    distance_t result = 0;
    for (int i = 0; i < this->dimension; i++) {
        result += abs(data1[i] - data2[i]);
    }
    return result;
}

template <typename T>
LInfHighDimensionalTree<T>::LInfHighDimensionalTree() : HighDimensionalTree<T>() {
}

template <typename T>
LInfHighDimensionalTree<T>::~LInfHighDimensionalTree() {
}

template <typename T>
distance_t LInfHighDimensionalTree<T>::distance(const T *data1, const T *data2) const {
    distance_t result = 0;
    for (int i = 0; i < this->dimension; i++) {
        distance_t temp = abs(data1[i] - data2[i]);
        if (temp > result) {
            result = temp;
        }
    }
    return result;
}

template <typename T>
CosineHighDimensionalTree<T>::CosineHighDimensionalTree() : HighDimensionalTree<T>() {
}

template <typename T>
CosineHighDimensionalTree<T>::~CosineHighDimensionalTree() {
}

template <typename T>
distance_t CosineHighDimensionalTree<T>::distance(const T *data1, const T *data2) const {
    distance_t result = 0;
    distance_t dotProduct = 0;
    distance_t baseLen1 = 0;
    distance_t baseLen2 = 0;
    for (int i = 0; i < this->dimension; i++) {
        dotProduct += data1[i] * data2[i];
        baseLen1 += pow(data1[i], 2);
        baseLen2 += pow(data2[i], 2);
    }
    baseLen1 = sqrt(baseLen1);
    baseLen2 = sqrt(baseLen2);
    result = dotProduct / (baseLen1 * baseLen2);
    return 1 - result;
}

// Path: src/high_dimensional_tree.cpp
// Path: src/kd_tree.cpp