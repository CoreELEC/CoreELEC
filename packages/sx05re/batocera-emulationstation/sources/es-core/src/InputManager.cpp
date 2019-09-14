#include "InputManager.h"

#include "utils/FileSystemUtil.h"
#include "CECInput.h"
#include "Log.h"
#include "platform.h"
#include "Scripting.h"
#include "Window.h"
#include <pugixml/src/pugixml.hpp>
#include <SDL.h>
#include <iostream>
#include <assert.h>
#include "Settings.h"

#define KEYBOARD_GUID_STRING "-1"
#define CEC_GUID_STRING      "-2"

// SO HEY POTENTIAL POOR SAP WHO IS TRYING TO MAKE SENSE OF ALL THIS (by which I mean my future self)
// There are like four distinct IDs used for joysticks (crazy, right?)
// 1. Device index - this is the "lowest level" identifier, and is just the Nth joystick plugged in to the system (like /dev/js#).
//    It can change even if the device is the same, and is only used to open joysticks (required to receive SDL events).
// 2. SDL_JoystickID - this is an ID for each joystick that is supposed to remain consistent between plugging and unplugging.
//    ES doesn't care if it does, though.
// 3. "Device ID" - this is something I made up and is what InputConfig's getDeviceID() returns.  
//    This is actually just an SDL_JoystickID (also called instance ID), but -1 means "keyboard" instead of "error."
// 4. Joystick GUID - this is some squashed version of joystick vendor, version, and a bunch of other device-specific things.
//    It should remain the same across runs of the program/system restarts/device reordering and is what I use to identify which joystick to load.

// hack for cec support
int SDL_USER_CECBUTTONDOWN = -1;
int SDL_USER_CECBUTTONUP   = -1;

InputManager* InputManager::mInstance = NULL;

InputManager::InputManager() : mKeyboardInputConfig(NULL)
{
}

InputManager::~InputManager()
{
	deinit();
}

InputManager* InputManager::getInstance()
{
	if(!mInstance)
		mInstance = new InputManager();

	return mInstance;
}

void InputManager::init()
{
	if(initialized())
		deinit();

	SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, 
		Settings::getInstance()->getBool("BackgroundJoystickInput") ? "1" : "0");
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	SDL_JoystickEventState(SDL_ENABLE);

        // batocera
	// first, open all currently present joysticks
        this->addAllJoysticks();
	computeLastKnownPlayersDeviceIndexes();

	mKeyboardInputConfig = new InputConfig(DEVICE_KEYBOARD, -1, "Keyboard", KEYBOARD_GUID_STRING, 0); // batocera
	loadInputConfig(mKeyboardInputConfig);

	SDL_USER_CECBUTTONDOWN = SDL_RegisterEvents(2);
	SDL_USER_CECBUTTONUP   = SDL_USER_CECBUTTONDOWN + 1;
	CECInput::init();
	mCECInputConfig = new InputConfig(DEVICE_CEC, -1, "CEC", CEC_GUID_STRING, 0); // batocera
	loadInputConfig(mCECInputConfig);
}

void InputManager::addJoystickByDeviceIndex(int id)
{
	assert(id >= 0 && id < SDL_NumJoysticks());
	
	// open joystick & add to our list
	SDL_Joystick* joy = SDL_JoystickOpen(id);
	assert(joy);

	// add it to our list so we can close it again later
	SDL_JoystickID joyId = SDL_JoystickInstanceID(joy);
	mJoysticks[joyId] = joy;

	char guid[65];
	SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(joy), guid, 65);

	// create the InputConfig
	mInputConfigs[joyId] = new InputConfig(joyId, id, SDL_JoystickName(joy), guid, SDL_JoystickNumAxes(joy)); // batocera
	if(!loadInputConfig(mInputConfigs[joyId]))
	{
		LOG(LogInfo) << "Added unconfigured joystick " << SDL_JoystickName(joy) << " (GUID: " << guid << ", instance ID: " << joyId << ", device index: " << id << ").";
	}else{
		LOG(LogInfo) << "Added known joystick " << SDL_JoystickName(joy) << " (instance ID: " << joyId << ", device index: " << id << ")";
	}

	// set up the prevAxisValues
	int numAxes = SDL_JoystickNumAxes(joy);
	mPrevAxisValues[joyId] = new int[numAxes];
	std::fill(mPrevAxisValues[joyId], mPrevAxisValues[joyId] + numAxes, 0); //initialize array to 0
}

