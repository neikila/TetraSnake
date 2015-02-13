#ifndef SETTINGS_H
#define SETTINGS_H

class FieldSettings
{
public:
    FieldSettings();
    int GetW();
    int GetH();
private:
    int WidthOfField;
    int HeightOfField;
};

#endif
