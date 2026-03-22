#ifndef DISK_SCHEDULER_SIM_METRICSCOLLECTOR_H
#define DISK_SCHEDULER_SIM_METRICSCOLLECTOR_H

#include <cstddef>
#include <unordered_map>

#include "../request/Request.h"
#include "../core/SimulationEvent.h"

namespace disksim {

    struct RequestStats {
        int arrivalTime = 0;
        int startTime = -1;
        int finishTime = -1;

        double seekTime = 0.0;
        double rotationalLatency = 0.0;
        double transferTime = 0.0;

        double waitingTime() const {
            return static_cast<double>(startTime - arrivalTime);
        }

        double serviceTime() const {
            return static_cast<double>(finishTime - startTime);
        }

        double turnaroundTime() const {
            return static_cast<double>(finishTime - arrivalTime);
        }
    };

    class MetricsCollector {
    public:
        void reset();

        void beginSimulation(int time);
        void endSimulation(int time);

        void onOperationFinished(const disksim::core::DiskOperationResult& result);

        std::size_t processedRequests() const;

        double averageWaitingTime() const;
        double maxWaitingTime() const;

        double averageTurnaroundTime() const;
        double maxTurnaroundTime() const;

        double averageServiceTime() const;
        double maxServiceTime() const;

        double averageSeekTime() const;
        double averageRotationalLatency() const;
        double averageTransferTime() const;

        double utilization() const;
        double throughput() const;

        int simulationStartTime() const;
        int simulationEndTime() const;
        int simulationDuration() const;

    private:
        static constexpr int kUnsetTime = -1;

        std::unordered_map<int, RequestStats> _stats;

        int _simulationStartTime = kUnsetTime;
        int _simulationEndTime = kUnsetTime;

        double _totalWaitingTime = 0.0;
        double _totalTurnaroundTime = 0.0;
        double _totalServiceTime = 0.0;

        double _totalSeekTime = 0.0;
        double _totalRotationalLatency = 0.0;
        double _totalTransferTime = 0.0;

        double _maxWaitingTime = 0.0;
        double _maxTurnaroundTime = 0.0;
        double _maxServiceTime = 0.0;

        double _busyTime = 0.0;
        std::size_t _processedRequests = 0;
    };

}

#endif // DISK_SCHEDULER_SIM_METRICSCOLLECTOR_H