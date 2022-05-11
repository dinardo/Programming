/*
 Simple example of application of decorators in C++

 The use case is related to adding messages to member
 function of an hypothetical detector calibration class

 c++ -std=c++1z -o Messages Messages.cpp
*/

#include <iostream>


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
  virtual void Start(int runNumber, float parameter) = 0;
  virtual int  Stop()                                = 0;

  template<typename M, typename FunType, typename ObjType, typename... Args>
  void MsgDecorator(M& msg, FunType fun, ObjType& obj, Args... args)
  {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    msg.output     = "DEFAULT";
    msg.defaultVal = -10;
  }
};


// ########################
// # Concrete calibration #
// ########################
class PixelAlive : public BaseCalibration
{
public:
  void Start(int runNumber, float parameter)
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

  template<typename M, typename FunType, typename ObjType, typename... Args>
  void MsgDecorator(M& msg, FunType fun, ObjType& obj, Args... args)
  {
    if constexpr (std::is_same<FunType, decltype(&ObjType::Start)>::value)
      {
        try
          {
            (obj.*fun)(args...);
            msg.val    = internal;
            msg.output = "START: GOOD";
          }
        catch(...)
          {
            msg.val    = 0;
            msg.output = "START: BAD";
          }
      }
    else if constexpr (std::is_same<FunType, decltype(&ObjType::Stop)>::value)
      {
        std::cout << "SPECIAL CASE" << std::endl;

        try
          {
            msg.val    = (obj.*fun)(args...);
            msg.output = "STOP: GOOD";
          }
        catch(...)
          {
            msg.val    = 0;
            msg.output = "STOP: BAD";
          }
      }
    else
      std::cout << "Unrecognized member function: " << typeid(fun).name() << std::endl;
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
  void Start(int runNumber, float parameter)
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


  MyMessage msg_pa = {"", "", 0, "", 0};
  PixelAlive pa;

  std::cout << "\n=== PixelAlive ===" << std::endl;
  pa.MsgDecorator(msg_pa, &PixelAlive::Start, pa, runNumber, parameter);
  std::cout << msg_pa.output << " " << msg_pa.val << " " << msg_pa.defaultVal << std::endl;

  pa.MsgDecorator(msg_pa, &PixelAlive::Stop, pa);
  std::cout << msg_pa.output << " " << msg_pa.val << " " << msg_pa.defaultVal << std::endl;


  BaseMessage msg_pn = {"", "", 0};
  PedeNoise pn;

  std::cout << "\n=== PedeNoise ===" << std::endl;
  pn.MsgDecorator(msg_pn, &PedeNoise::Start, pn, runNumber, parameter);
  std::cout << msg_pn.output << " " << msg_pn.defaultVal << std::endl;

  pn.MsgDecorator(msg_pn, &PedeNoise::Stop, pn);
  std::cout << msg_pn.output << " " << msg_pn.defaultVal << std::endl;


  return 0;
}
