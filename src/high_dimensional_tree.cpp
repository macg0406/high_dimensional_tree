/*
This is the implementation file for the high_dimensional_tree class.
*/

#include "high_dimensional_tree.h"


HighDimensionalNode::HighDimensionalNode(vector<float>& data, id_t id) {
    this->data = new float[data.size()];
    for (int i = 0; i < data.size(); i++) {
        this->data[i] = data[i];
    }
    this->left = NULL;
    this->right = NULL;
    this->sub_count = 1;
    // this->depth = 1;
    this->id = id;
}

HighDimensionalNode::HighDimensionalNode(float *data, int dimension, id_t id) {
    this->data = new float[dimension];
    for (int i = 0; i < dimension; i++) {
        this->data[i] = data[i];
    }
    this->left = NULL;
    this->right = NULL;
    this->sub_count = 1;
    // this->depth = 1;
    this->id = id;
}

HighDimensionalNode::HighDimensionalNode() {
    this->data = NULL;
    this->left = NULL;
    this->right = NULL;
    // this->depth = 1;
    this->sub_count = 1;
    this->id = 0;
}

HighDimensionalNode::~HighDimensionalNode() {
    if (this->data != NULL) {
        delete [] this->data;
    }
}

HighDimensionalTree::HighDimensionalTree() {
    this->root = NULL;
    this->dimension = 0;
}

HighDimensionalTree::HighDimensionalTree(int dimension) {
    this->root = NULL;
    this->dimension = dimension;
}

void HighDimensionalTree::delete_tree(HighDimensionalNode *node) {
    if (node == NULL) {
        return;
    }
    this->delete_tree(node->left);
    this->delete_tree(node->right);
    delete node;
}

HighDimensionalTree::~HighDimensionalTree() {
    if (this->root != NULL) {
        this->delete_tree(this->root);
    }
}

bool HighDimensionalTree::is_left_closer(const HighDimensionalNode *node, const vector<float>& data) const {
    distance_t distance_left = this->distance(node->left, data);
    distance_t distance_right = this->distance(node->right, data);
    if (distance_left < distance_right)
        return true;
    return false;
}

bool HighDimensionalTree::is_left_closer(const HighDimensionalNode *node, const HighDimensionalNode *new_node) const {
    distance_t distance_left = this->distance(node->left, new_node);
    distance_t distance_right = this->distance(node->right, new_node);
    if (distance_left < distance_right)
        return true;
    return false;
}

