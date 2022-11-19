#include "dpjoin.h"
#include "sort_list.h"
#include "events_queue.h"
#include "similarity.h"
#include <sys/timeb.h>

using namespace std;

typedef unordered_map<joinpair, bool> hashmap;
typedef unordered_set<int> yhash;

void dpjoin::PerformTopkDeltaJoin()
{
	
	int k = Getk();
	int delta = 2;//delta从2开始递增
	Similarity sim(k);
	int x, y, xlen, ylen, xpos, ypos, token, score, thres, curthres, curdelta,n=0;

	hightime htime;
	double totalpqtime = 0.0;

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
		
		curthres = getThreshold(tmpresults, k);		

		if (thres <= curthres)
			break;

		xlen = joinRecord[x].len;
		
		if (thres - curthres < delta)
		{
			if (delta > 1)	delta--;
			curdelta = (thres - curthres);
		}
		else if(delta<200)
		{
			delta++;
			curdelta = delta;
		}

		htime.hightimeEnd();
		totalpqtime += htime.interval;

		for (int i = 0; i < curdelta; i++)
		{
			if (sim.upperbound_probe(xlen, xpos + i) > curthres)
			{				
				addIndex(joinRecord[x].tokenArray[xpos + i], x, xpos + i);
			}
			else
			{
				curdelta = i;
				break;
			}
		}
		
		for (int i = 0; i < curdelta; i++)
		{
			if ((xpos + i) >= xlen) break;
			token = joinRecord[x].tokenArray[xpos + i];
			for (int j = 0; j < (int)invertedList[token].list.size(); j++)
			{
				y = invertedList[token].list[j].rec;
				if (y == x) continue;

				ylen = joinRecord[y].len;
				ypos = invertedList[token].list[j].pos;
				

				if (ylen >= curthres)
				{
					
					auto it = candidates.find(joinpair(x, y));
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
							curthres = getThreshold(tmpresults, k);
						}
					}
				}
			}
		}
		htime.hightimeStart();
		thres = sim.upperbound_probe(xlen, xpos + curdelta);
		if (xpos + curdelta < xlen&& thres> curthres)//添加x的下一个event
		{
			events.add(Event(x, xpos + delta, thres));
		}
		htime.hightimeEnd();
		totalpqtime += htime.interval;
	}

	cout << "DLJ Candidates: " + to_string(candidateNum) << endl;
	cout << "DLJ kth Result overlap: " + to_string(tmpresults.get().sim) << endl;
	cout << "PQtime: " + to_string(totalpqtime) << endl;
	
}

