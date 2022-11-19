#pragma once

#include <vector>
#include <algorithm>
#include "sort_list.h"
#include"inverted_index.h"
using namespace std;

//struct tmpResult {
//	result res;
//	bool operator <(const tmpResult& other)
//	{
//		return res.sim > other.res.sim;
//	}
//	tmpResult(result res) :res(res) {};
//};

class SortedVector
{
	int k;
	vector<tmpResult> ResultList;
public:
	SortedVector(int topk) { k = topk; };
	SortedVector(vector<tmpResult> tmpResultList) { addAll(tmpResultList); };
	~SortedVector();
	int size();
	bool add(tmpResult tmpResult);
	bool addAll(vector<tmpResult> tmpResultList);
	result get();
	vector<result> SortedVector::getResults(int n);
};