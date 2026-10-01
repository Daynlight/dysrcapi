// dysrcapi
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include "script_folder.h"



// =========================================
// ============ ScriptFolder ===========
// =========================================
// =============================
// === Constructors ============
// =============================
// core
template<typename T>
inline DST::ScriptFolder<T>::ScriptFolder(const std::filesystem::path& cpp_folder, const std::filesystem::path& so_folder) 
  : cpp_folder(cpp_folder),
    so_folder(so_folder) {};



template<typename T>
inline DST::ScriptFolder<T>::~ScriptFolder() {
  for(std::pair<const std::string, DST::ScriptFile<T>*>& el : scripts)
    delete el.second;
  scripts.clear();
};



// =============================
// === Control =================
// =============================
template<typename T>
inline void DST::ScriptFolder<T>::add(const std::string& name) {
  bool exists = exist(name);
  if(exists) {
    scripts[name]->updateModule();
    return;
  };
  scripts[name] = new DST::ScriptFile<T>(name, cpp_folder, so_folder);
};



template<typename T>
inline bool DST::ScriptFolder<T>::exist(const std::string& name) {
  const auto& it = scripts.find(name);
  if(it == scripts.end()) return false;
  return true;
};



template<typename T>
inline void DST::ScriptFolder<T>::erase(const std::string& name) {
  delete scripts[name];
  scripts.erase(name);
};



template<typename T>
inline void DST::ScriptFolder<T>::clear(){
  for(std::pair<const std::string, DST::ScriptFile<T>*>& el : scripts)
    delete el.second;
  scripts.clear();
};



template <typename T>
inline void DST::ScriptFolder<T>::observe() {
  for(std::pair<const std::string, DST::ScriptFile<T>*>& el : scripts) el.second->observe();
};



template<typename T>
inline DST::ScriptFile<T>* DST::ScriptFolder<T>::get(const std::string& name) {
  bool exists = exist(name);
  if(!exists){
    add(name);
  };

  return scripts[name];
};
