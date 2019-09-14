#pragma once

#include "GuiComponent.h"
#include "components/MenuComponent.h"
#include "components/BusyComponent.h"

#include <thread>

class GuiBezelInstall : public GuiComponent {
public:
    GuiBezelInstall(Window *window, char *theme);

    virtual ~GuiBezelInstall();

    void render(const Transform4x4f &parentTrans) override;

    bool input(InputConfig *config, Input input) override;

    std::vector<HelpPrompt> getHelpPrompts() override;

    void update(int deltaTime) override;

private:
    BusyComponent mBusyAnim;
    bool mLoading;
    int mState;
    std::pair<std::string, int> mResult;

    std::string mBezelName;
    
    std::thread *mHandle;

    void onInstallError(std::pair<std::string, int>);

    void onInstallOk();

    void threadBezel();

};

class GuiBezelUninstall : public GuiComponent {
public:
    GuiBezelUninstall(Window *window, char *theme);

    virtual ~GuiBezelUninstall();

    void render(const Transform4x4f &parentTrans) override;

    bool input(InputConfig *config, Input input) override;

    std::vector<HelpPrompt> getHelpPrompts() override;

    void update(int deltaTime) override;

private:
    BusyComponent mBusyAnim;
    bool mLoading;
    int mState;
    std::pair<std::string, int> mResult;

    std::string mBezelName;
    
    std::thread *mHandle;

    void onInstallError(std::pair<std::string, int>);

    void onInstallOk();

    void threadBezel();

};

