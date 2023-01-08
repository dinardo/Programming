/*
clang++  -ansi -pedantic -Wall -Wextra -m64 -O3 --std=c++11 -o myThreads myThreads.cc
*/

#include <iostream>
#include <unistd.h>
#include <random>
#include <deque>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>


// ###################################
// # Producer / Consumer interaction #
// ###################################

std::mutex myMutex;
static std::deque< std::pair<int, int> > sharedMemory;
const int endOfProcess = -1;
const unsigned int microSec = 1000000;


typedef struct
{
  int myID;
  unsigned int howMany;
}  producerData;


typedef struct
{
  int myID;
} consumerData;


void Producer (producerData myArgs)
{
  std::default_random_engine gen;
  std::uniform_int_distribution<int> niform(0.5*microSec,1.5*microSec);

  std::cout << "I'm producer " << myArgs.myID << ", producing " << myArgs.howMany << std::endl;

  unsigned int i = 0;
  while (i < myArgs.howMany)
    {
      usleep(niform(gen));

      myMutex.lock();
      sharedMemory.push_back(std::make_pair(myArgs.myID,i));
      myMutex.unlock();

      i++;
    }

  myMutex.lock();
  sharedMemory.push_back(std::make_pair(endOfProcess,0));
  myMutex.unlock();

  std::cout << "Producer " << myArgs.myID << " is done" << std::endl;
}


void Consumer (consumerData myArgs)
{
  std::pair<int,int> val = std::make_pair(0,0);
  bool popped;

  std::default_random_engine gen;
  std::uniform_int_distribution<int> niform(0.5*microSec,1.5*microSec);

  std::cout << "I'm consumer " << myArgs.myID << std::endl;

  while (val.first != endOfProcess)
    {
      std::this_thread::sleep_for(std::chrono::microseconds(niform(gen)));

      popped = false;
      myMutex.lock();
      if (sharedMemory.size() != 0)
        {
          val = sharedMemory.front();
          sharedMemory.pop_front();
          popped = true;
        }
      myMutex.unlock();

      if ((popped == true) && (val.first != endOfProcess)) std::cout << "Consumer " << myArgs.myID << " popped value " << val.second << " from producer " << val.first << std::endl;
    }

  std::cout << "Consumer " << myArgs.myID << " is done" << std::endl;
}


// #######################
// # Handle Start / Stop #
// #######################

bool                        doExit;
std::atomic<bool>           keepRunning;
std::condition_variable_any wakeUp;
std::recursive_mutex        theMtx;


void waitForCompletion()
{
  std::unique_lock<std::recursive_mutex> theGuard(theMtx);
  wakeUp.wait(theGuard, []() { return doExit; });
}


void informImDone()
{
    std::unique_lock<std::recursive_mutex> theGuard(theMtx);
    doExit = true;
    theGuard.unlock();
    wakeUp.notify_one();
}


void Start(float& parameter)
{
  std::cout << "Start: " << parameter << std::endl;

  // ########################
  // # Testing failure mode #
  // ########################
  while(keepRunning == true)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      throw std::runtime_error("Testig failure mode: while Starting");
    }

  informImDone();

  throw std::runtime_error("Testig failure mode: end of Start");
}


void Stop(std::thread& theThread)
{
  keepRunning = false;

  waitForCompletion();

  if(theThread.joinable() == true) theThread.join();

  std::cout << "Stop" << std::endl;
}


void wrapperStart(float& parameter, std::promise<void>&& promise)
{
  try
    {
      Start(parameter);
      promise.set_value();
    }
  catch (...)
    {
      informImDone();
      promise.set_exception(std::current_exception());
    }
}


int main ()
{
  // #####################
  // # Test Start / Stop #
  // #####################

  std::promise<void> promise;
  std::future<void> future = promise.get_future();
  float par = 0;

  keepRunning = true;
  doExit      = false;

  std::thread thrStart(wrapperStart, std::ref(par), std::move(promise));
  // thrStart.detach();


  // ##########################################
  // # Test Producer / Consumer through queue #
  // ##########################################

  unsigned int howMany = 10;
  producerData pData;
  consumerData cData;

  pData.myID    = 1;
  pData.howMany = howMany;
  std::thread thrProducer1(Producer, pData);

  cData.myID = 1;
  std::thread thrConsumer1(Consumer, cData);

  pData.myID    = 2;
  pData.howMany = howMany;
  std::thread thrProducer2(Producer, pData);

  cData.myID = 2;
  std::thread thrConsumer2(Consumer, cData);

  thrProducer1.join();
  thrProducer2.join();

  thrConsumer1.join();
  thrConsumer2.join();


  // #####################
  // # Test Start / Stop #
  // #####################
  Stop(thrStart);
  try
    {
      future.get();
    }
  catch(std::runtime_error& e)
    {
      std::cout << e.what() << std::endl;;
    }
}