void InputManager::removeJoystickByJoystickID(SDL_JoystickID joyId)
{
	assert(joyId != -1);

	// delete old prevAxisValues
	auto axisIt = mPrevAxisValues.find(joyId);
	delete[] axisIt->second;
	mPrevAxisValues.erase(axisIt);

	// delete old InputConfig
	auto it = mInputConfigs.find(joyId);
	delete it->second;
	mInputConfigs.erase(it);

	// close the joystick
	auto joyIt = mJoysticks.find(joyId);
	if(joyIt != mJoysticks.cend())
	{
		SDL_JoystickClose(joyIt->second);
		mJoysticks.erase(joyIt);
	}else{
		LOG(LogError) << "Could not find joystick to close (instance ID: " << joyId << ")";
	}
}

// batocera
void InputManager::addAllJoysticks()
{
    clearJoystick();
    int numJoysticks = SDL_NumJoysticks();
    for(int i = 0; i < numJoysticks; i++)
    {
            addJoystickByDeviceIndex(i);
    }
}

// batocera
void InputManager::clearJoystick()
{
    for(auto iter = mJoysticks.begin(); iter != mJoysticks.end(); iter++)
	{
		SDL_JoystickClose(iter->second);
	}
	mJoysticks.clear();

	for(auto iter = mInputConfigs.begin(); iter != mInputConfigs.end(); iter++)
	{
		delete iter->second;
	}
	mInputConfigs.clear();

	for(auto iter = mPrevAxisValues.begin(); iter != mPrevAxisValues.end(); iter++)
	{
		delete[] iter->second;
	}
	mPrevAxisValues.clear();

}

void InputManager::deinit()
{
	if(!initialized())
		return;

	for(auto iter = mJoysticks.cbegin(); iter != mJoysticks.cend(); iter++)
	{
		SDL_JoystickClose(iter->second);
	}
	mJoysticks.clear();

	for(auto iter = mInputConfigs.cbegin(); iter != mInputConfigs.cend(); iter++)
	{
		delete iter->second;
	}
	mInputConfigs.clear();

	for(auto iter = mPrevAxisValues.cbegin(); iter != mPrevAxisValues.cend(); iter++)
	{
		delete[] iter->second;
	}
	mPrevAxisValues.clear();

	if(mKeyboardInputConfig != NULL)
	{
		delete mKeyboardInputConfig;
		mKeyboardInputConfig = NULL;
	}

	if(mCECInputConfig != NULL)
	{
		delete mCECInputConfig;
		mCECInputConfig = NULL;
	}

	CECInput::deinit();

	SDL_JoystickEventState(SDL_DISABLE);
	SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}

int InputManager::getNumJoysticks() { return (int)mJoysticks.size(); }

int InputManager::getAxisCountByDevice(SDL_JoystickID id)
{
	return SDL_JoystickNumAxes(mJoysticks[id]);
}

int InputManager::getButtonCountByDevice(SDL_JoystickID id)
{
	if(id == DEVICE_KEYBOARD)
		return 120; //it's a lot, okay.
	else if(id == DEVICE_CEC)
#ifdef HAVE_CECLIB
		return CEC::CEC_USER_CONTROL_CODE_MAX;
#else // HAVE_LIBCEF
		return 0;
#endif // HAVE_CECLIB
	else
		return SDL_JoystickNumButtons(mJoysticks[id]);
}

InputConfig* InputManager::getInputConfigByDevice(int device)
{
	if(device == DEVICE_KEYBOARD)
		return mKeyboardInputConfig;
	else if(device == DEVICE_CEC)
		return mCECInputConfig;
	else
		return mInputConfigs[device];
}

