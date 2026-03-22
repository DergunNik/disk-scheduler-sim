#include "CScanScheduler.h"
#include <algorithm>
#include <ranges>

std::optional<std::size_t> disksim::scheduling::CScanScheduler::pickNext(
    const std::vector<Request>& requests,
    const DiskState& state,
    const DiskModel& model,
    int now
) const {
    if (requests.empty()) return std::nullopt;

    const int current = state.currentCylinder;

    auto forwardIt = std::ranges::min_element(requests,
        [current](int a, int b) {
            bool a_ahead = a >= current;
            bool b_ahead = b >= current;
            if (a_ahead != b_ahead) return a_ahead;
            return a < b;
        },
        &Request::targetCylinder
    );

    if (forwardIt->targetCylinder >= current) {
        return std::distance(requests.begin(), forwardIt);
    }

    auto restartIt
        = std::ranges::min_element(requests, {}, &Request::targetCylinder);

    return std::distance(requests.begin(), restartIt);
}
