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
#include "llvm/LLVMContext.h"
#include "llvm/Function.h"
#include "llvm/Module.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JITMemoryManager.h"
#include "llvm/Support/IRReader.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetSelect.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    llvm::raw_ostream& os = llvm::outs();
    os << "Usage: " << argv[0] << " LLVM-IR-file\n";
    os << argv[0] << " is a trivial JIT-Compiler using LLVM.\n";
    os << "It reads LLVM module from the input argument, creates a JIT-Compiler, and runs on 'main' function.\n";
    os << "Note: We assume 'main' needs no arguments.\n";
    return 1;
  }

  llvm::LLVMContext& ctx = llvm::getGlobalContext();
  
  // frontend
  // FIXME(Nowar): We should use clang frontend
  llvm::SMDiagnostic srcDiag;
  llvm::Module* module = llvm::getLazyIRFileModule(argv[1], srcDiag, ctx);

  // backend
  std::string errStr;
  llvm::InitializeNativeTarget();
  llvm::ExecutionEngine* ee = 
      llvm::ExecutionEngine::createJIT(module,
                                       &errStr,
                                       llvm::JITMemoryManager::CreateDefaultMemManager(),
                                       llvm::CodeGenOpt::Aggressive);
  if (!ee) {
    llvm::errs() << errStr << "\n";
    delete module;
    return 1;
  }

  llvm::Function* mainFunc = ee->FindFunctionNamed("main");
  if (!mainFunc) {
    llvm::errs() << "Error: Cannot find main function.\n";
    delete ee;
    return 1;
  }

  void* nativeMainFunc = ee->getPointerToFunction(mainFunc);
  typedef int (*pMainType)();
  reinterpret_cast<pMainType>(nativeMainFunc)();
  delete ee;

  return 0;
}
