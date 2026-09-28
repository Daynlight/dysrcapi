// dysrcapi
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include "dysrcapi.h"



// =========================================
// ============== ScriptRecord =============
// =========================================
// =============================
// === Constructors ============
// =============================
// core
template<typename T>
inline MC::ScriptRecord<T>::ScriptRecord() {
};



template<typename T>
inline MC::ScriptRecord<T>::ScriptRecord(const std::string& name, const std::filesystem::path& cpp_folder, const std::filesystem::path& so_folder)
  : path_cpp(cpp_folder / name), 
#ifdef WIN32
    path_so(so_folder / (name + ".dll"))
#else
    path_so(so_folder / (name + ".so")) 
#endif
    {
};



template<typename T>
inline MC::ScriptRecord<T>::~ScriptRecord() {
  removeModule();
};



// move
template<typename T>
inline MC::ScriptRecord<T>::ScriptRecord(ScriptRecord &&second)
  : path_cpp(std::move(second.path_cpp)),
    path_so(std::move(second.path_so)),
    last_time_write(std::move(second.last_time_write)),
    script_handler(std::move(second.script_handler)),
    instances(std::move(second.instances)),
    next_available_index(std::move(second.next_available_index)) {    
  second.script_handler = nullptr;
  second.instances.clear();
  second.next_available_index = 0;
};



template<typename T>
inline MC::ScriptRecord<T> &MC::ScriptRecord<T>::operator=(ScriptRecord &&second) {
  if(this == &second) return *this;

  removeModule();

  path_cpp = std::move(second.path_cpp);
  path_so = std::move(second.path_so);
  last_time_write = std::move(second.last_time_write);
  script_handler = std::move(second.script_handler);
  instances = std::move(second.instances);
  next_available_index = std::move(second.next_available_index);
  second.script_handler = nullptr;
  second.instances.clear();
  second.next_available_index = 0;
  
  
  return *this;
};



// =============================
// === Getters/Setters =========
// =============================
template <typename T>
inline void MC::ScriptRecord<T>::setCppPath(const std::filesystem::path &path){
  if(path_cpp == path) return;
  path_cpp = path;
  updateModule();
};



template <typename T>
inline std::filesystem::path MC::ScriptRecord<T>::getCppPath() const {
  return path_cpp;
};



template <typename T>
inline void MC::ScriptRecord<T>::setSoPath(const std::filesystem::path &path){
  if(path_so == path) return;
  path_so = path;
  updateModule();
};



template <typename T>
inline bool MC::ScriptRecord<T>::getScriptHandlerIsValid() const {
  return script_handler != nullptr;
};



template <typename T>
inline std::filesystem::path MC::ScriptRecord<T>::getSoPath() const {
  return path_so;
};



template <typename T>
inline int MC::ScriptRecord<T>::getNextAvailableIndex() const {
  return next_available_index;
};



template <typename T>
inline std::filesystem::file_time_type MC::ScriptRecord<T>::getLastTimeWrite() const {
  return last_time_write;
};



template <typename T>
inline void MC::ScriptRecord<T>::setCompiler(std::string compiler){
  if(this->compiler == compiler) return;
  this->compiler = compiler;
  updateModule();
};



template <typename T>
inline std::string MC::ScriptRecord<T>::getCompiler() const {
  return compiler;
};



template <typename T>
inline void MC::ScriptRecord<T>::setCompileFlags(const std::vector<std::string> &flags){
  if(compile_flags == flags) return;
  compile_flags = flags;
  updateModule();
};



template <typename T>
inline std::vector<std::string> MC::ScriptRecord<T>::getCompileFlags() const {
  return compile_flags;
};



// =============================
// === Control =================
// =============================
template<typename T>
inline void MC::ScriptRecord<T>::observe() {
  if(checkLastWrite()) {
    updateModule();
  };
};



template<typename T>
inline void MC::ScriptRecord<T>::updateModule() {
  removeModule();

  if(compile() != 0) {
    return;
  };

  loadModule();
};



