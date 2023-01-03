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


std::mutex myMutex;
static std::deque< std::pair<int,int> > sharedMemory;
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

void ThrowException()
{
  throw "Testig failure mode";
}


void Producer (producerData myArgs, std::promise<void>& prom)
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

  // ########################
  // # Testing failure mode #
  // ########################
  try
    {
      ThrowException();
      prom.set_value();
    }
  catch (...)
    {
      prom.set_exception(std::current_exception());
    }
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


int main ()
{
  std::promise<void> prom1;
  std::promise<void> prom2;

  unsigned int howMany = 10;
  producerData pData;
  consumerData cData;

  pData.myID    = 1;
  pData.howMany = howMany;
  std::thread thrProducer1(Producer, pData, std::ref(prom1));

  cData.myID = 1;
  std::thread thrConsumer1(Consumer, cData);

  pData.myID    = 2;
  pData.howMany = howMany;
  std::thread thrProducer2(Producer, pData, std::ref(prom2));

  cData.myID = 2;
  std::thread thrConsumer2(Consumer, cData);


  // ########################
  // # Testing failure mode #
  // ########################
  std::future<void> fu1 = prom1.get_future();
  try
    {
      fu1.get();
      std::cout << "Completed successfully\n";
    }
  catch (const char *msg)
    {
      std::cerr << "Exception: " << msg << "\n";
    }

  std::future<void> fu2 = prom2.get_future();
  try
    {
      fu2.get();
      std::cout << "Completed successfully\n";
    }
  catch (const char *msg)
    {
      std::cerr << "Exception: " << msg << "\n";
    }


  thrProducer1.join();
  thrProducer2.join();

  thrConsumer1.join();
  thrConsumer2.join();


  // Th.detach();
}
