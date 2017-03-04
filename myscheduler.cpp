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
		CPUList[i].processedTid = 0;
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
	ThreadDescriptorBlock *t = new ThreadDescriptorBlock();
	t->arriving_time = arriving_time;
	t->remaining_time = remaining_time;
	t->priority = priority;
	t->tid = tid;

	// add newly created thread to the vector
	ThreadsStatus *thread = new ThreadsStatus();
	thread->thread = t;
	thread->isScheduled = false;
	thread->CPU_id = 0;
	thread->isRunning = false;

	// insert thread into the thread vector based on arrival time
	auto it = threadVector.begin();

	while (it != threadVector.end() && it->thread->arriving_time <= thread->thread->arriving_time) {
		it++;
	}

	threadVector.insert(it, 1, *thread);

	// send message
	cout << "*** Thread #" << tid << " has been created. \n" << "\tArrival: " << arriving_time << "\tRemaining: " << remaining_time << "\tPriority: " << priority << endl << endl;

	return;
}

bool MyScheduler::Dispatch()
{
	// iterate through thread vector and schedule threads
	while (!threadVector.empty() && threadVector[0].thread->arriving_time <= timer) {
		push_to_ordered_list(&threadVector[0]);
		cout << "	*** Thread #" << threadVector[0].thread->tid << " has been scheduled.\n";
		threadVector.erase(threadVector.begin());
		
	}


	/*
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
	*/

	//Todo: Check if all the threads are finished; if so, return false
	//if (orderedVector.empty()) {
	//	return false;
	//}

	switch (policy)	{
	case FCFS: {		//First Come First Serve
		// iterate through cpus
		for (int i = 0; i < num_cpu; i++) {

			//if (CPUs[i] != NULL) {
			//	cout << "CPU " << i << "\tThread ID: " << CPUs[i]->tid << " Remaining: " << CPUs[i]->remaining_time << endl;
			//}
			//else {
			//	cout << "CPU " << i << ": null" << endl;
			//}

			if (CPUs[i] == NULL || CPUs[i]->remaining_time == 0) {
				//cout << "CPU " << i << " is free" << endl;
				auto it = orderedVector.begin();

				while (it != orderedVector.end() && (it->thread->remaining_time == 0 || it->isRunning)) {
					// remove from ordered vector if thread has been processed
					if (it->thread->remaining_time == 0) {
						it = orderedVector.erase(it);
					}
					else {	// move to next thread
						it++;
					}
				}

				// assign processor to next available thread in ordered vector;
				if (it != orderedVector.end()) {
					//cout << "CPU " << i << "\ttid: " << it->thread->tid << endl;
					CPUs[i] = it->thread;
					it->isRunning = true;
				}
				else {
					CPUs[i] = NULL;
				}
			}
		}
		break;
	}
	case STRFwoP: {	//Shortest Time Remaining First, without preemption
		// iterate through cpus
		for (int i = 0; i < num_cpu; i++) {
			if (CPUs[i] == NULL || CPUs[i]->remaining_time == 0) {
				auto it = orderedVector.begin();

				while (it != orderedVector.end() && (it->thread->remaining_time == 0 || it->isRunning)) {
					// remove from ordered vector if thread has been processed
					if (it->thread->remaining_time == 0) {
						it = orderedVector.erase(it);
					}
					else {	// move to next thread
						it++;
					}
				}

				// assign processor to next available thread in ordered vector;
				if (it != orderedVector.end()) {
					CPUs[i] = it->thread;
					it->isRunning = true;
				}
				else {
					CPUs[i] = NULL;
				}
			}
		}
		break;
	}
	case STRFwP: {	//Shortest Time Remaining First, with preemption
		auto it = orderedVector.begin();

		// assign first n threads in ordered vector to a cpu
		for (int i = 0; i < num_cpu; i++) {
			while (it != orderedVector.end() && it->thread->remaining_time == 0) {
				it = orderedVector.erase(it);
			}

			if (it != orderedVector.end()) {
				CPUs[i] = it->thread;
				it->isRunning = true;
				it++;
			}
			else {
				CPUs[i] = NULL;
			}
		}

		// update running status of preempted threads
		while (it != orderedVector.end() && it->isRunning) {
			it->isRunning = false;
			it++;
		}

		break;
	}
	case PBS: {		//Priority Based Scheduling, with preemption
		auto it = orderedVector.begin();

		// assign first n threads in ordered vector to a cpu
		for (int i = 0; i < num_cpu; i++) {
			while (it != orderedVector.end() && it->thread->remaining_time == 0) {
				it = orderedVector.erase(it);
			}

			if (it != orderedVector.end()) {
				CPUs[i] = it->thread;
				it->isRunning = true;
				it++;
			}
			else {
				CPUs[i] = NULL;
			}
		}

		// update running status of preempted threads
		while (it != orderedVector.end() && it->isRunning) {
			it->isRunning = false;
			it++;
		}

		break;
	}
	default:
		cout << "Invalid policy!";
		throw 0;
	}

	return !(orderedVector.empty() && threadVector.empty());
}

// add the new thread into the ordered vector 
void MyScheduler::push_to_ordered_list(ThreadsStatus *thread) {
	// find a proper place in the vector to push the thread based on policyv
	// vector head gets to process first, followed by the second item, etc.

	// get available CPU number; if none, send a message and return
	//if (unsigned int availableCPU = getAvailableCPUIndex() == NULL) {
	//	cout << "All CPUs are currently busy.\n";
	//	return;
	//}

	switch (policy)
	{
	case FCFS:		//First Come First Serve
		orderedVector.push_back(*thread);
		break;
	case STRFwoP: {	//Shortest Time Remaining First, without preemption
		auto it = orderedVector.begin();

		// find first thread with greater remaining time in ordered vector that is not currently running
		while (it != orderedVector.end() && (it->thread->remaining_time <= thread->thread->remaining_time || it->isRunning)) {
			it++;
		}

		orderedVector.insert(it, *thread);
		break;
	}
	case STRFwP: {	//Shortest Time Remaining First, with preemption
		auto it = orderedVector.begin();

		// find first thread with greater remaining time
		while (it != orderedVector.end() && it->thread->remaining_time <= thread->thread->remaining_time) {
			it++;
		}

		// insert thread in front of thread with greater remaining time
		orderedVector.insert(it, 1, *thread);
		break;
	}
	case PBS: {		//Priority Based Scheduling, with preemption
		auto it = orderedVector.begin();

		// find first thread with lower priority
		while (it != orderedVector.end() && it->thread->priority <= thread->thread->priority) {
			it++;
		}

		// insert thread in front of lower priority thread
		orderedVector.insert(it, 1, *thread);
		break;
	}
	default:
		cout << "Invalid policy!";
		throw 0;
	}

	return;

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
		if (CPUList[i].processedTid == tid) {
			CPUList[i].isBusy = false;
			CPUList[i].processedTid = 0;
		}
	}
}