bool InputManager::parseEvent(const SDL_Event& ev, Window* window)
{
	bool causedEvent = false;
	switch(ev.type)
	{
	case SDL_JOYAXISMOTION:
	  {
            // batocera
	    // some axes are "full" : from -32000 to +32000
	    // in this case, their unpressed state is not 0
	    // SDL provides a function to get this value
	    // in es, the trick is to minus this value to the value to do as if it started at 0
	    int initialValue = 0;
	    Sint16 x;
	    // SDL_JoystickGetAxisInitialState doesn't work with 8bitdo start+b
	    //if(SDL_JoystickGetAxisInitialState(mJoysticks[ev.jaxis.which], ev.jaxis.axis, &x)) {
	    //  initialValue = x;
	    //}

		//if it switched boundaries
		if((abs(ev.jaxis.value-initialValue) > DEADZONE) != (abs(mPrevAxisValues[ev.jaxis.which][ev.jaxis.axis]) > DEADZONE)) // batocera
		{
			int normValue;
			if(abs(ev.jaxis.value-initialValue) <= DEADZONE) // batocera
				normValue = 0;
			else
				if(ev.jaxis.value-initialValue > 0) // batocera
					normValue = 1;
				else
					normValue = -1;

			window->input(getInputConfigByDevice(ev.jaxis.which), Input(ev.jaxis.which, TYPE_AXIS, ev.jaxis.axis, normValue, false));
			causedEvent = true;
		}

		mPrevAxisValues[ev.jaxis.which][ev.jaxis.axis] = ev.jaxis.value-initialValue; // batocera
		return causedEvent;
	  }
	case SDL_JOYBUTTONDOWN:
	case SDL_JOYBUTTONUP:
		window->input(getInputConfigByDevice(ev.jbutton.which), Input(ev.jbutton.which, TYPE_BUTTON, ev.jbutton.button, ev.jbutton.state == SDL_PRESSED, false));
		return true;

	case SDL_JOYHATMOTION:
		window->input(getInputConfigByDevice(ev.jhat.which), Input(ev.jhat.which, TYPE_HAT, ev.jhat.hat, ev.jhat.value, false));
		return true;

	case SDL_KEYDOWN:
		if(ev.key.keysym.sym == SDLK_BACKSPACE && SDL_IsTextInputActive())
		{
			window->textInput("\b");
		}

		if(ev.key.repeat)
			return false;

                // batocera
		//if(ev.key.keysym.sym == SDLK_F4)
		//{
		//	SDL_Event* quit = new SDL_Event();
		//	quit->type = SDL_QUIT;
		//	SDL_PushEvent(quit);
		//	return false;
		//}

		window->input(getInputConfigByDevice(DEVICE_KEYBOARD), Input(DEVICE_KEYBOARD, TYPE_KEY, ev.key.keysym.sym, 1, false));
		return true;

	case SDL_KEYUP:
		window->input(getInputConfigByDevice(DEVICE_KEYBOARD), Input(DEVICE_KEYBOARD, TYPE_KEY, ev.key.keysym.sym, 0, false));
		return true;

	case SDL_TEXTINPUT:
		window->textInput(ev.text.text);
		break;

	case SDL_JOYDEVICEADDED:
		addJoystickByDeviceIndex(ev.jdevice.which); // ev.jdevice.which is a device index
		computeLastKnownPlayersDeviceIndexes(); // batocera
		return true;

	case SDL_JOYDEVICEREMOVED:
		removeJoystickByJoystickID(ev.jdevice.which); // ev.jdevice.which is an SDL_JoystickID (instance ID)
		computeLastKnownPlayersDeviceIndexes(); // batocera
		return false;
	}

	if((ev.type == (unsigned int)SDL_USER_CECBUTTONDOWN) || (ev.type == (unsigned int)SDL_USER_CECBUTTONUP))
	{
		window->input(getInputConfigByDevice(DEVICE_CEC), Input(DEVICE_CEC, TYPE_CEC_BUTTON, ev.user.code, ev.type == (unsigned int)SDL_USER_CECBUTTONDOWN, false));
		return true;
	}

	return false;
}

