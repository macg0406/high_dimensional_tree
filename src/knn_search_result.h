/**
 * this is the data class for KNN search result
 * 
*/

#ifndef KNN_SEARCH_RESULT_H
#define KNN_SEARCH_RESULT_H

typedef float distance_t;
typedef unsigned int id_t;

// this is a datatype for search result
struct DistanceResult {
    id_t id;
    distance_t distance;
};


class SearchResult
{
public:
    SearchResult();
    ~SearchResult();
    // virtual bool isInRadius(distance_t minDistance);
    virtual void add(DistanceResult result)=0;
    virtual distance_t getRadius()=0;
    virtual int getAcceptCount()=0;
    void add(id_t id, distance_t distance_t);
    void toList(vector<DistanceResult>& result_list);
    int calculateCount;
    int skipCount;
};

class TopkSearchResult : public SearchResult {
private:
    int k;
    int size;
    DistanceResult *resultList;

public:
    TopkSearchResult(int k);
    ~TopkSearchResult();
    bool isInRadius(distance_t minDistance);
    virtual distance_t getRadius();
    virtual int getAcceptCount();
    void add(id_t id, distance_t distance);
    void add(DistanceResult result);
    void toList(vector<DistanceResult>& result_list);
};

class RadiusSearchResult : public SearchResult {
private:
    distance_t radius;
    vector<DistanceResult> resultList;

public:
    RadiusSearchResult(distance_t radius);
    ~RadiusSearchResult();
    // bool isInRadius(distance_t minDistance);
    virtual distance_t getRadius();
    virtual int getAcceptCount();
    void add(DistanceResult result);
    void add(id_t id, distance_t distance);
    void toList(vector<DistanceResult>& result_list);
};


#endif // KNN_SEARCH_RESULT_H