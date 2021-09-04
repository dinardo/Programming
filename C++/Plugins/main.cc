#include <iostream>
#include <fstream>
#include <vector>

#include "PluginLoader.h"
#include "Mother.h"

int main(int argc, char** argv)
{
  if (argc > 1)
    {
      typedef PluginLoader<Mother> PLdef;

      std::vector<PLdef*> vPlugin;
      std::vector<Mother*> vMother;

      std::string libraryDir = argv[1];
      std::string plName;


      std::cout << "Loading and initializing the library handler" << std::endl;
      std::ifstream fileID(libraryDir.c_str());
      while (fileID >> plName)
        vPlugin.push_back(new PLdef(plName.c_str()));
      fileID.close();


      std::cout << "Instantiating the libraries" << std::endl;
      for (std::vector<PLdef*>::iterator it = vPlugin.begin(); it != vPlugin.end(); it++)
        {
          (*it)->libCreate();
          vMother.push_back((*it)->getInstance());
        }


      std::cout << "Using the libraries" << std::endl;
      for (unsigned long i = 0; i < vMother.size(); i++)
        vMother[i]->f();


      std::cout << "Destroing the libraries" << std::endl;
      for (std::vector<PLdef*>::iterator it = vPlugin.begin(); it != vPlugin.end(); it++)
        (*it)->libDestroy();


      vPlugin.clear();
      vMother.clear();


      // ############################################
      // # An alternative way to delete the library #
      // ############################################
      // std::cout << "Loading and initializing the library handler" << std::endl;
      // std::ifstream fileID(libraryDir.c_str());
      // fileID >> plName;
      // fileID.close();
      // PLdef tmp(plName.c_str());


      // std::cout << "Instantiating the libraries" << std::endl;
      // tmp.libCreate();
      // vMother.push_back(tmp.getInstance());


      // std::cout << "Using the libraries" << std::endl;
      // for (unsigned long i = 0; i < vMother.size(); i++)
      //   {
      //     vMother[i]->f();
      //     delete vMother[i];
      //   }


      return 1;
    }
  else
    {
      std::cout << "Parameter missing: " << std::endl;
      std::cout << "./main [plugin_file_name]" << std::endl;

      exit(EXIT_FAILURE);
    }
}
