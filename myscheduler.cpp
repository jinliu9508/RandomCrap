//myschedule.cpp
/*Define all the functions in 'myschedule.h' here.*/
#include "myscheduler.h"

void MyScheduler::CreateThread(int arriving_time, int remaining_time, int priority, int tid) //Thread ID not Process ID
{
	//Function to Create Thread(s) and insert them in the student
	//defined data structure

	// create a thread using passed in values
	ThreadDescriptorBlock *t;
	t->arriving_time = arriving_time;
	t->remaining_time = remaining_time;
	t->priority = priority;
	t->tid = tid;

	// add newly created thread to the list
	addThreadToList(t);
}

bool MyScheduler::Dispatch()
{
	//Todo: Check and remove finished threads
	ThreadsStatus *iterator = threadList;
	ThreadDescriptorBlock *thr;
	while (iterator != NULL) {
		thr = iterator->thread;
		if (thr->remaining_time == 0) {
			removeThreadFromList(thr->tid);
		}
	}

	//Todo: Check if all the threads are finished; if so, return false
	if (threadList == NULL)
		return false;
	

	switch(policy)
	{
		case FCFS:		//First Come First Serve

			break;
		case STRFwoP:	//Shortest Time Remaining First, without preemption

			break;
		case STRFwP:	//Shortest Time Remaining First, with preemption

			break;
		case PBS:		//Priority Based Scheduling, with preemption

			break;
		default :
			cout<<"Invalid policy!";
			throw 0;
	}
	return true;
}

void MyScheduler::addThreadToList(ThreadDescriptorBlock *t) {
	// initialize the list if empty
	if (threadList == NULL) {
		threadList = new ThreadsStatus;
		threadList->thread = t;
		threadList->isScheduled = false;
		threadList->CPU_id = 0;
		threadList->next = NULL;
	}
	else {		// add the thread to the end
		ThreadsStatus *iterator = threadList;
		while (iterator != NULL)
			iterator = iterator->next;
		iterator->thread = t;
		iterator->isScheduled = false;
		iterator->CPU_id = 0;
		iterator->next = NULL;
	}
}

// remove thread in the threadlist that has id of tid
void removeThreadFromList(unsigned int tid) {

}
