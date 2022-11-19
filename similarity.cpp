#include "similarity.h"
#include "dpjoin.h"

Similarity::~Similarity()
{
}

int Similarity::upperbound_access(int xlen, int xpos, int ylen, int ypos)
{
	int xsp = xlen - xpos;
	int ysp = ylen - ypos;
	return xsp < ysp ? xsp : ysp;
}

int Similarity::upperbound_probe(int xlen, int xpos)
{	
	return xlen - xpos;
}

int Similarity::upperbound_index(int xlen, int xpos)
{
	return xlen - xpos;
}
