/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/account/confirmations/redeem_unblinded_payment_tokens/redeem_unblinded_payment_tokens_util.h"

#include "base/time/time.h"
#include "brave/components/brave_ads/common/pref_names.h"
#include "brave/components/brave_ads/core/internal/ads_client_helper.h"
#include "brave/components/brave_ads/core/internal/flags/debug/debug_flag_util.h"
#include "brave/components/brave_ads/core/internal/privacy/random/random_util.h"

namespace brave_ads {

namespace {

constexpr base::TimeDelta kNextTokenRedemptionAfter = base::Days(1);
constexpr base::TimeDelta kDebugNextTokenRedemptionAfter = base::Minutes(2);
constexpr base::TimeDelta kMinimumDelayBeforeRedeemingTokens = base::Minutes(1);

}  // namespace

void SetNextTokenRedemptionAt(const base::Time next_token_redemption_at) {
  AdsClientHelper::GetInstance()->SetTimePref(prefs::kNextTokenRedemptionAt,
                                              next_token_redemption_at);
}

base::Time NextTokenRedemptionAt() {
  return AdsClientHelper::GetInstance()->GetTimePref(
      prefs::kNextTokenRedemptionAt);
}

bool HasPreviouslyRedeemedTokens() {
  return !NextTokenRedemptionAt().is_null();
}

base::TimeDelta DelayBeforeRedeemingTokens() {
  return NextTokenRedemptionAt() - base::Time::Now();
}

bool ShouldHaveRedeemedTokensInThePast() {
  return NextTokenRedemptionAt() < base::Time::Now();
}

base::Time CalculateNextTokenRedemptionAt() {
  return base::Time::Now() +
         (ShouldDebug() ? kDebugNextTokenRedemptionAfter
                        : privacy::RandTimeDelta(kNextTokenRedemptionAfter));
}

base::TimeDelta CalculateDelayBeforeRedeemingTokens() {
  if (!HasPreviouslyRedeemedTokens()) {
    return CalculateNextTokenRedemptionAt() - base::Time::Now();
  }

  if (ShouldHaveRedeemedTokensInThePast()) {
    return kMinimumDelayBeforeRedeemingTokens;
  }

  const base::TimeDelta delay = DelayBeforeRedeemingTokens();
  if (delay < kMinimumDelayBeforeRedeemingTokens) {
    return kMinimumDelayBeforeRedeemingTokens;
  }

  return delay;
}

}  // namespace brave_ads
