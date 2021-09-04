#ifndef _PLUGINLOADER_
#define _PLUGINLOADER_

#include <iostream>
#include <dlfcn.h>

template <class T>
class PluginLoader
{
 public:
  PluginLoader(const char* pluginLibrary);
  ~PluginLoader();

  void libCreate()  { element = pCreate(); }
  void libDestroy() { pDestroy(element); }
  T* getInstance()  { return element; }

 private:
  void* pluginHandle;
  T* element;

  T* (*pCreate) (void);  // pCreate  is a pointer to a function
  void (*pDestroy) (T*); // pDestroy is a pointer to a function
};


// ############################################################
// # Implementation of template class must stay in the header #
// ############################################################


template <class T>
PluginLoader<T>::PluginLoader(const char* pluginLibrary)
{
  char* error;

  pluginHandle = dlopen(pluginLibrary, RTLD_LAZY);
  if (!pluginHandle)
    {
      std::cout << dlerror() << std::endl;
      exit(EXIT_FAILURE);
    }

  pCreate = (T*(*)(void))dlsym(pluginHandle,"create"); // Call to external function of plugin
  if ((error = dlerror()) != NULL)
    {
      std::cout << error << std::endl;
      exit(EXIT_FAILURE);
    }

  pDestroy = (void(*)(T*))dlsym(pluginHandle,"destroy"); // Call to external function of plugin
  if ((error = dlerror()) != NULL)
    {
      std::cout << error << std::endl;
      exit(EXIT_FAILURE);
    }
}

template <class T>
PluginLoader<T>::~PluginLoader()
{
  dlclose(pluginHandle);
}

#endif
