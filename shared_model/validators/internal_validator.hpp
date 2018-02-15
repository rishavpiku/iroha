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

#include <memory>
#include "interfaces/common_objects/types.hpp"
#include "validators/default_validator.hpp"

namespace shared_model {

  namespace crypto {
    class Blob;
    class Hash;
  }

  namespace interface {
    class Block;
    class Proposal;
  }

  namespace detail {
    template <typename T>
    class PolymorphicWrapper;
  }

  namespace validation {

    class Answer;
    class FieldValidator;

    /**
     * Class that validates block and proposal.
     */
    class InternalValidator {
     public:
      InternalValidator();

      /**
       * Validates fields in block
       * @return Answer containing errors, if any
       */
      Answer validate(const interface::Block &block) const;

      /**
       * Validates fields in block
       * @return Answer containing errors, if any
       */
      Answer validate(
          const detail::PolymorphicWrapper<interface::Block> &block) const;

      /**
       * Validates fields in proposal
       * @return Answer containing errors, if any
       */
      Answer validate(const interface::Proposal &proposal) const;

      /**
       * Validates fields in proposal
       * @return Answer containing errors, if any
       */
      Answer validate(const detail::PolymorphicWrapper<interface::Proposal>
                          &proposal) const;

      /**
       * Reason of validation failure.
       */
      struct Reason {
        static const char *kSignature;
        static const char *kTxsNumber;
        static const char *kPrevHash;
      };

      // protected because we test private methods, in fixture this class is
      // Base class
     protected:
      void validate_signatures(
          Answer &answer,
          const interface::types::SignatureSetType &signatures,
          const crypto::Blob &payload) const;

      void validate_txsNumber(Answer &answer, uint64_t txNumber) const;

      void validate_prevHash(Answer &answer, const crypto::Hash &hash) const;

      void validate_createdTime(
          Answer &answer, const interface::types::TimestampType &time) const;

      void validate_transactions(
          Answer &answer,
          const interface::types::TransactionsCollectionType &transactions)
          const;

      // we do not need to validate height, it can be arbitrary number.

     private:
      std::shared_ptr<DefaultTransactionValidator> tx_validator_;
      std::shared_ptr<FieldValidator> field_validator_;
    };

  }  // namespace validation
}  // namespace shared_model

#endif  // IROHA_BLOCK_VALIDATOR_HPP
