/**
 * Semaphore example, written in C++ May 4, 2014
 * Compiled on OSX 10.9, using:
 * g++ -std=c++11 semaphore.cpp
 **/

#include <iostream>      
#include <thread>        
#include <mutex>         
#include <condition_variable>
 
std::mutex mtx;             // mutex for critical section
std::condition_variable cv; // condition variable for critical section  
bool ready = false;         // used to let other threads know when it is ready

/* Prints the thread id / max number of threads */
void print_num(int num, int max) {
  std::unique_lock<std::mutex> lck(mtx);
  while(!ready){ cv.wait(lck); }
  std::cout << "Thread: ";
  std::cout << num + 1 << " / " << max;
  std::cout << " currently has access" << std::endl;
}

/* Changes ready to true, and begins the threads printing */
void run(){
  std::unique_lock<std::mutex> lck(mtx);
  ready = true;
  cv.notify_all();
}
 
int main (){

  int threadnum = 15;
  std::thread threads[15];

  /* spawn threadnum threads */
  for (int id = 0; id < threadnum; id++)
    threads[id] = std::thread(print_num, id, threadnum);

  std::cout << "\nRunning " << threadnum << " in parallel: \n" << std::endl;

  run(); // Run the semaphores

  /* Merge all threads to the main thread */
  for(int id = 0; id < threadnum; id++)
    threads[id].join();

  std::cout << "\nCompleted semaphore example!\n" << std::endl;

  return 0;
}
