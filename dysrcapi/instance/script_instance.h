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
class ScriptInstance{
// ======================================
// ============== Data ==================
// ======================================
private:
  int index = -1;
  DST::ScriptFile<T>* record;


// ======================================
// ============== Functions =============
// ======================================
// =============================
// === Constructors ============
// =============================
public:
// core
  ScriptInstance(DST::ScriptFile<T>* record);
  ~ScriptInstance();
// copy
  ScriptInstance(const ScriptInstance& second);
  ScriptInstance& operator=(const ScriptInstance& second);
// move
  ScriptInstance(ScriptInstance&& second);
  ScriptInstance& operator=(ScriptInstance&& second);

// =============================
// === Getters/Setters =========
// =============================
public:
  T* get();
  void setScriptFile(DST::ScriptFile<T>* record);

// =============================
// === Helpers =================
// =============================
private:
  void create();
  void destroy();
};
};


#include "script_instance.hpp"
