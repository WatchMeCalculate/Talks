#ifndef CFG_SUGAR_MONITOR_API_
#define CFG_SUGAR_MONITOR_API_

#include "shared/BaseConfig.h"

namespace Spooky { namespace Config {

class SugarTooHotMonitorApi: public Spooky::Config::ConfigFile {
    public:
        SugarTooHotMonitorApi() {};
        struct VarNamesSxn: public Spooky::Config::ConfigFile::Section {
            std::string AlarmName;
        } VarNames;
};
}}
#endif
