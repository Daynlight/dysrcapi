// dysrcapi
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#define BUILDING_SCRIPT_DLL
#include "IScript.h"
#include <cstdio>



class TemplateScript : public IScript{
// ======================================
// ============== Data ==================
// ======================================
private:



// ======================================
// ============== Functions =============
// ======================================
// =============================
// === Constructors ============
// =============================
public:
  TemplateScript() {};
  ~TemplateScript() {};
  TemplateScript(const TemplateScript& second) = delete;
  TemplateScript& operator=(const TemplateScript& second) = delete;
  TemplateScript(TemplateScript&& second) = delete;
  TemplateScript& operator=(TemplateScript&& second) = delete;

// =============================
// === Functions ===============
// =============================
public:
  void exec() override {
    printf("Hello Script\n");  
  };

// =============================
// === Helpers =================
// =============================
private:

};



REGISTER_SCRIPT(TemplateScript, IScript);
