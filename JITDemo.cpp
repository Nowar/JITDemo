/// @file JitDemo.cpp
/// @brief A demo using LLVM JIT
/// @author Nowar (nowar100<at>gmail.com)
/// @version 0.1
/// @date 2011-01-04
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

void execMainFunc(llvm::ExecutionEngine* ee) {
  std::cout << "[Debug] Jitting and executing main function of the IR module...\n";
  llvm::Function* mainFunc = ee->FindFunctionNamed("main");
  void* nativeMainFunc = ee->getPointerToFunction(mainFunc);
  typedef int (*pMainType)();
  reinterpret_cast<pMainType>(nativeMainFunc)();
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " LLVM-IR-file\n";
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
  execMainFunc(ee);
  delete ee;
}
