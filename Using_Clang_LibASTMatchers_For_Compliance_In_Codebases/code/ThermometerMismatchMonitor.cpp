#include <memory>
#include "shared/AlarmClient.h"

class ThermometerMisMatchMonitor{
    private:
        std::unique_ptr<Spooky::Factory::AlarmClient> alarmClient_;
    public:
        void run() {
            alarmClient_ = std::make_unique<Spooky::Factory::AlarmClient>("SugarTooHot");

            alarmClient_->raise();
        };
};
