/**
 * Copyright Soramitsu Co., Ltd. 2018 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_TEST_BLOCK_BUILDER_HPP
#define IROHA_TEST_BLOCK_BUILDER_HPP

#include "builders/protobuf/builder_templates/block_template.hpp"
#include "interfaces/iroha_internal/block.hpp"
#include "utils/polymorphic_wrapper.hpp"
#include "validators/answer.hpp"

using namespace shared_model::proto;
using namespace shared_model::validation;

class NoValidation {
 public:
  using wrappedBlock =
      shared_model::detail::PolymorphicWrapper<shared_model::interface::Block>;

  Answer validate(wrappedBlock) const {
    return Answer();
  }
};

/**
 * Builder alias, to build shared model proto block object avoiding validation
 * and "required fields" check
 */
using TestBlockBuilder =
    TemplateBlockBuilder<(1 << TemplateBlockBuilder<>::total) - 1,
                         NoValidation,
                         Block>;
#endif  // IROHA_TEST_BLOCK_BUILDER_HPP