template<typename T>
inline T* MC::ScriptRecord<T>::get(int index) {
  if(!script_handler) loadModule();
  if(!script_handler){
    return nullptr;
  };

  const auto& it = instances.find(index);
  if(it == instances.end()){
    create(index);
  };

  const auto& it2 = instances.find(index);
  if(it2 == instances.end()){
    return nullptr;
  };

  return instances[index];
};



template <typename T>
inline int MC::ScriptRecord<T>::create(){
  if(!script_handler) loadModule();
  if(!script_handler){
    return -1;
  };
  
#if defined(_WIN32) || defined(_WIN64)
  typedef T* (*GetScriptFunc)();
  GetScriptFunc getScript = (GetScriptFunc)GetProcAddress((HMODULE)script_handler, "GetScript");
  
  if (!getScript) {
    return -1;
  };
#else
  typedef T* (*GetScriptFunc)();
  GetScriptFunc getScript = (GetScriptFunc)dlsym(script_handler, "GetScript");
  const char* dlsym_error = dlerror();
  
  if (dlsym_error || !getScript) {
    return -1;
  };
#endif
  
  instances[next_available_index] = getScript();
  int id = next_available_index;
  
  bool collide = true;
  while(collide){
    collide = false;
    const auto& id = instances.find(next_available_index);
    if(id != instances.end()) {
      next_available_index++;
      collide = true;
    };
  };

  return id;
};



template <typename T>
inline int MC::ScriptRecord<T>::create(int index){
  const auto& id = instances.find(index);
  if(id != instances.end()) {
    return -1;
  };

  if(!script_handler) loadModule();
  if(!script_handler){
    return -1;
  };
  
#if defined(_WIN32) || defined(_WIN64)
  typedef T* (*GetScriptFunc)();
  GetScriptFunc getScript = (GetScriptFunc)GetProcAddress((HMODULE)script_handler, "GetScript");
  
  if (!getScript) {
    return -1;
  };
#else
  typedef T* (*GetScriptFunc)();
  GetScriptFunc getScript = (GetScriptFunc)dlsym(script_handler, "GetScript");
  const char* dlsym_error = dlerror();
  
  if (dlsym_error || !getScript) {
    return -1;
  };
#endif
  
  instances[index] = getScript();
  
  bool collide = true;
  while(collide){
    collide = false;
    const auto& id = instances.find(next_available_index);
    if(id != instances.end()) {
      next_available_index++;
      collide = true;
    };
  }; 

  return index;
};



template <typename T>
inline void MC::ScriptRecord<T>::destroy(int index){
  const auto& id = instances.find(index);
  if(id == instances.end()){
    return;
  };

  if(!script_handler){
    return;
  };

#if defined(_WIN32) || defined(_WIN64)
  using DeleteScriptFunc = void (*)(T*);
  DeleteScriptFunc deleteScript = (DeleteScriptFunc)GetProcAddress((HMODULE)script_handler, "DeleteScript");

  if(!deleteScript){
    instances.erase(index);
    return;
  };
#else
  using DeleteScriptFunc = void (*)(T*);
  DeleteScriptFunc deleteScript = (DeleteScriptFunc)dlsym(script_handler, "DeleteScript");

  if(dlerror() || !deleteScript){
    instances.erase(index);
    return;
  };
#endif

  if(id->second) deleteScript(id->second);
  instances.erase(id);

  next_available_index = index;
  bool collide = true;
  while(collide){
    collide = false;
    const auto& id = instances.find(next_available_index);
    if(id != instances.end()) {
      next_available_index++;
      collide = true;
    };
  };
};



// =============================
// === Helpers =================
// =============================
template<typename T>
inline bool MC::ScriptRecord<T>::checkLastWrite() {
  bool file_exist = std::filesystem::exists(path_cpp);
  bool changed = 0;

  if(!file_exist) {
    return 0;
  };
  
  std::filesystem::file_time_type currentWriteTime{};

  try{
    currentWriteTime = std::filesystem::last_write_time(path_cpp);
  } catch(const std::filesystem::filesystem_error& e){
    return false;
  };

  if(currentWriteTime != last_time_write){
    changed = 1;
    last_time_write = currentWriteTime;
  };

  return changed;
};



