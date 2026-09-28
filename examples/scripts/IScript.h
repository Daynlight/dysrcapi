// dysrcapi
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#pragma once



#if (defined(_WIN32) || defined(_WIN64))
  #ifdef BUILDING_SCRIPT_DLL
    #define SCRIPT_API __declspec(dllexport)
  #else
    #define SCRIPT_API __declspec(dllimport)
  #endif
#else
  #define SCRIPT_API 
#endif



class IScript{
public:
  virtual void exec() = 0;
};


#ifdef BUILDING_SCRIPT_DLL
#define REGISTER_SCRIPT(ScriptClassName, ScriptInterfaceName) \
extern "C" ScriptInterfaceName* SCRIPT_API GetScript() { \
  ScriptClassName* script = new ScriptClassName(); \
  return (ScriptInterfaceName*)script; \
}; \
   \
   \
   \
extern "C" void SCRIPT_API DeleteScript(ScriptInterfaceName* script) { \
  ScriptClassName* temp_script = (ScriptClassName*)script; \
  delete temp_script; \
};
#endif
