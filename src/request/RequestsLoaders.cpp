#include "RequestsLoaders.h"

#include <fstream>
#include <random>
#include <sstream>

#include "../disk/DiskModel.h"

namespace disksim {
    OperationType intToOp(const int val) {
        return val == 0 ? OperationType::Read : OperationType::Write;
    }

    std::istream& operator>>(std::istream& is, Request& req) {
        std::string line;
        if (std::getline(is, line)) {
            std::stringstream ss(line);
            std::string temp;
            std::vector<std::string> fields;

            while (std::getline(ss, temp, ',')) {
                fields.push_back(temp);
            }

            if (fields.size() >= 5) {
                req.arrivalTime = std::stoi(fields[0]);
                req.targetCylinder = std::stoi(fields[1]);
                req.targetSector = std::stoi(fields[2]);
                req.bytes = std::stoi(fields[3]);
                req.type = intToOp(std::stoi(fields[4]));
                if (fields.size() == 6) req.id = std::stoi(fields[5]);
            }
        }
        return is;
    }

    std::vector<Request> loadRequestsFromFile(const std::string& path) {
        std::ifstream file(path);
        std::vector<Request> requests;
        Request temp{};

        while (file >> temp) {
            requests.push_back(temp);
        }

        file.close();
        return requests;
    }

    std::vector<Request> generateRequests(
        const int requestsNumber,
        const int maxTime,
        const int maxSectorsPerRequest,
        const int randSeed,
        const DiskModel& model)
    {
        if (requestsNumber <= 0) return {};

        std::vector<Request> requests;
        requests.reserve(requestsNumber);

        std::mt19937 gen(randSeed);
        std::uniform_int_distribution distCylinder(0, model.cylinders - 1);
        std::uniform_int_distribution distSector(0, model.sectorsPerCylinder - 1);
        std::uniform_int_distribution distArrival(0, maxTime - 1);
        std::uniform_int_distribution distOp(0, 1);
        std::uniform_int_distribution distBytes(1, maxSectorsPerRequest * model.bytesPerSector);

        for (int i = 0; i < requestsNumber; ++i) {
            Request req;
            req.id = i + 1;
            req.arrivalTime = distArrival(gen);
            req.targetCylinder = distCylinder(gen);
            req.targetSector = distSector(gen);
            req.bytes = distBytes(gen) ;
            req.type = intToOp(distOp(gen));
            requests.push_back(req);
        }

        return requests;
    }
}
