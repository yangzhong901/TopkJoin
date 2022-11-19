#pragma once
#ifndef INVERTED_INDEX_H_
#define INVERTED_INDEX_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>


using namespace std;
struct record
{
	int rid;
	int len;
	vector<int> tokenArray;
	int indexPrefixLength, probePrefixLength;
};

struct result
{
	int xid, yid, sim;
	//double sim;
	result(int x, int y, int sim) :xid(x), yid(y), sim(sim) {};
};

struct index_entry
{
	int rec;
	int pos;
	index_entry() {}
	index_entry(int object_id_input, int position_input) : rec(object_id_input), pos(position_input) {}
};

struct inverted_list
{
	int startingPosition;
	vector<index_entry> list;
	inverted_list() :startingPosition(-1) {}
	int size() { return list.size() - startingPosition; }
};

#endif