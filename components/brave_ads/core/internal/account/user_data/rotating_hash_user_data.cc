/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/account/user_data/rotating_hash_user_data.h"

#include <string>

#include "brave/components/brave_ads/core/internal/account/transactions/transaction_info.h"
#include "brave/components/brave_ads/core/internal/account/user_data/rotating_hash_user_data_util.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace brave_ads {

namespace {
constexpr char kRotatingHashKey[] = "rotating_hash";
}  // namespace

base::Value::Dict BuildRotatingHashUserData(
    const TransactionInfo& transaction) {
  base::Value::Dict user_data;

  const absl::optional<std::string> rotating_hash =
      BuildRotatingHash(transaction);
  if (rotating_hash) {
    user_data.Set(kRotatingHashKey, *rotating_hash);
  }

  return user_data;
}

}  // namespace brave_ads
