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

#ifndef COMPILER_PATH
  #define COMPILER_PATH "g++"
#endif

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <dlfcn.h>
#include <sys/wait.h>
#endif



#if defined(_WIN32) || defined(_WIN64)
inline const std::vector<std::string> DEFAULT_COMPILE_FLAGS = {
  "-std=c++20",
  "-Wl,--export-all-symbols",
  "-Wl,--enable-auto-import",
  "-DWIN32"
};
#else
inline const std::vector<std::string> DEFAULT_COMPILE_FLAGS = {
  "-std=c++20",
  "-rdynamic",
  "-shared",
  "-fPIC"
};
#endif



namespace MC{
template<typename T>
class ScriptRecord{
// ======================================
// ============== Data ==================
// ======================================
private:
  std::filesystem::path path_cpp{};
  std::filesystem::path path_so{};
  std::filesystem::file_time_type last_time_write{};
  void* script_handler = nullptr;
  std::unordered_map<int, T*> instances;
  int next_available_index = 0;
  std::string compiler = std::string(COMPILER_PATH);
  std::vector<std::string> compile_flags = DEFAULT_COMPILE_FLAGS;



// ======================================
// ============== Functions =============
// ======================================
// =============================
// === Constructors ============
// =============================
public:
// core
  ScriptRecord();
  ScriptRecord(const std::string& name, const std::filesystem::path& cpp_folder, const std::filesystem::path& so_folder);
  ~ScriptRecord();
// copy
  ScriptRecord(const ScriptRecord& second) = delete;
  ScriptRecord& operator=(const ScriptRecord& second) = delete;
// move
  ScriptRecord(ScriptRecord&& second);
  ScriptRecord& operator=(ScriptRecord&& second);

// =============================
// === Getters/Setters =========
// =============================
public:
  void setCppPath(const std::filesystem::path& path);
  std::filesystem::path getCppPath() const;
  void setSoPath(const std::filesystem::path& path);
  std::filesystem::path getSoPath() const;
  bool getScriptHandlerIsValid() const;
  int getNextAvailableIndex() const;
  std::filesystem::file_time_type getLastTimeWrite() const;
  void setCompiler(std::string compiler);
  std::string getCompiler() const;
  void setCompileFlags(const std::vector<std::string>& flags);
  std::vector<std::string> getCompileFlags() const;

// =============================
// === Control =================
// =============================
public:
  void observe();
  void updateModule();
  T* get(int index);
  int create();
  int create(int index);
  void destroy(int index);

// =============================
// === Helpers =================
// =============================
private:
  bool checkLastWrite();
  int loadModule();
  void removeModule();
  int compile();
};



template<typename T>
class ScriptInstance{
// ======================================
// ============== Data ==================
// ======================================
private:
  int index = -1;
  MC::ScriptRecord<T>* record;


// ======================================
// ============== Functions =============
// ======================================
// =============================
// === Constructors ============
// =============================
public:
// core
  ScriptInstance(MC::ScriptRecord<T>* record);
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
  void setRecord(MC::ScriptRecord<T>* record);

// =============================
// === Helpers =================
// =============================
private:
  void create();
  void destroy();
};



template<typename T>
class ScriptController{
// ======================================
// ============== Data ==================
// ======================================
private:
  std::unordered_map<std::string, MC::ScriptRecord<T>*> scripts;
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
  ScriptController(const std::filesystem::path& cpp_folder, const std::filesystem::path& so_folder);
  ~ScriptController();
// copy
  ScriptController(const ScriptController& second) = delete;
  ScriptController& operator=(const ScriptController& second) = delete;
// move
  ScriptController(ScriptController&& second) = delete;
  ScriptController& operator=(ScriptController&& second) = delete;
  
// =============================
// === Control =================
// =============================
  void add(const std::string& name);
  bool exist(const std::string& name);
  void erase(const std::string& name);
  void clear();
  void observe();
  MC::ScriptRecord<T>* get(const std::string& name);
};
};



#include "dysrcapi.hpp"
