#ifndef DISK_SCHEDULER_SIM_SSTFSCHEDULER_H
#define DISK_SCHEDULER_SIM_SSTFSCHEDULER_H

#include <cstddef>
#include <optional>
#include <vector>

#include "../IScheduler.h"

namespace disksim::scheduling {

    class SstfScheduler : public IScheduler {
    public:
        std::optional<std::size_t> pickNext(
            const std::vector<Request>& requests,
            const DiskState& state,
            const DiskModel& model,
            int now
        ) const override;
    };

}

#endif // DISK_SCHEDULER_SIM_SSTFSCHEDULER_H