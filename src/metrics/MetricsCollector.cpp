#include "MetricsCollector.h"
#include <algorithm>
#include <limits>

namespace disksim {

    namespace {
        double safeDivide(double numerator, double denominator) {
            return denominator > 0.0 ? numerator / denominator : 0.0;
        }
    }

    //---------------------------------------------------------

    void MetricsCollector::reset() {
        _stats.clear();
        _simulationStartTime = kUnsetTime;
        _simulationEndTime = kUnsetTime;
        _totalWaitingTime = 0.0;
        _totalTurnaroundTime = 0.0;
        _totalServiceTime = 0.0;
        _totalSeekTime = 0.0;
        _totalRotationalLatency = 0.0;
        _totalTransferTime = 0.0;
        _maxWaitingTime = 0.0;
        _maxTurnaroundTime = 0.0;
        _maxServiceTime = 0.0;
        _busyTime = 0.0;
        _processedRequests = 0;
    }

    void MetricsCollector::beginSimulation(int time) {
        if (_simulationStartTime == kUnsetTime) {
            _simulationStartTime = time;
        } else {
            _simulationStartTime = std::min(_simulationStartTime, time);
        }
    }

    void MetricsCollector::endSimulation(int time) {
        if (_simulationEndTime == kUnsetTime) {
            _simulationEndTime = time;
        } else {
            _simulationEndTime = std::max(_simulationEndTime, time);
        }
    }

    //---------------------------------------------------------

    void MetricsCollector::onOperationFinished(const discsim::core::DiskOperationResult& result) {
        const int id = result.request.id;

        RequestStats& stats = _stats[id];
        stats.arrivalTime = result.request.arrivalTime;
        stats.startTime = result.startTime;
        stats.finishTime = result.finishTime;
        stats.seekTime = result.seekTime;
        stats.rotationalLatency = result.rotationalLatency;
        stats.transferTime = result.transferTime;

        const double waiting = stats.waitingTime();
        const double turnaround = stats.turnaroundTime();
        const double service = stats.serviceTime();

        _totalWaitingTime += waiting;
        _totalTurnaroundTime += turnaround;
        _totalServiceTime += service;
        _totalSeekTime += stats.seekTime;
        _totalRotationalLatency += stats.rotationalLatency;
        _totalTransferTime += stats.transferTime;

        _maxWaitingTime = std::max(_maxWaitingTime, waiting);
        _maxTurnaroundTime = std::max(_maxTurnaroundTime, turnaround);
        _maxServiceTime = std::max(_maxServiceTime, service);

        _busyTime += service;
        ++_processedRequests;
    }

    //---------------------------------------------------------

    double MetricsCollector::averageWaitingTime() const {
        return safeDivide(_totalWaitingTime, static_cast<double>(_processedRequests));
    }

    double MetricsCollector::maxWaitingTime() const {
        return _maxWaitingTime;
    }

    double MetricsCollector::averageTurnaroundTime() const {
        return safeDivide(_totalTurnaroundTime, static_cast<double>(_processedRequests));
    }

    double MetricsCollector::maxTurnaroundTime() const {
        return _maxTurnaroundTime;
    }

    double MetricsCollector::averageServiceTime() const {
        return safeDivide(_totalServiceTime, static_cast<double>(_processedRequests));
    }

    double MetricsCollector::maxServiceTime() const {
        return _maxServiceTime;
    }

    //---------------------------------------------------------

    double MetricsCollector::averageSeekTime() const {
        return safeDivide(_totalSeekTime, static_cast<double>(_processedRequests));
    }

    double MetricsCollector::averageRotationalLatency() const {
        return safeDivide(_totalRotationalLatency, static_cast<double>(_processedRequests));
    }

    double MetricsCollector::averageTransferTime() const {
        return safeDivide(_totalTransferTime, static_cast<double>(_processedRequests));
    }

    //---------------------------------------------------------

    double MetricsCollector::utilization() const {
        return safeDivide(_busyTime, simulationDuration());
    }

    double MetricsCollector::throughput() const {
        return safeDivide(static_cast<double>(_processedRequests), simulationDuration());
    }

    std::size_t MetricsCollector::processedRequests() const {
        return _processedRequests;
    }

    //---------------------------------------------------------

    int MetricsCollector::simulationDuration() const {
        if (_simulationStartTime == kUnsetTime || _simulationEndTime == kUnsetTime) {
            return 0;
        }
        return _simulationEndTime - _simulationStartTime;
    }

    int MetricsCollector::simulationStartTime() const {
        return _simulationStartTime;
    }

    int MetricsCollector::simulationEndTime() const {
        return _simulationEndTime;
    }

}
