// dysrcapi
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>

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



namespace DST {
template<typename T>
class ScriptFile{
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

  #if defined(_WIN32) || defined(_WIN64)
  std::vector<std::string> compile_flags = {
    "-std=c++20",
    "-Wl,--export-all-symbols",
    "-Wl,--enable-auto-import",
    "-DWIN32"
  };
  #else
  std::vector<std::string> compile_flags = {
    "-std=c++20",
    "-rdynamic",
    "-shared",
    "-fPIC"
  };
  #endif

// ======================================
// ============== Functions =============
// ======================================
// =============================
// === Constructors ============
// =============================
public:
// core
  ScriptFile();
  ScriptFile(const std::string& name, const std::filesystem::path& cpp_folder, const std::filesystem::path& so_folder);
  ~ScriptFile();
// copy
  ScriptFile(const ScriptFile& second) = delete;
  ScriptFile& operator=(const ScriptFile& second) = delete;
// move
  ScriptFile(ScriptFile&& second);
  ScriptFile& operator=(ScriptFile&& second);

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
};



#include "script_file.hpp"
