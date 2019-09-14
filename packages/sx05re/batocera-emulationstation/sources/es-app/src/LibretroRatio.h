//
// Created by matthieu on 03/04/16.
//

#ifndef EMULATIONSTATION_ALL_LIBRETRORATIO_H
#define EMULATIONSTATION_ALL_LIBRETRORATIO_H

#include <map>
#include <string>

class LibretroRatio {
public :
    std::map<std::string,std::string> * getRatio();
    static LibretroRatio* getInstance();
private:
    static LibretroRatio* sInstance;
    std::map<std::string,std::string> * ratioMap;
    LibretroRatio();
};


#endif //EMULATIONSTATION_ALL_LIBRETRORATIO_H
