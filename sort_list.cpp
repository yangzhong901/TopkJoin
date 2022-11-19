#include "sort_list.h"

SortedList::~SortedList()
{
}

int SortedList::size()
{
	return ResultList.size();
}

bool SortedList::add(tmpResult tmpResult)
{
	int size = ResultList.size();
	ResultList.push_front(tmpResult);
	ResultList.sort();
	int size2 = ResultList.size();
	if (size2 > k)
		ResultList.pop_back();
	return (size != size2);
}

bool SortedList::addAll(list<tmpResult> tmpResultList)
{
	int size1 = ResultList.size();
	tmpResultList.sort();
	ResultList.merge(tmpResultList);
	return ((int)ResultList.size() >= size1);
}

result SortedList::get()
{
	list<tmpResult>::reverse_iterator it = ResultList.rbegin();
	return it->res;
}

vector<result> SortedList::getResults(int n)
{
	vector<result> results;
	list<tmpResult>::reverse_iterator it = ResultList.rbegin();
	for (int i = 0; i < n; i++)
	{
		results.push_back(it->res);
		advance(it, 1);
	}	
	return results;
}

