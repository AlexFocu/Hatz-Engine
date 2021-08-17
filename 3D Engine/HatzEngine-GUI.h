#pragma once
#include "olcPGEX_FrostUI.h"

class GUI
{
private:

    olc::FrostUI ui;

    std::shared_ptr<olc::FUI_Element> meshX_ref;
    std::shared_ptr<olc::FUI_Element> meshY_ref;
    std::shared_ptr<olc::FUI_Element> meshZ_ref;

    std::shared_ptr<olc::FUI_Element> meshTag_ref;

    float posX;
    float posY;
    float posZ;

    bool isColliding = false;

public:

    void Create();

    void Run(std::string& name, std::string& tag, float* x, float& y, float& z);

    //Send the mesh position
    void UI_SendPosition();

    //Check to see if there is a letter/symbol 
    bool UI_CheckForMistakes(std::string& sText);

    //Create all the tags
    void UI_CreateTags();
};