#include "events_queue.h"

EventQueue::EventQueue()
{
}

EventQueue::~EventQueue()
{
}

void EventQueue::add(Event evn)
{
	events_queue.push(evn);
}

Event EventQueue::top()
{	
	Event env = events_queue.top();
	events_queue.pop();
	return env;
}

bool EventQueue::isEmpty()
{
	return events_queue.size()== 0;
}

