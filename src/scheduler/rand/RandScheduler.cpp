#include "RandScheduler.h"

#include <ctime>
#include <random>

std::optional<std::size_t> disksim::scheduling::RandScheduler::pickNext(
    const std::vector<Request> &requests,
    const DiskState &state,
    const DiskModel &model,
    int now
) const {
    if (requests.empty()) return std::nullopt;
    std::uniform_int_distribution<std::size_t> dis(0, requests.size() - 1);
    return dis(_gen);
}
