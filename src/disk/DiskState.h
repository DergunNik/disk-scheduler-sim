#ifndef DISK_SCHEDULER_SIM_DISKSTATE_H
#define DISK_SCHEDULER_SIM_DISKSTATE_H

namespace disksim {

    enum class DiskHeadDirection {
        Inward, Outward
    };

    struct DiskState {
        int currentCylinder = 0;
        double currentAngle = 0.0; // [0, 1)
        DiskHeadDirection direction = DiskHeadDirection::Inward;
    };

}

#endif //DISK_SCHEDULER_SIM_DISKSTATE_H