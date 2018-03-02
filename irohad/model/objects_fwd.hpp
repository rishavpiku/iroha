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

#ifndef IROHA_OLDMODEL_OBJECTS_FWD_HPP_
#define IROHA_OLDMODEL_OBJECTS_FWD_HPP_

#include "model/commands/commands_fwd.hpp"
#include "model/queries/queries_fwd.hpp"
#include "model/queries/responses/responses_fwd.hpp"

/**
 * @brief This file contains forward declarations for model.
 */

namespace iroha {
  namespace model {

    struct Account;
    struct AccountAsset;
    struct Asset;
    struct Block;
    struct Command;
    struct Commit;
    struct Domain;
    struct Peer;
    struct Proposal;
    struct Query;
    struct QueryResponse;
    struct Signature;
    struct Transaction;
    struct TransactionResponse;
  }
}

#endif  //  IROHA_OLDMODEL_OBJECTS_FWD_HPP_
