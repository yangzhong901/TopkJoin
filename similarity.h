#pragma once
class Similarity
{
public:
	int k;
	Similarity(int num) { k = num; };
	~Similarity();

	int upperbound_access(int xlen, int xpos, int ylen, int ypos);
	int upperbound_probe(int xlen, int xpos);
	int upperbound_index(int xlen, int xpos);
};
