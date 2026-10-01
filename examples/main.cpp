// dysrcapi
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include <filesystem>

#include "fmt/base.h"
#include "fmt/color.h"

#include "dysrcapi/dysrcapi.h"

#include "scripts/IScript.h"



int main(){
  fmt::println(fg(fmt::color::yellow) | fmt::emphasis::bold, "===========================");
  fmt::println(fg(fmt::color::yellow) | fmt::emphasis::bold, "==== Dysrcapi Examples ====");
  fmt::println(fg(fmt::color::yellow) | fmt::emphasis::bold, "===========================");
  
  DST::ScriptFolder<IScript> controller(std::filesystem::path(__FILE__).parent_path() / "scripts/", std::filesystem::path(__FILE__).parent_path() / "scripts" / "DLL" );

  controller.add("Test.cpp");

  DST::ScriptInstance<IScript> inst(controller.get("Test.cpp"));

  inst.get()->exec();

  return 0;
};
