#include "gtest/gtest.h"
#include "../src/reward/RewardManager.hpp"

using namespace pixelverse::reward;

TEST(RewardQueueTest, RewardQueue_PriorityOrder) {
    RewardManager rm;
    rm.clearQueue();

    Reward normalRew(101, "Normal Supply", RewardType::DailyLogin, 100, 5, 20, "", false);
    Reward vipRew(999, "VIP Mythic Chest", RewardType::VIPSpecial, 5000, 200, 1000, "Mythic Blade", true);

    rm.enqueueReward(normalRew);
    rm.enqueueReward(vipRew);

    // Priority VIP reward MUST be claimed first
    // In Bug 5, VIP reward was placed at back, so normalRew is claimed first!
    Reward claimed;
    ASSERT_TRUE(rm.claimNextReward(claimed));
    EXPECT_EQ(claimed.id, 999);
    EXPECT_TRUE(claimed.isPriority);
}

TEST(RewardQueueTest, RewardQueue_MultiplePriorityRewards) {
    RewardManager rm;
    rm.clearQueue();

    Reward n1(1, "Normal 1", RewardType::DailyLogin, 100, 5, 20, "", false);
    Reward vip1(2, "VIP 1", RewardType::VIPSpecial, 1000, 50, 200, "", true);
    Reward vip2(3, "VIP 2", RewardType::VIPSpecial, 2000, 100, 400, "", true);

    rm.enqueueReward(n1);
    rm.enqueueReward(vip1);
    rm.enqueueReward(vip2);

    // First two claimed rewards must be priority rewards
    Reward c1, c2;
    ASSERT_TRUE(rm.claimNextReward(c1));
    ASSERT_TRUE(rm.claimNextReward(c2));

    EXPECT_TRUE(c1.isPriority);
    EXPECT_TRUE(c2.isPriority);
}

TEST(RewardQueueTest, RewardQueue_EmptyQueueHandling) {
    RewardManager rm;
    rm.clearQueue();

    Reward out;
    EXPECT_FALSE(rm.claimNextReward(out));
    EXPECT_EQ(rm.getQueueSize(), 0u);
}

TEST(RewardQueueTest, RewardQueue_StandardRewardFIFO) {
    RewardManager rm;
    rm.clearQueue();

    Reward n1(10, "Order 1", RewardType::DailyLogin, 50, 0, 10, "", false);
    Reward n2(20, "Order 2", RewardType::DailyLogin, 50, 0, 10, "", false);

    rm.enqueueReward(n1);
    rm.enqueueReward(n2);

    Reward firstClaimed;
    ASSERT_TRUE(rm.claimNextReward(firstClaimed));
    // Check that standard rewards maintain relative order
    EXPECT_EQ(firstClaimed.id, 10);
}

TEST(RewardQueueTest, RewardQueue_ClaimAllRewards) {
    RewardManager rm;
    rm.clearQueue();

    rm.enqueueReward(Reward(1, "Rew1", RewardType::DailyLogin, 100, 0, 10));
    rm.enqueueReward(Reward(2, "Rew2", RewardType::DailyLogin, 100, 0, 10));
    rm.enqueueReward(Reward(3, "Rew3", RewardType::DailyLogin, 100, 0, 10));

    EXPECT_EQ(rm.getQueueSize(), 3u);

    Reward r;
    int count = 0;
    while (rm.claimNextReward(r)) {
        count++;
    }
    EXPECT_EQ(count, 3);
    EXPECT_EQ(rm.getQueueSize(), 0u);
}
