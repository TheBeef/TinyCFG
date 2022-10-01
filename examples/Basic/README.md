This is a basic example of using TinyCFG.

It makes a global structure for settings.  The structure has 3 members:

```
RegisterAllMembers()
SaveSettings()
LoadSettings()
```

The RegisterAllMembers() function connects all the data in the structure to the variables and gives them xml names.

The LoadSettings() loads the "Settings.xml" into the current object.

The SaveSettings() saves the current object data into "Settings.xml".

# Compiling
```
g++ ../../TinyCFG.cpp Basic.cpp -o Basic
```

# Generated xml
```
<?xml version="1.0" standalone="no" ?>
<Settings>
    <DefaultName>Paul</DefaultName>
    <DefaultAge>25</DefaultAge>
    <Names>
        <Data>Paul</Data>
        <Data>Ted</Data>
        <Data>Sarah</Data>
    </Names>
</Settings>
```