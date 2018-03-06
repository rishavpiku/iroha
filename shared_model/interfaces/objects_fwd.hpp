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

#ifndef IROHA_INTERFACES_OBJECTS_FWD_HPP_
#define IROHA_INTERFACES_OBJECTS_FWD_HPP_

#include "interfaces/commands/commands_fwd.hpp"
#include "interfaces/queries/queries_fwd.hpp"
#include "interfaces/query_responses/query_responses_fwd.hpp"

namespace shared_model {
  namespace interface {

    class Transaction;
    class Block;
    class Proposal;
  }  // namespace interface
}  // namespace shared_model

#endif  //  IROHA_INTERFACES_OBJECTS_FWD_HPP_
