#ifndef PIXELVERSE_REWARDMANAGER_HPP
#define PIXELVERSE_REWARDMANAGER_HPP

#include "Reward.hpp"
#include <deque>
#include <vector>

namespace pixelverse {
namespace reward {

class RewardManager {
private:
    std::deque<Reward> rewardQueue;
    int claimedRewardsCount = 0;

public:
    RewardManager();

    // BUG 5 (Intentional): Reward Queue Bug (DSA)
    // Concept: Queue / Deque
    // Symptom: Priority rewards appear after normal rewards.
    void enqueueReward(const Reward& reward);

    bool claimNextReward(Reward& outReward);
    size_t getQueueSize() const { return rewardQueue.size(); }
    bool hasPendingRewards() const { return !rewardQueue.empty(); }
    void clearQueue() { rewardQueue.clear(); }
    int getClaimedCount() const { return claimedRewardsCount; }

    std::vector<Reward> getQueueAsVector() const;
};

} // namespace reward
} // namespace pixelverse

#endif // PIXELVERSE_REWARDMANAGER_HPP
