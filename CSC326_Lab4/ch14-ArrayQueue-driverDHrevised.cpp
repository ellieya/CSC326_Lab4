//  Created by Frank M. Carrano and Timothy M. Henry.
//  Copyright (c) 2017 Pearson Education, Hoboken, New Jersey.

#include "ArrayQueue.h" // ADT Queue operations
//#include "LinkedQueue.h" // ADT Queue operations

#include <iostream>
#include <string>


void queueTester(QueueInterface<std::string>* queuePtr);


int main()
{
	QueueInterface<std::string>* queuePtr = new ArrayQueue<std::string>( );
    queueTester(queuePtr);
    std::cout << "Thanks for testing!\n";
   return 0;
}  // end main


void queueTester(QueueInterface<std::string>* queuePtr)
{
   std::string items[] = {"one", "two", "three", "four", "five", "six"};
	std::cout << "Empty: " << queuePtr->isEmpty() << std::endl;
	for (int i = 0; i < 6; i++)
   {
		std::cout<<"Adding " << items[i] << std::endl;
      bool success = queuePtr->enqueue(items[i]);
      if (!success)
         std::cout << "Failed to add " << items[i] << " to the queue." << std::endl;
	}
   
	std::cout << "Empty?: " << queuePtr->isEmpty() << std::endl;
	
	for (int i = 0; i < 6; i++) 
   {
		std::cout << "Loop " << i << std::endl;
      
      try 
      {
         std::cout << "peekFront: " << queuePtr->peekFront() << std::endl;
      } 
      catch (PrecondViolatedExcep e) 
      {
         std::cout << e.what() << std::endl;
      }  // end try/catch

		std::cout << "Empty: " << queuePtr->isEmpty() << std::endl;
		std::cout << "dequeue: " << queuePtr->dequeue() << std::endl;
	}
   
   std::cout << "dequeue with an empty queue: " << std::endl;
	std::cout << "Empty: " << queuePtr->isEmpty() << std::endl;
	std::cout << "dequeue: " << queuePtr->dequeue() << std::endl; // nothing to dequeue!
   
   try 
   {
      std::cout << "peekFront with an empty queue: " << std::endl;
	   std::cout << "peekFront: " << queuePtr->peekFront() << std::endl; // nothing to see!
   } 
   catch (PrecondViolatedExcep e) 
   {
      std::cout << e.what() << std::endl;
   }  // end try/catch
   
}  // end queueTester


/*
 Testing the Array-based queue:
 Empty: 1
 Adding one
 Adding two
 Adding three
 Adding four
 Adding five
 Adding six
 Empty?: 0
 Loop 0
 peekFront: one
 Empty: 0
 dequeue: 1
 Loop 1
 peekFront: two
 Empty: 0
 dequeue: 1
 Loop 2
 peekFront: three
 Empty: 0
 dequeue: 1
 Loop 3
 peekFront: four
 Empty: 0
 dequeue: 1
 Loop 4
 peekFront: five
 Empty: 0
 dequeue: 1
 Loop 5
 peekFront: six
 Empty: 0
 dequeue: 1
 dequeue with an empty queue:
 Empty: 1
 dequeue: 0
 peekFront with an empty queue:
 peekFront: Precondition Violated Exception: peekFront() called with empty queue
 Thanks for testing!
 */