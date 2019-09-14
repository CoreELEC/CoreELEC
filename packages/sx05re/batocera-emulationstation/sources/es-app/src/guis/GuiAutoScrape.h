#pragma once

#include "GuiComponent.h"
#include "components/MenuComponent.h"
#include "components/BusyComponent.h"

#include <thread>

class GuiAutoScrape : public GuiComponent {
public:
    GuiAutoScrape(Window *window);

    virtual ~GuiAutoScrape();

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

    void onAutoScrapeError(std::pair<std::string, int>);

    void onAutoScrapeOk();

    void threadAutoScrape();
};
