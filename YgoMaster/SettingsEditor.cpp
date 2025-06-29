#include <windows.h>
#include <fstream>
#include <string>
#include <ctime>
#include "json.hpp" // https://github.com/nlohmann/json

using json = nlohmann::json;

// Parameters for settings
struct Param {
    const char* key;
    enum Type { BOOL, INT, STRING, DOUBLE } type;
    int dlgId;
};

// Difficulty presets
struct DifficultyPreset {
    const char* name;
    double multiplier;
    int minValue;
    int maxValue;
    int loseMin;
    int loseMax;
};
DifficultyPreset DIFFICULTY_PRESETS[] = {
    {"Easy", 1.5, 600, 800, 400, 600},
    {"Normal", 1.0, 400, 600, 200, 400},
    {"Hard", 1.0, 200, 400, 0, 200},
	{"Insane", 1.0, 0, 200, 0, 50},
    {"Roguelike", 1.0, 0, 0, 0, 0}
};
const int DIFFICULTY_COUNT = sizeof(DIFFICULTY_PRESETS) / sizeof(DIFFICULTY_PRESETS[0]);

// Character data for Random Character Challenge
struct Character {
    const char* name;
    const char* portraitPath; // .bmp file path!
    const char* description;
};
Character CHARACTERS[] = {
    {"Weevil Underwood", "Data/ClientData/Images/ChallengerPortrait/weevil.bmp", "Weevil's decks tend to prioritize summoning a large number of insect monsters, using them to overwhelm the opponent or create a defensive wall.\r\nWeevil's strategy often involves summoning powerful insect monsters, such as 'Insect Queen' and 'Perfectly Ultimate Great Moth', to deliver the final blow.\r\nHe also utilizes cards that can disrupt the opponent's strategy, such as 'DNA Surgery' to turn all monsters into insects and allow for specific interactions with his insect-based cards. "},
    {"Mai Valentine", "Data/ClientData/Images/ChallengerPortrait/mai.bmp", "Mai doesn't rush into duels. She assesses her opponents, plans her moves carefully, and often targets weaker opponents to build up her resources.\r\nHer deck heavily revolves around Harpie Lady and her sisters, utilizing their swarm potential and synergy with other cards like Cyber Shield.\r\n"},
	{"Bandit Keith", "Data/ClientData/Images/ChallengerPortrait/keith.bmp", "Keith's deck primarily utilizes Machine monsters, often of the DARK attribute.\r\nHe employs a variety of Spell and Trap cards to enhance his monsters' power and disrupt his opponents.\r\nSimilar to Joey, Keith tends to favor aggressive tactics, sometimes overlooking face-down Spell and Trap cards."},
	{"Joey Wheeler", "Data/ClientData/Images/ChallengerPortrait/joeywheeler.bmp", "Joey often relies on luck, using cards that can turn the tide of battle unexpectedly. His luck can lead to surprising victories.\r\nHe tends to adopt an aggressive approach, focusing on attacking directly and putting pressure on his opponents.\r\nJoey’s deck features iconic cards like 'Red-Eyes Black Dragon;, which reflects his fiery and determined nature."},
	{"Blue Angel", "Data/ClientData/Images/ChallengerPortrait/blueangel.bmp", "Her deck features powerful cards like 'Cynet Fusion' and 'Cynet Mining', reflecting her focus on technology and innovation.\r\nShe often uses cards that create illusions or manipulate her opponent's actions, making her dueling unpredictable.\r\nHer playstyle emphasizes quick, precise moves, allowing her to outmaneuver opponents and set up powerful combos."},
	{"Crow Hogan", "Data/ClientData/Images/ChallengerPortrait/crowhogan.bmp", "Crow primarily uses the Blackwing cards, which focus on swarming the field with powerful monsters that can attack swiftly and aggressively.\r\nHis style is characterized by fast-paced plays, often overwhelming opponents with quick attacks and a barrage of powerful creatures.\r\nCrow often employs tactics that involve summoning multiple monsters in a single turn, maximizing damage output and field control."},
	{"Ghost Gal", "Data/ClientData/Images/ChallengerPortrait/ghostgal.bmp", "Ghost Gal primarily uses Psychic-type monsters, focusing on their abilities to manipulate the game and control the duel.\r\nShe excels at controlling the field, using effects that allow her to influence her opponent's moves and strategies.\r\nGhost Gal often employs defensive tactics, setting up barriers and protective measures to thwart attacks while waiting for the right moment to strike."},
	{"Thelonious Viper", "Data/ClientData/Images/ChallengerPortrait/viper.bmp", "Viper’s deck heavily features reptilian and poison-themed cards, emphasizing damage over time and debilitating effects on opponents.\r\nHe often adopts a fast-paced, aggressive strategy, aiming to overwhelm opponents quickly with powerful attacks.\r\nHis playstyle often includes effects that deal direct damage, focusing on reducing opponents' life points rapidly."},
	{"Gong Strong", "Data/ClientData/Images/ChallengerPortrait/gong.bmp", "Gong primarily uses Warrior-type monsters, which often feature high attack power and strong battle effects, reflecting his focus on brute strength.\r\nGong excels in strategies that enhance his monsters' combat capabilities, often using cards that boost attack power or grant additional effects during battle.\r\nHe incorporates defensive elements into his strategy, ensuring he can withstand attacks while setting up his counter-offensive."},
	{"Bonz", "Data/ClientData/Images/ChallengerPortrait/bonz.bmp", "Bonz's decks revolve around Zombie monsters, utilizing their graveyard recursion and synergy.\r\nCards like 'Call of the Haunted' and later additions like 'Jack-o-Bolan' allow him to repeatedly bring back monsters from the graveyard, either as Zombies or temporarily.\r\nBonz's strategies often involve using the graveyard as a resource, bringing back monsters, banishing them, or using them for tribute fodder."},
	{"Ishizu Ishtar", "Data/ClientData/Images/ChallengerPortrait/ishizu.bmp", "Her deck often features cards with Ancient Egyptian lore, particularly those related to the Millennium Items, which enhance her strategic depth.\r\nIshizu is known for her ability to predict her opponent's moves, often using cards that allow her to manipulate the game state and control the flow of the duel.\r\nIshizu employs a balanced strategy that combines offensive plays with strong defensive tactics, allowing her to adapt to various situations."},
	{"Rex Raptor", "Data/ClientData/Images/ChallengerPortrait/rex.bmp", "Rex's core strategy involves summoning strong Dinosaur monsters with high attack points and attacking directly to reduce his opponent's life points.\r\nHe utilizes support cards to enhance his Dinosaurs' power and often relies on brute force to win duels.\r\nWhile he primarily uses Dinosaurs, his strongest monsters can include Dragon-types like Red-Eyes Black Dragon."},
	{"Mako Tsunami", "Data/ClientData/Images/ChallengerPortrait/mako.bmp", "Mako Tsunami's playing style in Yu-Gi-Oh! revolves around WATER-attribute monsters and the 'Umi' Field Spell.\r\nHe utilizes powerful WATER monsters, often Normal Monsters, and aims to control the field with Umi and its related cards, creating a beatdown strategy that leverages the environment to his advantage.\r\nMako's decks also incorporate cards for resource management, such as searching and recycling WATER monsters to maintain a consistent presence on the field."},
	{"Axel Brodie", "Data/ClientData/Images/ChallengerPortrait/brodie.bmp", "Axel's deck is built around dealing direct damage to the opponent, often using his 'Volcanic' monsters and spell/trap cards like 'Blaze Accelerator' to inflict burn damage. \r\nAxel frequently uses his 'Volcanic' monsters and spells to send cards to the graveyard, which then fuels the effects of other cards, creating a cycle of destruction and damage."},
	{"Chumley Huffington", "Data/ClientData/Images/ChallengerPortrait/chumley.bmp", "Chumley's deck revolves around Beast-type monsters, particularly those associated with Australia, like the Koala and Kangaroo cards.\r\nHis playstyle is generally depicted as lazy and unmotivated, but he also has a kind heart and cares for his friends."}
    // Add more characters as needed
};
const int CHARACTER_COUNT = sizeof(CHARACTERS) / sizeof(CHARACTERS[0]);

