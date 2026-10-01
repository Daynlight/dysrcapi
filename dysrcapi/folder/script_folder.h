// dysrcapi
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <memory>

#include "../file/script_file.h"


namespace DST {
template<typename T>
class ScriptFolder{
// ======================================
// ============== Data ==================
// ======================================
private:
  std::unordered_map<std::string, DST::ScriptFile<T>*> scripts;
  std::filesystem::path cpp_folder;
  std::filesystem::path so_folder;



// ======================================
// ============== Functions =============
// ======================================
// =============================
// === Constructors ============
// =============================
public:
// core
  ScriptFolder(const std::filesystem::path& cpp_folder, const std::filesystem::path& so_folder);
  ~ScriptFolder();
// copy
  ScriptFolder(const ScriptFolder& second) = delete;
  ScriptFolder& operator=(const ScriptFolder& second) = delete;
// move
  ScriptFolder(ScriptFolder&& second) = delete;
  ScriptFolder& operator=(ScriptFolder&& second) = delete;
  
// =============================
// === Control =================
// =============================
  void add(const std::string& name);
  bool exist(const std::string& name);
  void erase(const std::string& name);
  void clear();
  void observe();
  DST::ScriptFile<T>* get(const std::string& name);
};
};



#include "script_folder.hpp"
