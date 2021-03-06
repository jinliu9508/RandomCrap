#pragma once
//myschedule.h
/* Students need to define their own data structure to contain
and access objects from 'thread class'. The 'CreateThread()'
function should insert the threads into the student defined
data structure after creating them.
They are allowed to add any additional functionality (only
declaration in this file, define in 'myschedule.cpp')
which they might find helpful.*/

#include "scheduler.h"
#include <vector>

//Define your data structure here.
struct ThreadsStatus {
	ThreadDescriptorBlock *thread;		// thread pointer
	bool isScheduled;					// whether in queue or already scheduled
	unsigned int CPU_id;				// if scheduled, record the id of CPU that handles this thread
	bool isRunning;						// true if thread is currently being processed by a cpu
};


class MyScheduler : public Scheduler {
public:
	MyScheduler(Policy p, unsigned int n);				// constructor
	~MyScheduler();										// destructor
	bool Dispatch() override; //Function to implement scheduling policy and to keep a check on processed threads
	void CreateThread(int arriving_time, int remaining_time, int priority, int tid) override; //Function to create threads and insert them in student defined data structure

	// variables
	vector<ThreadsStatus> threadVector;			// vector to store all threads
	vector<ThreadsStatus> orderedVector;		// vector to store ordered threads
	unsigned int num_scheduledCPU;				// used for checking available cpu

	// additional functions
	void push_to_ordered_list(ThreadsStatus *thread);
	void clear_CPU();
};
