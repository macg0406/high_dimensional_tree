/**
 * @file knn_search_result.cpp
 * @brief this is the data class for KNN search result
 * 
*/

#include <iostream>
#include <vector>
using std::vector;

#include "knn_search_result.h"

SearchResult::SearchResult()
{
    this->skipCount = 0;
    this->calculateCount = 0;
}

SearchResult::~SearchResult()
{
}

void SearchResult::add(id_t id, distance_t distance) {
    DistanceResult result;
    result.id = id;
    result.distance = distance;
    this->add(result);
}

void SearchResult::toList(vector<DistanceResult>& result_list) {
    return;
}

TopkSearchResult::TopkSearchResult(int k) {
    this->k = k;
    this->size = 0;
    this->resultList = new DistanceResult[k+1];
}

TopkSearchResult::~TopkSearchResult() {
    delete[] this->resultList;
}

// bool TopkSearchResult::isInRadius(distance_t minDistance) {
//     if (this->size < this->k) {
//         return true;
//     } else {
//         return this->resultList[0].distance > minDistance;
//     }
// }

int TopkSearchResult::getAcceptCount() {
    if (this->size < this->k) {
        return (this->k - this->size);
    }
    return 0;
}

distance_t TopkSearchResult::getRadius() {
    return this->resultList[0].distance;
}

void TopkSearchResult::add(id_t id, distance_t distance) {
    DistanceResult result;
    result.id = id;
    result.distance = distance;
    this->add(result);
}

void swapSearchResult(DistanceResult* heapArray, int i, int j){
    DistanceResult temp = heapArray[i];
    heapArray[i] = heapArray[j];
    heapArray[j] = temp;
}

/**
 * @brief Max heapify the heap array starting from a given index.
 * 
 * @param heapArray Pointer to the heap array.
 * @param heapSize The size of the heap.
 * @param start The start index of the heap.
 */
void maxHeapify(DistanceResult* heapArray, int heapSize, int start) {
    int dad = start;
    int son = dad * 2 + 1;

    while (son < heapSize) {
        // Find the larger child
        if (son + 1 < heapSize && heapArray[son].distance < heapArray[son + 1].distance) {
            son++;
        }

        // If the parent is larger, exit the loop
        if (heapArray[dad].distance > heapArray[son].distance) {
            return;
        } else {
            // Swap parent and child
            swapSearchResult(heapArray, dad, son);
            dad = son;
            son = dad * 2 + 1;
        }
    }
}

void TopkSearchResult::add(DistanceResult result) {
    // std::cout << "add " << result.id << ", and distance is: " << result.distance << ", size: " << this->size << ", k: " << this->k << std::endl;
    if (this->size < this->k) {
        this->resultList[this->size] = result;
        this->size++;
        if (this->size == this->k) {
            std::make_heap(this->resultList, this->resultList + this->k, [](DistanceResult a, DistanceResult b) {return a.distance < b.distance;});
            // for (int i=this->k/2-1; i>=0; i++) {
            //     maxHeapDown(this->resultList, this->k, i);
            // }
        }
    } else {
        if (result.distance < this->resultList[0].distance) {
            this->resultList[0] = result;
            std::make_heap(this->resultList, this->resultList + this->k, [](DistanceResult a, DistanceResult b) {return a.distance < b.distance;});
            // maxHeapDown(this->resultList, this->k, 0);
        }
    }
}

void TopkSearchResult::toList(vector<DistanceResult>& result_list) {
    if (this->size < this->k) {
        std::make_heap(this->resultList, this->resultList + this->size, [](DistanceResult a, DistanceResult b) {return a.distance < b.distance;});
        // for (int i= this->size / 2 - 1; i>=0; i--) {
        //     maxHeapDown(this->resultList, this->size, i);
        // }
    }
    std::sort_heap(this->resultList, this->resultList + this->size, [](DistanceResult a, DistanceResult b) {return a.distance < b.distance;});
    // for (int i = this->size - 1; i > 0; i--) {
    //     swapSearchResult(this->resultList, 0, i);
    //     maxHeapDown(this->resultList, i, 0);
    // }
    for (int i = 0; i < this->size; i++) {
        result_list.push_back(this->resultList[i]);
    }
}

RadiusSearchResult::RadiusSearchResult(distance_t radius) {
    this->radius = radius;
}

RadiusSearchResult::~RadiusSearchResult() {
}


// bool RadiusSearchResult::isInRadius(distance_t minDistance) {
//     return minDistance <= this->radius;
// }

/**
 * for the radius search, it dont accept any value other than accept value with in the radius
 *
 * @return The accept count, always 0.
 */
int RadiusSearchResult::getAcceptCount() {
    return 0;
}

distance_t RadiusSearchResult::getRadius() {
    return this->radius;
}

/**
 * @brief Adds a distance result to the radius search result if the distance is within the specified radius.
 * 
 * @param result The distance result to add.
 */
void RadiusSearchResult::add(DistanceResult result) {
    if (result.distance <= this->radius) {
        this->resultList.push_back(result);
    }
}

void RadiusSearchResult::add(id_t id, distance_t distance) {
    if (distance > this->radius) return;
    DistanceResult result;
    result.id = id;
    result.distance = distance;
    this->add(result);
}

void RadiusSearchResult::toList(vector<DistanceResult>& result_list) {
    for (int i=0; i<this->resultList.size(); i++) {
        result_list.push_back(this->resultList[i]);
    }
}
