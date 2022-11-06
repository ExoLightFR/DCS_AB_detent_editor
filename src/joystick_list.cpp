#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include <SDL.h>

#define ARRAY_LEN(x)    sizeof(x) / sizeof(x[0])

int display_joysticks_Combo()
{
	static int  selected = 0;
	const int	numjoysticks = SDL_NumJoysticks();
	const char  *preview_name = SDL_JoystickNameForIndex(selected);
    if (preview_name == NULL)
        preview_name = "No joystick found";

    if (ImGui::BeginCombo("Peripheral", preview_name))
    {
        for (int i = 0; i < numjoysticks; ++i)
        {
            if (ImGui::Selectable(SDL_JoystickNameForIndex(i)))
                selected = i;
            if (selected == i)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    return numjoysticks == 0 ? -1 : selected;
}

int display_joystick_axies_Combo(SDL_Joystick *stick)
{
    static int  selected = 0;
    const int   num_axies = SDL_JoystickNumAxes(stick);
    const char  *axis_names[] = {"JOY_X", "JOY_Y", "JOY_Z", "JOY_RX", "JOY_RY", "JOY_RZ", "Slider", "Slider 2"};
    const char  *preview_name = axis_names[selected];

    if (num_axies == 0 || stick == NULL)
        preview_name = "No axis found";
    if (selected >= num_axies)
        selected = 0;
    if (ImGui::BeginCombo("Axis", preview_name))
    {
        for (int i = 0; i < num_axies && i < ARRAY_LEN(axis_names); ++i)
        {
            if (ImGui::Selectable(axis_names[i]))
                selected = i;
            if (selected == i)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    return selected;
}
