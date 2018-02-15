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
#include <gtest/gtest.h>
#include "cryptography/crypto_provider/crypto_defaults.hpp"
#include "cryptography/crypto_provider/crypto_signer.hpp"
#include "module/shared_model/builders/protobuf/test_block_builder.hpp"
#include "validators/answer.hpp"
#include "validators/transaction_validator.hpp"

using namespace shared_model;
using namespace shared_model::detail;
using shared_model::validation::InternalValidator;
using reason = shared_model::validation::InternalValidator::Reason;
using namespace crypto;

class InternalValidatorFixture : public InternalValidator,
                                 public ::testing::Test {
 public:
  InternalValidatorFixture()
      : keypair(DefaultCryptoAlgorithmType::generateKeypair(
            DefaultCryptoAlgorithmType::generateSeed("seed"))) {}

  const crypto::Keypair keypair;
};

/**
 * @given a block without signatures
 * @when passed to block_validator
 * @then reason is "no signatures"
 */
TEST_F(InternalValidatorFixture, NoSignatures) {
  Answer answer;
  auto block = TestBlockBuilder().height(1).build();
  this->validate_signatures(answer, block.signatures(), block.payload());
  ASSERT_TRUE(answer.hasErrors());
  ASSERT_TRUE(answer.hasReason(reason::kSignature));
}

/**
 * @given a block with one valid signature
 * @when passed to block_validator
 * @then no errors
 */
TEST_F(InternalValidatorFixture, ValidOneSignature) {
  Answer answer;
  auto block = TestBlockBuilder().height(1).build();
  UnsignedWrapper<Block> w(block);
  w.signAndAddSignature(keypair);
  this->validate_signatures(answer, w.get().signatures(), w.get().payload());
  ASSERT_FALSE(answer.hasErrors());
}

/**
 * @given a block with txNumber=0 (invalid)
 * @when passed to block_validator
 * @then error
 */
TEST_F(InternalValidatorFixture, InvalidTxNumber) {
  Answer answer;
  auto block = TestBlockBuilder().txNumber(0).build();
  this->validate_txsNumber(answer, block.txsNumber());
  ASSERT_TRUE(answer.hasErrors());
  ASSERT_TRUE(answer.hasReason(Reason::kTxsNumber));
}

/**
 * @given a block with valid tx number
 * @when passed to block_validator
 * @then no errors
 */
TEST_F(InternalValidatorFixture, ValidTxNumber) {
  Answer answer;
  auto block = TestBlockBuilder().txNumber(1).build();
  this->validate_txsNumber(answer, block.txsNumber());
  ASSERT_FALSE(answer.hasErrors());
}

/**
 * @given a block with invalid "previous hash"
 * @when passed to block_validator
 * @then error
 */
TEST_F(InternalValidatorFixture, InvalidPrevHash) {
  Answer answer;
  std::string s = "zdarov rabotyagi";
  crypto::Hash invalid_hash(s);
  auto block = TestBlockBuilder().prevHash(invalid_hash).build();
  this->validate_prevHash(answer, block.prevHash());
  ASSERT_TRUE(answer.hasErrors());
  ASSERT_TRUE(answer.hasReason(Reason::kPrevHash));
}

/**
 * @given a block with valid "previous hash"
 * @when passed to block_validator
 * @then no errors
 */
TEST_F(InternalValidatorFixture, ValidPrevHash) {
  Answer answer;

  // TODO(warchant): IR-977 replace hardcoded number with default hash size
  std::string s(32, 'a');
  crypto::Hash valid_hash(s);
  auto block = TestBlockBuilder().prevHash(valid_hash).build();
  this->validate_prevHash(answer, block.prevHash());
  ASSERT_FALSE(answer.hasErrors());
}
