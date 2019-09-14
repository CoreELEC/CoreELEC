#pragma once

#include "GuiComponent.h"
#include "components/MenuComponent.h"
#include "components/BusyComponent.h"

#include <thread>

class GuiUpdate : public GuiComponent {
public:
    GuiUpdate(Window *window);

    virtual ~GuiUpdate();

    void render(const Transform4x4f &parentTrans) override;

    bool input(InputConfig *config, Input input) override;

    std::vector<HelpPrompt> getHelpPrompts() override;

    void update(int deltaTime) override;

private:
    BusyComponent mBusyAnim;
    bool mLoading;
    int mState;
    std::pair<std::string, int> mResult;

	std::thread *mHandle;
	std::thread *mPingHandle;

    void onUpdateError(std::pair<std::string, int>);

    void onUpdateOk();

    void threadUpdate();

    void threadPing();

    void onUpdateAvailable();

    void onNoUpdateAvailable();

    void onPingError();

};
