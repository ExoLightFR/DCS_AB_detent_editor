#include <DCS_AB_detent_editor.h>
#include <AModule.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include <SDL.h>

#include <memory>
#include <vector>

#define ARRAY_LEN(x)    sizeof(x) / sizeof(x[0])

// Returns a vector of shared_ptr<AModule>, which contains all of the supported modules by the software.
// AModule is derived by classes of specific modules, see AModule.h
static std::vector< std::shared_ptr<AModule> >
    _get_modules_vector(std::string const &DCS_path)
{
    std::vector< std::shared_ptr<AModule> >    modules;
    for (auto i : AModule::supported_modules)
        modules.push_back(AModule::getModule(DCS_path, i));
    modules.shrink_to_fit();
    return modules;
}

// Returns first module in vector with is installed. If none are installed, return first element of vector.
inline std::shared_ptr<AModule>
    _get_first_installed_module(std::vector< std::shared_ptr<AModule> > const &modules)
{
    auto first_installed_module = std::find_if(modules.begin(), modules.end(),
        [](auto const &x) { return x->is_installed(); });

    return (first_installed_module == modules.end() ? modules[0] : *first_installed_module);
}

std::shared_ptr<AModule> selected_module_Combo(std::string const &DCS_path)
{
    static auto modules = _get_modules_vector(DCS_path);
    static std::shared_ptr<AModule> selected = _get_first_installed_module(modules);
    static std::string old_DCS_path = DCS_path;

    if (old_DCS_path != DCS_path)   // DCS path changed by user, update modules and selected
    {
        modules = _get_modules_vector(DCS_path);
        selected = _get_first_installed_module(modules);
        old_DCS_path = DCS_path;
    }

    const bool no_modules = selected->is_installed() == false;
    const char *preview = (no_modules ? "No modules found" : selected->name().c_str());

    if (no_modules)
        ImGui::BeginDisabled();
    if (ImGui::BeginCombo("Module", preview))
    {
        for (auto &i : modules)
        {
            if (!i->is_installed())
            {
                ImGui::BeginDisabled();
                ImGui::Selectable(i->name().c_str());
                ImGui::EndDisabled();
            }
            else
            {
                if (ImGui::Selectable(i->name().c_str(), selected == i))
                    selected = i;
                if (selected == i)
                    ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    if (no_modules)
        ImGui::EndDisabled();

    return selected;
}


int display_joysticks_Combo()
{
	static int  selected = 0;
	const int	num_joysticks = SDL_NumJoysticks();
	const char  *preview_name = SDL_JoystickNameForIndex(selected);

    if (num_joysticks == 0)
    {
        preview_name = "No joystick found";
        ImGui::BeginDisabled();
    }
    if (ImGui::BeginCombo("Peripheral", preview_name))
    {
        for (int i = 0; i < num_joysticks; ++i)
        {
            if (ImGui::Selectable(SDL_JoystickNameForIndex(i), selected == i))
                selected = i;
            if (selected == i)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (num_joysticks == 0)
        ImGui::EndDisabled();
    return num_joysticks == 0 ? -1 : selected;
}

int display_joystick_axies_Combo(SDL_Joystick *stick)
{
    static int  selected = 0;
    const int   num_axies = SDL_JoystickNumAxes(stick);
    const char  *axis_names[] = {"JOY_X", "JOY_Y", "JOY_Z", "JOY_RX", "JOY_RY", "JOY_RZ", "Slider", "Slider 2"};
    const char  *preview_name = axis_names[selected];

    // Resets selected if you had selected an axis on a previous stick that doesn't exist on this stick
    if (selected >= num_axies)
        selected = 0;
    if (num_axies <= 0)
    {
        preview_name = "No axis found";
        ImGui::BeginDisabled();
    }
    if (ImGui::BeginCombo("Axis", preview_name))
    {
        for (int i = 0; i < num_axies && i < ARRAY_LEN(axis_names); ++i)
        {
            if (ImGui::Selectable(axis_names[i], selected == i))
                selected = i;
            if (selected == i)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (num_axies <= 0)
        ImGui::EndDisabled();
    return num_axies <= 0 ? -1 : selected;
}

