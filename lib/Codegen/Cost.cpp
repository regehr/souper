// Copyright 2014 The Souper Authors. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "souper/Codegen/Codegen.h"
#include "souper/Inst/Inst.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Value.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include <map>

#define DEBUG_TYPE "souper"

using namespace llvm;

namespace souper {

void optimizeModule(llvm::Module &M) {
  llvm::LoopAnalysisManager LAM;
  llvm::FunctionAnalysisManager FAM;
  llvm::CGSCCAnalysisManager CGAM;
  llvm::ModuleAnalysisManager MAM;

  llvm::PassBuilder PB;
  PB.registerModuleAnalyses(MAM);
  PB.registerCGSCCAnalyses(CGAM);
  PB.registerFunctionAnalyses(FAM);
  PB.registerLoopAnalyses(LAM);
  PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

  llvm::FunctionPassManager FPM =
    PB.buildFunctionSimplificationPipeline(llvm::PassBuilder::OptimizationLevel::O2,
                                           llvm::PassBuilder::ThinLTOPhase::None);
  llvm::ModulePassManager MPM;
  MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
  MPM.run(M, MAM);
}

struct TargetInfo {
  std::string Trip, CPU;
};

std::vector<TargetInfo> Targets {
  { "x86_64", "skylake" },
  { "aarch64", "apple-a12" },
};

// FIXME
using namespace llvm;
  
void getBackendCost(InstContext &IC, souper::Inst *I, BackendCost &BC) {
  llvm::LLVMContext C;
  llvm::Module M("souper.ll", C);
  if (genModule(IC, I, M))
    llvm::report_fatal_error("codegen error in getBackendCost()");
  optimizeModule(M);

  for (auto &T : Targets) {
    std::string Error;
    auto Target = TargetRegistry::lookupTarget(T.Trip, Error);
    if (!Target) {
      errs() << Error;
      report_fatal_error("can't lookup target");
    }

    auto Features = "";
    TargetOptions Opt;
    auto RM = Optional<Reloc::Model>();
    auto TM = Target->createTargetMachine(T.Trip, T.CPU, Features, Opt, RM);

    
  }
}
 
} // namespace souper
