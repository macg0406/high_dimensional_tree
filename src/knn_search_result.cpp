/**
 * @file knn_search_result.cpp
 * @brief this is the data class for KNN search result
 * 
*/

#include <vector>
using std::vector;

#include "knn_search_result.h"

SearchResult::SearchResult(/* args */)
{
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

void SearchResult::add(DistanceResult result) {
    this->resultCount++;
    if (result.distance > this->max_distance) {
        this->max_distance = result.distance;
    }
}

distance_t SearchResult::get_max_distance() {
    return this->max_distance;
}

void SearchResult::toList(vector<DistanceResult>& result_list) {
    return;
}

TopkSearchResult::TopkSearchResult(int k, bool smallIsBetter) {
    this->k = k;
    this->size = 0;
    this->resultList = new DistanceResult[k+1];
}

TopkSearchResult::~TopkSearchResult() {
    delete[] this->resultList;
}

distance_t TopkSearchResult::get_max_distance() {
    if (this->size < this->k) {
        return -1.0;
    } else {
        return this->resultList[0].distance;
    }
}

void TopkSearchResult::add(id_t id, distance_t distance) {
    DistanceResult result;
    result.id = id;
    result.distance = distance;
    this->add(result);
}

void TopkSearchResult::add(DistanceResult result) {
    if (this->size < this->k) {
        this->resultList[this->size] = result;
        this->size++;
        if (this->size == this->k) {
            std::make_heap(this->resultList, this->resultList + this->k, [](DistanceResult a, DistanceResult b) {return a.distance < b.distance;});
        }
    } else {
        if (result.distance < this->resultList[0].distance) {
            this->resultList[0] = result;
            std::make_heap(this->resultList, this->resultList + this->k, [](DistanceResult a, DistanceResult b) {return a.distance < b.distance;});
        }
    }
}

void TopkSearchResult::toList(vector<DistanceResult>& result_list) {
    std::sort_heap(this->resultList, this->resultList + this->k, [](DistanceResult a, DistanceResult b) {return a.distance < b.distance;});
    for (int i = 0; i < this->k; i++) {
        result_list.push_back(this->resultList[i]);
    }
}

RadiusSearchResult::RadiusSearchResult(distance_t radius) {
    this->radius = radius;
}

RadiusSearchResult::~RadiusSearchResult() {
}

distance_t RadiusSearchResult::get_max_distance() {
    return this->radius;
}

void RadiusSearchResult::add(DistanceResult result) {
    if (result.distance < this->radius) {
        this->resultList.push_back(result);
    }
}

void RadiusSearchResult::toList(vector<DistanceResult>& result_list) {
    result_list = this->resultList;
}
