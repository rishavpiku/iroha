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

#ifndef IROHA_SHARED_MODEL_DEFAULT_VALIDATOR_HPP
#define IROHA_SHARED_MODEL_DEFAULT_VALIDATOR_HPP

namespace shared_model {
  namespace validation {

    class FieldValidator;

    template <typename T>
    class CommandValidatorVisitor;

    template <typename A, typename B>
    class TransactionValidator;

    template <typename T>
    class QueryValidatorVisitor;

    template <typename A, typename B>
    class QueryValidator;

    class InternalValidator;

    using DefaultTransactionValidator =
        TransactionValidator<FieldValidator,
                             CommandValidatorVisitor<FieldValidator>>;
    using DefaultQueryValidator =
        QueryValidator<FieldValidator, QueryValidatorVisitor<FieldValidator>>;

    using DefaultProposalValidator = InternalValidator;

    using DefaultBlockValidator = InternalValidator;

  }  // namespace validation
}  // namespace shared_model

#endif  // IROHA_SHARED_MODEL_DEFAULT_VALIDATOR_HPP
