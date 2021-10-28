#ifndef ALARM_CLIENT_
#define ALARM_CLIENT_

#include <string>

namespace Spooky {  namespace Factory {

class AlarmClient {
    public:
        AlarmClient() = delete;
        AlarmClient(std::string alarmName): alarmName_(alarmName) {};
        void raise() {};
    private:
        std::string alarmName_;
};
}}
#endif
