#include "ScanScheduler.h"
#include <algorithm>
#include <ranges>
#include <cmath>

std::optional<std::size_t> disksim::scheduling::ScanScheduler::pickNext(
    const std::vector<Request>& requests,
    const DiskState& state,
    const DiskModel& model,
    int now
) const {
    if (requests.empty()) return std::nullopt;

    const int current = state.currentCylinder;
    const auto dir = state.direction;

    auto isInDirection = [&](int target) {
        return (dir == DiskHeadDirection::Inward) ? (target >= current) : (target <= current);
    };

    auto forwardIt = std::ranges::min_element(requests,
        [&](const Request& a, const Request& b) {
            bool a_valid = isInDirection(a.targetCylinder);
            bool b_valid = isInDirection(b.targetCylinder);
            if (a_valid != b_valid) return a_valid;
            return std::abs(a.targetCylinder - current) < std::abs(b.targetCylinder - current);
        }
    );

    if (isInDirection(forwardIt->targetCylinder)) {
        return std::distance(requests.begin(), forwardIt);
    }

    auto backwardIt
        = std::ranges::min_element(requests, {}, [&](const Request& r) {
        return std::abs(r.targetCylinder - current);
    });

    return std::distance(requests.begin(), backwardIt);
}
