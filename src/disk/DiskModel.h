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

    private:
        const double _anglePerSector;

        const double _rotationSpeed; // rotations / ms
        const double _seekSpeed; // cylinders / ms
        const double _readTransferRate; // bytes / ms
        const double _writeTransferRate; // bytes / ms

        const int _cylinders;
        const int _sectorsPerCylinder;
        const int _bytesPerSector;
    };
}

#endif // DISK_SCHEDULER_SIM_DISKMODEL_H