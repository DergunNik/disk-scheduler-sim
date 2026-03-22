#ifndef DISK_SCHEDULER_SIM_REQUESTSLOADERS_H
#define DISK_SCHEDULER_SIM_REQUESTSLOADERS_H
#include <string>
#include <vector>

#include "Request.h"

namespace disksim {
    class DiskModel;

    std::vector<Request> loadRequestsFromFile(const std::string& path);
    std::vector<Request> generateRequests(
        int requestsNumber,
        int maxTime,
        int maxSectorsPerRequest,
        int randSeed,
        const DiskModel& model);
}

#endif //DISK_SCHEDULER_SIM_REQUESTSLOADERS_H