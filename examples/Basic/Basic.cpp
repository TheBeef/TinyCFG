#include "../../TinyCFG.h"
#include <string>
#include <list>

struct Settings
{
   std::string DefaultName;
   int DefaultAge;
   std::list<std::string> NameList;

    void RegisterAllMembers(class TinyCFG &cfg)
    {
        cfg.Register("DefaultName",DefaultName);
        cfg.Register("DefaultAge",DefaultAge);
        cfg.Register("Names",NameList);
    }
    void SaveSettings(void);
    void LoadSettings(void);
} g_Settings;

void Settings::SaveSettings(void)
{
   class TinyCFG cfg("Settings");
   RegisterAllMembers(cfg);
   cfg.SaveCFGFile("Settings.xml");
}

void Settings::LoadSettings(void)
{
   class TinyCFG cfg("Settings");
   RegisterAllMembers(cfg);
   cfg.LoadCFGFile("Settings.xml");
}

int main(void)
{
    g_Settings.DefaultName="<Paul>";
    g_Settings.DefaultAge=25;
    g_Settings.NameList.push_back("Paul");
    g_Settings.NameList.push_back("Ted");
    g_Settings.NameList.push_back("Sarah");

    g_Settings.SaveSettings();
    printf("Before:\"%s\"\n",g_Settings.DefaultName.c_str());

    g_Settings.LoadSettings();
    printf("After:\"%s\"\n",g_Settings.DefaultName.c_str());

    return 0;
}
