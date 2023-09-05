/**
 * high_dimensional_area is a class that represents a high dimensional area,
 * It used by searching.
 * It is a stack of plant objects defined by a point in it and the normal vector to identify the values. 
*/

#ifndef HIGH_DIMENSIONAL_AREA_H
#define HIGH_DIMENSIONAL_AREA_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>

using namespace std;
#include "knn_search_result.h"

class PlaneLinkNode {
public:
    bool isVisited; 
    // float *basePoint;
    float constantTerm;
    float *normalVector;
    PlaneLinkNode *next;
    // HighDimensionalNode *toNode;
    // distance_t distance;
    PlaneLinkNode();
    PlaneLinkNode(int dimension);
    PlaneLinkNode(const vector<float>& normalVector, float constantTerm);
    ~PlaneLinkNode();
    bool flip(int dimension);
    bool isInArea(const vector<float>& point) const;
};


class HighDimensionalArea {
public:
    HighDimensionalArea(int dimension);
    ~HighDimensionalArea();
    void push(const vector<float>& normalVector, float constantTerm);
    void push(PlaneLinkNode *node);
    void pop();
    bool flipLast(int dimension);
    bool getMinDistancePoint(const vector<float> &dist_point, float maxDistance, vector<float> &projectionPoint) const;

private:
    int count;
    int dimension;
    PlaneLinkNode *head;
    // PlaneLinkNode *tail;
    
};

#endif // HIGH_DIMENSIONAL_AREA_H