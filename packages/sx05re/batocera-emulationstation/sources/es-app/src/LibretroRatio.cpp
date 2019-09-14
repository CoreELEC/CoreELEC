//
// Created by matthieu on 03/04/16.
//

#include "LibretroRatio.h"

LibretroRatio *LibretroRatio::sInstance = NULL;

LibretroRatio::LibretroRatio() {
    ratioMap = new std::map<std::string, std::string>
            {
                    {"Auto",          "auto"},
                    {"4/3",           "4/3"},
                    {"16/9",          "16/9"},
                    {"16/10",         "16/10"},
                    {"16/15",         "16/15"},
		    {"21/9",          "21/9"},
                    {"1/1",           "1/1"},
                    {"2/1",           "2/1"},
                    {"3/2",           "3/2"},
                    {"3/4",           "3/4"},
                    {"4/1",           "4/1"},
                    {"4/4",           "4/4"},
                    {"5/4",           "5/4"},
                    {"6/5",           "6/5"},
                    {"7/9",           "7/9"},
                    {"8/3",           "8/3"},
                    {"8/7",           "8/7"},
                    {"19/12",         "19/12"},
                    {"19/14",         "19/14"},
                    {"30/17",         "30/17"},
                    {"32/9",          "32/9"},
                    {"Square pixel",  "squarepixel"},
                    {"Custom",        "custom"}
            };
}

LibretroRatio *LibretroRatio::getInstance() {
    if (sInstance == NULL)
        sInstance = new LibretroRatio();

    return sInstance;
}

std::map<std::string, std::string> *LibretroRatio::getRatio() {
    return ratioMap;
};
