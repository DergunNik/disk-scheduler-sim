#include <cmath>
#include <cstdlib>
#include <stdexcept>

#include "DiskModel.h"
#include "../request/Request.h"

namespace disksim {
    namespace {
        double normalizeAngle(double angle) {
            angle = std::fmod(angle, 1.0);
            if (angle < 0.0) {
                angle += 1.0;
            }
            return angle;
        }
    }

    DiskModel::DiskModel(double rotationSpeed,
                         double seekSpeed,
                         double readTransferRate,
                         double writeTransferRate,
                         int cylinders,
                         int sectorsPerCylinder,
                         int bytesPerSector)
        : _anglePerSector(1.0 / static_cast<double>(sectorsPerCylinder))
        , rotationSpeed(rotationSpeed)
        , seekSpeed(seekSpeed)
        , readTransferRate(readTransferRate)
        , writeTransferRate(writeTransferRate)
        , cylinders(cylinders)
        , sectorsPerCylinder(sectorsPerCylinder)
        , bytesPerSector(bytesPerSector) {

        if (rotationSpeed <= 0.0) {
            throw std::invalid_argument("rotationSpeed must be > 0");
        }
        if (seekSpeed <= 0.0) {
            throw std::invalid_argument("seekSpeed must be > 0");
        }
        if (readTransferRate <= 0.0) {
            throw std::invalid_argument("readTransferRate must be > 0");
        }
        if (writeTransferRate <= 0.0) {
            throw std::invalid_argument("writeTransferRate must be > 0");
        }
        if (cylinders <= 0) {
            throw std::invalid_argument("cylinders must be > 0");
        }
        if (sectorsPerCylinder <= 0) {
            throw std::invalid_argument("sectorsPerCylinder must be > 0");
        }
        if (bytesPerSector <= 0) {
            throw std::invalid_argument("bytesPerSector must be > 0");
        }
    }

    double DiskModel::seekTime(int from, int to) const {
        return std::abs(from - to) / seekSpeed;
    }

    double DiskModel::seekTime(const DiskState& state, const Request& req) const {
        return seekTime(state.currentCylinder, req.targetCylinder);
    }

    double DiskModel::rotationalLatency(double currentAngle, int targetSector) const {
        const double angle = normalizeAngle(currentAngle);
        double targetAngle = targetSector * _anglePerSector;

        if (targetAngle < angle) targetAngle += 1.0;

        return (targetAngle - angle) / rotationSpeed;
    }

    double DiskModel::rotationalLatency(const DiskState& state, const Request& req) const {
        return rotationalLatency(state.currentAngle, req.targetSector);
    }

    double DiskModel::transferTime(int sizeBytes, OperationType type) const {
        const double rate = (type == OperationType::Read) ? readTransferRate : writeTransferRate;
        const int sectorCount = (sizeBytes + bytesPerSector - 1) / bytesPerSector;
        const int physicalBytes = sectorCount * bytesPerSector;
        return physicalBytes / rate;
    }

    double DiskModel::transferTime(const Request& req) const {
        return transferTime(req.bytes, req.type);
    }

    double DiskModel::serviceTime(const DiskState& state, const Request& req) const {
        return seekTime(state, req)
             + rotationalLatency(state, req)
             + transferTime(req);
    }
}