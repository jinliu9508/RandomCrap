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

//Define your data structure here.
struct ThreadsStatus {
	ThreadDescriptorBlock *thread;		// thread pointer
	bool isScheduled;					// whether in queue or already scheduled
	unsigned int CPU_id;				// if scheduled, record the id of CPU that handles this thread
	ThreadsStatus *next;				// linked list, point to next thread
};

struct CPUStatus {
	unsigned int id;					// id of CPU
	bool isBusy;						// is there thread is processing in this CPU
	unsigned int proccessedTid;			// if there is, what is the id of the thread
};


class MyScheduler: public Scheduler {
public:
	MyScheduler(Policy p, unsigned int n) : Scheduler(p, n) {
		// initialize the data structure
		threadList = NULL;
		for (int i = 0; i < num_cpu; i++) {
			CPUList[i].id = i;
			CPUList[i].isBusy = false;
			CPUList[i].proccessedTid = 0;
		}
	}
	bool Dispatch() override; //Function to implement scheduling policy and to keep a check on processed threads
	void CreateThread(int arriving_time, int remaining_time, int priority, int tid) override; //Function to create threads and insert them in student defined data structure

	//Declare additional methods(s) below if needed.
	ThreadsStatus *threadList = NULL;					// declare and initialize the thread list
	CPUStatus *CPUList = new CPUStatus[num_cpu];
	void addThreadToList(ThreadDescriptorBlock*);		// add a thread to the list
	void removeThreadFromList(unsigned int tid);
};
