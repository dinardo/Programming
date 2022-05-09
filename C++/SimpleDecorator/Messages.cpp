/*
c++ -std=c++11 -o Messages Messages.cpp
*/

#include <iostream>
#include <string>
#include <typeinfo>


// #################
// # Basic message #
// #################
struct BaseMessage
{
  std::string input;
  std::string output;

  float defaultVal;
};


// #####################
// # Message extension #
// #####################
struct MyMessage : public BaseMessage
{
  std::string others;

  float val;
};


// ########################
// # Abstract calibration #
// ########################
class BaseCalibration
{
public:
  virtual void Start(int runNumbe, float parameter) = 0;
  virtual int  Stop()                               = 0;

  template<typename M, typename funType, typename classType, typename... Args>
  void MsgDecorator(M* msg, funType func, classType& obj, Args... args)
  {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    msg->output = "DEFAULT";
    msg->val    = -10;
  }
};


// ########################
// # Concrete calibration #
// ########################
class PixelAlive : public BaseCalibration
{
public:
  void Start(int runNumbe, float parameter)
  {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    internal = 10;
  }

  int Stop()
  {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    internal = 20;
    return 30;
  }

  template<typename M, typename funType, typename classType, typename... Args>
  void MsgDecorator(M msg, funType func, classType& obj, Args... args)
  {
    std::cout << "Function type: " << typeid(func).name() << " " << typeid(funType).name() << std::endl;

    if (std::is_same<funType, decltype(&classType::Start)>::value == true)
      {
        try
          {
            (obj.*func)(args...);
            msg->val    = internal;
            msg->output = "GOOD";
          }
        catch(...)
          {
            msg->val    = 0;
            msg->output = "BAD";
          }
      }
    else if (std::is_same<funType, decltype(&classType::Stop)>::value == true)
      {
        std::cout << "SPECIAL CASE" << std::endl;

        try
          {
            // msg->defaultVal = (obj.*func)(args...); // Not working
            (obj.*func)(args...);
            msg->defaultVal = 1;
            msg->output     = "GOOD STOP";
          }
        catch(...)
          {
            msg->defaultVal = 0;
            msg->output     = "BAD STOP";
          }
      }
  }

protected:
  double internal;
};


// #######################
// # Another calibration #
// #######################
class PedeNoise : public BaseCalibration
{
public:
  void Start(int runNumbe, float parameter)
  {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }

  int Stop()
  {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return 0;
  }
};


// ################
// # Main program #
// ################
int main()
{
  int   runNumber = 1;
  float parameter = 0;
  MyMessage* msg  = new MyMessage;


  PixelAlive pa;
  std::cout << "PixelAlive" << std::endl;
  pa.MsgDecorator(msg, &PixelAlive::Start, pa, runNumber, parameter);
  std::cout << msg->output << " " << msg->val << " " << msg->defaultVal << std::endl;

  pa.MsgDecorator(msg, &PixelAlive::Stop, pa);
  std::cout << msg->output << " " << msg->val << " " << msg->defaultVal << std::endl;


  PedeNoise pn;
  std::cout << "\nPedeNoise" << std::endl;
  pn.MsgDecorator(msg, &PedeNoise::Start, pn, runNumber, parameter);
  std::cout << msg->output << " " << msg->val << " " << msg->defaultVal << std::endl;

  pn.MsgDecorator(msg, &PedeNoise::Stop, pn);
  std::cout << msg->output << " " << msg->val << " " << msg->defaultVal << std::endl;


  return 0;
}
