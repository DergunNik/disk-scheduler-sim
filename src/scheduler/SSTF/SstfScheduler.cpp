#include "SstfScheduler.h"

#include <algorithm>
#include <limits>
#include <cmath>

std::optional<std::size_t> disksim::scheduling::SstfScheduler::pickNext(
    const std::vector<Request>& requests,
    const DiskState& state,
    const DiskModel& model,
    int now
) const {

    if (requests.empty()) {
        return std::nullopt;
    }

    return std::distance(
        requests.begin(),
        std::ranges::min_element(
            requests,
            [&model, &state](const Request& a, const Request& b) {
                return model.serviceTime(state, a) < model.serviceTime(state, b);
            }
        )
    );
}
