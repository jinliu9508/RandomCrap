//myschedule.cpp
/*Define all the functions in 'myschedule.h' here.*/
#include "myscheduler.h"

// constructor
MyScheduler::MyScheduler(Policy p, unsigned int n) : Scheduler(p, n) {
	// initialize the data structure
	threadVector.clear();
	CPUList = new CPUStatus[num_cpu];
	for (unsigned int i = 0; i < num_cpu; i++) {		// clear out the array of CPU indicator
		CPUList[i].id = i;
		CPUList[i].isBusy = false;
		CPUList[i].proccessedTid = 0;
	}
}

// destructor
MyScheduler::~MyScheduler() {
	threadVector.clear();
	delete CPUList;
}

void MyScheduler::CreateThread(int arriving_time, int remaining_time, int priority, int tid) //Thread ID not Process ID
{
	//Function to Create Thread(s) and insert them in the student
	//defined data structure

	// create a thread using passed in values
	ThreadDescriptorBlock t;
	t.arriving_time = arriving_time;
	t.remaining_time = remaining_time;
	t.priority = priority;
	t.tid = tid;

	// add newly created thread to the vector
	ThreadsStatus thread;
	thread.thread = &t;
	thread.isScheduled = false;
	thread.CPU_id = 0;
	thread.next = NULL;

	threadVector.push_back(thread);
}

bool MyScheduler::Dispatch()
{
	//Todo: Check if all the threads are finished; if so, return false
	if (threadVector.empty())
		return false;

	//Todo: Check and remove finished threads
	vector<ThreadsStatus> iterator = threadVector.begin;
	// loop through the vector
	for (auto it = begin(threadVector); it != end(threadVector); ++it) {
		if (it->thread->remaining_time == 0) {
			threadVector.erase(it);		// delete if a thread has been processes
		}
	}

	switch (policy)
	{
	case FCFS:		//First Come First Serve

		break;
	case STRFwoP:	//Shortest Time Remaining First, without preemption

		break;
	case STRFwP:	//Shortest Time Remaining First, with preemption

		break;
	case PBS:		//Priority Based Scheduling, with preemption

		break;
	default:
		cout << "Invalid policy!";
		throw 0;
	}
	return true;
}

