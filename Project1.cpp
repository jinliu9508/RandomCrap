// via : Jinwei Liu

#include "myscheduler.h"
#include <iostream>
#include <stdlib.h>     /* srand, rand */

int main(int argc, char* argv[])
{
	cout << "Test bench via: Jinwei Liu\n";

	Policy policy = PBS;			// replace policy: FCFS, STRFwoP, STRFwP, PBS
	unsigned int CPU_num = 2;		// number of CPU
	int thread_num = 5;	// number of thread to be scheduled

	MyScheduler scheduler(policy, CPU_num);
	
	//intializing variables:
	int randomRemainingTime = 0;
	int randomPriority = 0;
	
	srand(time(0)); //seeds rand() so it gives random # every time
	
	// create thread
	for (int i = 0; i < thread_num; i++) {
		randomPriority = rand() % thread_num;		// random int from 0 to thread_num -1
		randomRemainingTime = rand() % thread_num + 1;	// remaining time from 1 to thread_num

		scheduler.CreateThread(i, randomRemainingTime, randomPriority, i);
		cout << "\t*** Thread #" << i << " has been created. \n" << "\tArrival: " << i << "\tRemaining: " << randomRemainingTime << "\tPriority: " << randomPriority << endl << endl;
	}

	// print the scheduler policy type
	switch (policy)
	{
	case FCFS:
		cout << "FCFS ";
		break;
			
	case STRFwoP:
		cout << "STRFwoP ";
		break;
			
	case STRFwP:
		cout << "STRFwP ";
		break;
			
	case PBS:
		cout << "PBS " << endl;
		break;
			
	default:
		cout << "Invalid policy!";
		throw 0;
		break;
	}
	
	cout << "running result: " << endl;
	scheduler.Go();

	return 0;
}
