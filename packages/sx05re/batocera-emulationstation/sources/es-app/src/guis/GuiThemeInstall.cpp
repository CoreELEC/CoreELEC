#include "guis/GuiBackup.h"
#include "guis/GuiMsgBox.h"
#include "Window.h"
#include <string>
#include "Log.h"
#include "Settings.h"
#include "ApiSystem.h"
#include "LocaleES.h"
#include "GuiThemeInstall.h"
#include "views/ViewController.h"

GuiThemeInstall::GuiThemeInstall(Window* window, const char *theme) : GuiComponent(window), mBusyAnim(window)
{
	setSize((float)Renderer::getScreenWidth(), (float)Renderer::getScreenHeight());
        mLoading = true;
	mState = 1;
        mBusyAnim.setSize(mSize);
	mThemeName = theme;
}

GuiThemeInstall::~GuiThemeInstall()
{
}

bool GuiThemeInstall::input(InputConfig* config, Input input)
{
        return false;
}

std::vector<HelpPrompt> GuiThemeInstall::getHelpPrompts()
{
	return std::vector<HelpPrompt>();
}

void GuiThemeInstall::render(const Transform4x4f& parentTrans)
{
        Transform4x4f trans = parentTrans * getTransform();

        renderChildren(trans);

        Renderer::setMatrix(trans);
        Renderer::drawRect(0.f, 0.f, mSize.x(), mSize.y(), 0x00000011);

        if(mLoading)
        mBusyAnim.render(trans);

}

void GuiThemeInstall::update(int deltaTime) {
        GuiComponent::update(deltaTime);
        mBusyAnim.update(deltaTime);
       
        Window* window = mWindow;
        if(mState == 1){
	  mLoading = true;
	  mHandle = new std::thread(&GuiThemeInstall::threadTheme, this);
	  mState = 0;
        }

        if(mState == 2){
	  window->pushGui(
			  new GuiMsgBox(window, _("FINISHED") + "\n" + _("THEME INSTALLED SUCCESSFULLY"), _("OK"),
					[this] {
					  mState = -1;
					}
					)
			  );
	  // In case you have no active theme
	  ViewController::get()->reloadAll();
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

void GuiThemeInstall::threadTheme() 
{
    // Batocera script will be invoked there 
    std::pair<std::string,int> updateStatus = ApiSystem::getInstance()->installBatoceraTheme(&mBusyAnim, mThemeName);
    if(updateStatus.second == 0){
        this->onInstallOk();
    }else {
        this->onInstallError(updateStatus);
    }  
}

void GuiThemeInstall::onInstallError(std::pair<std::string, int> result)
{
    mLoading = false;
    mState = 3;
    mResult = result;
    mResult.first = _("AN ERROR OCCURED");
}

void GuiThemeInstall::onInstallOk()
{
    mLoading = false;
    mState = 2;
}
