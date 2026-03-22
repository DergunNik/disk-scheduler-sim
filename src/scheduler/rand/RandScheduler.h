#ifndef DISK_SCHEDULER_SIM_RANDSCHEDULER_H
#define DISK_SCHEDULER_SIM_RANDSCHEDULER_H
#include <random>

#include "../IScheduler.h"

namespace disksim::scheduling {

    class RandScheduler : IScheduler {
        mutable std::mt19937 _gen{std::random_device{}()};
    public:
        std::optional<std::size_t> pickNext(
            const std::vector<Request>& requests,
            const DiskState& state,
            const DiskModel& model,
            int now
        ) const override;
    };
}

#endif //DISK_SCHEDULER_SIM_RANDSCHEDULER_H