// Supported parameters
Param PARAMS[] = {
    {"UnlockAllCards", Param::BOOL, 1001},
    {"UnlockAllItems", Param::BOOL, 1002},
    {"CardCraftableAll", Param::BOOL, 1003},
    {"DisableNoDismantle", Param::BOOL, 1004},
    {"DefaultGems", Param::INT, 1005},
    {"DuelRewards.ChapterStatusChangedMultiplier", Param::DOUBLE, 1009},
    {"DuelRewards.win[4].min", Param::INT, 1010},
    {"DuelRewards.win[4].max", Param::INT, 1011},
    {"DuelRewards.lose[0].min", Param::INT, 1012},
    {"DuelRewards.lose[0].max", Param::INT, 1013}
};
const int PARAM_COUNT = sizeof(PARAMS) / sizeof(PARAMS[0]);

json settings;
std::string settingsPath = "Data/Settings.json";

// Helper: Get pointer to nested JSON field by path like "DuelRewards.win[4].max"
json* get_json_ptr_by_path(json& j, const std::string& path) {
    json* p = &j;
    size_t pos = 0, next;
    while ((next = path.find('.', pos)) != std::string::npos) {
        std::string key = path.substr(pos, next - pos);
        size_t arr_open = key.find('[');
        if (arr_open != std::string::npos) {
            std::string arr_key = key.substr(0, arr_open);
            int idx = std::stoi(key.substr(arr_open + 1, key.size() - arr_open - 2));
            p = &(*p)[arr_key][idx];
        } else {
            p = &(*p)[key];
        }
        pos = next + 1;
    }
    std::string key = path.substr(pos);
    size_t arr_open = key.find('[');
    if (arr_open != std::string::npos) {
        std::string arr_key = key.substr(0, arr_open);
        int idx = std::stoi(key.substr(arr_open + 1, key.size() - arr_open - 2));
        p = &(*p)[arr_key][idx];
    } else {
        p = &(*p)[key];
    }
    return p;
}

