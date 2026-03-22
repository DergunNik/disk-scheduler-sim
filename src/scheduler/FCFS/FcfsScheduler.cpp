#include "FcfsScheduler.h"

#include <algorithm>

std::optional<std::size_t> disksim::scheduling::FcfsScheduler::pickNext(
    const std::vector<Request> &requests,
    const DiskState &state,
    const DiskModel &model,
    int now
) const {
    if (requests.empty()) return std::nullopt;
    return std::distance(
        requests.begin(),
        std::ranges::min_element(requests, {}, &Request::arrivalTime)
    );
}
