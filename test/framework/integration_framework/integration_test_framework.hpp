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

#ifndef IROHA_INTEGRATION_FRAMEWORK_HPP
#define IROHA_INTEGRATION_FRAMEWORK_HPP

#include <algorithm>
#include <chrono>
#include <exception>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include <tbb/concurrent_queue.h>
#include "backend/protobuf/block.hpp"
#include "backend/protobuf/proposal.hpp"
#include "backend/protobuf/queries/proto_query.hpp"
#include "backend/protobuf/query_responses/proto_query_response.hpp"
#include "backend/protobuf/transaction.hpp"
#include "backend/protobuf/transaction_responses/proto_tx_response.hpp"
#include "crypto/keys_manager_impl.hpp"
#include "cryptography/blob.hpp"
#include "cryptography/ed25519_sha3_impl/internal/sha3_hash.hpp"
#include "cryptography/keypair.hpp"
#include "framework/integration_framework/iroha_instance.hpp"
#include "logger/logger.hpp"

namespace integration_framework {

  using std::chrono::milliseconds;

  class IntegrationTestFramework {
   private:
    using ProposalType = std::shared_ptr<shared_model::interface::Proposal>;
    using BlockType = std::shared_ptr<shared_model::interface::Block>;

   public:
    /**
     * Construct test framework instance
     * @param maximum_proposal_size - (default = 10) Maximum amount of
     * transactions per proposal
     * @param destructor_lambda - (default nullptr) Pointer to function which
     * receives pointer to constructed instance of Integration Test Framework.
     * If specified, then will be called instead of default destructor's code
     */
    explicit IntegrationTestFramework(
        size_t maximum_proposal_size = 10,
        void (*destructor_lambda)(IntegrationTestFramework *) = nullptr)
        : maximum_proposal_size_(maximum_proposal_size),
          destructor_lambda_(destructor_lambda) {}

    /**
     * Initialize Iroha instance with default genesis block and provided signing
     * key
     * @param keypair - signing key
     * @return this
     */
    IntegrationTestFramework &setInitialState(
        const shared_model::crypto::Keypair &keypair);

    /**
     * Initialize Iroha instance with provided genesis block and signing key
     * @param keypair - signing key
     * @param block - genesis block used for iroha initialization
     * @return this
     */
    IntegrationTestFramework &setInitialState(
        const shared_model::crypto::Keypair &keypair,
        const shared_model::interface::Block &block);

    /**
     * Send transaction to Iroha and validate its status
     * @tparam Lambda - type of status validation callback function
     * @param tx - transaction
     * @param validation - callback for transaction status validation that
     * receives object of type \relates shared_model::proto::TransactionResponse
     * @return this
     */
    template <typename Lambda>
    IntegrationTestFramework &sendTx(const shared_model::proto::Transaction &tx,
                                     Lambda validation);

    /**
     * Send transaction to Iroha without status validation
     * @param tx - transaction
     * @return this
     */
    IntegrationTestFramework &sendTx(
        const shared_model::proto::Transaction &tx);

    /**
     * Check current status of transaction
     * @param hash - hash of transaction to check
     * @return TransactonResponse object
     */
    shared_model::proto::TransactionResponse getTxStatus(
        const shared_model::crypto::Hash &hash);

    /**
     * Send query to Iroha and validate the response
     * @tparam Lambda - type of function that handles query result
     * @param qry - query
     * @param validation - callback for query result check that receives object
     * of type \relates shared_model::proto::QueryResponse
     * @return this
     */
    template <typename Lambda>
    IntegrationTestFramework &sendQuery(const shared_model::proto::Query &qry,
                                        Lambda validation);

    /**
     * Send query to Iroha without response validation
     * @param qry - query
     * @return this
     */
    IntegrationTestFramework &sendQuery(const shared_model::proto::Query &qry);

    /**
     * Request next proposal from queue and serve it with custom handler
     * @tparam Lambda - type of function that operates over proposal
     * @param validation - callback that receives object of type \relates
     * ProposalType
     * @return this
     */
    template <typename Lambda>
    IntegrationTestFramework &checkProposal(Lambda validation);

    /**
     * Request next proposal from queue and skip it
     * @return this
     */
    IntegrationTestFramework &skipProposal();

