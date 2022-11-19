#include "dpjoin.h"
#include <math.h>
#include <algorithm>

void dpjoin::CalcPrefix()
{
	int i, j, len;
	int widowTokenNum = 0;
	int indexPrefixTokenNum = 0;
	tokenFrequency.resize(tokenNum);
	for (i = 0; i < tokenNum; i++) tokenFrequency[i] = 0; // initialize token frequency

	for (i = 0; i < recordNum; i++) {
		len = joinRecord[i].len;

		for (j = 0; j < joinRecord[i].len; j++)
		++tokenFrequency[joinRecord[i].tokenArray[j]];
	}

	/* count widow tokens and non-widow tokens */
	for (i = 0; i < tokenNum; i++) {
		if (tokenFrequency[i] == maxWidowTokenFrequency) ++widowTokenNum;
		if (tokenFrequency[i]) ++indexPrefixTokenNum;
	}
	cerr << "# Distinct Tokens: " << tokenNum << endl;
	cerr << "# Distinct Indexed Tokens: " << indexPrefixTokenNum << endl;
	cerr << "# Distinct Widow Tokens: " << widowTokenNum << endl;
	invertedList.resize(tokenNum);
	
}

void dpjoin::ReorderRecs()
{
	for (int i = 0; i < recordNum; i++)
	{		
		quickSort(joinRecord[i].tokenArray, 0, joinRecord[i].tokenArray.size()-1);
	}
	
}

void dpjoin::BuildIndex()
{
	delta_number =1;// (int)ceil((threshold - 1) / 10);
	//delta_number = (int)(threshold + EPS) - 1;
	int i, j, len, token;
	int delta_id;
	int indexEntryNum = 0;
	index_entry indexEntry;

	/* indexing phase */
	invertedList.resize(tokenNum);
	deltaIndex.IniReverseDeltaInvertedIndex(tokenNum, delta_number);
	
	for (i = 0; i < tokenNum; i++) {
		invertedList[i].startingPosition = -1;
	}
	if(overlap)
	{
		for (i = 0; i < recordNum; i++) {
			len = joinRecord[i].len;

			if (len < threshold) continue;
			delta_id = 0;

			for (j = 0; j < (int)joinRecord[i].tokenArray.size(); j++)
			{

				token = joinRecord[i].tokenArray[j]; // fetch token

				if (tokenFrequency[token] <= maxWidowTokenFrequency) continue; // widow token

				/* index record id and its position */
				indexEntry.rec = i, indexEntry.pos = j;
				invertedList[token].startingPosition = 0;
				invertedList[token].list.push_back(indexEntry);
				
				++indexEntryNum;
			}
			
		}	
		
	}

	/* output statistics */
	cerr << "# Inverted Index Entries: " << indexEntryNum << endl;
}


void dpjoin::quickSort(vector<int> a, int l, int r) {
	if (l >= r)
		return;

	int i = l; int j = r; int key = tokenFrequency[a[l]];
	while (i<j) {

		while (i<j && tokenFrequency[a[j]] >= key)
			j--;
		if (i<j) {
			a[i] = a[j];
			i++;
		}

		while (i<j && tokenFrequency[a[i]]< key)
			i++;

		if (i<j) {
			a[j] = a[i];
			j--;
		}
	}
	//i == j
	a[i] = key;
	quickSort(a, l, i - 1);
	quickSort(a, i + 1, r);
}

void dpjoin::addIndex(int token, int y, int ypos)
{
	index_entry indexEntry;
	int len = joinRecord[y].len;
	if (tokenFrequency[token] <= maxWidowTokenFrequency) return; // widow token

	/* index record id and its position */
	indexEntry.rec = y, indexEntry.pos = ypos;
	invertedList[token].startingPosition = 0;
	invertedList[token].list.push_back(indexEntry);
	return;
}
void dpjoin::Clear()
{
	candidateNum = 0;
	resultNum = 0;
	for (size_t i = 0; i < invertedList.size(); i++)
	{
		invertedList[i].list.clear();
		invertedList[i].startingPosition = -1;
	}
	invertedList.clear();
};

