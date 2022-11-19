#pragma once
#include <vector>
#include <list>
#include"inverted_index.h"
using namespace std;

struct tmpResult{
	result res;
	bool operator <(const tmpResult& other)
	{
		return res.sim > other.res.sim;
	}
	tmpResult(result res) :res(res) {};
};

class SortedList
{
	int k;
	list<tmpResult> ResultList;
public:
	SortedList(int topk) { k = topk; };
	SortedList(list<tmpResult> tmpResultList) { addAll(tmpResultList); };
	~SortedList();
	int size();
	bool add(tmpResult tmpResult);
	bool addAll(list<tmpResult> tmpResultList);
	result get();
	vector<result> SortedList::getResults(int n);
};