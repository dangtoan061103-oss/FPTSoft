// #pragma once
#include <iostream>

//------Alias Smart Pointer-----
#include <memory>

// using SensorPtr = std::unique_ptr<ISensor, SensorCustomDeleter>; // Duy chuyen xuong duoi: dong 77
//-------
class ISensor {
public:
    virtual ~ISensor() = default;
    virtual void read() = 0;
};

class TemperatureSensor : public ISensor {
    int id;
    public:
        TemperatureSensor(int id_) : id(id_) {}

        void read() override {
            std::cout << "[TempSensor " << id << "] reading...\n";
        }
};


#include <array>
#include <stack>

class SensorMemoryPool {
public:
    static constexpr size_t POOL_SIZE = 5;

private:
    std::array<TemperatureSensor, POOL_SIZE> pool{
        TemperatureSensor(0),
        TemperatureSensor(1),
        TemperatureSensor(2),
        TemperatureSensor(3),
        TemperatureSensor(4)
    };

    std::stack<TemperatureSensor*> freeList;

public:
    SensorMemoryPool() {
        for (auto& sensor : pool) {
            freeList.push(&sensor);
        }
    }

    TemperatureSensor* acquire() {
        if (freeList.empty()) {
            return nullptr;
        }
        auto* s = freeList.top();
        freeList.pop();
        return s;
    }

    void release(TemperatureSensor* sensor) {
        freeList.push(sensor);
        std::cout << "Sensor returned to pool\n";
    }
};

struct SensorCustomDeleter {
    SensorMemoryPool* pool;

    void operator()(ISensor* sensor) const {
        // Không delete!
        pool->release(static_cast<TemperatureSensor*>(sensor));
    }
};


//------
using SensorPtr = std::unique_ptr<ISensor, SensorCustomDeleter>;
#include <vector>

class ECUManager {
    std::vector<SensorPtr> sensors;

public:
    void addSensor(SensorPtr sensor) {
        sensors.push_back(std::move(sensor));
    }

    void checkAllSensors() {
        for (auto& s : sensors) {
            s->read();
        }
    }
};

//-------
int main() {
    SensorMemoryPool pool;
    ECUManager ecu;

    {
        auto* raw = pool.acquire();
        if (raw) {
            SensorPtr sensor(
                raw,
                SensorCustomDeleter{ &pool }
            );
            ecu.addSensor(std::move(sensor));
        }
    } // sensor vẫn sống trong ECUManager

    ecu.checkAllSensors();

    std::cout << "ECU shutting down...\n";
}
