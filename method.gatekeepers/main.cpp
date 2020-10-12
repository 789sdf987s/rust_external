/**
 * got backstabbed by someone I trusted, and my user-mode source was stolen, then he threatened to leak it.

 * however, that silly fucking goose took the wrong version without comments -- here's a much older build with comments. hopefully it will provide a learning opportunity for some of you.

 * this version of the source is dated around June 2020, before I had a good grasp on how to do shit efficiently, externally on rust. what's uploaded below only includes the usermode part of the cheat, so consequently, this cheat will not work on EAC rust without any bypass.

 * i've updated the cheat, so it works at the moment. as stated before, the threads are not done efficiently, so I'll leave that up to you to fix. many of the features have been snipped as well, including prediction.

 * hopefully, someone will be able to learn from this source and proceed to make their own shit. the one thing I request is that you don't make this into a shitty P2C like others have done in the past with previous self-leaks.
 
 * ----------
 *  CREDITS
 * ----------

 * shitty fuckin ImGUI Sk**t-Inspired Menu 
     * - Unknown

 * source provider
     * - skrrt.xyz
     * - skit
     * - sofneo
 */

#include "include.hpp"
#include "globals/globals.hpp"
#include "classes/GOM.hpp"
#include "classes/BaseNetworkable.hpp"
#include "threads/threads.hpp"
#include "features/aimbot/Aimbot.hpp"
#include "features/features.hpp"
#include "menu/menu.h"

/* main function */
int main()
{
    /* allocates a new console */
    {
		AllocConsole();
		SetConsoleTitleW(L"method.gatekeepers 🍆");
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
    }

    /* Find Rust Window */
    drawing->h_Game = FindWindowA("UnityWndClass", "Rust");

    if (!drawing->h_Game)
    {
        print(4, "[:(] ", "Rust Not Found! \n");
        SleepEx(3000, false);
        exit(0);
    }
    else
    {
        print(10, "[!] ", "Found Rust process! \n");
        print(14, "[-] ", "Attempting to attach to Rust process! \n");

        /* Initialize a memory interface object */
        mem = new Memory();

        /* Attempt to attach to RustClient.exe */
        if (!mem->Attach("RustClient.exe"))
        {
            /* Log failure */
            print(4, "[:(] ", "Failed to attach to RustClient.exe! \n");

            /* Let the nigger read */
            SleepEx(3000, false);

            /* Exit because fuck you sector */
            return EXIT_FAILURE;
        }
        else
            print(10, "[!] ", "Attached to Rust process! \n");

        /* Sets Window Size */
        globals->ScreenSize = { utils->getWindowSize(drawing->h_Game).first, utils->getWindowSize(drawing->h_Game).second };

        /* Grab Offsets */
        offsets = new Offsets();

        /* Create BN */
        bn = new BaseNetworkable();

        /* Create GOM */
        gom = new GameObjectManager();

        /* Get the camera address */
        globals->camera = gom->GetCamera();

        /* Check that we got the camera address */
        if (!globals->camera)
        {
            /* Log failure */
            print(4, "[:(] ", "Failed to get the camera. \n");

            /* Let the user read */
            SleepEx(3000, false);

            /* Exit because FUCK sector */
            return EXIT_FAILURE;
        }

        /* Sets The Foreground Window Back To Rust */
        SetForegroundWindow(drawing->h_Game);

        /* Spawn the render thread */
        std::thread(&Drawing::Initialize, drawing).detach();

        /* Spawn the processing thread */
        std::thread(threads::EntityUpdate).detach();

        /* Spawn the discovery thread */
        std::thread(threads::EntityDiscovery, bn->cents_addr).detach();

        /* Spawn the aimbot thread */
        std::thread(&Aimbot::FindTarget, aimbot).detach();

        /* Spawn the features thread */
        std::thread(&Features::HandleFeatures, features).detach();

        /* gay fucking overlay dogshit */
        bool b_ov;
        while (1)
        {
            /* sleepy sleepy */
            SleepEx(1, false);

            /* if the overlay is non existent, shutdown drawing ;( */
            if (!IsWindow(drawing->h_Game))
                drawing->b_Shutdown = true;

            HWND h_Foreground = GetForegroundWindow();
            /* if the current foreground window does not equal rust */
            if (h_Foreground != drawing->h_Game && h_Foreground != drawing->h_hWnd) {
                if (!b_ov) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    continue;
                }

                /* hide window */
                SetWindowPos(drawing->h_hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
                b_ov = false;
            }
            else {
                if (b_ov) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
                    continue;
                }

                /* show window */
                SetWindowPos(drawing->h_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

                /* if menu is open, set foreground window to the overlay */
                if (menu->isOpen)
                    SetForegroundWindow(drawing->h_hWnd);

                b_ov = true;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
    }
}