#include "sort_vector.h"

SortedVector::~SortedVector()
{
}

int SortedVector::size()
{
	return ResultList.size();
}

bool SortedVector::add(tmpResult tmpResult)
{
	int size = ResultList.size();
	ResultList.push_back(tmpResult);
	sort(ResultList.begin(), ResultList.end());
	int size2 = ResultList.size();
	if (size2 > k)
		ResultList.pop_back();
	return (size != size2);
}

bool SortedVector::addAll(vector<tmpResult> tmpResultList)
{	
	return ((int)ResultList.size() >= 0);
}

result SortedVector::get()
{	
	return ResultList[ResultList.size()-1].res;
}

vector<result> SortedVector::getResults(int n)
{
	vector<result> results;
	vector<tmpResult>::reverse_iterator it = ResultList.rbegin();
	for (int i = 0; i < n; i++)
	{
		results.push_back(it->res);
		advance(it, 1);
	}
	return results;
}

