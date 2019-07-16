# EmuELEC  
Retro emulation for Amlogic devices.  
Based on  [CoreELEC](https://github.com/CoreELEC/CoreELEC) and [Lakka](https://github.com/libretro/Lakka-LibreELEC), I just combine them with [Emulationstation](https://github.com/RetroPie/EmulationStation) and some standalone emulators ([Advancemame](https://github.com/amadvance/advancemame), [PPSSPP](https://github.com/hrydgard/ppsspp), [Reicast](https://github.com/reicast/reicast-emulator), [Amiberry](https://github.com/midwan/amiberry) and others). 

To build use:  

git clone https://github.com/shantigilbert/EmuELEC.git EmuELEC    
cd EmuELEC  
git checkout EmuELEC  
PROJECT=Amlogic ARCH=arm DISTRO=EmuELEC make image   

For the Odroid N2:   
PROJECT=Amlogic-ng ARCH=arm DISTRO=EmuELEC make image   

if you want to build the addon: 
cd EmuELEC
./emuelec-addon.sh

resulting zip files will be inside EmuELEC/repo

Remember to use the proper DTB for your device!

Need help? have suggestions? join us on our EmuELEC Discord: https://discord.gg/QqGYBzG

**EmuELEC DOES NOT INCLUDE KODI**

Please note, this is mostly a personal project made for my S905 box I can't guarantee that it will work for your box, I've spent many hours trying to optimize and make sure everything works, but I can't test everything and things might just not work at all, also keep in mind the limitations of the hardware and don't expect everything to run at 60FPS (specially N64, PSP and Reicast) I can't guarantee changes to fit your personal needs, but I do appreciate any PRs, help on testing other boxes and fixing issues in general.  
I work on this project on my personal time, I don't make any money out of it, so it takes a while for me to properly test any changes, but I will do my best to help you fix issues you might have on other boxes limited to my time and experience. 


Happy retrogaming! 

# CoreELEC

CoreELEC is a 'Just enough OS' Linux distribution for running the award-winning [Kodi](https://kodi.tv) software on popular low-cost hardware. CoreELEC is a minor fork of [LibreELEC](https://libreelec.tv), it's built by the community for the community. [CoreELEC website](http://coreelec.org).

**Issues & Support**

Please report issues via the CoreELEC [Forum](https://discourse.coreelec.org).

**Donations**

At this moment we do not accept Donations. We are doing this for fun not for profit.

**License**

CoreELEC original code is released under [GPLv2](https://www.gnu.org/licenses/gpl-2.0.html).

**Copyright**

As CoreELEC includes code from many upstream projects it includes many copyright owners. CoreELEC makes NO claim of copyright on any upstream code. Patches to upstream code have the same license as the upstream project, unless specified otherwise. For a complete copyright list please checkout the source code to examine license headers. Unless expressly stated otherwise all code submitted to the CoreELEC project (in any form) is licensed under [GPLv2](https://www.gnu.org/licenses/gpl-2.0.html). You are absolutely free to retain copyright. To retain copyright simply add a copyright header to each submitted code page. If you submit code that is not your own work it is your responsibility to place a header stating the copyright.
