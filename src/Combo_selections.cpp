#include <DCS_AB_detent_editor.h>
#include <AModule.h>

#include "imgui.h"

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
