#include "shared/AlarmClient.h"
#include "CfgSugarTooHotMonitorApi.h"

class SugarTooHotMonitorFromConfig {
    private:
        Spooky::Config::SugarTooHotMonitorApi cfgApi_;
    public:
        void run() {
            Spooky::Factory::AlarmClient alarmClient{cfgApi_.VarNames.AlarmName};

            alarmClient.raise();
        };
};
