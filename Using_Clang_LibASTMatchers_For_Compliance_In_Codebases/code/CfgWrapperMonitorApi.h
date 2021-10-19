#ifndef CFG_WRAPPER_MONITOR_API_
#define CFG_WRAPPER_MONITOR_API_

#include "shared/BaseConfig.h"

namespace Spooky::Config {

class WrapperMonitorApi: public Spooky::Config::ConfigFile {
    public:
        WrapperMonitorApi() {};
        struct VarNamesSxn: public Spooky::Config::ConfigFile::Section {
            std::string NoWrapperAlarmName;
            std::string WrongWrapperAlarmName;
        } VarNames;
};
    
}
#endif 