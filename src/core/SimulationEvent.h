#ifndef DISK_SCHEDULER_SIM_SIMULATIONEVENT_H
#define DISK_SCHEDULER_SIM_SIMULATIONEVENT_H
#include <variant>

#include "../request/Request.h"
#include "../disk/DiskState.h"

namespace disksim::core {
     struct DiskOperationResult {
          Request request;

          DiskState atStart;

          int startTime;
          int finishTime;

          double seekTime;
          double rotationalLatency;
          double transferTime;
     };

     struct NewRequestEvent {
          int time;
          Request request;
     };

     struct DiskOperationFinishedEvent {
          int time;
          DiskOperationResult result;
     };

     using EngineEvent = std::variant<NewRequestEvent, DiskOperationFinishedEvent>;

}

#endif //DISK_SCHEDULER_SIM_SIMULATIONEVENT_H