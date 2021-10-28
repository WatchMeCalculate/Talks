#include <memory>
#include "shared/AlarmClient.h"

class ThermometerMismatchMonitor {
    private:
        std::unique_ptr<Spooky::Factory::AlarmClient> alarmClient_;
    public:
        void run() {
            alarmClient_ = std::make_unique<Spooky::Factory::AlarmClient>("ThermometerMismatch");

            alarmClient_->raise();
        };
};
