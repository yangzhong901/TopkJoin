#pragma once
#include<queue>

using namespace std;
struct Event
{
	int rid;
	int pos;
	int threshold;

	Event(int rid, int pos, int threshold) : rid(rid), pos(pos), threshold(threshold){};
	bool operator <(const Event& other)
	{
		return threshold <= other.threshold;
	}
};

struct EventGreat
{
	bool operator()(Event e1, Event e2) { return e1.threshold <= e2.threshold; };
};

struct EventLess
{
	bool operator()(Event e1, Event e2) { return e1.threshold > e2.threshold; };
};

class EventQueue
{
	priority_queue<Event, vector<Event>, EventGreat> events_queue;
public:		
	EventQueue();
	~EventQueue();
	void add(Event evn);
	Event top();
	bool isEmpty() ;
	int size() { return events_queue.size(); };
};