// Load/save functions (with error messages)
bool LoadSettings() {
    std::ifstream in(settingsPath);
    if (!in) {
        MessageBoxA(0, "Could not open Data/Settings.json", "Error", MB_ICONERROR);
        return false;
    }
    try {
        in >> settings;
        if (settings.is_null() || settings.empty()) {
            MessageBoxA(0, "Settings.json is empty or contains only null/empty object!", "Error", MB_ICONERROR);
            return false;
        }
    } catch (const std::exception& ex) {
        MessageBoxA(0, ex.what(), "Invalid JSON in Data/Settings.json", MB_ICONERROR);
        return false;
    }
    return true;
}
bool SaveSettings() {
    std::ofstream out(settingsPath);
    if (!out) return false;
    out << settings.dump(4);
    return true;
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    // For character portrait bitmap management
    static HBITMAP hPrevBmp = NULL;
    switch (msg) {
    case WM_INITDIALOG:
        // Seed random number generator
        srand((unsigned int)time(NULL));

        // Load all controls based on PARAMS table
        for (int i = 0; i < PARAM_COUNT; ++i) {
            const Param& p = PARAMS[i];
            json* ptr = get_json_ptr_by_path(settings, p.key);
            if (!ptr || ptr->is_null()) continue;
            switch (p.type) {
            case Param::BOOL:
                SendDlgItemMessageA(hDlg, p.dlgId, BM_SETCHECK,
                    ptr->get<bool>() ? BST_CHECKED : BST_UNCHECKED, 0);
                break;
            case Param::STRING:
                SetDlgItemTextA(hDlg, p.dlgId, ptr->get<std::string>().c_str());
                break;
            case Param::INT:
                SetDlgItemInt(hDlg, p.dlgId, ptr->get<int>(), TRUE);
                break;
            case Param::DOUBLE: {
                char buf[64];
                snprintf(buf, sizeof(buf), "%.6f", ptr->get<double>());
                SetDlgItemTextA(hDlg, p.dlgId, buf);
                break;
            }
            }
        }
        // Populate character combo box
        for (int i = 0; i < CHARACTER_COUNT; ++i) {
            SendDlgItemMessageA(hDlg, 1021, CB_ADDSTRING, 0, (LPARAM)CHARACTERS[i].name);
        }
        SendDlgItemMessageA(hDlg, 1021, CB_SETCURSEL, 0, 0); // Default to first character

        // Show initial portrait and description
        SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(1021, CBN_SELCHANGE), 0);

        // Populate difficulty combo box
        for (int i = 0; i < DIFFICULTY_COUNT; ++i) {
            SendDlgItemMessageA(hDlg, 1008, CB_ADDSTRING, 0, (LPARAM)DIFFICULTY_PRESETS[i].name);
        }
        SendDlgItemMessageA(hDlg, 1008, CB_SETCURSEL, 1, 0); // "Normal"
        return TRUE;

    case WM_COMMAND:
        // Character Combo Selection Changed
        if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == 1021) {
            int sel = (int)SendDlgItemMessageA(hDlg, 1021, CB_GETCURSEL, 0, 0);
            if (sel >= 0 && sel < CHARACTER_COUNT) {
                const auto& ch = CHARACTERS[sel];
                // Free previous bitmap
                if (hPrevBmp) {
                    DeleteObject(hPrevBmp);
                    hPrevBmp = NULL;
                }
                // Load and show portrait (BMP only!)
                hPrevBmp = (HBITMAP)LoadImageA(NULL, ch.portraitPath, IMAGE_BITMAP, 130, 130, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
                SendDlgItemMessageA(hDlg, 1022, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hPrevBmp);
                // Show description
                SetDlgItemTextA(hDlg, 1023, ch.description);
            }
            return TRUE;
        }
        switch (LOWORD(wParam)) {
        case 1020: // Random Character Challenge button
        {
            int randomIdx = rand() % CHARACTER_COUNT;
            SendDlgItemMessageA(hDlg, 1021, CB_SETCURSEL, randomIdx, 0);
            SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(1021, CBN_SELCHANGE), 0);
            break;
        }
        case 1014: // Reset Player Progress
        {
            char baseDir[MAX_PATH];
            GetCurrentDirectoryA(MAX_PATH, baseDir);
            std::string playerPath = std::string(baseDir) + "\\Players\\Local\\Player.json";

            WIN32_FILE_ATTRIBUTE_DATA fad;
            if (GetFileAttributesExA(playerPath.c_str(), GetFileExInfoStandard, &fad)) {
                // File exists
                SYSTEMTIME st;
                GetLocalTime(&st);
                char dateStr[32];
                sprintf_s(dateStr, "%04d%02d%02d_%02d%02d%02d",
                    st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

                std::string backupPath = std::string(baseDir) + "\\Players\\Local\\Player_" + dateStr + ".json";
                if (MoveFileA(playerPath.c_str(), backupPath.c_str())) {
                    MessageBoxA(hDlg, "Player progress has been reset and backup created.", "Success", MB_OK | MB_ICONINFORMATION);
                } else {
                    MessageBoxA(hDlg, "Could not rename Player.json. Check file permissions.", "Error", MB_OK | MB_ICONERROR);
                }
            } else {
                MessageBoxA(hDlg, "No Player.json found to reset.", "Info", MB_OK | MB_ICONINFORMATION);
            }
            break;
        }
        case 1015: // Save and Start YGOMasterClient
        {
            // Save settings first (reuse your IDOK logic)
            for (int i = 0; i < PARAM_COUNT; ++i) {
                const Param& p = PARAMS[i];
                json* ptr = get_json_ptr_by_path(settings, p.key);
                if (!ptr) continue;
                switch (p.type) {
                case Param::BOOL:
                    *ptr = (SendDlgItemMessageA(hDlg, p.dlgId, BM_GETCHECK, 0, 0) == BST_CHECKED);
                    break;
                case Param::STRING: {
                    char buf[256] = {0};
                    GetDlgItemTextA(hDlg, p.dlgId, buf, sizeof(buf));
                    *ptr = std::string(buf);
                    break;
                }
                case Param::INT:
                    *ptr = (int)GetDlgItemInt(hDlg, p.dlgId, NULL, TRUE);
                    break;
                case Param::DOUBLE: {
                    char buf[64];
                    GetDlgItemTextA(hDlg, p.dlgId, buf, sizeof(buf));
                    try {
                        *ptr = std::stod(buf);
                    } catch (...) {
                        *ptr = 0.0;
                    }
                    break;
                }
                }
            }
            if (!SaveSettings()) {
                MessageBoxA(hDlg, "Could not save Data/Settings.json!", "Error", MB_ICONERROR);
                break;
            }

            // Launch YGOMasterClient.exe in the same folder as the editor
            char exePath[MAX_PATH];
            GetModuleFileNameA(NULL, exePath, MAX_PATH);
            char* lastSlash = strrchr(exePath, '\\');
            if (lastSlash) strcpy(lastSlash + 1, "YGOMasterClient.exe");

            STARTUPINFOA si = { sizeof(si) };
            PROCESS_INFORMATION pi = {};
            if (!CreateProcessA(exePath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                MessageBoxA(hDlg, "Failed to start YGOMasterClient.exe", "Error", MB_ICONERROR);
                break;
            }
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            EndDialog(hDlg, IDOK); // Optionally close the editor
            break;
        }
        } // end switch (LOWORD(wParam))

        if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == 1008) {
            int sel = (int)SendDlgItemMessageA(hDlg, 1008, CB_GETCURSEL, 0, 0);
            if (sel >= 0 && sel < DIFFICULTY_COUNT) {
                const auto& preset = DIFFICULTY_PRESETS[sel];
                SetDlgItemInt(hDlg, 1010, preset.minValue, TRUE);
                SetDlgItemInt(hDlg, 1011, preset.maxValue, TRUE);
                SetDlgItemTextA(hDlg, 1009, std::to_string(preset.multiplier).c_str());
                SetDlgItemInt(hDlg, 1012, preset.loseMin, TRUE);
                SetDlgItemInt(hDlg, 1013, preset.loseMax, TRUE);
            }
        }
        if (LOWORD(wParam) == IDOK) {
            // Save all controls based on PARAMS table
            for (int i = 0; i < PARAM_COUNT; ++i) {
                const Param& p = PARAMS[i];
                json* ptr = get_json_ptr_by_path(settings, p.key);
                if (!ptr) continue;
                switch (p.type) {
                case Param::BOOL:
                    *ptr = (SendDlgItemMessageA(hDlg, p.dlgId, BM_GETCHECK, 0, 0) == BST_CHECKED);
                    break;
                case Param::STRING: {
                    char buf[256] = {0};
                    GetDlgItemTextA(hDlg, p.dlgId, buf, sizeof(buf));
                    *ptr = std::string(buf);
                    break;
                }
                case Param::INT:
                    *ptr = (int)GetDlgItemInt(hDlg, p.dlgId, NULL, TRUE);
                    break;
                case Param::DOUBLE: {
                    char buf[64];
                    GetDlgItemTextA(hDlg, p.dlgId, buf, sizeof(buf));
                    try {
                        *ptr = std::stod(buf);
                    } catch (...) {
                        *ptr = 0.0;
                    }
                    break;
                }
                }
            }
            if (!SaveSettings()) {
                MessageBoxA(hDlg, "Could not save Data/Settings.json!", "Error", MB_ICONERROR);
            }
            EndDialog(hDlg, IDOK);
            return TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL) {
            // Free bitmap resource before closing
            if (hPrevBmp) {
                DeleteObject(hPrevBmp);
                hPrevBmp = NULL;
            }
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    if (!LoadSettings()) return 1;
    INT_PTR ret = DialogBoxParamA(hInstance, MAKEINTRESOURCEA(101), 0, DlgProc, 0);
    if (ret == -1) {
        MessageBoxA(0, "DialogBoxParamA failed! Check your .rc file and dialog resource ID.", "Error", MB_ICONERROR);
    }
    return 0;
}