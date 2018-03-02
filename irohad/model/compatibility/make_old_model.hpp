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

#ifndef IROHA_MAKE_OLD_MODEL_HPP
#define IROHA_MAKE_OLD_MODEL_HPP

namespace shared_model {
  namespace interface {
    class Transaction;
  }  // namespace interface
}  // namespace shared_model

namespace iroha {
  namespace model {

    struct Transaction;

    namespace compatibility {

      namespace om = iroha::model;
      namespace sm = shared_model::interface;

      om::Transaction *makeOldModel(const sm::Transaction &tx);

    }  // namespace compatibility
  }    // namespace model
}  // namespace iroha

#endif  // IROHA_MAKE_OLD_MODEL_HPP
