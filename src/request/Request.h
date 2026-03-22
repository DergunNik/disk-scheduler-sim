#ifndef DISK_SCHEDULER_SIM_REQUEST_H
#define DISK_SCHEDULER_SIM_REQUEST_H

namespace disksim {
    enum class OperationType {
        Read, Write
    };

    struct Request {
        int arrivalTime;
        int bytes;
        int targetCylinder;
        int targetSector;
        OperationType type;
        int id;
    };
}
#endif //DISK_SCHEDULER_SIM_REQUEST_H