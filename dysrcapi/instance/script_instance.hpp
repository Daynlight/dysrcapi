// dysrcapi
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include "script_instance.h"



// =========================================
// ============== ScriptInstance ===========
// =========================================
// =============================
// === Constructors ============
// =============================
// core
template <typename T>
inline DST::ScriptInstance<T>::ScriptInstance(DST::ScriptFile<T>* record)
  : record(record) {
  create();
};



template <typename T>
inline DST::ScriptInstance<T>::~ScriptInstance(){
  destroy();
};



// copy
template <typename T>
inline DST::ScriptInstance<T>::ScriptInstance(const ScriptInstance<T> &second) 
  : record(second.record) {
  create();
};



template <typename T>
inline DST::ScriptInstance<T> &DST::ScriptInstance<T>::operator=(const ScriptInstance<T> &second){
  if(this == &second) return *this;

  destroy();
  record = second.record;
  create();

  return *this;
};



// move
template <typename T>
inline DST::ScriptInstance<T>::ScriptInstance(ScriptInstance<T>&& second) 
  : record(std::move(second.record)),
    index(std::move(second.index)) {
  second.record = nullptr;
  second.index = -1;
};



template <typename T>
inline DST::ScriptInstance<T> &DST::ScriptInstance<T>::operator=(ScriptInstance<T>&& second){
  if(this == &second) return *this;

  destroy();
  record = second.record;
  index = second.index;
  second.index = -1;
  second.record = nullptr;

  return *this;
};



// =============================
// === Getters/Setters =========
// =============================
template <typename T>
inline T *DST::ScriptInstance<T>::get(){
  if(!record){
    return nullptr;  
  };

  if(index < 0) create();
  if(index < 0){
    return nullptr;
  };
  
  return record->get(index);
};



template <typename T>
inline void DST::ScriptInstance<T>::setScriptFile(DST::ScriptFile<T> *record){
  if(this->record == record) return;

  destroy();
  this->record = record;
  create();
};



// =============================
// === Helpers =================
// =============================
template <typename T>
inline void DST::ScriptInstance<T>::create() {
  if(index >= 0){
    return;
  };
  
  if(!record){
    index = -1;
    return;
  };

  index = record->create();
};



template <typename T>
inline void DST::ScriptInstance<T>::destroy(){
  if(index < 0){
    return;
  };
  
  if(!record){
    index = -1;
    return;
  };

  record->destroy(index);
  index = -1;
};
