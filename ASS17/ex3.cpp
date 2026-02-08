#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
 
// ===============================
// Base Sensor Interface
// ===============================

class ISensor {
    public:

        virtual ~ISensor() = default;
        virtual int getId() const = 0;
        virtual void runDiagnostics() const = 0;

};

// Concrete Sensor
class TemperatureSensor : public ISensor {
    int m_id;
    public:
        TemperatureSensor(int id) : m_id(id) {
            std::cout << "TemperatureSensor " << m_id << " constructed\n";
        }

        ~TemperatureSensor() {
            std::cout << "TemperatureSensor " << m_id << " destroyed\n";
        }
        int getId() const override { return m_id; }
        void runDiagnostics() const override {
            std::cout << "Running diagnostics on sensor " << m_id << "\n";
        }
};

 

// Forward declaration

class DiagnosticTool;
// ===============================
// ECUManager
// ===============================
class ECUManager {
    public:

        std::vector<std::shared_ptr<ISensor>> m_sensors;
        std::shared_ptr<DiagnosticTool> m_diagTool; // ECUManager owns DiagnosticTool

        ECUManager() { std::cout << "ECUManager constructed\n"; }
        ~ECUManager() { std::cout << "ECUManager destroyed\n"; }

        void addSensor(std::shared_ptr<ISensor> sensor) {
            m_sensors.push_back(sensor);
        }

};

// ===============================
// DiagnosticTool
// ===============================
class DiagnosticTool {
    public:
        // Scenario A: shared_ptr back to ECUManager → cycle
        std::shared_ptr<ECUManager> m_ecu;
    
        // Scenario B: fix with weak_ptr
        //std::weak_ptr<ECUManager> m_ecu;

        std::vector<std::weak_ptr<ISensor>> m_sensorsToMonitor;
        DiagnosticTool() { std::cout << "DiagnosticTool constructed\n"; }
        ~DiagnosticTool() { std::cout << "DiagnosticTool destroyed\n"; }

        void checkSensors() {
            for (auto& weakSensor : m_sensorsToMonitor) {
                if (auto sensor = weakSensor.lock()) {
                    sensor->runDiagnostics();
                } else {
                    std::cout << "Sensor has been disconnected\n";
                }
            }
        }
};

// ===============================
// Demonstration
// ===============================
int main() {
    std::cout << "=== Scenario A: Circular Dependency (memory leak) ===\n";
    {
        auto ecu = std::make_shared<ECUManager>();
        auto diag = std::make_shared<DiagnosticTool>();
 
        ecu->m_diagTool = diag;
        diag->m_ecu = ecu;

        // If DiagnosticTool also holds shared_ptr back to ECUManager:
        // diag->m_ecu = ecu; // (shared_ptr) → cycle
        // In this case, neither ECUManager nor DiagnosticTool will be destroyed
        // when going out of scope because reference counts never reach zero.
    }
    std::cout << "End of Scenario A block\n\n";
    // std::cout << "=== Scenario B: Using weak_ptr (no leak) ===\n";

    // {

    //     auto ecu = std::make_shared<ECUManager>();

    //     auto diag = std::make_shared<DiagnosticTool>();

    //     ecu->m_diagTool = diag;
    //     diag->m_ecu = ecu; // weak_ptr → no cycle
    //     // Add a sensor
    //     auto sensor = std::make_shared<TemperatureSensor>(101);
    //     ecu->addSensor(sensor);
    //     diag->m_sensorsToMonitor.push_back(sensor);
    //     // ECUManager releases sensor
    //     ecu->m_sensors.clear();

    //     // DiagnosticTool checks sensors
    //     diag->checkSensors();
    // }
    // std::cout << "End of Scenario B block\n";

    return 0;
}

 

/**
 * EXPLANATION:
 * ------------
 * - Scenario A:
 *   ECUManager holds shared_ptr<DiagnosticTool>.
 *   DiagnosticTool holds shared_ptr<ECUManager>.
 *   → Reference count never reaches zero → memory leak (objects never destroyed).
 *
 * - Scenario B:
 *   DiagnosticTool holds weak_ptr<ECUManager>.
 *   → weak_ptr does not increase reference count.
 *   → When both shared_ptr owners go out of scope, objects are destroyed correctly.
 *
 * - Scenario C:
 *   DiagnosticTool monitors sensors via weak_ptr.
 *   → weak_ptr.lock() tries to get a shared_ptr.
 *   → If valid, runDiagnostics() is called.
 *   → If expired, report "Sensor has been disconnected".
 *
 * KEY CONCEPTS:
 * -------------
 * - shared_ptr: shared ownership, reference counting.
 * - weak_ptr: non-owning reference, does not affect lifetime.
 * - Use weak_ptr to break cycles and to observe objects without keeping them alive.
 *
 * BENEFIT:
 * --------
 * - Prevents memory leaks caused by circular dependencies.
 * - Allows safe observation of objects that may be destroyed elsewhere.
 */

 