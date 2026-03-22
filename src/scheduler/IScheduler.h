#ifndef DISK_SCHEDULER_SIM_ISCHEDULER_H
#define DISK_SCHEDULER_SIM_ISCHEDULER_H

#include <cstddef>
#include <optional>
#include <vector>

#include "../request/Request.h"
#include "../disk/DiskState.h"
#include "../disk/DiskModel.h"

namespace disksim::scheduling {

    class IScheduler {
    public:
        virtual ~IScheduler() = default;

        virtual std::optional<std::size_t> pickNext(
            const std::vector<Request>& requests,
            const DiskState& state,
            const DiskModel& model,
            int now
        ) const = 0;
    };

}

#endif // DISK_SCHEDULER_SIM_ISCHEDULER_H