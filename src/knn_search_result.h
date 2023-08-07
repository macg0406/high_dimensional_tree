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
protected:
    int resultCount = 0;
    distance_t max_distance = -1.0;
    /* data */
public:
    SearchResult(/* args */);
    ~SearchResult();
    virtual distance_t get_max_distance();
    virtual void add(DistanceResult result);
    void add(id_t id, distance_t distance_t);
    void toList(vector<DistanceResult>& result_list);
};

class TopkSearchResult : public SearchResult {
private:
    int k;
    int size;
    DistanceResult *resultList;

public:
    TopkSearchResult(int k, bool smallIsBetter);
    ~TopkSearchResult();
    distance_t get_max_distance();
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
    distance_t get_max_distance();
    void add(DistanceResult result);
    void toList(vector<DistanceResult>& result_list);
};


#endif // KNN_SEARCH_RESULT_H