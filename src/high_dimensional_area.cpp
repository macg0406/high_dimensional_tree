
/**
 * this is the implementation of the high_dimensional_area class
*/

#include <iostream>
using std::cout;
using std::endl;

#include "high_dimensional_area.h"


PlaneLinkNode::PlaneLinkNode() {
    isVisited = false;
    next = NULL;
    normalVector = NULL;
    constantTerm = 0.0;
}

PlaneLinkNode::PlaneLinkNode(int dimension) {
    isVisited = false;
    next = NULL;
    normalVector = new float[dimension];
    constantTerm = 0.0;
}

PlaneLinkNode::PlaneLinkNode(const vector<float>& normalVector, float constTerm) {
    int dimension = normalVector.size();
    isVisited = false;
    next = NULL;
    this->normalVector = new float[dimension];
    this->constantTerm = constTerm;
    for (int i = 0; i < dimension; i++) {
        this->normalVector[i] = normalVector[i];
    }
}

PlaneLinkNode::~PlaneLinkNode() {
    if (this->normalVector != NULL) {
        delete [] this->normalVector;
    }
}

bool PlaneLinkNode::flip(int dimension) {
    if (isVisited) return false;
    isVisited = true;
    for (int i = 0; i < dimension; i++) {
        this->normalVector[i] *= -1;
    }
    this->constantTerm *= -1;
    return true;
}

bool PlaneLinkNode::isInArea(const vector<float>& point) const {
    int dimension = point.size();
    float sum = 1e-6 + this->constantTerm;
    for (int i = 0; i < dimension; i++) {
        sum += point[i] * this->normalVector[i];
    }
    return sum > 0;
}


HighDimensionalArea::HighDimensionalArea(int dimension) : dimension(dimension){
    count = 0;
    head = NULL;
}

HighDimensionalArea::~HighDimensionalArea() {
    while (head != NULL) {
        PlaneLinkNode* tmp = head;
        head = head->next;
        delete tmp;
    }
}

void HighDimensionalArea::push(const vector<float>& normalVector, float constTerm) {
    PlaneLinkNode * newNode = new PlaneLinkNode(normalVector, constTerm);
    push(newNode);
}

void HighDimensionalArea::push(PlaneLinkNode *plane) {
    plane->next = head;
    head = plane;
    this->count++;
}

/**
 * Pop the first element from the stack of the HighDimensionalArea list.
 */
void HighDimensionalArea::pop() {
    PlaneLinkNode* tmp = head;
    head = head->next;
    delete tmp;
    this->count--;
}

/**
 * Flips the last element in the specified dimension.
 *
 * @param dimension the dimension to flip the last element in
 *
 * @return true if the element was successfully flipped, false otherwise
 */
bool HighDimensionalArea::flipLast(int dimension) {
    return this->head->flip(dimension);
}

bool HighDimensionalArea::getMinDistancePoint(const vector<float> &distPoint, float maxDistance, vector<float> &projectionPoint) const {
    cout << "======================== Start searching ========================" << endl;
    cout << " max distance: " << maxDistance << ", Depth: " << this->count << endl; 
    int dimension = distPoint.size();
    int depth = this->count;
    distance_t distance = 0.0;
    vector<float> prevNormalVector(dimension);
    // float *prevNormalVector = new float[dimension];
    for (int i = 0; i < dimension; i++) {
        distance += distPoint[i] * head->normalVector[i];
    }
    distance = 0 - head->constantTerm - distance;
    cout << "First distance: " << distance << endl;
    if (distance > maxDistance + 1e-6) {
        // delete [] prevNormalVector;
        cout << " =========== end with first ================ " << endl;
        return false;
    }
    for (int i = 0; i < dimension; i++) {
        projectionPoint[i] += distPoint[i] + head->normalVector[i] * distance;
        prevNormalVector[i] = head->normalVector[i];
    }

    PlaneLinkNode* planeNode = head->next;
    float prevDistanceSqr = distance * distance;
    while (planeNode != NULL) {
        depth--;
        distance = planeNode->constantTerm;
        for (int i = 0; i < dimension; i++) {
            distance += projectionPoint[i] * planeNode->normalVector[i];
        }
        if (distance + 1e-6 > 0){
            planeNode = planeNode->next;
            cout << "Depth:" << depth << ", in the area: " << distance << endl;
            continue;
        }
        distance = 0 - distance;
        float cosOfNormal = 0.0;
        for (int i=0; i<dimension; i++) {
            cosOfNormal += planeNode->normalVector[i] * prevNormalVector[i];
        }
        float factor1 = distance / (1 - cosOfNormal * cosOfNormal);
        float factor2 = factor1 * cosOfNormal;
        float newDistance = distance * factor1 + prevDistanceSqr * (1 - factor2 * factor2);
        cout << "Depth:" << depth << ", new distance: " << newDistance << endl;
        if (newDistance > maxDistance * maxDistance + 1e-6){
            // delete [] prevNormalVector;
            cout << " =========== end with false ================ " << endl;
            return false;
        }
        newDistance = 0;
        for (int i=0;i<dimension; i++) {
            projectionPoint[i] += factor1 * planeNode->normalVector[i] - factor2 * prevNormalVector[i];
            prevNormalVector[i] = projectionPoint[i] - distPoint[i];
            newDistance += prevNormalVector[i] * prevNormalVector[i];
        }
        prevDistanceSqr = newDistance;
        newDistance = sqrt(newDistance);
        cout << "Depth:" << depth << ", new distance2: " << newDistance << endl;
        if (newDistance > maxDistance) {
            cout << " =========== end with false ================ " << endl;
            return false;
        }
        for (int i = 0; i < dimension; i++) {
            prevNormalVector[i] = prevNormalVector[i] / newDistance;
        }
        planeNode = planeNode->next;
    }
    // return abs(distance);
    // delete [] prevNormalVector;
    cout << " =========== end with true ================ " << endl;
    return true;
}