Mod for YGOMaster
- Master Duel - TCG Solo Campaign.

Motivation:
I started to play the TCG in its early days, spent most of my childhood with it, but also stopped even before synchro summons. When I recently started to play the video games, I had questions:
How did the gameplay evolve into this?
When were all the mechanisms and archetypes introduced?
How did it feel, when those things first happened: Why were some cards banned, when did some become obsolete?

So, I wanted to re-create an experience, as if child-me would naturally face the changing meta through the years.

Game Features:
- progressive campaign, where the levels are determined by the introduced core sets of the TCG
- campaign is divided in 6 parts by era: DM, GX, 5D, Zexal, Arc-V, VRAINS
- 75 progression levels in total from Legend of Blue Eyes up to Eternity Code
- each level is represented by 6 different opponents to battle against: 75*6 = 450 campaign duels
- each level has a matching ban list of that time, so you can build your deck accordingly
- the chapter description will tell you, which banlist you should apply
- cards not available at that time are automatically excluded by the ban list
- additional sets, structure decks etc, that were available inbetween the core sets can be unlocked as duel rewards
- that makes around 300 packs available in the shop, each with custom pack art
- additional post game: extensive post game campaign, that brings 228 decks into modern meta

Installation:
- unzip in your "Yu-Gi-Oh! Master Duel" folder and run "YgoMasterClient.exe"
- needs to look like this or similar "C:\Program Files (x86)\Steam\steamapps\common\Yu-Gi-Oh!  Master Duel\YgoMaster_1.60 _TCG_20250614\YgoMasterClient.exe"

![image](https://github.com/user-attachments/assets/a4714a24-af25-4f38-be21-9f951a8e8127)

![1](https://github.com/user-attachments/assets/549a16f0-6708-4250-b5e6-32dd42659315)
![2](https://github.com/user-attachments/assets/cbc5b7c6-191e-4aef-946e-3491e3ee6673)
![3](https://github.com/user-attachments/assets/a97af667-b98d-4e98-be20-dff6d363cbe6)


# YgoMaster

Offline Yu-Gi-Oh! Master Duel (PC)

*Progress is not shared with the live game.*

## Features

- Create decks
- Open packs
- Solo content
- Custom CPU duels
- [PvP duels / friends / trading](Docs/PvP.md)
- Duel replays
- YDK / YDKe support
- Card collection stats / deck editor sub menu improvements

## Requirements

- .NET Framework 4.0 (or above)
- The game downloaded on Steam (complete the in-game tutorial to download all data)

YgoMaster is portable and can be used on any machine without Steam installed after being fully downloaded

## Usage

- Download the latest release from https://github.com/pixeltris/YgoMaster/releases
- Copy the `YgoMaster` folder (the folder, not the contents of the folder) into the game folder
- Run `YgoMasterClient.exe` (this should also auto run `YgoMaster.exe`)
- *[If you see file load error popups, infinite loading screens, corrupt screens, etc follow these instructions](Docs/FileLoadError.md)*

Additionally...

- [It's recommended that you tailor the server settings to your preferences](Docs/Settings.md)
- Download [VG.TCG.Decks.7z](https://github.com/pixeltris/YgoMaster/releases/download/v1.4/VG.TCG.Decks.7z) for ~6000 decks from the YGO video games
- The custom duel starter UI can be accessed using the DUEL button on the home screen
- When updating copy your `/YgoMaster/Data/Players/` folder
- [How to change language](Docs/ChangingLanguage.md)
- [How to run on Linux](Docs/Linux.md)

## Compiling from source

- Install Visual Studio with C++ compilers
- Run `Build.bat`
- Copy the `YgoMaster` folder into the game folder as mentioned above

Running `Build.bat` is the equivilant of:

- Compiling `YgoMaster.sln` with Visual Studio
- Compiling `YgoMasterLoader.cpp` with `cl`

## Related

- https://www.nexusmods.com/yugiohmasterduel/mods - community mods
- https://www.nexusmods.com/yugiohmasterduel/articles/3 - modding guide
- https://github.com/SethPDA/MasterDuel-Modding/wiki - modding guide
- https://github.com/crazydoomy/MD-Replay-Editor - save / load replays
- https://code.mycard.moe/sherry_chaos/MDPro3 - forked YGOPro2 with Master Duel assets

## Screenshots

![Alt text](Docs/Pics/ss1.jpg)
![Alt text](Docs/Pics/ss2.jpg)
![Alt text](Docs/Pics/ss3.jpg)
![Alt text](Docs/Pics/ss4.jpg)
![Alt text](Docs/Pics/ss5.jpg)
![Alt text](Docs/Pics/ss6.jpg)
![Alt text](Docs/Pics/ss7.jpg)
