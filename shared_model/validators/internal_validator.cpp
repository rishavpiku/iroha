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

#include "validators/internal_validator.hpp"

#include "cryptography/blob.hpp"
#include "cryptography/crypto_provider/crypto_verifier.hpp"
#include "cryptography/hash.hpp"
#include "interfaces/iroha_internal/block.hpp"
#include "interfaces/iroha_internal/proposal.hpp"
#include "utils/polymorphic_wrapper.hpp"
#include "validators/answer.hpp"
#include "validators/field_validator.hpp"
#include "validators/transaction_validator.hpp"

using shared_model::detail::PolymorphicWrapper;
using shared_model::interface::Block;
using shared_model::interface::types::SignatureType;
using shared_model::crypto::CryptoVerifier;

using namespace shared_model::interface;

namespace shared_model {
  namespace validation {

    const char *InternalValidator::Reason::kSignature = "signature";
    const char *InternalValidator::Reason::kTxsNumber = "txsNumber";
    const char *InternalValidator::Reason::kPrevHash = "prevHash";

    Answer InternalValidator::validate(const interface::Block &block) const {
      Answer answer;

      // strategy:
      // check all fields, if field is incorrect, we do not interrupt
      // validation, but attach all reasons to "answer"
      validate_signatures(answer, block.signatures(), block.payload());
      validate_createdTime(answer, block.createdTime());
      validate_txsNumber(answer, block.txsNumber());
      validate_prevHash(answer, block.prevHash());
      validate_transactions(answer, block.transactions());

      return answer;
    }

    Answer InternalValidator::validate(
        const interface::Proposal &proposal) const {
      Answer answer;

      validate_transactions(answer, proposal.transactions());
      validate_createdTime(answer, proposal.created_time());

      return answer;
    }

    void InternalValidator::validate_signatures(
        Answer &answer,
        const types::SignatureSetType &signatures,
        const crypto::Blob &payload) const {
      if (signatures.empty()) {
        answer.addReason(Reason::kSignature, {"unsigned block"});
        return;
      }

      for (const auto &sig : signatures) {
        bool is_valid = CryptoVerifier<>::verify(
            sig->signedData(), payload, sig->publicKey());

        if (not is_valid) {
          answer.addReason(
              Reason::kSignature,
              {"signature of " + sig->publicKey().hex() + " is invalid"});
        }
      }
    }

    void InternalValidator::validate_txsNumber(Answer &answer,
                                               uint64_t txNumber) const {
      if (txNumber == 0) {
        answer.addReason(Reason::kTxsNumber,
                         {"number of transactions in block can not be 0"});
      }
    }

    void InternalValidator::validate_prevHash(Answer &answer,
                                              const crypto::Hash &hash) const {
      auto size = hash.size();
      // TODO(warchant): IR-977, replace magic number with default hash size
      if (size != 32) {
        answer.addReason(Reason::kPrevHash,
                         {"hash of previous block has incorrect length ("
                          + std::to_string(size)
                          + ")"});
      }
    }

    void InternalValidator::validate_createdTime(
        Answer &answer, const types::TimestampType &time) const {
      // TODO(warchant): IR-979 refactor with "key-value" style "Answer"
      std::string tx_reason_name = "Timestamp";
      ReasonsGroupType tx_reason(tx_reason_name, GroupedReasons());

      field_validator_->validateCreatedTime(tx_reason, time);

      if (not tx_reason.second.empty()) {
        answer.addReason(std::move(tx_reason));
      }
    }

    void InternalValidator::validate_transactions(
        Answer &answer,
        const types::TransactionsCollectionType &transactions) const {
      for (const auto &tx : transactions) {
        Answer a = tx_validator_->validate(tx);
        answer.merge(a);
      }
    }

    InternalValidator::InternalValidator()
        : tx_validator_(std::make_shared<DefaultTransactionValidator>()),
          field_validator_(std::make_shared<FieldValidator>()) {}

    Answer InternalValidator::validate(
        const detail::PolymorphicWrapper<interface::Proposal> &proposal) const {
      return validate(*proposal.get());
    }

    Answer InternalValidator::validate(
        const PolymorphicWrapper<Block> &block) const {
      return validate(*block.get());
    }
  }
}
