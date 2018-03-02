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

#include "model/compatibility/make_old_model.hpp"

#include "interfaces/transaction.hpp"
#include "model/transaction.hpp"

namespace iroha {
  namespace model {

    namespace om = iroha::model;
    namespace sm = shared_model::interface;

    om::Transaction *makeOldModel(const sm::Transaction &tx) {
      auto *oldStyleTransaction = new om::Transaction();
      oldStyleTransaction->created_ts = tx.createdTime();
      oldStyleTransaction->creator_account_id = tx.creatorAccountId();
      oldStyleTransaction->tx_counter = tx.transactionCounter();

      std::for_each(
          tx.commands().begin(),
          tx.commands().end(),
          [oldStyleTransaction](auto &command) {
            oldStyleTransaction->commands.emplace_back(
                std::shared_ptr<om::Command>(command->makeOldModel()));
          });

      std::for_each(
          tx.signatures().begin(),
          tx.signatures().end(),
          [oldStyleTransaction](auto &sig) {
            oldStyleTransaction->signatures.emplace_back(*sig->makeOldModel());
          });

      return oldStyleTransaction;
    }

  }  // namespace model
}  // namespace iroha
