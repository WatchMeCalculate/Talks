#include <memory>
#include "shared/AlarmClient.h"
#include "CfgWrapperMonitorApi.h"

namespace Spooky {  namespace Factory {

class WrapperMonitor {
    private:
        Spooky::Config::WrapperMonitorApi cfgApi_;
        std::unique_ptr<Spooky::Factory::AlarmClient>  noWrapperAlarm_;
        std::unique_ptr<Spooky::Factory::AlarmClient>  wrongWrapperAlarm_;
    public:
        WrapperMonitor():
        noWrapperAlarm_(std::make_unique<Spooky::Factory::AlarmClient>(cfgApi_.VarNames.NoWrapperAlarmName))
        {
            wrongWrapperAlarm_= std::make_unique<Spooky::Factory::AlarmClient>(cfgApi_.VarNames.WrongWrapperAlarmName);
        }
        void run() {
            /*
             * We do some sort of logic and determine to raise an alarm
             */
            noWrapperAlarm_->raise();
        }

};

}}
