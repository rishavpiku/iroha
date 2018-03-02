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

#ifndef IROHA_OLDMODEL_COMMANDS_FWD_HPP_
#define IROHA_OLDMODEL_COMMANDS_FWD_HPP_

/**
 * @brief This file contains forward declarations for commands.
 */

namespace iroha {
  namespace model {

    struct AddAssetQuantity;
    struct AddPeer;
    struct AddSignatory;
    struct AppendRole;
    struct CreateAccount;
    struct CreateAsset;
    struct CreateDomain;
    struct CreateRole;
    struct DetachRole;
    struct GrantPermission;
    struct RemoveSignatory;
    struct RevokePermission;
    struct SetAccountDetail;
    struct SetQuorum;
    struct SubtractAssetQuantity;
    struct TransferAsset;
  }
}

#endif  //  IROHA_OLDMODEL_COMMANDS_FWD_HPP_
