#ifndef DISK_SCHEDULER_SIM_SIMULATIONENGINE_H
#define DISK_SCHEDULER_SIM_SIMULATIONENGINE_H

#include <memory>
#include <queue>
#include <span>
#include <vector>

#include "../core/SimulationEvent.h"
#include "../disk/DiskModel.h"
#include "../disk/DiskState.h"
#include "../metrics/MetricsCollector.h"
#include "../request/Request.h"
#include "../scheduler/IScheduler.h"

namespace disksim::core {

    class SimulationEngine {
    public:
        SimulationEngine(
            DiskModel model,
            std::unique_ptr<scheduling::IScheduler> scheduler,
            std::shared_ptr<MetricsCollector> metrics
        );

        void addRequest(const Request& request);
        void addRequestsRange(std::span<const Request> requests);

        void clearRequests();

        void run(int startTime);

        void rerunWithNewScheduler(
            int startTime,
            std::unique_ptr<scheduling::IScheduler> scheduler
        );

        const DiskModel& model() const noexcept;
        const DiskState& state() const noexcept;

    private:
        void resetRuntimeState(int startTime);
        void rebuildEventQueue();

        void handleEvent(const EngineEvent& event);
        void handleEvent(const NewRequestEvent& event);
        void handleEvent(const DiskOperationFinishedEvent& event);

        void tryStartNextOperation();

        struct EventCompare {
            bool operator()(const EngineEvent& a, const EngineEvent& b) const;
        };

        using EventQueue = std::priority_queue<
            EngineEvent,
            std::vector<EngineEvent>,
            EventCompare
        >;

        DiskModel _model;
        std::unique_ptr<scheduling::IScheduler> _scheduler;
        std::shared_ptr<MetricsCollector> _metrics;

        std::vector<Request> _initialRequests;
        std::vector<Request> _pendingRequests;

        DiskState _state{};
        int _now = 0;
        bool _busy = false;

        EventQueue _eventQueue;
    };

}

#endif // DISK_SCHEDULER_SIM_SIMULATIONENGINE_H