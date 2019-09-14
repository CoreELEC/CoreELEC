//
// Created by matthieu on 03/08/15.
//

#ifndef EMULATIONSTATION_ALL_GUILOADING_H
#define EMULATIONSTATION_ALL_GUILOADING_H


#include "GuiComponent.h"
#include "components/MenuComponent.h"
#include "components/BusyComponent.h"

#include <thread>

class GuiLoading  : public GuiComponent {
public:
    GuiLoading(Window *window, const std::function<void *()> &mFunc, const std::function<void(void *)> &mFunc2);
    GuiLoading(Window *window, const std::function<void *()> &mFunc);

    virtual ~GuiLoading();

    void render(const Transform4x4f &parentTrans) override;

    bool input(InputConfig *config, Input input) override;

    std::vector<HelpPrompt> getHelpPrompts() override;

    void update(int deltaTime) override;

private:
    BusyComponent mBusyAnim;
    std::thread *mHandle;
    bool mRunning;
    const std::function<void*()> mFunc;
    const std::function<void(void *)> mFunc2;
    void threadLoading();
    void * result;
};


#endif //EMULATIONSTATION_ALL_GUILOADING_H
