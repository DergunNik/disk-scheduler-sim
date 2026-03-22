#ifndef DISK_SCHEDULER_SIM_DISKMODEL_H
#define DISK_SCHEDULER_SIM_DISKMODEL_H

#include <cstddef>

#include "DiskState.h"

namespace disksim {
    struct Request;
    enum class OperationType;

    class DiskModel {
    public:
        DiskModel(
            double rotationSpeed,
            double seekSpeed,
            double readTransferRate,
            double writeTransferRate,
            int cylinders,
            int sectorsPerCylinder,
            int bytesPerSector);

        double seekTime(int from, int to) const;
        double seekTime(const DiskState& state, const Request& req) const;

        double rotationalLatency(double currentAngle, int targetSector) const;
        double rotationalLatency(const DiskState& state, const Request& req) const;

        double transferTime(int sizeBytes, OperationType type) const;
        double transferTime(const Request& req) const;

        double serviceTime(const DiskState& state, const Request& req) const;

        const double rotationSpeed;      // rotations / ms
        const double seekSpeed;          // cylinders / ms
        const double readTransferRate;   // bytes / ms
        const double writeTransferRate;  // bytes / ms

        const int cylinders;
        const int sectorsPerCylinder;
        const int bytesPerSector;

    private:
        const double _anglePerSector;
    };
}

#endif // DISK_SCHEDULER_SIM_DISKMODEL_H