#include <windows.h>
#include <fstream>
#include <string>
#include "json.hpp" // https://github.com/nlohmann/json

using json = nlohmann::json;

// Parameters for settings
struct Param {
    const char* key;
    enum Type { BOOL, INT, STRING } type;
    int dlgId;
};

// Difficulty presets
struct DifficultyPreset {
    const char* name;
    int minValue;
    int maxValue;
};
DifficultyPreset DIFFICULTY_PRESETS[] = {
    {"Easy", 500, 700},
    {"Normal", 300, 500},
    {"Hard", 100, 200}
};
const int DIFFICULTY_COUNT = sizeof(DIFFICULTY_PRESETS) / sizeof(DIFFICULTY_PRESETS[0]);

// Supported parameters
Param PARAMS[] = {
    {"UnlockAllCards", Param::BOOL, 1001},
    {"UnlockAllItems", Param::BOOL, 1002},
    {"CardCraftableAll", Param::BOOL, 1003},
    {"DisableNoDismantle", Param::BOOL, 1004},
    {"DefaultGems", Param::STRING, 1005},
    {"DuelRewards.win[4].min", Param::INT, 1009},
    {"DuelRewards.win[4].max", Param::INT, 1010}
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
    switch (msg) {
    case WM_INITDIALOG:
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
            }
        }
        // Populate difficulty combo box
        for (int i = 0; i < DIFFICULTY_COUNT; ++i) {
            SendDlgItemMessageA(hDlg, 1008, CB_ADDSTRING, 0, (LPARAM)DIFFICULTY_PRESETS[i].name);
        }
        SendDlgItemMessageA(hDlg, 1008, CB_SETCURSEL, 1, 0); // "Normal"
        return TRUE;

    case WM_COMMAND:
        if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == 1008) {
            int sel = (int)SendDlgItemMessageA(hDlg, 1008, CB_GETCURSEL, 0, 0);
            if (sel >= 0 && sel < DIFFICULTY_COUNT) {
                int minValue = DIFFICULTY_PRESETS[sel].minValue;
                int maxValue = DIFFICULTY_PRESETS[sel].maxValue;
                SetDlgItemInt(hDlg, 1009, minValue, TRUE);
                SetDlgItemInt(hDlg, 1010, maxValue, TRUE);
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
                }
            }
            if (!SaveSettings()) {
                MessageBoxA(hDlg, "Could not save Data/Settings.json!", "Error", MB_ICONERROR);
            }
            EndDialog(hDlg, IDOK);
            return TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL) {
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