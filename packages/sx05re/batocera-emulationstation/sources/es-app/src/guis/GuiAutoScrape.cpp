#include "guis/GuiAutoScrape.h"
#include "guis/GuiMsgBox.h"

#include "views/ViewController.h"
#include "Gamelist.h"
#include "SystemData.h"

#include "Window.h"
#include <string>
#include "Log.h"
#include "Settings.h"
#include "ApiSystem.h"
#include "LocaleES.h"

GuiAutoScrape::GuiAutoScrape(Window* window) : GuiComponent(window), mBusyAnim(window)
{
	setSize((float)Renderer::getScreenWidth(), (float)Renderer::getScreenHeight());
        mLoading = true;
	mState = 1;
        mBusyAnim.setSize(mSize);
}

GuiAutoScrape::~GuiAutoScrape()
{
	// view type probably changed (basic -> detailed)
	ViewController::get()->reloadAll(mWindow);
	mWindow->endRenderLoadingScreen();
}

bool GuiAutoScrape::input(InputConfig* config, Input input)
{
        return false;
}

std::vector<HelpPrompt> GuiAutoScrape::getHelpPrompts()
{
	return std::vector<HelpPrompt>();
}

void GuiAutoScrape::render(const Transform4x4f& parentTrans)
{
        Transform4x4f trans = parentTrans * getTransform();

        renderChildren(trans);

        Renderer::setMatrix(trans);
        Renderer::drawRect(0.f, 0.f, mSize.x(), mSize.y(), 0x00000011);

        if(mLoading)
        mBusyAnim.render(trans);

}

void GuiAutoScrape::update(int deltaTime) {
        GuiComponent::update(deltaTime);
        mBusyAnim.update(deltaTime);
        
        Window* window = mWindow;

        if(mState == 1){
	  mLoading = true;
	  mHandle = new std::thread(&GuiAutoScrape::threadAutoScrape, this);
	  mState = 0;
        }
	
        if(mState == 4){
            window->pushGui(
			    new GuiMsgBox(window, _("FINISHED"), _("OK"),
                [this] {
					    mState = -1;
                })
            );
            mState = 0;
        }
	
        if(mState == 5){
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

void GuiAutoScrape::threadAutoScrape() 
{
  std::pair<std::string,int> scrapeStatus = ApiSystem::getInstance()->scrape(&mBusyAnim);
  if(scrapeStatus.second == 0){
    this->onAutoScrapeOk();
  }else {
    this->onAutoScrapeError(scrapeStatus);
  }
}

void GuiAutoScrape::onAutoScrapeError(std::pair<std::string, int> result)
{
    mLoading = false;
    mState = 5;
    mResult = result;
    mResult.first = _("AN ERROR OCCURED") + std::string(": ") + mResult.first;
}

void GuiAutoScrape::onAutoScrapeOk()
{
    mLoading = false;
    mState = 4;
}
