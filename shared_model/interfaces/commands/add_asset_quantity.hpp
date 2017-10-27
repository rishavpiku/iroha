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

#ifndef IROHA_SHARED_MODEL_ADD_ASSET_QUANTITY_HPP
#define IROHA_SHARED_MODEL_ADD_ASSET_QUANTITY_HPP

#include <string>
#include "amount/amount.hpp"  // TODO 26/10/2017 muratovv replace with amount from shared lib
#include "interfaces/common_objects/types.hpp"
#include "interfaces/primitive.hpp"
#include "model/commands/add_asset_quantity.hpp"

namespace shared_model {
  namespace interface {

    /**
     * Add amount of asset to an account
     */
    class AddAssetQuantity
        : public Primitive<AddAssetQuantity, iroha::model::AddAssetQuantity> {
     public:
      /**
       * @return Identity of user, that add quantity
       */
      virtual const types::AccountIdType &accountId() const = 0;

      /// Type of asset id
      using AssetIdType = std::string;
      /**
       * @return asset identifier
       */
      virtual const AssetIdType &assetId() const = 0;

      /// Type of amount
      using AmountType = iroha::Amount;
      /**
       * @return quantity of asset for adding
       */
      virtual const AmountType &amount() const = 0;
    };
  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_ADD_ASSET_QUANTITY_HPP
