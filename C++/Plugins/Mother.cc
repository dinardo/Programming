#include "Mother.h"


// #######################################################################
// # Thisis necessary even if the destrutor is defined as purely virtual #
// #######################################################################


Mother::~Mother()
{
  std::cout << "Pure virtual Dtor of Mother" << std::endl;
}
