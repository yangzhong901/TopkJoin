#pragma once

#ifndef PPJOIN_H_
#define PPJOIN_H_

#include "delta_inverted_index.h"
#include "reverse_delta_inverted_index.h"
#include "inverted_index.h"
#include "sort_list.h"
#include "sort_vector.h"
#include "hightime.h"

#include "events_queue.h"
#include "similarity.h"
#include <iostream>
#include <iomanip>


using namespace std;

struct Counter
{
	Counter() : count1(0), count2(0), last_overlap_pos(0) {}
	void increment1() {
		++count1;
	}
	void increment2() {
		++count2;
	}
	int count1;
	int count2;
	int last_overlap_pos;
};
struct joinpair
{
	int x;
	int y;
	joinpair(int x, int y) :x(x), y(y) {};
	
	bool operator==(const joinpair & p) const
	{
		return (x == p.x && y == p.y)||(x == p.y && y == p.x);
	}
};
namespace std {
	template <> 
	class hash<joinpair> {
	public:
		size_t operator()(const joinpair &p) const
		{
			auto h1 = hash<int>{}(p.x);
			auto h2 = hash<int>{}(p.y);
			return h1 ^ h2;
		}
	};
	template<>
	struct equal_to<joinpair> {//等比的模板定制
	public:
		bool operator()(const joinpair& p1, const joinpair& p2) const
		{
			return (p1.x == p2.x && p1.y == p2.y) || (p1.x == p2.y && p1.y == p2.x);
		}

	};
};

typedef unordered_map<joinpair, bool> hashmap;

class dpjoin {
	static const int INF = 1 << 24;
	const double EPS = 1e-8;
	bool jaccard, cosine, overlap;
	double threshold;
	int k, fixedelta;
	int recordNum;
	int tokenNum;
	int maxWidowTokenFrequency;
	int delta_number;
	int candidateNum, intersectionNum, resultNum;
	vector<record> joinRecord;
	vector<result> joinResult;
public: vector<inverted_list> invertedList;
	vector<int> tokenFrequency;
	DeltaInvertedIndex deltaIndexS;
	ReverseDeltaInvertedIndex deltaIndex;
public:
	dpjoin() {
		jaccard = cosine =overlap= 0;
		recordNum = 0;
		tokenNum = -1;
		maxWidowTokenFrequency = 1;
		fixedelta = 2;
	}

	int ReadParameters(char*, char*, char* k, char* delta);
	int ReadParameters(char*, char*, char*);
	void ReadData(char*);
	void CalcPrefix();
	void BuildIndex();
	void PerformJoin();
	void WriteResults();

	void PerformTopkJoin();
	void PerformTopkDualJoin();
	void PerformTopkDeltaJoin();
	void Clear();
	int getThreshold(SortedList tmplist, int k); 
	int getThreshold2(SortedVector tmplist, int k);
	int CountOverlap(int x, int y, int xStart, int yStart);
	void addIndex(int token, int y,int pos);

	void ReorderRecs();
	void quickSort(vector<int> a, int l, int r);
	void WriteData(vector<record> joinRecord);

	//void VerifyCandidatesOld(int x, int y, int score, int requiredOverlap);
	//void VerifyCandidates(int xid, int yid, int score, int dualPrefixLen, int requiredOverlap);
	//void VerifyCandidatesWithLastOverlapPosInY(int xid, int yid, int score,int dualPrefixLen, int y_last_overlap_position, int requiredOverlap);
	int CountOverlap(vector<int>&, vector<int>&, int, int, int, int, int);
	int BinarySearch(vector<index_entry>&, int, int, int);
	int BinarySearch(vector<int>&, int, int, int);
	int BinarySearchPos(vector<int>&, int, int, int);
	int Getk();
	int GetListSize();
};
#endif
