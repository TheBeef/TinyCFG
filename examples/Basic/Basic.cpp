#include "../../TinyCFG.h"
#include <string>
#include <list>
#include <map>
#include <algorithm>
#include <stdint.h>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef std::map<std::string,std::string> t_KeyValue;
typedef t_KeyValue::iterator i_KeyValue;

class KeyValueCFG : public TinyCFGBaseData
{
   public:
    t_KeyValue *Ptr;
    bool LoadElement(class TinyCFG *CFG);
    bool SaveElement(class TinyCFG *CFG);
};

bool KeyValueCFG::LoadElement(class TinyCFG *CFG)
{
    const char *Key;
    const char *Value;

    Ptr->clear();
    while(CFG->ReadNextTag(&Key,&Value))
        Ptr->insert(make_pair(Key,Value));

    return true;
}

bool KeyValueCFG::SaveElement(class TinyCFG *CFG)
{
    i_KeyValue i;
    string tmp;

    for(i=Ptr->begin();i!=Ptr->end();i++)
    {
        tmp=i->first;
        std::replace(tmp.begin(),tmp.end(),'&','_');
        std::replace(tmp.begin(),tmp.end(),'<','_');
        std::replace(tmp.begin(),tmp.end(),'>','_');
        CFG->WriteDataElement(tmp.c_str(),i->second.c_str());
    }

    return true;
}

bool Settings_RegisterKeyValueType(class TinyCFG &cfg,
        const char *XmlName,t_KeyValue *Data)
{
    class KeyValueCFG *NewDataClass;

    /* Make a new class to handle this new piece of data */
    try
    {
        NewDataClass=new KeyValueCFG;
    }
    catch(std::bad_alloc const &)
    {
        return false;
    }

    /* Setup the data */
    NewDataClass->Ptr=Data;
    NewDataClass->XmlName=XmlName;

    return cfg.RegisterGeneric(NewDataClass);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
struct TestData
{
    std::string StrID;
    t_KeyValue Data;
    void RegisterAllMembers(class TinyCFG &cfg)
    {
        cfg.Register("StrID",StrID);
        Settings_RegisterKeyValueType(cfg,"Settings",&Data);
    }
};

typedef std::list<struct TestData> t_Plugins;
typedef t_Plugins::iterator i_Plugins;

class TestDataList : public TinyCFGBaseData
{
    public:
        t_Plugins *Ptr;
        bool LoadElement(class TinyCFG *CFG);
        bool SaveElement(class TinyCFG *CFG);
};
bool TestDataList::LoadElement(class TinyCFG *CFG)
{
    const char *Str;
    struct TestData NewData;
    class TinyCFG SubCFG("Plugin");

    Ptr->clear();

    NewData.RegisterAllMembers(SubCFG);
    SubCFG.ConnectToParentCFGForReading(CFG);

    while(SubCFG.ReadNextCFG())
        Ptr->push_back(NewData);

    return true;
}
bool TestDataList::SaveElement(class TinyCFG *CFG)
{
    i_Plugins i;
    char buff[100];
    class TinyCFG SubCFG("Plugin");
    struct TestData NewData;

    NewData.RegisterAllMembers(SubCFG);

    for(i=Ptr->begin();i!=Ptr->end();i++)
    {
        NewData=*i;
        SubCFG.WriteCFGUsingParentCFG(CFG);
    }
    return true;
}

bool RegisterTestDataList(class TinyCFG &cfg,const char *XmlName,
        t_Plugins &Data)
{
    class TestDataList *NewDataClass;

    /* Make a new class to handle this new piece of data */
    try
    {
        NewDataClass=new TestDataList;
    }
    catch(std::bad_alloc)
    {
        return false;
    }

    /* Setup the data */
    NewDataClass->Ptr=&Data;
    NewDataClass->XmlName=XmlName;

    return cfg.RegisterGeneric(NewDataClass);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

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
    t_Plugins m_Plugins;
    t_Plugins m_PluginsLoad;
    struct TestData d;
    struct TestData l;
    class TinyCFG cfg("Settings");
    class TinyCFG cfg2("Settings");
    class TinyCFG cfgLoad("Settings");
    i_KeyValue i;
    i_Plugins p;

    d.StrID="ComputerPicker";
    d.Data.insert(make_pair("Key&Value","Value"));
    d.Data.insert(make_pair("Computer","Amiga"));
    d.Data.insert(make_pair("Year","1985"));
    m_Plugins.push_back(d);

    d.Data.clear();
    d.StrID="Highlighter";
    d.Data.insert(make_pair("Color","#FFFFFF"));
    m_Plugins.push_back(d);

//    g_Settings.SaveSettings();
//    g_Settings.LoadSettings();

    RegisterTestDataList(cfg,"PlugsinSettings",m_Plugins);
    cfg.SaveCFGFile("Settings.xml");

    RegisterTestDataList(cfgLoad,"PlugsinSettings",m_PluginsLoad);
    cfgLoad.LoadCFGFile("Settings.xml");

    for(p=m_PluginsLoad.begin();p!=m_PluginsLoad.end();p++)
    {
        printf("Plugin:%s\n",p->StrID.c_str());
        for(i=p->Data.begin();i!=p->Data.end();i++)
        {
            printf("    %s:%s\n",i->first.c_str(),i->second.c_str());
        }
        printf("----\n");
    }

////    Settings_RegisterKeyValueType(cfg,"KeyValues",&d.Data);
////
////    cfg.SaveCFGFile("Settings.xml");
////
////    d.Data.clear();
////
////    Settings_RegisterKeyValueType(cfg2,"KeyValues",&l.Data);
////    cfg2.LoadCFGFile("Settings.xml");
////
////    printf("=========================\n");
////
////    for(i=l.Data.begin();i!=l.Data.end();i++)
////    {
////        printf("%s:%s\n",i->first.c_str(),i->second.c_str());
////    }
////
////    printf("=========================\n");

    return 0;
}
