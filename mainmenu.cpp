#include "stdafx.h"
// You'll only need to include <menu.h> to start using it,
// after setting paths up properly.
#include "menu.h"
#include "features.h"
#include "items.h"


// You can ignore these includes as they're only used to resolve
// paths, log things to a file or to provide nice wrappers for natives.
#include "paths.h"

int textureBgId;
int textureBgId2;

// For demonstration purposes of image handling
class MenuTexture {
public:
    MenuTexture() : Handle(-1),
        Width(480),
        Height(270) { }

    MenuTexture(std::string filename, int handle, int width, int height) :
        Filename(filename), Handle(handle), Width(width), Height(height) { }

    std::string Filename;
    int Handle;
    int Width;
    int Height;
};

std::vector<MenuTexture> g_textures;
int prevNotification;

// A few state variables for demoing the menu
bool checkBoxStatus = false;
float someFloat = 0.0f;
int someInt = 0;
int intStep = 1;
int stringsPos = 0;
int stepChoice = 0;
int numberOfOptions = 16;

// Choice of step size to demonstrate variable precision display
std::vector<float> floatSteps = {
    1.0f,
    0.1f,
    0.01f,
    0.001f,
    0.0001f,
    0.00001f,
    0.000001f,
};

// Random words to go through in the menu
std::vector<std::string> strings = {
    "Hello, ",
    "world!",
};

namespace MainMenu {
    NativeMenu::Menu menu;
    std::string settingsMenuFile;
    extern void update_game() {
        /*
         * Whatever happens normally in your script!
         * For new, let's just update the player ped.
         */
       // printf("tick");
    }

    void onRight() {
        // menu.ReadSettings();
    }

    void onLeft() {
        // menu.ReadSettings();
    }

    void onExit() {
        //  menu.ReadSettings();
    }


    void update_mainmenu() {
        // The title is NOT optional.
        menu.Title("Spektre Menu");
        menu.Subtitle("here i go hacking again...");

        // This is a normal option. It'll return true when "select" is presed.
        if (menu.Option("Teleport To Waypoint")) {
            features::WPTP();
        }
  

        // This will open a submenu with the name "submenu"
        menu.MenuOption("Teleport Menu", "tpmenu");

        if (menu.Option("Reload Settings", { "for when shit's broke" })) {
            menu.ReadSettings();
        }

        // Showing static information is also possible if a string vector only contains one element.
        // int nothing = 0;
       // menu.StringArray("Version", { DISPLAY_VERSION }, nothing, { "Thanks for checking out this menu!", "Author: ikt" });
    }

    void update_tpmenu() {
        menu.Title("Teleport Menu");
        menu.Subtitle("");
        menu.MenuOption("card_cups", "card_cups");
        menu.MenuOption("card_swords", "card_swords");
        menu.MenuOption("card_wands", "card_wands");
        menu.MenuOption("card_pentacles", "card_pentacles");
        menu.MenuOption("coins", "coins");
        menu.MenuOption("bird_eggs", "bird_eggs");
        menu.MenuOption("antique_bottles", "antique_bottles");
        menu.MenuOption("arrowhead", "arrowhead");
        menu.MenuOption("family_heirlooms", "family_heirlooms");
        menu.MenuOption("american_flowers", "american_flowers");
        menu.MenuOption("lost_ring", "lost_ring");
        menu.MenuOption("lost_necklaces", "lost_necklaces");
        menu.MenuOption("lost_earrings", "lost_earrings");
        menu.MenuOption("lost_bracelet", "lost_bracelet");
        menu.MenuOption("random", "random");
    }

