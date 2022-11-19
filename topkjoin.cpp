#include "dpjoin.h"

using namespace std;

typedef unordered_set<int> yhash;
void dpjoin::PerformTopkJoin()
{
	

	hightime htime;
	double totalpqtime=0.0;

	int k = Getk();
	Similarity sim(k);
	int x, y, xlen, ylen, xpos, ypos, token, score, thres, curthres,n=0;

	EventQueue events;
	SortedList tmpresults(k);
	
	hashmap candidates;
	
	for (x = 0; x < recordNum; x++) {
		Event env = Event(x, 0, joinRecord[x].len);
		events.add(env);

	}
	invertedList.resize(tokenNum);
	while (!events.isEmpty())
	{
		htime.hightimeStart();

		Event evn = events.top();

		x = evn.rid;
		xpos = evn.pos;
		thres = evn.threshold;
				
		curthres = getThreshold(tmpresults,k);
		if (thres <= curthres)
			break;
		
		xlen = joinRecord[x].len;
		token = joinRecord[x].tokenArray[xpos];

		htime.hightimeEnd();
		totalpqtime += htime.interval * 1000;

		addIndex(token, x, xpos);
		for (int j = 0; j < (int)invertedList[token].list.size(); j++)
		{
			y = invertedList[token].list[j].rec;
			if (x == y) continue;

			ylen = joinRecord[y].len;
			ypos = invertedList[token].list[j].pos;
			
			if (ylen >= curthres)
			{
				auto it = candidates.find(joinpair(x,y));
				if (it != candidates.end())
					continue;
				else
				{
					candidateNum++;
					candidates[joinpair(x, y)] = true;
					score = CountOverlap(x, y, xpos, ypos);
					if (tmpresults.size() >= k && score < curthres)
						continue;
					else
					{
						tmpresults.add(result(x, y, score));
						curthres = getThreshold(tmpresults,k);
					}
				}
			}
		}

		htime.hightimeStart();
		thres = sim.upperbound_probe(xlen, xpos+1);		

		if (xpos + 1 < xlen)//添加x的下一个event		
		{			
			events.add(Event(x, xpos + 1, thres));
		}
		htime.hightimeEnd();
		totalpqtime += htime.interval * 1000;				
		
	}

	cout << "SSJ Candidates: " + to_string(candidateNum) << endl;
	cout << "SSJ kth Result overlap: " + to_string(tmpresults.get().sim) << endl;
	cout << "PQtime: " + to_string(totalpqtime) << endl;	
}



int dpjoin::GetListSize()
{
	int entries = 0;

	for (size_t i = 0; i < invertedList.size(); i++)
	{
		if (invertedList[i].startingPosition == 0)
		{
			entries += invertedList[i].size();

		}
	}
	return entries;

};

int dpjoin::getThreshold(SortedList tmplist, int k)
{
	if (tmplist.size() >= k)
	{
		return tmplist.get().sim;
	}
	return 0;
}
int dpjoin::getThreshold2(SortedVector tmplist, int k)
{
	if (tmplist.size() >= k)
	{
		return tmplist.get().sim;
	}
	return 0;
}
int dpjoin::CountOverlap(int x, int y,  int xStart, int yStart)
{
	vector<int>& xArray = joinRecord[x].tokenArray;
	vector<int>& yArray = joinRecord[y].tokenArray;
	int xLen = xArray.size();
	int yLen = yArray.size();
	int i = xStart, j = yStart, ans = 0;
	while (i < xLen && j < yLen) {
		if (xArray[i] == yArray[j]) ++i, ++j, ++ans;
		else {
			if (xArray[i] < yArray[j]) ++i;
			else ++j;
		}
	}
	return ans;
}


