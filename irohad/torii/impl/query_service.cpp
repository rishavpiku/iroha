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

#include "torii/query_service.hpp"
#include "common/types.hpp"
#include "model/sha3_hash.hpp"
#include "backend/protobuf/transaction_responses/proto_tx_response.hpp"

namespace torii {

  QueryService::QueryService(
      std::shared_ptr<iroha::model::converters::PbQueryFactory>
          pb_query_factory,
      std::shared_ptr<iroha::model::converters::PbQueryResponseFactory>
          pb_query_response_factory,
      std::shared_ptr<iroha::torii::QueryProcessor> query_processor)
      : pb_query_factory_(pb_query_factory),
        pb_query_response_factory_(pb_query_response_factory),
        query_processor_(query_processor) {
    // Subscribe on result from iroha
    query_processor_->queryNotifier().subscribe(
        [this](const auto &iroha_response) {
          // Find client to respond
          auto hash = shared_model::crypto::Hash(
              iroha_response->query_hash.to_string());
          auto res = cache_.findItem(hash);
          // Serialize to proto an return to response
          if (res) {
            shared_model::proto::TransactionResponse model_response(iroha_response);
//            res = pb_query_response_factory_->serialize(iroha_response).value();
            std::unique_ptr<iro>model_response.makeOldModel()
            cache_.addItem(hash, res.value());
          }
        });
  }

  void QueryService::Find(iroha::protocol::Query const &request,
                          iroha::protocol::QueryResponse &response) {
    using iroha::operator|;

    shared_model::proto::TransportBuilder<
        shared_model::proto::Query,
        shared_model::validation::DefaultQueryValidator>()
        .build(request)
        .match(
            [this,
             &response](const iroha::expected::Value<shared_model::proto::Query>
                            &query) {
              auto hash = query.value.hash();
              if (cache_.findItem(hash)) {
                // Query was already processed
                response.mutable_error_response()->set_reason(
                    iroha::protocol::ErrorResponse::STATELESS_INVALID);
              } else {
                // Query - response relationship
                cache_.addItem(hash, response);
                // Send query to iroha
                query_processor_->queryHandle(
                    std::shared_ptr<iroha::model::Query>(
                        query.value.makeOldModel()));
              }
              response.set_query_hash(
                  shared_model::crypto::toBinaryString(hash));
            },
            [&response](const iroha::expected::Error<std::string> &error) {
              response.mutable_error_response()->set_reason(
                  iroha::protocol::ErrorResponse::STATELESS_INVALID);
            });
  }

  grpc::Status QueryService::Find(grpc::ServerContext *context,
                                  const iroha::protocol::Query *request,
                                  iroha::protocol::QueryResponse *response) {
    Find(*request, *response);
    return grpc::Status::OK;
  }

}  // namespace torii
