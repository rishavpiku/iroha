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

#ifndef IROHA_ANSWER_HPP
#define IROHA_ANSWER_HPP

#include <boost/range/numeric.hpp>
#include <map>
#include "utils/string_builder.hpp"

namespace shared_model {
  namespace validation {

    using ConcreteReasonType = std::string;
    using GroupedReasons = std::vector<ConcreteReasonType>;
    using ReasonsGroupName = std::string;
    using ReasonsGroupType = std::pair<ReasonsGroupName, GroupedReasons>;

    /**
     * Class which represents the answer to stateless validation: whether
     * validation is done right and if not it explains the reason
     */
    class Answer {
     public:
      operator bool() const {
        return not reasons_map_.empty();
      }

      /**
       * @return string representation of errors
       */
      std::string reason() const {
        return boost::accumulate(
            reasons_map_,
            std::string{},
            [](auto &&acc, const auto &command_reasons) {
              acc += detail::PrettyStringBuilder()
                         .init(command_reasons.first)
                         .appendAll(command_reasons.second,
                                    [](auto &element) { return element; })
                         .finalize()
                  + "\n";
              return std::forward<decltype(acc)>(acc);
            });
      }

      /**
       * Check if any error has been recorded to the answer
       * @return true if there are any errors, false otherwise
       */
      bool hasErrors() {
        return not reasons_map_.empty();
      }

      /**
       * Adds error to map
       * @param reasons
       */
      void addReason(ReasonsGroupType &&reasons) {
        reasons_map_.insert(std::move(reasons));
      }

      void addReason(const ReasonsGroupName &type,
                     const GroupedReasons &group) {
        // try to find the type
        auto r = reasons_map_.find(type);
        if (r != reasons_map_.end()) {
          // found group
          for (const auto &item : group) {
            // append reasons to group's array one-by-one
            reasons_map_[type].push_back(item);
          }
        } else {
          // group is not found, then create new group
          addReason({type, group});
        }
      }

      bool hasReason(const ReasonsGroupName &type) {
        auto it = reasons_map_.find(type);
        // true if has reason type
        return it != reasons_map_.end();
      }

      void clear() {
        reasons_map_.clear();
      }

      /**
       * Merge two Answers - this and other
       */
      void merge(const Answer &other) {
        for (const auto &e : other.reasons_map_) {
          auto &key = e.first;
          auto &val = e.second;

          auto it = reasons_map_.find(key);
          if (it != reasons_map_.end()) {
            // key found
            for (const auto &v : val) {
              reasons_map_[key].push_back(v);
            }
          } else {
            reasons_map_[key] = val;
          }
        }
      }

      std::map<ReasonsGroupName, GroupedReasons> getReasonsMap() {
        return reasons_map_;
      };

     private:
      std::map<ReasonsGroupName, GroupedReasons> reasons_map_;
    };

  }  // namespace validation
}  // namespace shared_model

#endif  // IROHA_ANSWER_HPP
