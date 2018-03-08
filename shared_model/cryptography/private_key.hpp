/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
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

#ifndef IROHA_SHARED_MODEL_PRIVATE_KEY_HPP
#define IROHA_SHARED_MODEL_PRIVATE_KEY_HPP

#include "cryptography/blob.hpp"
#include "utils/string_builder.hpp"

#include "common/types.hpp"

namespace shared_model {
  namespace crypto {
    /**
     * A special class for storing private keys.
     */
    class PrivateKey : public Blob {
     public:
      explicit PrivateKey(const std::string &private_key) : Blob(private_key) {}

      explicit PrivateKey(const Blob &blob) : Blob(blob.blob()) {}

#ifndef DISABLE_BACKWARD
      using OldPrivateKeyType = iroha::privkey_t;
#endif

      std::string toString() const override {
        return detail::PrettyStringBuilder()
            .init("PrivateKey")
            .append("<Data is hidden>")
            .finalize();
      }
    };
  }  // namespace crypto
}  // namespace shared_model

#endif  // IROHA_SHARED_MODEL_PRIVATE_KEY_HPP