template<typename T>
inline int MC::ScriptRecord<T>::loadModule() {
  removeModule();

  bool file_exist = std::filesystem::exists(path_so);
  if(!file_exist) {
    compile();
  };

  file_exist = std::filesystem::exists(path_so);
  if(!file_exist) {
    return -1;
  };

#if defined(_WIN32) || defined(_WIN64)
  script_handler = LoadLibraryA(path_so.string().c_str());
  if (!script_handler) {
    return -1;
  };

  typedef T* (*GetScriptFunc)();
  GetScriptFunc getScript = (GetScriptFunc)GetProcAddress((HMODULE)script_handler, "GetScript");
  
  if (!getScript) {
    return -1;
  };
#else
  script_handler = dlopen((path_so).c_str(), RTLD_NOW);
  if (!script_handler) {
    return -1;
  };
  dlerror();

  typedef T* (*GetScriptFunc)();
  GetScriptFunc getScript = (GetScriptFunc)dlsym(script_handler, "GetScript");
  const char* dlsym_error = dlerror();
  
  if (dlsym_error || !getScript) {
    return -1;
  };
#endif

  for(std::pair<const int, T*>& el : instances){
    el.second = getScript();
  };

  return 0;
};



template<typename T>
inline void MC::ScriptRecord<T>::removeModule() {
  if (!script_handler) {
    instances.clear();
    return;
  };

#if defined(_WIN32) || defined(_WIN64)
    using DeleteScriptFunc = void (*)(T*);
    DeleteScriptFunc deleteScript = (DeleteScriptFunc)GetProcAddress((HMODULE)script_handler, "DeleteScript");

    if (!deleteScript) {
      instances.clear();
    }
    else {
      for(auto& [index, script] : instances){
        if(script) deleteScript(script);
        script = nullptr;
      };
    };
#else
    dlerror();
    using DeleteScriptFunc = void (*)(T*);
    DeleteScriptFunc deleteScript = (DeleteScriptFunc)dlsym(script_handler, "DeleteScript");
    const char* dlsym_error = dlerror();

    if (dlsym_error || !deleteScript) {
      instances.clear();
    }
    else{
      for(auto& [index, script] : instances){
        if(script) deleteScript(script);
        script = nullptr;
      };
    };
#endif

#if defined(_WIN32) || defined(_WIN64)
  if(script_handler) {
    FreeLibrary((HMODULE)script_handler);
  }
#else
  if(script_handler) {
    dlclose(script_handler);
  }
#endif

  instances.clear();
  script_handler = nullptr;
};



