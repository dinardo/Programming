#include <vector>

namespace EventData
{
  struct SubData
  {
    int subData1;
    double subData2;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar& subData1;
      ar& subData2;
    }
  };

  struct DataStructure
  {
    int data1;
    double data2;
    std::vector<SubData> subVec;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar& data1;
      ar& data2;
      ar& subVec;
    }
  };
}
