#define OLC_PGEX_FUI
#include "HatzEngine-GUI.h"
#include "HatzEngine.h"

#include<ctype.h>

void GUI::Create()
{
    auto debug_window = ui.create_window("debug_window", "DUBUG", { 0, 0 }, { 450, 270 });

    //Set origin on window
    ui.set_active_window("debug_window");

    ui.add_groupbox("groupbox2", "", { 5, 5 }, { 430, 235 });

    olc::vi2d groupbox_pos = ui.find_groupbox("groupbox2")->get_position();

    //Mesh Position
    ui.add_inputfield("meshX", "X: ", { groupbox_pos.x + 40, groupbox_pos.y + 20 }, { 150, 20 });
    ui.add_inputfield("meshY", "Y: ", { groupbox_pos.x + 40, groupbox_pos.y + 45 }, { 150, 20 });
    ui.add_inputfield("meshZ", "Z: ", { groupbox_pos.x + 40, groupbox_pos.y + 70 }, { 150, 20 });

    meshX_ref = ui.find_element("meshX");
    meshY_ref = ui.find_element("meshY");
    meshZ_ref = ui.find_element("meshZ");

    //Mesh Tag
    ui.add_dropdown("meshTag", "Tag: ", { groupbox_pos.x + 300, groupbox_pos.y + 20 }, { 100, 15 });

    meshTag_ref = ui.find_element("meshTag");

    UI_CreateTags();

    //Mesh Collision
    ui.add_checkbox("collisionBox", "Collision: ", { groupbox_pos.x + 80, groupbox_pos.y + 110 }, { 10, 10}, &isColliding);

}

void GUI::Run(std::string& name, std::string& tag, float* x, float& y, float& z)
{
    ui.run();

    //set input field string to x,y,z & checkbox 
    meshX_ref->inputfield_text = std::to_string(*x);
    meshY_ref->inputfield_text = std::to_string(y);
    meshZ_ref->inputfield_text = std::to_string(z);

    //Set the tag of the mesh
    meshTag_ref->set_item(tag);

    UI_SendPosition();

}

//Send the mesh position
void GUI::UI_SendPosition()
{
    //Get position from boxes
    if (meshX_ref->is_enterPressed)
    {
        std::string meshX_input = meshX_ref->get_inputfield_value();

        if (!UI_CheckForMistakes(meshX_input))
        {
            meshX_ref->set_text_color(olc::BLACK);
            
            std::cout << "sent 1 !" << std::endl;

            posX = std::stof(meshX_input);
        }
        else
        {
            meshX_ref->set_text_color(olc::DARK_RED);
        }
    }
    else if (meshY_ref->is_enterPressed)
    {
        std::string meshY_input = meshY_ref->get_inputfield_value();

        if (!UI_CheckForMistakes(meshY_input))
        {
            meshY_ref->set_text_color(olc::BLACK);

            std::cout << "sent 2 !" << std::endl;
            
            posY = std::stof(meshY_input);
        }
        else
        {
            meshY_ref->set_text_color(olc::DARK_RED);
        }
    }
    else if (meshZ_ref->is_enterPressed)
    {
        std::string meshZ_input = meshZ_ref->get_inputfield_value();

        if (!UI_CheckForMistakes(meshZ_input))
        {
            meshZ_ref->set_text_color(olc::BLACK);

            std::cout << "sent 3 !" << std::endl;
            
            posZ = std::stof(meshZ_input);
        }
        else
        {
            meshZ_ref->set_text_color(olc::DARK_RED);
        }
    }
}

bool GUI::UI_CheckForMistakes(std::string& sText)
{
    for (char& c : sText)
    {
        if (isalpha(c) || 
            c == '/' || c == '.' || c == ';' || 
            c == ':' || c == '[' || c == ']' ||
            c == '?' || c == '>' || c == '<' ||
            c == '{' || c == '}')
        {
            std::cout << "ERROR!" << std::endl;
            return true;
        }
    }

    return false;
}

void GUI::UI_CreateTags()
{
    meshTag_ref->add_item("Default");
    meshTag_ref->add_item("Player");
    meshTag_ref->add_item("Ground");
}