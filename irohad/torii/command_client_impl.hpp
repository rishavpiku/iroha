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

#ifndef TORII_COMMAND_CLIENT_IMPL_HPP
#define TORII_COMMAND_CLIENT_IMPL_HPP

#include <endpoint.grpc.pb.h>
#include <grpc++/grpc++.h>
#include <memory>
#include <thread>
#include "torii/command_client.hpp"

namespace torii {

  /**
   * CommandSyncClient
   */
  class CommandSyncClient : public CommandClient {
   public:
    CommandSyncClient(const std::string &ip, size_t port);

    CommandSyncClient(const CommandSyncClient &);
    CommandSyncClient &operator=(CommandSyncClient);

    CommandSyncClient(CommandSyncClient &&) noexcept;
    CommandSyncClient &operator=(CommandSyncClient &&) noexcept;

    ~CommandSyncClient() override = default;

    /**
     * requests tx to a torii server and returns response (blocking, sync)
     * @param tx
     * @return grpc::Status - returns connection is success or not.
     */
    grpc::Status Torii(const iroha::protocol::Transaction &tx) const override;

    /**
     * @param tx
     * @param response returns ToriiResponse if succeeded
     * @return grpc::Status - returns connection is success or not.
     */
    grpc::Status Status(const iroha::protocol::TxStatusRequest &tx,
                        iroha::protocol::ToriiResponse &response) const override;

    /**
     * Acquires stream of transaction statuses from the request
     * moment until final.
     * @param tx - transaction to send.
     * @param response - vector of all statuses during tx pipeline.
     */
    void StatusStream(
        const iroha::protocol::TxStatusRequest &tx,
        std::vector<iroha::protocol::ToriiResponse> &response) const override;

   private:
    void swap(CommandSyncClient &lhs, CommandSyncClient &rhs);
    std::string ip_;
    size_t port_;
    std::unique_ptr<iroha::protocol::CommandService::Stub> stub_;
  };

}  // namespace torii

#endif  // TORII_COMMAND_CLIENT_IMPL_HPP