bool InputManager::loadInputConfig(InputConfig* config)
{
	std::string path = getConfigPath();
	if(!Utils::FileSystem::exists(path))
		return false;
	
	pugi::xml_document doc;
	pugi::xml_parse_result res = doc.load_file(path.c_str());

	if(!res)
	{
		LOG(LogError) << "Error parsing input config: " << res.description();
		return false;
	}

	pugi::xml_node root = doc.child("inputList");
	if(!root)
		return false;

        // batocera
	// looking for a device having the same guid and name, or if not, one with the same guid or in last chance, one with the same name
	pugi::xml_node configNode(NULL);

	bool found_guid = false;
	bool found_exact = false;
	for (pugi::xml_node item = root.child("inputConfig"); item; item = item.next_sibling("inputConfig")) {
	  // check the guid
	  if(strcmp(config->getDeviceGUIDString().c_str(), item.attribute("deviceGUID").value()) == 0) {
	    // found a correct guid
	    found_guid = true; // no more need to check the name only
	    configNode = item;
	    
	    if(strcmp(config->getDeviceName().c_str(), item.attribute("deviceName").value()) == 0) {
	      // found the exact device
	      found_exact = true;
	      configNode = item;
	      break;
	    }
	  }

	  // check for a name if no guid is found
	  if(found_guid == false) {
	    if(strcmp(config->getDeviceName().c_str(), item.attribute("deviceName").value()) == 0) {
	      configNode = item;
	    }
	  }
	}
	    
	if(!configNode)
		return false;

        // batocera
	if(found_exact == false) {
	  LOG(LogInfo) << "Approximative device found using guid=" << configNode.attribute("deviceGUID").value() << " name=" << configNode.attribute("deviceName").value() << ")";
	}
	  
	config->loadFromXML(configNode);
	return true;
}

//used in an "emergency" where no keyboard config could be loaded from the inputmanager config file
//allows the user to select to reconfigure in menus if this happens without having to delete es_input.cfg manually
void InputManager::loadDefaultKBConfig()
{
	InputConfig* cfg = getInputConfigByDevice(DEVICE_KEYBOARD);

	cfg->clear();
	cfg->mapInput("up", Input(DEVICE_KEYBOARD, TYPE_KEY, SDLK_UP, 1, true));
	cfg->mapInput("down", Input(DEVICE_KEYBOARD, TYPE_KEY, SDLK_DOWN, 1, true));
	cfg->mapInput("left", Input(DEVICE_KEYBOARD, TYPE_KEY, SDLK_LEFT, 1, true));
	cfg->mapInput("right", Input(DEVICE_KEYBOARD, TYPE_KEY, SDLK_RIGHT, 1, true));

	cfg->mapInput(BUTTON_OK, Input(DEVICE_KEYBOARD, TYPE_KEY, SDLK_RETURN, 1, true));
	cfg->mapInput(BUTTON_BACK, Input(DEVICE_KEYBOARD, TYPE_KEY, SDLK_ESCAPE, 1, true));
	cfg->mapInput("start", Input(DEVICE_KEYBOARD, TYPE_KEY, SDLK_F1, 1, true));
	cfg->mapInput("select", Input(DEVICE_KEYBOARD, TYPE_KEY, SDLK_F2, 1, true));

	cfg->mapInput("pageup", Input(DEVICE_KEYBOARD, TYPE_KEY, SDLK_RIGHTBRACKET, 1, true));
	cfg->mapInput("pagedown", Input(DEVICE_KEYBOARD, TYPE_KEY, SDLK_LEFTBRACKET, 1, true));
}

