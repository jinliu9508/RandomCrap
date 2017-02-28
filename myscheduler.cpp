//myschedule.cpp
/*Define all the functions in 'myschedule.h' here.*/
#include "myscheduler.h"

// constructor
MyScheduler::MyScheduler(Policy p, unsigned int n) : Scheduler(p, n) {
	// initialize the data structure
	threadVector.clear();
	CPUList = new CPUStatus[num_cpu];
	num_scheduledCPU = 0;
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

	// create a thread using passed-in values
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

	threadVector.push_back(thread);

	// send message
	cout << "	*** Thread #" << tid << " has been created.\n";
}

bool MyScheduler::Dispatch()
{
	while (!threadVector.empty() && num_scheduledCPU <= num_cpu) {
		push_to_ordered_list(threadVector.begin);
		threadVector.erase(threadVector.begin);
		cout << "	*** Thread #" << threadVector.begin()->thread->tid << " has been scheduled.\n";
	}


	//Todo: Check and remove finished threads
	if (!orderedVector.empty()) {
		for (auto it = begin(orderedVector); it != end(orderedVector); ++it) {	// loop through the vector
			if (it->thread->remaining_time == 0) {
				orderedVector.erase(it);		// evict if a thread that has done processes (Remaining time = 0)
				free_CPU(it->thread->tid);
				cout << "The thread #" << it->thread->tid << " has done processing and removed from the vector.\n";
			}
		}
	}
	//Todo: Check if all the threads are finished; if so, return false
	if (orderedVector.empty()) {
		return false;
	}


	switch (policy)
	{
	case FCFS:		//First Come First Serve
		while (!orderedVector.empty()) {
			// ***** process threads in ordered vector
		}
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

// add the new thread into the ordered vector 
void MyScheduler::push_to_ordered_list(ThreadsStatus thread) {
	// find a proper place in the vector to push the thread based on policyv
	// vector head gets to process first, followed by the second item, etc.

	// get available CPU number; if none, send a message and return
	if (unsigned int availableCPU = getAvailableCPUIndex() == NULL) {
		cout << "All CPUs are currently busy.\n";
		return;
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


}

unsigned int MyScheduler::getAvailableCPUIndex() {
	for (unsigned int i = 0; i < num_cpu; i++) {		// find nearest CPU that is not busy
		if (CPUList[i].isBusy == false) {
			return i;
		}
	}
	return NULL;
}

void MyScheduler::free_CPU(unsigned int tid) {
	// free the CPU that is currently processing tid
	for (unsigned int i = 0; i < num_cpu; i++) {
		if (CPUList[i].proccessedTid == tid) {
			CPUList[i].isBusy = false;
			CPUList[i].proccessedTid = 0;
		}
	}
}

