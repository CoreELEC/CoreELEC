#include "guis/GuiInstall.h"
#include "guis/GuiMsgBox.h"

#include "Window.h"
#include <string>
#include "Log.h"
#include "Settings.h"
#include "ApiSystem.h"
#include "LocaleES.h"

GuiInstall::GuiInstall(Window* window, std::string storageDevice, std::string architecture) : GuiComponent(window), mBusyAnim(window)
{
	setSize((float)Renderer::getScreenWidth(), (float)Renderer::getScreenHeight());
        mLoading = true;
	mState = 1;
        mBusyAnim.setSize(mSize);
	mstorageDevice = storageDevice;
	marchitecture = architecture;
}

GuiInstall::~GuiInstall()
{
}

bool GuiInstall::input(InputConfig* config, Input input)
{
        return false;
}

std::vector<HelpPrompt> GuiInstall::getHelpPrompts()
{
	return std::vector<HelpPrompt>();
}

void GuiInstall::render(const Transform4x4f& parentTrans)
{
        Transform4x4f trans = parentTrans * getTransform();

        renderChildren(trans);

        Renderer::setMatrix(trans);
        Renderer::drawRect(0.f, 0.f, mSize.x(), mSize.y(), 0x00000011);

        if(mLoading)
        mBusyAnim.render(trans);

}

void GuiInstall::update(int deltaTime) {
        GuiComponent::update(deltaTime);
        mBusyAnim.update(deltaTime);
        
        Window* window = mWindow;
        if(mState == 1){
	  mLoading = true;
	  mHandle = new std::thread(&GuiInstall::threadInstall, this);
	  mState = 0;
        }

        if(mState == 2){
	  window->pushGui(
			  new GuiMsgBox(window, _("FINISHED"), _("OK"),
					[this] {
					  mState = -1;
					}
					)
			  );
	  mState = 0;
        }
        if(mState == 3){
            window->pushGui(
                    new GuiMsgBox(window, mResult.first, _("OK"),
                                  [this] {
                                      mState = -1;
                                  }
                    )
            );
            mState = 0;
        }

        if(mState == -1){
	  delete this;
        }
}

void GuiInstall::threadInstall() 
{
    std::pair<std::string,int> updateStatus = ApiSystem::getInstance()->installSystem(&mBusyAnim, mstorageDevice, marchitecture);
    if(updateStatus.second == 0){
        this->onInstallOk();
    }else {
        this->onInstallError(updateStatus);
    }  
}

void GuiInstall::onInstallError(std::pair<std::string, int> result)
{
    mLoading = false;
    mState = 3;
    mResult = result;
    mResult.first = _("AN ERROR OCCURED") + std::string(": check the system/logs directory");
}

void GuiInstall::onInstallOk()
{
    mLoading = false;
    mState = 2;
}
