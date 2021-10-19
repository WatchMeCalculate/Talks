
#include "shared/AlarmClient.h"

class SugarTooHotMonitor {
    public:
        void run() {
            Spooky::Factory::AlarmClient alarmClient("SugarTooHot");

            alarmClient.raise();
        };
};
