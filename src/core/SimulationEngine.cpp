#include "SimulationEngine.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>
#include <variant>

namespace disksim::core {
    namespace {
        int eventTime(const EngineEvent& event) {
            return std::visit([](const auto& e) -> int {
                return e.time;
            }, event);
        }

        double normalizeAngle(double angle) {
            angle = std::fmod(angle, 1.0);
            if (angle < 0.0) {
                angle += 1.0;
            }
            return angle;
        }
    }

    SimulationEngine::SimulationEngine(
        DiskModel model,
        std::unique_ptr<scheduling::IScheduler> scheduler,
        std::shared_ptr<MetricsCollector> metrics
    )
        : _model(std::move(model))
        , _scheduler(std::move(scheduler))
        , _metrics(std::move(metrics)) {
        if (!_scheduler) {
            throw std::invalid_argument("SimulationEngine: scheduler must not be null");
        }
        if (!_metrics) {
            throw std::invalid_argument("SimulationEngine: metrics must not be null");
        }
    }

    void SimulationEngine::addRequest(const Request& request) {
        _initialRequests.push_back(request);
    }

    void SimulationEngine::addRequestsRange(std::span<const Request> requests) {
        _initialRequests.insert(_initialRequests.end(), requests.begin(), requests.end());
    }

    void SimulationEngine::clearRequests() {
        _initialRequests.clear();
        _pendingRequests.clear();
        _eventQueue = EventQueue{};
        _state = DiskState{};
        _now = 0;
        _busy = false;
    }

    void SimulationEngine::run(int startTime) {
        _metrics->reset();

        resetRuntimeState(startTime);
        rebuildEventQueue();

        _metrics->beginSimulation(startTime);

        while (!_eventQueue.empty()) {
            const EngineEvent event = _eventQueue.top();
            _eventQueue.pop();

            _now = std::max(_now, eventTime(event));
            handleEvent(event);
        }

        _metrics->endSimulation(_now);
    }

    void SimulationEngine::rerunWithNewScheduler(
        int startTime,
        std::unique_ptr<scheduling::IScheduler> scheduler
    ) {
        if (!scheduler) {
            throw std::invalid_argument("SimulationEngine: scheduler must not be null");
        }

        _scheduler = std::move(scheduler);
        run(startTime);
    }

    const DiskModel& SimulationEngine::model() const noexcept {
        return _model;
    }

    const DiskState& SimulationEngine::state() const noexcept {
        return _state;
    }

    void SimulationEngine::resetRuntimeState(int startTime) {
        _pendingRequests = _initialRequests;
        _eventQueue = EventQueue{};

        _state = DiskState{};
        _now = startTime;
        _busy = false;
    }

    void SimulationEngine::rebuildEventQueue() {
        for (const Request& request : _initialRequests) {
            _eventQueue.push(NewRequestEvent{
                .time = request.arrivalTime,
                .request = request
            });
        }
    }

    void SimulationEngine::handleEvent(const EngineEvent& event) {
        std::visit([this](const auto& e) {
            this->handleEvent(e);
        }, event);
    }

    void SimulationEngine::handleEvent(const NewRequestEvent& event) {
        _pendingRequests.push_back(event.request);
        tryStartNextOperation();
    }

    void SimulationEngine::handleEvent(const DiskOperationFinishedEvent& event) {
        const DiskOperationResult& result = event.result;

        _metrics->onOperationFinished(result);

        _state.currentCylinder = result.request.targetCylinder;
        _state.currentAngle = normalizeAngle(
            result.atStart.currentAngle +
            static_cast<double>(result.finishTime - result.startTime) * _model.rotationSpeed
        );

        _busy = false;
        tryStartNextOperation();
    }

    void SimulationEngine::tryStartNextOperation() {
        if (_busy || _pendingRequests.empty()) {
            return;
        }

        const auto nextIndex =
            _scheduler->pickNext(_pendingRequests, _state, _now);

        if (!nextIndex.has_value() || *nextIndex >= _pendingRequests.size()) {
            return;
        }

        const std::size_t index = *nextIndex;
        const Request request = _pendingRequests[index];

        _pendingRequests[index] = _pendingRequests.back();
        _pendingRequests.pop_back();

        const double seek = _model.seekTime(_state, request);
        const double latency = _model.rotationalLatency(_state, request);
        const double transfer = _model.transferTime(request);
        const int finishTime = _now + static_cast<int>(std::ceil(seek + latency + transfer));

        DiskOperationResult result{
            .request = request,
            .atStart = _state,
            .startTime = _now,
            .finishTime = finishTime,
            .seekTime = seek,
            .rotationalLatency = latency,
            .transferTime = transfer
        };

        _busy = true;

        _eventQueue.push(DiskOperationFinishedEvent{
            .time = finishTime,
            .result = std::move(result)
        });
    }

    bool SimulationEngine::EventCompare::operator()(
        const EngineEvent& a,
        const EngineEvent& b
    ) const {
        const int timeA = eventTime(a);
        const int timeB = eventTime(b);

        if (timeA != timeB) {
            return timeA > timeB;
        }

        return a.index() > b.index();
    }

}