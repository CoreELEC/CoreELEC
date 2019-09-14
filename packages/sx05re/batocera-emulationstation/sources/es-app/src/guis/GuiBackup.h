#pragma once

#include "GuiComponent.h"
#include "components/MenuComponent.h"
#include "components/BusyComponent.h"

#include <thread>

class GuiBackup : public GuiComponent {
public:
    GuiBackup(Window *window, std::string storageDevice);

    virtual ~GuiBackup();

    void render(const Transform4x4f &parentTrans) override;

    bool input(InputConfig *config, Input input) override;

    std::vector<HelpPrompt> getHelpPrompts() override;

    void update(int deltaTime) override;

private:
    BusyComponent mBusyAnim;
    bool mLoading;
    int mState;
    std::pair<std::string, int> mResult;

    std::string mstorageDevice;
    
    std::thread *mHandle;

    void onBackupError(std::pair<std::string, int>);

    void onBackupOk();

    void threadBackup();

};
