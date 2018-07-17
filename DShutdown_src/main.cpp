#include <windowsx.h>
#include <windows.h>
#include <winuser.h>
#include <ctime>
#include "main.h"
#include "lang.h"

LPSTR ClassName = "Window Class";
MSG Message;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define ID_EKRAN 500
HWND hEkran;
#define ID_OK 501
#define ID_CLOSE 502
#define ID_CHECKBOX 503
#define ID_ABORT 504

int mtime;
int mtimetoshutdown;
int hcount;
int mcount;
bool CheckboxState;
bool AbortMode;
char TempPath[MAX_PATH] = { 0 };
char DSTimePath[512];
char Text2[100];
time_t currenttime;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    // FILLING STRUCTURE
    WNDCLASSEX wc;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = (HICON)LoadImage(
        hInstance, MAKEINTRESOURCE(2000), IMAGE_ICON, 48, 48, LR_LOADTRANSPARENT);
    wc.hIconSm = (HICON)LoadImage(
        hInstance, MAKEINTRESOURCE(2000), IMAGE_ICON, 48, 48, LR_LOADTRANSPARENT);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = ClassName;

    // REGISTERING WINDOW CLASS
    if (!RegisterClassEx(&wc)) {
        MessageBox(
            NULL, "Couldn't register window!", "Error!", MB_ICONERROR | MB_TASKMODAL | MB_OK);
        PostQuitMessage(0);
        return 1;
    }

    // CREATING WINDOW
    HWND hwnd;

    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, ClassName, "DShutdown 1.0.0", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 330, 265, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, "Couldn't create window!", "Error!", MB_ICONERROR | MB_TASKMODAL | MB_OK);
        PostQuitMessage(0);
        return 1;
    }

    // CHECKING IF SHUTDOWN ARLEADY SCHEDULED
    GetTempPathA(MAX_PATH, TempPath);
    std::snprintf(DSTimePath, sizeof(DSTimePath), "%s\DSTime.DShutdown", TempPath);
    int stimefromfile = atoi(ReadFromFile(DSTimePath));
    if (stimefromfile != NULL) {
        AbortMode = true;
    }

    //****************************************************************************************//
    //                                        CONTROLS                                        //
    //****************************************************************************************//

    // BUTTONS
    HWND 
        hButton1 = CreateWindowEx(WS_EX_CLIENTEDGE, "BUTTON", SET, WS_CHILD | WS_VISIBLE | WS_BORDER,
        220, 100, 80, 40, hwnd, (HMENU)ID_OK, hInstance, NULL),
        hButton2 = CreateWindowEx(WS_EX_CLIENTEDGE, "BUTTON", CLOSE, WS_CHILD | WS_VISIBLE | WS_BORDER,
        10, 100, 80, 40, hwnd, (HMENU)ID_CLOSE, hInstance, NULL),
        hButton3 = CreateWindowEx(WS_EX_CLIENTEDGE, "BUTTON", ABORT_SCHEDULED_SHUTDOWN, WS_CHILD | WS_VISIBLE | WS_BORDER,
        10, 165, 290, 40, hwnd, (HMENU)ID_ABORT, hInstance, NULL),
        hCheckbox1 = CreateWindowEx(0, "BUTTON", SHOW_WARNINGS, WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
        22, 143, 270, 20, hwnd, (HMENU)ID_CHECKBOX, hInstance, NULL),
    // INPUT
        hTime = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 
        100, 100, 110, 40, hwnd, NULL, hInstance, NULL),
    // TEXTS
        hText1 = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER, 
        10, 10, 290, 20, hwnd, NULL, hInstance, NULL),
        hText2 = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER, 
        0, 40, 290, 50, hwnd, NULL, hInstance, NULL);
    SetWindowText(hText1, "DShutdown 1.0.0");
    SetWindowText(hText2, MAIN_TEXT);

    // SIGN
    HWND hSign = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER, 
    225, 208, 85, 12, hwnd, NULL, hInstance, NULL);
    SetWindowText(hSign, "Made by Distaff");

    // FONTS
    HFONT fSign;
    fSign = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, PROOF_QUALITY, 0, "Arial");
    HFONT fTime;
    fTime = CreateFont(30, 0, 0, 0, 600, 0, 0, 0, 0, 0, 0, 0, 0, "Consolas");
    HFONT fText1;
    fText1 = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, PROOF_QUALITY, 0, "Lucida Console");
    SendMessage(hSign, WM_SETFONT, WPARAM(fSign), TRUE);
    SendMessage(hTime, WM_SETFONT, WPARAM(fTime), TRUE);
    SendMessage(hText1, WM_SETFONT, WPARAM(fText1), TRUE);

    if (AbortMode == true) {
        Button_Enable(hButton1, false);
        Button_Enable(hButton2, false);
        Button_Enable(hButton3, true);
        Button_Enable(hCheckbox1, false);
        Button_Enable(hTime, false);
        currenttime = time(NULL);
        mtimetoshutdown = (stimefromfile - currenttime) / 60 + 1;
        hcount = mtimetoshutdown / 60;
        mcount = mtimetoshutdown - hcount * 60;
        std::snprintf(Text2, sizeof(Text2), "%s %d %s %s %d %s.", TO_SHUTDOWN, hcount, HOURS, AND, mcount, MINUTES);
        SetWindowText(hText2, Text2);
    }

    else {
        Button_Enable(hButton1, true);
        Button_Enable(hButton2, true);
        Button_Enable(hButton3, false);
        Button_Enable(hCheckbox1, true);
        Button_Enable(hTime, true);
    }

    //***********************************************************************************************//
    //                                        END OF CONTROLS                                        //
    //***********************************************************************************************//

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // MSG LOOP
    while (GetMessage(&Message, NULL, 0, 0)) {
        // COUNTDOWN
        if (AbortMode == true) {
            currenttime = time(NULL);
            mtimetoshutdown = (stimefromfile - currenttime) / 60 + 1;
            hcount = mtimetoshutdown / 60;
            int lstmcount = mcount;
            mcount = mtimetoshutdown - hcount * 60;
            std::snprintf(Text2, sizeof(Text2), "%s %d %s %s %d %s.", TO_SHUTDOWN, hcount, HOURS, AND, mcount, MINUTES);
            if (lstmcount != mcount) {
                SetWindowText(hText2, Text2);
            }
        }
        else {
            DWORD dlugosc = GetWindowTextLength(hTime);
            LPSTR Bufor = (LPSTR)GlobalAlloc(GPTR, dlugosc + 1);
            GetWindowText(hTime, Bufor, dlugosc + 1);
            mtime = atoi(Bufor);
        }

        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return Message.wParam;
}

