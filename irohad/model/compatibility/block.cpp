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

#include "interfaces/iroha_internal/block.hpp"
#include "model/block.hpp"

namespace iroha {
  namespace model {
    namespace compatibility {

      om::Block *makeOldModel(const sm::interface::Block &b) {
        iroha::model::Block *old_block = new iroha::model::Block();
        old_block->height = b.height();
        constexpr auto hash_size = iroha::model::Block::HashType::size();
        old_block->prev_hash =
            *iroha::hexstringToArray<hash_size>(b.prevHash().hex());
        old_block->txs_number = b.txsNumber();
        std::for_each(b.transactions().begin(),
                      b.transactions().end(),
                      [&old_block](auto &tx) {
                        std::unique_ptr<iroha::model::Transaction> old_tx(
                            tx->makeOldModel());
                        old_block->transactions.emplace_back(*old_tx);
                      });
        old_block->created_ts = b.createdTime();
        old_block->hash = *iroha::hexstringToArray<hash_size>(b.hash().hex());
        std::for_each(b.signatures().begin(),
                      b.signatures().end(),
                      [&old_block](auto &sig) {
                        std::unique_ptr<iroha::model::Signature> old_sig(
                            sig->makeOldModel());
                        old_block->sigs.emplace_back(*old_sig);
                      });
        return old_block;
      }
    }  // namespace compatibility
  }    // namespace model
}  // namespace iroha
