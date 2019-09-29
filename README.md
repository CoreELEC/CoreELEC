# EmuELEC  
Retro emulation for Amlogic devices.  
Based on  [CoreELEC](https://github.com/CoreELEC/CoreELEC) and [Lakka](https://github.com/libretro/Lakka-LibreELEC), I just combine them with [Emulationstation](https://github.com/RetroPie/EmulationStation) and some standalone emulators ([Advancemame](https://github.com/amadvance/advancemame), [PPSSPP](https://github.com/hrydgard/ppsspp), [Reicast](https://github.com/reicast/reicast-emulator), [Amiberry](https://github.com/midwan/amiberry) and others). 

To build use:  

```
sudo apt update && sudo apt upgrade
sudo apt-get install gcc make git unzip wget xz-utils libsdl2-dev libsdl2-mixer-dev libfreeimage-dev libfreetype6-dev libcurl4-openssl-dev rapidjson-dev libasound2-dev libgl1-mesa-dev build-essential libboost-all-dev cmake fonts-droid-fallback libvlc-dev libvlccore-dev vlc-bin texinfo
git clone https://github.com/shantigilbert/EmuELEC.git EmuELEC    
cd EmuELEC  
git checkout EmuELEC  
PROJECT=Amlogic ARCH=arm DISTRO=EmuELEC make image   
```
For the Odroid N2:   
`PROJECT=Amlogic-ng ARCH=arm DISTRO=EmuELEC make image`

if you want to build the addon: 
```
cd EmuELEC
./emuelec-addon.sh
```
resulting zip files will be inside EmuELEC/repo

**Remember to use the proper DTB for your device!**

Need help? have suggestions? check out the Wiki at https://github.com/shantigilbert/EmuELEC/wiki or join us on our EmuELEC Discord: https://discord.gg/QqGYBzG

**EmuELEC DOES NOT INCLUDE KODI**

Please note, this is mostly a personal project made for my S905 box I can't guarantee that it will work for your box, I've spent many hours trying to optimize and make sure everything works, but I can't test everything and things might just not work at all, also keep in mind the limitations of the hardware and don't expect everything to run at 60FPS (specially N64, PSP and Reicast) I can't guarantee changes to fit your personal needs, but I do appreciate any PRs, help on testing other boxes and fixing issues in general.  
I work on this project on my personal time, I don't make any money out of it, so it takes a while for me to properly test any changes, but I will do my best to help you fix issues you might have on other boxes limited to my time and experience. 

Happy retrogaming! 
