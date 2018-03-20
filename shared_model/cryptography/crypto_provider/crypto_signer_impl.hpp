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

#ifndef IROHA_CRYPTO_SIGNER_IMPL_HPP
#define IROHA_CRYPTO_SIGNER_IMPL_HPP

#include "backend/protobuf/common_objects/signature.hpp"
#include "cryptography/crypto_provider/crypto_defaults.hpp"
#include "cryptography/crypto_provider/crypto_signer.hpp"
#include "interfaces/iroha_internal/block.hpp"
#include "interfaces/queries/query.hpp"
#include "interfaces/transaction.hpp"

namespace shared_model {
  namespace crypto {
    template <typename Algorithm = DefaultCryptoAlgorithmType>
    class CryptoSignerImpl : public CryptoSigner {
     public:
      explicit CryptoSignerImpl(const shared_model::crypto::Keypair &keypair);

      virtual ~CryptoSignerImpl() = default;

      void sign(shared_model::interface::Block &block) const override;

      void sign(shared_model::interface::Query &query) const override;

      void sign(
          shared_model::interface::Transaction &transaction) const override;

     private:
#ifndef DISABLE_BACKWARD
      template <typename Model, typename OldModel>
      void internal_sign(interface::Signable<Model, OldModel> &signable) const
          noexcept {
#else
      template <typename Model>
      void CryptoModelSigner<Algorithm>::sign(
          interface::Signable<Model> &signable) const noexcept {
#endif
        auto signedBlob =
            shared_model::crypto::DefaultCryptoAlgorithmType::sign(
                shared_model::crypto::Blob(signable.payload()), keypair_);
        iroha::protocol::Signature protosig;
        protosig.set_pubkey(
            shared_model::crypto::toBinaryString(keypair_.publicKey()));
        protosig.set_signature(
            shared_model::crypto::toBinaryString(signedBlob));
        auto *signature = new shared_model::proto::Signature(protosig);
        signable.addSignature(shared_model::detail::PolymorphicWrapper<
                              shared_model::proto::Signature>(signature));
      }

      shared_model::crypto::Keypair keypair_;
    };

    template <typename Algorithm>
    CryptoSignerImpl<Algorithm>::CryptoSignerImpl(
        const shared_model::crypto::Keypair &keypair)
        : keypair_(keypair) {}

    template <typename Algorithm>
    void CryptoSignerImpl<Algorithm>::sign(
        shared_model::interface::Block &block) const {
      internal_sign(block);
    }

    template <typename Algorithm>
    void CryptoSignerImpl<Algorithm>::sign(
        shared_model::interface::Query &query) const {
      internal_sign(query);
    }

    template <typename Algorithm>
    void CryptoSignerImpl<Algorithm>::sign(
        shared_model::interface::Transaction &transaction) const {
      internal_sign(transaction);
    }

  }  // namespace crypto
}  // namespace shared_model

#endif  // IROHA_CRYPTO_SIGNER_IMPL_HPP