void HighDimensionalTree::insert_node(HighDimensionalNode *node, HighDimensionalNode *new_node) {
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
    if (this->is_left_closer(node, new_node)) {
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

void HighDimensionalTree::insert(vector<float> data, id_t id) {
    HighDimensionalNode *new_node = new HighDimensionalNode(data, id);
    if (this->root == NULL) {
        this->root = new_node;
        this->dimension = data.size();
        return;
    }
    HighDimensionalNode *node = this->root;
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
        if (this->is_left_closer(node, new_node)) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
}

void HighDimensionalTree::construct(vector<vector<float> >& data, vector<id_t>& ids) {
    for (int i = 0; i < data.size(); i++) {
        this->insert(data[i], ids[i]);
    }
}

void HighDimensionalTree::print() {
    this->print(this->root);
}

void HighDimensionalTree::print(const HighDimensionalNode *node) {
    if (node == NULL) {
        return;
    }
    cout << "data: ";
    for (int i = 0; i < this->dimension; i++) {
        cout << node->data[i] << " ";
    }
    cout << endl;
    cout << "sub_count: " << node->sub_count << endl;
    cout << "left: ";
    if (node->left != NULL) {
        cout << "data: ";
        for (int i = 0; i < this->dimension; i++) {
            cout << node->left->data[i] << " ";
        }
        cout << endl;
        cout << "sub_count: " << node->left->sub_count << endl;
    } else {
        cout << "NULL" << endl;
    }
    cout << "right: ";
    if (node->right != NULL) {
        cout << "data: ";
        for (int i = 0; i < this->dimension; i++) {
            cout << node->right->data[i] << " ";
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

void HighDimensionalTree::addToSearchArea(const HighDimensionalNode *node, HighDimensionalArea &searchArea, bool leftDirection) const {
    PlaneLinkNode* newPlane = new PlaneLinkNode(this->dimension);
    float baseLen = 0.0;
    for (int i = 0; i < this->dimension; i++) {
        // newPlane->basePoint[i] = (node->left->data[i] + node->right->data[i]) / 2;
        if (leftDirection) {
            newPlane->normalVector[i] = (node->left->data[i] - node->right->data[i]);
        } else {
            newPlane->normalVector[i] = (node->right->data[i] - node->left->data[i]);
        }
        baseLen += newPlane->normalVector[i] * newPlane->normalVector[i];
    }
    baseLen = sqrt(baseLen);
    newPlane->constantTerm = 0.0;
    for (int i=0; i < this->dimension; i++) {
        newPlane->normalVector[i] /= baseLen;
        newPlane->constantTerm += (node->left->data[i] + node->right->data[i]) * newPlane->normalVector[i] / 2;
    }
    newPlane->constantTerm = 0 - newPlane->constantTerm;
    searchArea.push(newPlane);
}

/**
 * Performs a recursive search on the high-dimensional tree without any condition.
 *
 * @param currNode a pointer to the current node in the tree
 * @param dist_point a vector of floating-point values representing the distance from the search point
 * @param result a reference to the SearchResult object to store the search results
 *
 * @return void
 *
 * @throws None
*/
void HighDimensionalTree::_searchWithoutCondition(const HighDimensionalNode *currNode, const vector<float> &dist_point, SearchResult &result) const {
    if (currNode->left == NULL) {
        return;
    } else {
        float distance = this->distance(currNode->left->data, dist_point);
        result.add(currNode->left->id, distance);
        result.calculateCount++;
    }
    if (currNode->right == NULL) {
        return;
    } else {
        float distance = this->distance(currNode->right->data, dist_point);
        result.add(currNode->right->id, distance);
        result.calculateCount++;
    }
    this->_searchWithoutCondition(currNode->left, dist_point, result);
    this->_searchWithoutCondition(currNode->right, dist_point, result);
}

/**
 * params
*/
void HighDimensionalTree::_search(
    const HighDimensionalNode *curr_node,
    const vector<float> &data,
    const vector<float> &dist_point,
    SearchResult &result,
    HighDimensionalArea &searchArea) const
{
    distance_t left_distance;
    distance_t right_distance;
    if (curr_node->left == NULL) {
        return;
    } else {
        left_distance = this->distance(curr_node->left->data, dist_point);
        result.add(curr_node->left->id, left_distance);
        result.calculateCount++;
    }
    if (curr_node->right == NULL) {
        return;
    } else {
        right_distance = this->distance(curr_node->right->data, dist_point);
        result.add(curr_node->right->id, right_distance);
        result.calculateCount++;
    }

    if (this->is_left_closer(curr_node, data)) {
        addToSearchArea(curr_node, searchArea, true);
        this->_search(curr_node->left, data, dist_point, result, searchArea);
        if (curr_node->right->sub_count <= result.getAcceptCount() + 1) {
            this->_searchWithoutCondition(curr_node->right, dist_point, result);
        }
        else {
            vector<float> projectionPoint(this->dimension);
            searchArea.flipLast(this->dimension);
            bool hasPointInRadius = searchArea.getMinDistancePoint(dist_point, result.getRadius(), projectionPoint);
            if (hasPointInRadius) {
                this->_search(curr_node->right, projectionPoint, dist_point, result, searchArea);
            } else {
                result.skipCount += curr_node->right->sub_count - 1;
            }
        }
        searchArea.pop();
    } else {
        addToSearchArea(curr_node, searchArea, false);
        this->_search(curr_node->right, data, dist_point, result, searchArea);
        if (result.getAcceptCount() + 1 >= curr_node->left->sub_count) {
            this->_searchWithoutCondition(curr_node->left, dist_point, result);
        }
        else {
            vector<float> projectionPoint(this->dimension);
            searchArea.flipLast(this->dimension);
            bool hasPointInRadius = searchArea.getMinDistancePoint(dist_point, result.getRadius(), projectionPoint);
            if (hasPointInRadius) {
                this->_search(curr_node->left, projectionPoint, dist_point, result, searchArea);
            } else {
                result.skipCount += curr_node->left->sub_count - 1;
            }
            // distance_t minDistance = this->minDistance(curr_node, dist_point, projectionPoint);
            // if (result.isInRadius(minDistance)) {
            //     this->_search(curr_node->left, projectionPoint, dist_point, result, searchArea);
            // } else {
            //     result.skipCount += curr_node->left->sub_count - 1;
            // }
        }
        searchArea.pop();
    }
}
// */

distance_t HighDimensionalTree::minDistance(const HighDimensionalNode *node, const vector<float>& new_node, vector<float>& projectionPoint) const {
    distance_t distance = 0.0, dotProduct = 0.0, baseLen = 0.0;
    for (int i = 0; i < this->dimension; i++) {
        dotProduct += (node->left->data[i] + node->right->data[i] - new_node[i] * 2) * (node->right->data[i] - node->left->data[i]);
        baseLen += pow((node->right->data[i] - node->left->data[i]), 2);
    }
    baseLen = sqrt(baseLen);
    distance = dotProduct / baseLen / 2;
    for (int i = 0; i < this->dimension; i++) {
        projectionPoint[i] = new_node[i] + (node->right->data[i] - node->left->data[i]) * distance / baseLen;
    }
    return abs(distance);
}
/**
 * this is the search method by recursion.
*/
vector<DistanceResult> HighDimensionalTree::search(vector<float>& data, int k) {
    vector<DistanceResult> answer;
    TopkSearchResult result(k);
    HighDimensionalArea searchArea(this->dimension);

    if (this->root == NULL) return answer;
    distance_t root_distance = this->distance(this->root, data);
    result.add(this->root->id, root_distance);
    result.calculateCount++;
    this->_search(this->root, data, data, result, searchArea);
    result.toList(answer);
    std::cout << "Search Count: " << result.calculateCount << ", skip: " << result.skipCount << endl;
    return answer;
} // */

/* this is the search method that use stash other than recursion。
* /
vector<DistanceResult> HighDimensionalTree::search(vector<float>& data, int k) {
    vector<DistanceResult> answer;
    TopkSearchResult result(k);
    PlaneLinkNode *stackTop=NULL;
    PlaneLinkNode *stackBottom = NULL;
    HighDimensionalNode *currNode;

    if (this->root == NULL) return answer;
    distance_t root_distance = this->distance(this->root, data);
    result.add(this->root->id, root_distance);
    result.calculateCount++;

    distance_t left_distance;
    distance_t right_distance;
    currNode = this->root;
    while (stackTop != NULL) {
        if (currNode->left != NULL) {
            left_distance = this->distance(currNode->left->data, data);
            result.add(currNode->left->id, left_distance);
            result.calculateCount++;
        }
        if (currNode->right != NULL) {
            right_distance = this->distance(currNode->right->data, data);
            result.add(currNode->right->id, right_distance);
            result.calculateCount++;

            if (this->is_left_closer(currNode, data)) {
                if (currNode->right->sub_count > 1) {
                    vector<float> projectionPoint(this->dimension);
                    distance_t minDistance = this->minDistance(currNode, data, projectionPoint);
                    if (result.isInRadius(minDistance)) {
                        this->_search(curr_node->left, projectionPoint, dist_point, result);
                    } else {
                        result.skipCount += curr_node->left->sub_count - 1;
                    }
                }
                stackTop = new PlaneLinkNode(currNode, stackTop);
                currNode = currNode->left;
            } else {
                currNode = currNode->right;
            }
        } else {
            while (stackTop != NULL && stackTop->isVisited) {
                PlaneLinkNode* tmp = stackTop;
                stackTop = stackTop->next;
                delete tmp;
            }
            if (stackTop != NULL) {
                currNode = stackTop->toNode;
                stackTop->isVisited = true;
            }
            continue;
        }
    }
    this->_search(this->root, data, data, result);
    result.toList(answer);
    std::cout << "Search Count: " << result.calculateCount << ", skip: " << result.skipCount << endl;
    return answer;
} 
// */

vector<DistanceResult> HighDimensionalTree::search(vector<float>& data, distance_t radius){
    vector<DistanceResult> answer;
    RadiusSearchResult result(radius);
    HighDimensionalArea searchArea(this->dimension);

    if (this->root == NULL) return answer;
    distance_t root_distance = this->distance(this->root, data);
    result.add(this->root->id, root_distance);
    this->_search(this->root, data, data, result, searchArea);
    result.toList(answer);
    return answer;
}

vector<DistanceResult> HighDimensionalTree::search(vector<float>& data, int k, distance_t radius) {
    vector<DistanceResult> answer;
    RadiusSearchResult result(radius);
    HighDimensionalArea searchArea(this->dimension);

    if (this->root == NULL) return answer;
    this->_search(this->root, data, data, result, searchArea);
    result.toList(answer);
    return answer;
}

distance_t HighDimensionalTree::distance(const float *data1, const float *data2) const {
    distance_t result = 0;
    for (int i = 0; i < this->dimension; i++) {
        result += pow(data1[i] - data2[i], 2);
    }
    return sqrt(result);
}

distance_t HighDimensionalTree::distance(const float *data1, const vector<float>& data2) const {
    return this->distance(data1, data2.data());
}

distance_t HighDimensionalTree::distance(const vector<float>& data1, const vector<float>& data2) const {
    return this->distance(data1.data(), data2.data());
}

distance_t HighDimensionalTree::distance(const HighDimensionalNode *node1, const HighDimensionalNode *node2) const {
    return this->distance(node1->data, node2->data);
}

distance_t HighDimensionalTree::distance(const HighDimensionalNode *node, const vector<float>& data) const {
    return this->distance(node->data, data);
}

L2HighDimensionalTree::L2HighDimensionalTree() : HighDimensionalTree() {
}

L2HighDimensionalTree::L2HighDimensionalTree(int dimension) : HighDimensionalTree(dimension) {
}

L2HighDimensionalTree::~L2HighDimensionalTree() {
}

distance_t L2HighDimensionalTree::distance(const float *data1, const float *data2) const {
    distance_t result = 0;
    for (int i = 0; i < this->dimension; i++) {
        result += pow(data1[i] - data2[i], 2);
    }
    return sqrt(result);
}

L1HighDimensionalTree::L1HighDimensionalTree() : HighDimensionalTree() {
}

L1HighDimensionalTree::L1HighDimensionalTree(int dimension) : HighDimensionalTree(dimension) {
}

L1HighDimensionalTree::~L1HighDimensionalTree() {
}

distance_t L1HighDimensionalTree::distance(const float *data1, const float *data2) const {
    distance_t result = 0;
    for (int i = 0; i < this->dimension; i++) {
        result += abs(data1[i] - data2[i]);
    }
    return result;
}

LInfHighDimensionalTree::LInfHighDimensionalTree() : HighDimensionalTree() {
}

LInfHighDimensionalTree::LInfHighDimensionalTree(int dimension) : HighDimensionalTree(dimension) {
}

LInfHighDimensionalTree::~LInfHighDimensionalTree() {
}

distance_t LInfHighDimensionalTree::distance(const float *data1, const float *data2) const {
    distance_t result = 0;
    for (int i = 0; i < this->dimension; i++) {
        distance_t temp = abs(data1[i] - data2[i]);
        if (temp > result) {
            result = temp;
        }
    }
    return result;
}

CosineHighDimensionalTree::CosineHighDimensionalTree() : HighDimensionalTree() {
}

CosineHighDimensionalTree::CosineHighDimensionalTree(int dimension) : HighDimensionalTree(dimension) {
}

CosineHighDimensionalTree::~CosineHighDimensionalTree() {
}

distance_t CosineHighDimensionalTree::distance(const float *data1, const float *data2) const {
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