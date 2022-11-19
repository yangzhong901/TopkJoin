#include <math.h>

#include <unordered_map>
#include "dpjoin.h"

using namespace std;
typedef unordered_map<int, Counter> hashmap_t;
typedef unordered_map<int, Counter>::iterator hashmap_iterator_t;

void dpjoin::PerformJoin()
{
	int i, j, x, y, lenx, leny, token, score;
	int lengthLowerBound;

	//result resultFound;
	vector<int> requiredOverlap;
	vector<double> sqrtLen;
	vector<int> candidatesKeys;
	hashmap_t candidates;
	hashmap_iterator_t it;


	cerr << "Algorithm: dpjoin" << endl;

	candidateNum = intersectionNum = resultNum = 0; // statistics

	/* store square root of length for cosine similarity */
	if (cosine)
	{
		sqrtLen.resize(joinRecord[recordNum - 1].len + 1);
		for (i = 0; i <= joinRecord[recordNum - 1].len; i++) sqrtLen[i] = sqrt(i);
	}
	for (x = 0; x < recordNum; x++) {
		lenx = joinRecord[x].len; // fetch length of x

		//candidates.clear_no_resize(); // initialize candidate set
		candidates.clear();

		/* initialize required overlap */
		requiredOverlap.clear();
		requiredOverlap.resize(lenx + 1);

		/* calculate required overlap */
		if (jaccard) {
			lengthLowerBound = int(lenx * threshold - EPS) + 1;
			for (i = lengthLowerBound; i <= lenx; i++)//x是按照长度排序的，只比较长度比x小的y，遍历x就会遍历所有（x，y）对且不会重复
				requiredOverlap[i] = int((i + lenx) * threshold / (1 + threshold) - EPS) + 1;//如果y的长度是i，那么y和x的overlap应该为requiredOverlap[i]
		}
		else if (cosine) {
			lengthLowerBound = int(lenx * threshold * threshold - EPS) + 1;
			for (i = lengthLowerBound; i <= lenx; i++)
				requiredOverlap[i] = int(sqrt(lenx) * sqrtLen[i] * threshold - EPS) + 1;
		}
		else if (overlap)
		{
			lengthLowerBound = (int)(threshold - EPS) + 1;//y的最小长度，overlap为threshold，那么y的长度至少要大于等于threshold
			if (lenx < lengthLowerBound)//如果x小于threshold，直接下一个
				continue;
			for (i = lengthLowerBound; i <= lenx; i++)
				requiredOverlap[i] = (int)(threshold - EPS) + 1;
		}


		/* probing phase */
		for (i = 0; i < joinRecord[x].probePrefixLength; i++) {
			token = joinRecord[x].tokenArray[i]; // fetch token x[i]
			if (invertedList[token].startingPosition == -1) continue; // no inverted list found for x[i]

			for (j = invertedList[token].startingPosition; j < (int)invertedList[token].list.size() && joinRecord[invertedList[token].list[j].rec].len < lengthLowerBound; j++); // move forward the head of inverted list of x[i]

			/* access inverted list of x[i] */
			for (invertedList[token].startingPosition = j; j < (int)invertedList[token].list.size(); j++) {
				y = invertedList[token].list[j].rec; // fetch y

				leny = joinRecord[y].len; // fetch length of y
				
				if (y >= x) break; // y >= x --> length of y >= length of x; note that we sort records according to length

				it = candidates.find(y); // check if candidate y has been found before
				if (it != candidates.end())// y has been found 
				{
					it->second.increment1();
					it->second.last_overlap_pos = invertedList[token].list[j].pos;//记录最后重复的位置
					
				}
				else {
					if (lenx - i < requiredOverlap[leny] || leny - invertedList[token].list[j].pos < requiredOverlap[leny]) continue; 
					
					candidates[y].increment1(); // y has not been found, form new candidate	
					candidates[y].last_overlap_pos = invertedList[token].list[j].pos;//记录最后重复的位置
					candidatesKeys.push_back(y);
				}
			}
		}



	/* output statistics */
	cerr << "# Results: " << resultNum << endl;
	cerr << "# Candidate Pairs: " << candidateNum << endl;
	//cerr << "# Intersections Performed: " << intersectionNum << endl;
}

void dpjoin::VerifyCandidatesWithLastOverlapPosInY(int xid, int yid, int score, int dualPrefixLen, int y_last_overlap_position, int requiredOverlap)
{
	
	int lenx = joinRecord[xid].len;
	int leny = joinRecord[yid].len; // fetch length of y
	++candidateNum;

	int x_lasttoken = joinRecord[xid].tokenArray[joinRecord[xid].probePrefixLength - 1 + dualPrefixLen];
	int y_lasttoken = joinRecord[yid].tokenArray[joinRecord[yid].indexPrefixLength - 1 + dualPrefixLen];

	if (x_lasttoken <= y_lasttoken)
	{
		if (score + lenx - joinRecord[xid].probePrefixLength - dualPrefixLen >= requiredOverlap)
		{
			score += CountOverlap(joinRecord[xid].tokenArray, joinRecord[yid].tokenArray, lenx, leny, joinRecord[xid].probePrefixLength + dualPrefixLen,
				//y_last_overlap_position + 1, requiredOverlap - score);
				score, requiredOverlap - score);
		}
		if (score >= 0)
		{
			++resultNum;
			joinResult.push_back(result(xid,yid,score));
		}
	}
	else
	{
		if (score + leny - joinRecord[yid].indexPrefixLength - dualPrefixLen >= requiredOverlap)
		{
			score += CountOverlap(joinRecord[xid].tokenArray, joinRecord[yid].tokenArray, lenx, leny, score,
				joinRecord[yid].indexPrefixLength + dualPrefixLen, requiredOverlap - score);
		}
		if (score >= 0)
		{
			++resultNum;
			joinResult.push_back(result(xid, yid, score));
		}
	}
}

void dpjoin::VerifyCandidatesOld(int x, int y, int score, int requiredOverlap)
{
	int lenx = joinRecord[x].len;
	int leny = joinRecord[y].len; // fetch length of y
	++candidateNum;

	int x_lasttoken = joinRecord[x].tokenArray[joinRecord[x].probePrefixLength - 1 ];
	int y_lasttoken = joinRecord[y].tokenArray[joinRecord[y].indexPrefixLength - 1 ];
	if (x_lasttoken < y_lasttoken) {
		if (score + lenx - joinRecord[x].probePrefixLength >= requiredOverlap) { // check the upper bound of overlap
			//++intersectionNum;
			score += CountOverlap(joinRecord[x].tokenArray, joinRecord[y].tokenArray, lenx, leny, joinRecord[x].probePrefixLength, score, requiredOverlap - score); // calculate exact similarity

			/* insert into join results */
			if (score >= 0) {
				int xid = joinRecord[x].rid;
				int yid = joinRecord[y].rid;
				joinResult.push_back(result(xid, yid, score));
				++resultNum;
			}
		}
	}
	else {
		if (score + leny - joinRecord[y].indexPrefixLength >= requiredOverlap) { // check the upper bound of overlap
			//++intersectionNum;
			score += CountOverlap(joinRecord[x].tokenArray, joinRecord[y].tokenArray, lenx, leny, score, joinRecord[y].indexPrefixLength, requiredOverlap - score); // calculate exact similarity

			/* insert into join results */
			if (score >= 0) {
				int xid = joinRecord[x].rid;
				int yid = joinRecord[y].rid;
				joinResult.push_back(result(xid, yid, score));
				++resultNum;
			}
		}
	}
}
void dpjoin::VerifyCandidates(int xid, int yid, int score, int dualPrefixLen, int requiredOverlap)
{
	//yid = it->first, score = it->second.count; // fetch y and A[y]
	int lenx = joinRecord[xid].len;
	int leny = joinRecord[yid].len; // fetch length of y
	++candidateNum;

	int x_lasttoken = joinRecord[xid].tokenArray[joinRecord[xid].probePrefixLength - 1+ dualPrefixLen];
	int y_lasttoken = joinRecord[yid].tokenArray[joinRecord[yid].indexPrefixLength - 1+ dualPrefixLen];

	if (x_lasttoken < y_lasttoken)
	{
		if (score + lenx - joinRecord[xid].probePrefixLength - dualPrefixLen >= requiredOverlap)
		{
			score += CountOverlap(joinRecord[xid].tokenArray, joinRecord[yid].tokenArray, lenx, leny, joinRecord[xid].probePrefixLength  + dualPrefixLen,
				score, requiredOverlap - score);
		}
		if (score >= 0)
		{
			++resultNum;
			joinResult.push_back(result(xid, yid, score));
		}
		
	}
	else
	{
		if (score + leny - joinRecord[yid].indexPrefixLength - dualPrefixLen >= requiredOverlap)
		{
			score += CountOverlap(joinRecord[xid].tokenArray, joinRecord[yid].tokenArray, lenx, leny, score,
				joinRecord[yid].indexPrefixLength + dualPrefixLen, requiredOverlap - score);
		}
		if (score >= 0)
		{			
			++resultNum;
			joinResult.push_back(result(xid, yid, score));
		}
	}

}

int dpjoin::CountOverlap(vector<int>& xArray, vector<int>& yArray, int xLen, int yLen, int xStart, int yStart, int requiredOverlap)
{
	int i = xStart, j = yStart, ans = 0;
	while (i < xLen && j < yLen) {
		if (xArray[i] == yArray[j]) ++i, ++j, ++ans;
		else {
			if (xArray[i] < yArray[j]) ++i;
			else ++j;
		}
	}
	if (ans < requiredOverlap) return -INF;
	else return ans;
}

int dpjoin::BinarySearch(vector<int>& array, int value, int start, int end)
{
	int mid;
	while (start < end) {
		mid = (start + end) / 2;
		if (array[mid] < value) start = mid + 1;
		else if (array[mid] > value) end = mid;
		else
			return value;			
	}
	return array[start];
}

int dpjoin::BinarySearchPos(vector<int>& array, int value, int start, int end)
{
	int mid;
	while (start < end) {
		mid = (start + end) / 2;
		if (array[mid] < value) start = mid + 1;
		else if (array[mid] > value) end = mid;
		else
			return start;
	}
	return start;
}

int dpjoin::BinarySearch(vector<index_entry>& array, int value, int start, int end)
{
	int mid;
	while (start < end) {
		mid = (start + end) / 2;
		if (array[mid].rec < value) start = mid + 1;
		else end = mid;
	}
	return start;
}
