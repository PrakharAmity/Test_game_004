#include "RewardManager.hpp"

namespace pixelverse {
namespace reward {

RewardManager::RewardManager() {
    // Initial starter daily and event rewards
    enqueueReward(Reward(1, "Daily Bronze Supply", RewardType::DailyLogin, 250, 10, 100, "", false));
    enqueueReward(Reward(2, "Weekly Tournament Bounty", RewardType::WeeklyTrophy, 1000, 50, 400, "Neon Dagger", false));
}

void RewardManager::enqueueReward(const Reward& reward) {
#ifdef FIX_BUGS
    // FIX for Bug 5: Priority rewards go to the front so they are claimed first
    if (reward.isPriority) {
        rewardQueue.push_front(reward);
    } else {
        rewardQueue.push_back(reward);
    }
#else
    // BUG 5 (Intentional): Reward Queue Bug (DSA)
    // Concept: Queue / Deque
    // Symptom: Priority rewards appear after normal rewards (inverted insertion logic)!
    if (reward.isPriority) {
        rewardQueue.push_back(reward);
    } else {
        rewardQueue.push_front(reward);
    }
#endif
}

bool RewardManager::claimNextReward(Reward& outReward) {
    if (rewardQueue.empty()) return false;
    outReward = rewardQueue.front();
    rewardQueue.pop_front();
    claimedRewardsCount++;
    return true;
}

std::vector<Reward> RewardManager::getQueueAsVector() const {
    return std::vector<Reward>(rewardQueue.begin(), rewardQueue.end());
}

} // namespace reward
} // namespace pixelverse