    /**
     * Request next block from queue and serve it with custom handler
     * @tparam Lambda - type of function that operates over block
     * @param validation - callback that receives object of type \relates
     * BlockType
     * @return this
     */
    template <typename Lambda>
    IntegrationTestFramework &checkBlock(Lambda validation);

    /**
     * Request next block from queue and skip it
     * @return this
     */
    IntegrationTestFramework &skipBlock();

    /**
     * Shutdown ITF instance
     */
    void done();

    virtual ~IntegrationTestFramework() {
      if (destructor_lambda_) {
        destructor_lambda_(this);
      } else {
        done();
      }
    }

   protected:
    /**
     * general way to fetch object from concurrent queue
     * @tparam Queue - Type of queue
     * @tparam ObjectType - Type of fetched object
     * @tparam WaitTime - time for waiting if data doesn't appear
     * @param queue - queue instance for fetching
     * @param ref_for_insertion - reference to insert object
     * @param wait - time of waiting
     * @param error_reason - reason if there is no appeared object at all
     */
    template <typename Queue, typename ObjectType, typename WaitTime>
    void fetchFromQueue(Queue &queue,
                        ObjectType &ref_for_insertion,
                        const WaitTime &wait,
                        const std::string &error_reason);

    tbb::concurrent_queue<ProposalType> proposal_queue_;
    tbb::concurrent_queue<BlockType> block_queue_;
    std::shared_ptr<IrohaInstance> iroha_instance_ =
        std::make_shared<IrohaInstance>();

    // config area

    /// maximum time of waiting before appearing next proposal
    // TODO 21/12/2017 muratovv make relation of time with instance's config
    const milliseconds proposal_waiting = milliseconds(20000);

    /// maximum time of waiting before appearing next committed block
    const milliseconds block_waiting = milliseconds(20000);

    const std::string default_domain = "test";
    const std::string default_role = "user";

    size_t maximum_proposal_size_;

   private:
    logger::Logger log_ = logger::log("IntegrationTestFramework");
    std::mutex queue_mu;
    std::condition_variable queue_cond;
    void (*destructor_lambda_)(IntegrationTestFramework *);
  };

  template <typename Lambda>
  IntegrationTestFramework &IntegrationTestFramework::sendTx(
      const shared_model::proto::Transaction &tx, Lambda validation) {
    log_->info("send transaction");
    iroha_instance_->getIrohaInstance()->getCommandService()->Torii(
        tx.getTransport());
    // fetch status of transaction
    shared_model::proto::TransactionResponse status = getTxStatus(tx.hash());

    // check validation function
    validation(status);
    return *this;
  }

  template <typename Lambda>
  IntegrationTestFramework &IntegrationTestFramework::sendQuery(
      const shared_model::proto::Query &qry, Lambda validation) {
    log_->info("send query");

    iroha::protocol::QueryResponse response;
    iroha_instance_->getIrohaInstance()->getQueryService()->Find(
        qry.getTransport(), response);
    auto query_response =
        shared_model::proto::QueryResponse(std::move(response));

    validation(query_response);
    return *this;
  }

  template <typename Lambda>
  IntegrationTestFramework &IntegrationTestFramework::checkBlock(
      Lambda validation) {
    // fetch first from block queue
    log_->info("check block");
    BlockType block;
    fetchFromQueue(block_queue_, block, block_waiting, "missed block");
    validation(block);
    return *this;
  }

  template <typename Lambda>
  IntegrationTestFramework &IntegrationTestFramework::checkProposal(
      Lambda validation) {
    log_->info("check proposal");
    // fetch first proposal from proposal queue
    ProposalType proposal;
    fetchFromQueue(
        proposal_queue_, proposal, proposal_waiting, "missed proposal");
    validation(proposal);
    return *this;
  }

  template <typename Queue, typename ObjectType, typename WaitTime>
  void IntegrationTestFramework::fetchFromQueue(
      Queue &queue,
      ObjectType &ref_for_insertion,
      const WaitTime &wait,
      const std::string &error_reason) {
    std::unique_lock<std::mutex> lk(queue_mu);
    queue_cond.wait_for(lk, wait, [&]() { return not queue.empty(); });
    if (!queue.try_pop(ref_for_insertion)) {
      throw std::runtime_error(error_reason);
    }
  }
}  // namespace integration_framework

#endif  // IROHA_INTEGRATION_FRAMEWORK_HPP