template<typename T>
inline int MC::ScriptRecord<T>::compile() {
  if (compiler.empty()) {
    return 0;
  }

  std::filesystem::path dir = path_so.parent_path();
  if (!dir.empty() && !std::filesystem::exists(dir)) {
    std::filesystem::create_directories(dir);
  }

#if defined(_WIN32) || defined(_WIN64)
  std::filesystem::path temp_so = path_so;
  temp_so += ".tmp";
  
  std::string cmd = "\"" + compiler + "\" -shared";
  for (const auto& flag : compile_flags) cmd += " " + flag;
  cmd += " -o \"" + temp_so.generic_string() + "\" \"" + path_cpp.generic_string() + "\"";


  STARTUPINFOA si = { sizeof(si) };
  PROCESS_INFORMATION pi = { 0 };

  std::vector<char> cmd_buffer(cmd.begin(), cmd.end());
  cmd_buffer.push_back('\0');

  BOOL success = CreateProcessA(NULL, cmd_buffer.data(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
  
  if (success) {
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exit_code;
    GetExitCodeProcess(pi.hProcess, &exit_code);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    if (exit_code == 0) {
      std::error_code ec;
      std::filesystem::rename(temp_so, path_so, ec);
      if (!ec) {
        return 0;  
      } else {
        return -1;
      }
    } else {
    }
  } else {
  }
  
  return -1;

#else
  std::vector<std::string> args;
  args.push_back(compiler);
  for (const auto& flag : compile_flags) args.push_back(flag);
  args.push_back("-o");
  args.push_back(path_so.string());
  args.push_back(path_cpp.string());
  
  std::vector<char*> argv;
  argv.reserve(args.size() + 1);
  for (auto& arg : args) argv.push_back(arg.data());
  argv.push_back(nullptr);

  pid_t pid = fork();
  if (pid == 0) {
    execvp(argv[0], argv.data()); 
    exit(-1);
  } else if (pid > 0) {
    int status = 0; 
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
      return 0; 
    } else {
      return -1;
    } 
  } else {
  }

  return -1;
#endif
};





// =========================================
// ============== ScriptInstance ===========
// =========================================
// =============================
// === Constructors ============
// =============================
// core
template <typename T>
inline MC::ScriptInstance<T>::ScriptInstance(MC::ScriptRecord<T>* record)
  : record(record) {
  create();
};



template <typename T>
inline MC::ScriptInstance<T>::~ScriptInstance(){
  destroy();
};



// copy
template <typename T>
inline MC::ScriptInstance<T>::ScriptInstance(const ScriptInstance<T> &second) 
  : record(second.record) {
  create();
};



template <typename T>
inline MC::ScriptInstance<T> &MC::ScriptInstance<T>::operator=(const ScriptInstance<T> &second){
  if(this == &second) return *this;

  destroy();
  record = second.record;
  create();

  return *this;
};



// move
template <typename T>
inline MC::ScriptInstance<T>::ScriptInstance(ScriptInstance<T>&& second) 
  : record(std::move(second.record)),
    index(std::move(second.index)) {
  second.record = nullptr;
  second.index = -1;
};



template <typename T>
inline MC::ScriptInstance<T> &MC::ScriptInstance<T>::operator=(ScriptInstance<T>&& second){
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
inline T *MC::ScriptInstance<T>::get(){
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
inline void MC::ScriptInstance<T>::setRecord(MC::ScriptRecord<T> *record){
  if(this->record == record) return;

  destroy();
  this->record = record;
  create();
};



// =============================
// === Helpers =================
// =============================
template <typename T>
inline void MC::ScriptInstance<T>::create() {
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
inline void MC::ScriptInstance<T>::destroy(){
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






// =========================================
// ============ ScriptController ===========
// =========================================
// =============================
// === Constructors ============
// =============================
// core
template<typename T>
inline MC::ScriptController<T>::ScriptController(const std::filesystem::path& cpp_folder, const std::filesystem::path& so_folder) 
  : cpp_folder(cpp_folder),
    so_folder(so_folder) {};



template<typename T>
inline MC::ScriptController<T>::~ScriptController() {
  for(std::pair<const std::string, MC::ScriptRecord<T>*>& el : scripts)
    delete el.second;
  scripts.clear();
};



// =============================
// === Control =================
// =============================
template<typename T>
inline void MC::ScriptController<T>::add(const std::string& name) {
  bool exists = exist(name);
  if(exists) {
    scripts[name]->updateModule();
    return;
  };
  scripts[name] = new MC::ScriptRecord<T>(name, cpp_folder, so_folder);
};



template<typename T>
inline bool MC::ScriptController<T>::exist(const std::string& name) {
  const auto& it = scripts.find(name);
  if(it == scripts.end()) return false;
  return true;
};



template<typename T>
inline void MC::ScriptController<T>::erase(const std::string& name) {
  delete scripts[name];
  scripts.erase(name);
};



template<typename T>
inline void MC::ScriptController<T>::clear(){
  for(std::pair<const std::string, MC::ScriptRecord<T>*>& el : scripts)
    delete el.second;
  scripts.clear();
};



template <typename T>
inline void MC::ScriptController<T>::observe() {
  for(std::pair<const std::string, MC::ScriptRecord<T>*>& el : scripts) el.second->observe();
};



template<typename T>
inline MC::ScriptRecord<T>* MC::ScriptController<T>::get(const std::string& name) {
  bool exists = exist(name);
  if(!exists){
    add(name);
  };

  return scripts[name];
};
