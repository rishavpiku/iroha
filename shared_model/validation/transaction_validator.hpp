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

#ifndef IROHA_TRANSACTIONVALIDATOR_HPP
#define IROHA_TRANSACTIONVALIDATOR_HPP

#include "interfaces/transaction.hpp"
#include <utility>

namespace shared_model {
  namespace validation {
    template <typename CreatorAccountIdValidator,
              typename TransactionCounterValidator,
              typename CommandValidator>
    class ComposableTransactionValidator {
     public:
      template <typename CreatorValidator,
                typename TXCounterValidator,
                typename CmValidator>
      ComposableTransactionValidator(CreatorValidator &&cv,
                           TXCounterValidator &&tcv,
                           CmValidator &&comv)
          : validate_creator{std::forward<CreatorValidator>(cv)},
            validate_tx_counter{std::forward<TXCounterValidator>(tcv)},
            validate_command{std::forward<CmValidator>(comv)} {

            };

      bool operator()(const shared_model::interface::Transaction &t) {
        bool a = validate_creator(t.creatorAccountId());
        bool b = validate_tx_counter(t.transactionCounter());
        for (const auto &command : t.commands()) {
          b &= validate_command(command);
        }
        return a && b;
      }

     private:
      CreatorAccountIdValidator validate_creator;
      TransactionCounterValidator validate_tx_counter;
      CommandValidator validate_command;
    };

    template <typename CreatorValidator,
              typename TXCounterValidator,
              typename CommandValidator>
    auto makeTransactionValidator(CreatorValidator creatorValidator,
                                  TXCounterValidator tx_counter_validator,
                                  CommandValidator command_validator) {
      return ComposableTransactionValidator<CreatorValidator,
                                  TXCounterValidator,
                                  CommandValidator>(
          creatorValidator, tx_counter_validator, command_validator);
    };
  }  // namespace validation
}  // namespace shared_model

#endif  // IROHA_TRANSACTIONVALIDATOR_HPP
