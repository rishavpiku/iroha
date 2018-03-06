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

#ifndef IROHA_BLOCK_VALIDATOR_HPP
#define IROHA_BLOCK_VALIDATOR_HPP

#include <boost/format.hpp>
#include "datetime/time.hpp"
#include "interfaces/common_objects/types.hpp"
#include "interfaces/iroha_internal/block.hpp"
#include "utils/polymorphic_wrapper.hpp"
#include "validators/answer.hpp"

// TODO 22/01/2018 x3medima17: write stateless validator IR-837

namespace shared_model {
  namespace validation {

    /**
     * Class that validates block
     */
    template <typename FieldValidator, typename TransactionValidator>
    class BlockValidator {
     private:
      void validateHeight(ReasonsGroupType &reason,
                          const interface::types::HeightType &height) const {
        if (height <= 0) {
          auto message =
              (boost::format("Height should be > 0, passed value: %d")
               % height)
                  .str();
          reason.second.push_back(message);
        }
      }
      void validateTransaction(
          ReasonsGroupType &reason,
          const interface::Transaction &transaction) const {
        auto answer = transaction_validator_.validate(transaction);
        if (answer.hasErrors()) {
          auto message = (boost::format("%d %s")
                          % transaction.transactionCounter() % answer.reason())
                             .str();
          reason.second.push_back(message);
        }
      }

     public:
      BlockValidator(const TransactionValidator &transaction_validator =
                         TransactionValidator(),
                     const FieldValidator &field_validator = FieldValidator())
          : transaction_validator_(transaction_validator),
            field_validator_(field_validator) {}

      /**
       * Applies validation on block
       * @param block
       * @return Answer containing found error if any
       */
      Answer validate(const interface::Block &block) const {
        Answer answer;
        ReasonsGroupType reason;
        reason.first = "Block";
        field_validator_.validateCreatedTime(reason, block.createdTime());
        validateHeight(reason, block.height());
        for (const auto &tx : block.transactions()) {
          validateTransaction(reason, *tx);
        }
        if (not reason.second.empty()) {
          answer.addReason(std::move(reason));
        }

        return answer;
      }
      TransactionValidator transaction_validator_;
      FieldValidator field_validator_;
    };

  }  // namespace validation
}  // namespace shared_model

#endif  // IROHA_BLOCK_VALIDATOR_HPP