void InputManager::writeDeviceConfig(InputConfig* config)
{
	assert(initialized());

	std::string path = getConfigPath();

	pugi::xml_document doc;

	if(Utils::FileSystem::exists(path))
	{
		// merge files
		pugi::xml_parse_result result = doc.load_file(path.c_str());
		if(!result)
		{
			LOG(LogError) << "Error parsing input config: " << result.description();
		}
		else
		{
			// successfully loaded, delete the old entry if it exists
			pugi::xml_node root = doc.child("inputList");
			if(root)
			{
			  // batocera
			  pugi::xml_node oldEntry(NULL);
			  for (pugi::xml_node item = root.child("inputConfig"); item; item = item.next_sibling("inputConfig")) {
			    if(strcmp(config->getDeviceGUIDString().c_str(), item.attribute("deviceGUID").value()) == 0 &&
			       strcmp(config->getDeviceName().c_str(),       item.attribute("deviceName").value()) == 0) {
			      oldEntry = item;
			      break;
			    }
			  }
			  if(oldEntry)
			    root.remove_child(oldEntry);
			}
		}
	}

	pugi::xml_node root = doc.child("inputList");
	if(!root)
		root = doc.append_child("inputList");

	config->writeToXML(root);
	doc.save_file(path.c_str());

        // batocera
	/* create a es_last_input.cfg so that people can easily share their config */
	pugi::xml_document lastdoc;
	pugi::xml_node lastroot = lastdoc.append_child("inputList");
	config->writeToXML(lastroot);
	std::string lastpath = getTemporaryConfigPath();
	lastdoc.save_file(lastpath.c_str());

	Scripting::fireEvent("config-changed");
	Scripting::fireEvent("controls-changed");
	
	// execute any onFinish commands and re-load the config for changes
	doOnFinish();
	loadInputConfig(config);
}

void InputManager::doOnFinish()
{
	assert(initialized());
	std::string path = getConfigPath();
	pugi::xml_document doc;

	if(Utils::FileSystem::exists(path))
	{
		pugi::xml_parse_result result = doc.load_file(path.c_str());
		if(!result)
		{
			LOG(LogError) << "Error parsing input config: " << result.description();
		}
		else
		{
			pugi::xml_node root = doc.child("inputList");
			if(root)
			{
				root = root.find_child_by_attribute("inputAction", "type", "onfinish");
				if(root)
				{
					for(pugi::xml_node command = root.child("command"); command;
							command = command.next_sibling("command"))
					{
						std::string tocall = command.text().get();

						LOG(LogInfo) << "	" << tocall;
						std::cout << "==============================================\ninput config finish command:\n";
						int exitCode = runSystemCommand(tocall);
						std::cout << "==============================================\n";

						if(exitCode != 0)
						{
							LOG(LogWarning) << "...launch terminated with nonzero exit code " << exitCode << "!";
						}
					}
				}
			}
		}
	}
}

std::string InputManager::getConfigPath()
{
	return Utils::FileSystem::getEsConfigPath() + "/es_input.cfg";
}

std::string InputManager::getTemporaryConfigPath()
{
	return Utils::FileSystem::getEsConfigPath() + "/es_temporaryinput.cfg";
}

bool InputManager::initialized() const
{
	return mKeyboardInputConfig != NULL;
}

int InputManager::getNumConfiguredDevices()
{
	int num = 0;
	for(auto it = mInputConfigs.cbegin(); it != mInputConfigs.cend(); it++)
	{
		if(it->second->isConfigured())
			num++;
	}

	if(mKeyboardInputConfig->isConfigured())
		num++;

	if(mCECInputConfig->isConfigured())
		num++;

	return num;
}

std::string InputManager::getDeviceGUIDString(int deviceId)
{
	if(deviceId == DEVICE_KEYBOARD)
		return KEYBOARD_GUID_STRING;

	if(deviceId == DEVICE_CEC)
		return CEC_GUID_STRING;

	auto it = mJoysticks.find(deviceId);
	if(it == mJoysticks.cend())
	{
		LOG(LogError) << "getDeviceGUIDString - deviceId " << deviceId << " not found!";
		return "something went horribly wrong";
	}

	char guid[65];
	SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(it->second), guid, 65);
	return std::string(guid);
}

// batocera
void InputManager::computeLastKnownPlayersDeviceIndexes() {
  std::map<int, InputConfig*> playerJoysticks = computePlayersConfigs();

  m_lastKnownPlayersDeviceIndexes.clear();
  for (int player = 0; player < MAX_PLAYERS; player++) {
    if(playerJoysticks[player] != NULL){
      m_lastKnownPlayersDeviceIndexes[player] = playerJoysticks[player]->getDeviceIndex();
    }
  }
}

// batocera
std::map<int, int> InputManager::lastKnownPlayersDeviceIndexes() {
  return m_lastKnownPlayersDeviceIndexes;
}

