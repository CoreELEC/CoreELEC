#ifndef EMULATIONSTATION_ALL_SYSTEMCONF_H
#define EMULATIONSTATION_ALL_SYSTEMCONF_H


#include <string>
#include <map>

class SystemConf {

public:
    SystemConf();

    bool loadSystemConf();

    bool saveSystemConf();

    std::string get(const std::string &name);
    std::string get(const std::string &name, const std::string &defaut);

    bool set(const std::string &name, const std::string &value);

    static SystemConf *sInstance;

    static SystemConf *getInstance();
private:
    std::map<std::string, std::string> confMap;
	bool mWasChanged;
};


#endif //EMULATIONSTATION_ALL_SYSTEMCONF_H