//**********************************************************************************************//
//                                        EVENT HANDLING                                        //
//**********************************************************************************************//

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_COMMAND:
        switch (wParam) {

        case ID_OK: {
            char filetime[100];
            int filestime = time(NULL);
            filestime = filestime + mtime * 60;
            std::snprintf(filetime, sizeof(filetime), "%d", filestime);
            WriteToFile(DSTimePath, filetime);
            if (mtime > 0) {
                // IF "SHOW WARNINGS" TICKED
                if (CheckboxState == true) {
                    char buff[128];
                    hcount = mtime / 60;
                    mcount = mtime - hcount * 60;
                    std::snprintf(buff, sizeof(buff), "%s %d %s %s %d %s.", TO_SHUTDOWN, hcount, HOURS, AND, mcount, MINUTES);
                    ShowWindow(hwnd, SW_HIDE);
                    MessageBox(NULL, buff, SHUTDOWN_SCHEDULED, MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON1 | MB_TASKMODAL);
                    if (mtime > 5) {
                        if (mtime > 30) {
                            Sleep((mtime - 30) * 60000);
                            MessageBox(NULL, TO_SHUTDOWN30, SHUTDOWN_SCHEDULED, MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON1);
                            Sleep(1500000);
                            MessageBox(NULL, TO_SHUTDOWN5, SHUTDOWN_SCHEDULED, MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON1);
                            Sleep(300000);
                            system("shutdown /s /f");
                        }
                        else {
                            Sleep((mtime - 5) * 60000);
                            MessageBox(NULL, TO_SHUTDOWN5, SHUTDOWN_SCHEDULED, MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON1);
                            Sleep(300000);
                            system("shutdown /s /f");
                        };
                    }
                    else {
                        Sleep(mtime * 60000);
                        system("shutdown /s /f");
                    };
                    DestroyWindow(hwnd);
                    PostQuitMessage(0);
                }
                else {
                    DestroyWindow(hwnd);
                    Sleep(mtime * 60000);
                    system("shutdown /s /f");
                    PostQuitMessage(0);
                };
            }
            else {
                MessageBox(hwnd, INSERT_CORRECT, INCORRECT_VALUE, MB_ICONWARNING | MB_OK);
            };
        } break;

        case ID_CLOSE: {
            DestroyWindow(hwnd);
            PostQuitMessage(0);
        } break;

        case ID_CHECKBOX: {
            CheckboxState = (IsDlgButtonChecked(hwnd, ID_CHECKBOX) == BST_CHECKED);
            if (CheckboxState == true) {
                CheckDlgButton(hwnd, ID_CHECKBOX, BST_UNCHECKED);
            }
            else {
                CheckDlgButton(hwnd, ID_CHECKBOX, BST_CHECKED);
            };
            CheckboxState = (IsDlgButtonChecked(hwnd, ID_CHECKBOX) == BST_CHECKED);
        } break;

        case ID_ABORT: {
            system("start Abort.exe");
        } break;

        default:
            break;
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}