// batocera
std::map<int, InputConfig*> InputManager::computePlayersConfigs() {
    // 1 recuperer les configurated
    std::list<InputConfig *> availableConfigured;

    for(auto iter = mJoysticks.begin(); iter != mJoysticks.end(); iter++) {
        InputConfig * config = getInputConfigByDevice(iter->first);
        if(config != NULL && config->isConfigured()) {
            availableConfigured.push_back(config);
        }
    }

    //2 pour chaque joueur verifier si il y a un configurated
    // associer le input au joueur
    // enlever des disponibles
    std::map<int, InputConfig*> playerJoysticks;

	// First loop, search for GUID + NAME. High Priority
    for (int player = 0; player < MAX_PLAYERS; player++) {
        std::stringstream sstm;
        sstm << "INPUT P" << player+1;
		std::string confName = sstm.str()+"NAME";
		std::string confGuid = sstm.str()+"GUID";

		std::string playerConfigName = Settings::getInstance()->getString(confName);
		std::string playerConfigGuid = Settings::getInstance()->getString(confGuid);

        for (std::list<InputConfig *>::iterator it1=availableConfigured.begin(); it1!=availableConfigured.end(); ++it1)
        {
            InputConfig * config = *it1;
			bool nameFound = playerConfigName.compare(config->getDeviceName()) == 0;
			bool guidfound = playerConfigGuid.compare(config->getDeviceGUIDString()) == 0;

			if(nameFound && guidfound) {
					availableConfigured.erase(it1);
					playerJoysticks[player] = config;
					break;
			}
        }
    }
	// Second loop, search for NAME. Low Priority
	for (int player = 0; player < MAX_PLAYERS; player++) {
		std::stringstream sstm;
		sstm << "INPUT P" << player+1;
		std::string confName = sstm.str()+"NAME";

		std::string playerConfigName = Settings::getInstance()->getString(confName);

		for (std::list<InputConfig *>::iterator it1=availableConfigured.begin(); it1!=availableConfigured.end(); ++it1)
		{
			InputConfig * config = *it1;
			bool nameFound = playerConfigName.compare(config->getDeviceName()) == 0;
			if(nameFound) {
					availableConfigured.erase(it1);
					playerJoysticks[player] = config;
					break;
			}
		}
	}

    // Last loop, search for free controllers for remaining players.
    for (int player = 0; player < MAX_PLAYERS; player++) {
        // si aucune config a été trouvé pour le joueur, on essaie de lui filer un libre
        if(playerJoysticks[player] == NULL){
            for (std::list<InputConfig *>::iterator it1=availableConfigured.begin(); it1!=availableConfigured.end(); ++it1)
            {
                playerJoysticks[player] = *it1;
                availableConfigured.erase(it1);
                break;
            }
        }
    }

    // in case of hole (player 1 missing, but player 4 set, fill the holes with last players joysticks)
    for (int player = 0; player < MAX_PLAYERS; player++) {
      if(playerJoysticks[player] == NULL){
    	for (int repplayer = MAX_PLAYERS; repplayer > player; repplayer--) {
    	  if(playerJoysticks[player] == NULL && playerJoysticks[repplayer] != NULL){
    	    playerJoysticks[player]    = playerJoysticks[repplayer];
	    playerJoysticks[repplayer] = NULL;
    	  }
    	}
      }
    }

    return playerJoysticks;
}

// batocera
std::string InputManager::configureEmulators() {
  std::map<int, InputConfig*> playerJoysticks = computePlayersConfigs();
  std::stringstream command;

  for (int player = 0; player < MAX_PLAYERS; player++) {
    InputConfig * playerInputConfig = playerJoysticks[player];
    if(playerInputConfig != NULL){
      command << "-p" << player+1 << "index " <<  playerInputConfig->getDeviceIndex() << " -p" << player+1 << "guid " << playerInputConfig->getDeviceGUIDString() << " -p" << player+1 << "name \"" <<  playerInputConfig->getDeviceName() << "\" -p" << player+1 << "nbaxes " << playerInputConfig->getDeviceNbAxes() << " ";
    }
  }
  LOG(LogInfo) << "Configure emulators command : " << command.str().c_str();
  return command.str();
}
