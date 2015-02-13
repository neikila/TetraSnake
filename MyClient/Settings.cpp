#include <stdlib.h>
#include "Settings.h"
#include "tinyxml.h"
#include "tinystr.h"

/* 	Field		*/
FieldSettings::FieldSettings()
{
    TiXmlDocument *xml_file = new TiXmlDocument("FieldSettings.xml");
    if(!xml_file->LoadFile())
        exit(3);

    TiXmlElement *xml_level = 0;
    TiXmlElement *xml_body = 0;
    TiXmlElement *xml_entity = 0;

    xml_level = xml_file->FirstChildElement("level");
    xml_entity = xml_level->FirstChildElement("entity");


    xml_body = xml_entity->FirstChildElement("body");
    HeightOfField = atoi(xml_body->Attribute("Height"));
    if (HeightOfField < 15)
        HeightOfField = 15;
    xml_body = xml_body->NextSiblingElement("body");
    WidthOfField = atoi(xml_body->Attribute("Width"));
    if (WidthOfField < 10)
        WidthOfField = 10;
    delete xml_file;
}

int FieldSettings::GetW()
{
    return WidthOfField;
}

int FieldSettings::GetH()
{
    return HeightOfField;
}
