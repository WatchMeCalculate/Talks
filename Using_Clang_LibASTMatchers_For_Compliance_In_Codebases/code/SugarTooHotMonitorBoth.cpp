#include "shared/AlarmClient.h"
#include "CfgSugarTooHotMonitorApi.h"

class SugarTooHotMonitorBoth{
    private:
        Spooky::Config::SugarTooHotMonitorApi cfgApi_;
    public:
        void run() {
            Spooky::Factory::AlarmClient alarmClient{cfgApi_.VarNames.AlarmName};
            second();

            alarmClient.raise();
        };
        void second() {
            Spooky::Factory::AlarmClient alarmClient{"SugarTooHot"};
            alarmClient.raise();
        };
};
