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

#include "llvm/Support/raw_ostream.h"
#include "souper/Codegen/Codegen.h"
#include "gtest/gtest.h"

using namespace souper;

TEST(ParserTest, Errors) {
  struct {
    std::vector<BackendCost> Costs;
    BackendCost Best;
  } Tests[];

  for (const auto &T : Tests) {
    std::string ErrStr;
    ParseReplacement(IC, "<input>", T.Test, ErrStr);
    EXPECT_EQ(T.WantError, ErrStr);
  }
}
