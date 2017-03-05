//myschedule.cpp
/*Define all the functions in 'myschedule.h' here.*/


/* 
	Group members: Ghunley Zhang, Jinwei Liu, Kevin Jiang
	Date last modified: 3/4/2017
	Due: 3/5/2017 11:59pm

	Description: In this project we implement a schduler for a n-processor architecture.
		It would work for four policies, FCFS, SRTFwoP, SRTFwP, and PBSwP. 
*/

#include "myscheduler.h"

// constructor
MyScheduler::MyScheduler(Policy p, unsigned int n) : Scheduler(p, n) {
	// initialize the data structure
	threadVector.clear();
	num_scheduledCPU = 0;
	clear_CPU();
}

// destructor
MyScheduler::~MyScheduler() {
	threadVector.clear();
	orderedVector.clear();
	clear_CPU();
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
	//cout << "*** Thread #" << tid << " has been created. \n" << "\tArrival: " << arriving_time << "\tRemaining: " << remaining_time << "\tPriority: " << priority << endl << endl;

	return;
}

bool MyScheduler::Dispatch()
{
	// iterate through thread vector and schedule threads
	while (!threadVector.empty() && threadVector[0].thread->arriving_time <= timer) {
		push_to_ordered_list(&threadVector[0]);
		//cout << "	*** Thread #" << threadVector[0].thread->tid << " has been scheduled.\n";
		threadVector.erase(threadVector.begin());

	}

	//Todo: Check and remove finished threads
	//Todo: Check if all the threads are finished; if so, return false

	switch (policy)	{

	case FCFS: {		//First Come First Serve
		// iterate through cpus
		for (unsigned int i = 0; i < num_cpu; i++) {
			// an empty CPU is found
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

	case STRFwoP: {	//Shortest Time Remaining First, without preemption
		// iterate through cpus
		for (unsigned int i = 0; i < num_cpu; i++) {
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
		for (unsigned int i = 0; i < num_cpu; i++) {
			// get next ready thread
			it = orderedVector.begin();
			while (it != orderedVector.end() && (it->thread->remaining_time == 0 || it->isRunning)) {
				// remove from ordered vector if thread has been processed
				if (it->thread->remaining_time == 0) {
					it = orderedVector.erase(it);
				}
				else {	// move to next thread
					it++;
				}
			}

			// check if the CPU is able to be assigned to this thread
			if (it != orderedVector.end()) {
				if (CPUs[i] == NULL || CPUs[i]->remaining_time == 0 || CPUs[i]->remaining_time < it->thread->remaining_time) {
					if (CPUs[i] == NULL || CPUs[i]->remaining_time == 0) {
						CPUs[i] = it->thread;
						it->isRunning = true;
					}
					
					else {	// preemption
						// find the thread status that contains CPUs[i] thread
						auto iter = orderedVector.begin();
						while (iter->thread != CPUs[i] && iter != orderedVector.end()) {
							iter++;
						}

						iter->isRunning = false;	// turn off the status of the evict thread
						CPUs[i] = it->thread;
						it->isRunning = true;
					}
				}
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
		for (unsigned int i = 0; i < num_cpu; i++) {
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

	auto it = orderedVector.begin();

	switch (policy)
	{
	case FCFS:		//First Come First Serve
		while (it != orderedVector.end() && (it->thread->arriving_time <= thread->thread->arriving_time || it->isRunning)) {
			it++;
		}

		orderedVector.insert(it, *thread);
		break;
	case STRFwoP: {	//Shortest Time Remaining First, without preemption
		// find first thread with greater remaining time in ordered vector that is not currently running
		while (it != orderedVector.end() && (it->thread->remaining_time <= thread->thread->remaining_time || it->isRunning)) {
			it++;
		}

		orderedVector.insert(it, *thread);
		break;
	}
	case STRFwP: {	//Shortest Time Remaining First, with preemption
		bool inserted = false;

		// find first thread with greater remaining time
		while (it != orderedVector.end() && it->thread->remaining_time <= thread->thread->remaining_time) {
			if (it->isRunning && it != orderedVector.begin())	// preemption
			{
				orderedVector.insert(it-1, 1, *thread);
				inserted = true;
				break;
			}
			else {
				it++;
			}
				
		}

		// insert thread in front of thread with greater remaining time
		if (!inserted)
			orderedVector.insert(it, 1, *thread);

		break;
	}
	case PBS: {		//Priority Based Scheduling, with preemption
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

void MyScheduler::clear_CPU() {
	for (unsigned int i = 0; i < num_cpu; i++) {		// clear out the array of CPU indicator
		CPUs[i] = NULL;
	}
}
