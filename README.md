# TinyCFG
A tiny library for saving / loading C++ structures to / from disk in XML

This is still a work in progress.  The code works but the doc's suck, the example suck, and it doesn't look very pretty.

# Quick Summary

You make a struct / class with your data in it:

```
struct MyData
{
    int One;
    int Two;
    std::string CppString;

    void RegisterAllMembers(class TinyCFG &cfg);
    void load(void);
    void save(void);
};
```

Then you make a function that connects the members of your struct with TinyCFG:

```
void MyData::RegisterAllMembers(class TinyCFG &cfg)
{
    cfg.Register("One",One);
    cfg.Register("Two",Two);
    cfg.Register("CppString",CppString);
}
```

Then went you want to load/save data:

```
void save(void)
{
    class TinyCFG cfg("MyData");

    RegisterAllMembers(cfg);

    cfg.SaveCFGFile("MyData.xml");
}

void load(void)
{
    class TinyCFG cfg("MyData");

    RegisterAllMembers(cfg);

    cfg.LoadCFGFile("MyData.xml");
}
```

That's it.  Very simple.

(It also support sub structs, adding your own data types, and sub xml blocks).
