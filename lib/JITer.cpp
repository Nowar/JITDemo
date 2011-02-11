// Copyright (C) 2011 Nowar
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// 
#include <iostream>
#include <string>
#include <vector>

#include "llvm/LLVMContext.h"
#include "llvm/Function.h"
#include "llvm/Module.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JITMemoryManager.h"
#include "llvm/Support/IRReader.h"
#include "llvm/Target/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

#include "Usage.hpp"

// TODO(Nowar): Refactor this file. DO NOT use it anymore.

llvm::Module* getModuleFromFile(const char* name, llvm::LLVMContext& ctx) {
  llvm::SMDiagnostic srcDiag;
  return llvm::getLazyIRFileModule(name, srcDiag, ctx);
}

llvm::ExecutionEngine* createExecEngine(llvm::Module* module, std::string* errStr) {
  llvm::InitializeNativeTarget();
  llvm::JITMemoryManager* jitMemMgr = 
      llvm::JITMemoryManager::CreateDefaultMemManager();
  llvm::ExecutionEngine* ee = llvm::ExecutionEngine::createJIT(module,
                                                              errStr,
                                                              jitMemMgr,
                                                              llvm::CodeGenOpt::Aggressive);
  return ee;
}

bool execMainFunc(llvm::ExecutionEngine* ee) {
  llvm::Function* mainFunc = ee->FindFunctionNamed("main");
  if (!mainFunc) {
    return false;
  }
  void* nativeMainFunc = ee->getPointerToFunction(mainFunc);
  typedef int (*pMainType)();
  reinterpret_cast<pMainType>(nativeMainFunc)();
  return true;
}
/*
int main(int argc, char** argv) {
  if (argc != 2) {
    Usage::showUsage(llvm::outs(), argv[0]);
    return 1;
  }

  // Get IR module from file.
  llvm::LLVMContext& ctx = llvm::getGlobalContext();
  llvm::Module* module = getModuleFromFile(argv[1], ctx);
  if (!module) {
    std::cerr << "Input file is not an LLVM IR.\n";
    return 1;
  }

  // Create JIT-Compiler from module
  std::string errStr;
  llvm::ExecutionEngine* ee = createExecEngine(module, &errStr);
  if (!ee) {
    std::cerr << errStr;
    delete module;
    return 1;
  }

  // Run 'main' function
  std::cerr << "[Debug] Jitting and executing main function of the IR module...\n";
  if (!execMainFunc(ee)) {
    std::cerr << "[Debug] Error! Cannot find main function.\n";
  }
  delete ee;
}
*/