    void update_card_cups() {
        menu.Title("Teleport Menu");
        menu.Subtitle("");
        for (Treasure i : card_cups) {
            if (menu.Option("TP To:" + i.name)) {
                features::cordtp(i.x, i.y);
            }
        };
    }
    void update_card_swords() {
        menu.Title("Teleport Menu");
        menu.Subtitle(""); 
        for (Treasure i : card_swords) {
            if (menu.Option("TP To:" + i.name)) {
                features::cordtp(i.x, i.y);
            }
        };
    }
    void update_card_wands() {
        menu.Title("Teleport Menu");
        menu.Subtitle("");
        for (Treasure i : card_wands) {
            if (menu.Option("TP To:" + i.name)) {
                features::cordtp(i.x, i.y);
            }
        };
    }
    void update_card_pentacles() {
        menu.Title("Teleport Menu");
        menu.Subtitle("");
        for (Treasure i : card_pentacles) {
            if (menu.Option("TP To:" + i.name)) {
                features::cordtp(i.x, i.y);
            }
        };
    }
    void update_coins() {
        menu.Title("Teleport Menu");
        menu.Subtitle("");
        for (Treasure i : coins) {
            if (menu.Option("TP To:" + i.name)) {
                features::cordtp(i.x, i.y);
            }
        };
    }
    void update_bird_eggs() {
        menu.Title("Teleport Menu");
        menu.Subtitle("");
        for (Treasure i : bird_eggs) {
            if (menu.Option("TP To:" + i.name)) {
                features::cordtp(i.x, i.y);
            }
        };
    }
    void update_antique_bottles() {
        menu.Title("Teleport Menu");
        menu.Subtitle("");
        for (Treasure i : antique_bottles) {
            if (menu.Option("TP To:" + i.name)) {
                features::cordtp(i.x, i.y);
            }
        };
    }
    void update_arrowhead() {
        menu.Title("Teleport Menu");
        menu.Subtitle("");
        for (Treasure i : arrowhead) {
            if (menu.Option("TP To:" + i.name)) {
                features::cordtp(i.x, i.y);
            }
        };
    }
    void update_family_heirlooms() {
        menu.Title("Teleport Menu");
        menu.Subtitle("");
        for (Treasure i : family_heirlooms) {
            if (menu.Option("TP To:" + i.name)) {
                features::cordtp(i.x, i.y);
            }
        };
    }
    void update_american_flowers() {
        menu.Title("Teleport Menu");
        menu.Subtitle("");
        for (Treasure i : american_flowers) {
            if (menu.Option("TP To:" + i.name)) {
                features::cordtp(i.x, i.y);
            }
        };
    }
    void update_lost_ring() {
        menu.Title("Teleport Menu");
        menu.Subtitle("");
        for (Treasure i : lost_ring) {
            if (menu.Option("TP To:" + i.name)) {
                features::cordtp(i.x, i.y);
            }
        };
    }
    void update_lost_necklaces() {
        menu.Title("Teleport Menu");
        menu.Subtitle("");
        for (Treasure i : lost_necklaces) {
            if (menu.Option("TP To:" + i.name)) {
                features::cordtp(i.x, i.y);
            }
        };
    }
    void update_lost_earrings() {
        menu.Title("Teleport Menu");
        menu.Subtitle("");
        for (Treasure i : lost_earrings) {
            if (menu.Option("TP To:" + i.name)) {
                features::cordtp(i.x, i.y);
            }
        };
    }
    void update_lost_bracelet() {
        menu.Title("Teleport Menu");
        menu.Subtitle("");
        for (Treasure i : lost_bracelet) {
            if (menu.Option("TP To:" + i.name)) {
                features::cordtp(i.x, i.y);
            }
        };
    }
    void update_random() {
        menu.Title("Teleport Menu");
        menu.Subtitle("");
        for (Treasure i : random) {
            if (menu.Option("TP To:" + i.name)) {
                features::cordtp(i.x, i.y);
            }
        };
    }


    /*
     * update_menu() should be called each tick. This example only shows static
     * menus with some dynamic options, but practically any menu and submenu can
     * be dynamic. Check my other project how that was done:
     * https://github.com/E66666666/GTAVAddonLoader
     */
    extern void update_menu() {
        /*
         * Each tick, the controls are checked. If the key is hit to open
         * or close the menu, the binded functions are called.
         * This function has to be called for navigation to work.
         */
        menu.CheckKeys();

        /*
         *  You can define a menu like this.
         *  The main menu should always be called "mainmenu".
         */
        if (menu.CurrentMenu("mainmenu")) {
            update_mainmenu();
        }

        /*
         * Any submenus can have any titles. They should only need to match
         * the name used to call them.
         */
        if (menu.CurrentMenu("tpmenu")) {
            update_tpmenu();
        }

        if (menu.CurrentMenu("card_cups")) {
            update_card_cups();
        }
        if (menu.CurrentMenu("card_swords")) {
            update_card_swords();
        }
        if (menu.CurrentMenu("card_wands")) {
            update_card_wands();
        }
        if (menu.CurrentMenu("card_pentacles")) {
            update_card_pentacles();
        }
        if (menu.CurrentMenu("coins")) {
            update_coins();
        }
        if (menu.CurrentMenu("bird_eggs")) {
            update_bird_eggs();
        }
        if (menu.CurrentMenu("antique_bottles")) {
            update_antique_bottles();
        }
        if (menu.CurrentMenu("arrowhead")) {
            update_arrowhead();
        }
        if (menu.CurrentMenu("family_heirlooms")) {
            update_family_heirlooms();
        }
        if (menu.CurrentMenu("american_flowers")) {
            update_american_flowers();
        }
        if (menu.CurrentMenu("lost_ring")) {
            update_lost_ring();
        }
        if (menu.CurrentMenu("lost_necklaces")) {
            update_lost_necklaces();
        }
        if (menu.CurrentMenu("lost_earrings")) {
            update_lost_earrings();
        }
        if (menu.CurrentMenu("lost_bracelet")) {
            update_lost_bracelet();
        }
        if (menu.CurrentMenu("random")) {
            update_random();
        }



        /*
         * Finally, draw all textures.
         */
        menu.EndMenu();
    }

    /*
     * This simple function is executed when the menu opens.
     */
    void onMain() {
        printf("\n onMain");
        // menu.ReadSettings();
    }

    /*
    * The settings may be read multiple times if needed.
    */
    extern void start_menu() {
        printf("\n startMenu");

        // Check the paths on runtime, though this could also be hardcoded with a relative path.
        settingsMenuFile = Paths::GetModuleFolder(Paths::GetOurModuleHandle()) + modDir + "\\settings_menu.ini";
        menu.SetFiles(settingsMenuFile);
        menu.ReadSettings();

        // Register callbacks for menu entry and exit.
        menu.RegisterOnMain(std::bind(onMain));
        menu.RegisterOnExit(std::bind(onExit));
    }
}