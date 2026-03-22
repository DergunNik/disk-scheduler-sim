#ifndef DISK_SCHEDULER_SIM_FCSFSCHEDULER_H
#define DISK_SCHEDULER_SIM_FCSFSCHEDULER_H

#include "../IScheduler.h"

namespace disksim::scheduling {

    class FcfsScheduler : IScheduler {
    public:
        std::optional<std::size_t> pickNext(
            const std::vector<Request>& requests,
            const DiskState& state,
            const DiskModel& model,
            int now
        ) const override;
    };
}

#endif //DISK_SCHEDULER_SIM_FCSFSCHEDULER_H