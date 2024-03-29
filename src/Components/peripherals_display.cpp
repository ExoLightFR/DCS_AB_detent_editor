#include "Peripheral.hpp"

/* ============================================================================================== */
/* ========================================== HELPERS =========================================== */
/* ============================================================================================== */

static BOOL CALLBACK	InsertPeripheralCallback(LPCDIDEVICEINSTANCE instance, LPVOID userData)
{
	auto data = reinterpret_cast<
		std::pair<std::vector<Peripheral>*, IDirectInput8*>*
	>(userData);
	auto& [peripherals, dinput] = *data;

	IDirectInputDevice8* device_raw;
	dinput->CreateDevice(instance->guidInstance, &device_raw, NULL);
	device_raw->SetCooperativeLevel(GetActiveWindow(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	device_raw->SetDataFormat(&c_dfDIJoystick);
	device_raw->Acquire();

	peripherals->emplace_back(device_raw, instance->guidInstance,
		InteropString::wcs_to_mbs(instance->tszInstanceName));

	return DIENUM_CONTINUE;
}

static std::vector<Peripheral>	get_peripherals()
{
	std::vector<Peripheral> peripherals;

	IDirectInput8 *dinput = nullptr;
	HINSTANCE hInstance = GetModuleHandle(0);
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, 0);

	auto data = std::make_pair(&peripherals, dinput);
	dinput->EnumDevices(DI8DEVCLASS_GAMECTRL, InsertPeripheralCallback,
		(void*)&data, DIEDFL_ALLDEVICES);

	dinput->Release();	// TESTME
	return peripherals;
}

static Axis	*get_default_selected_axis(Peripheral *periph)
{
	if (!periph || periph->axies.empty())
		return nullptr;
	return &periph->axies[0];
}

/* ============================================================================================== */
/* ========================================= COMPONENTS ========================================= */
/* ============================================================================================== */

static void component_help_marker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

static void component_peripheral_combo(std::vector<Peripheral> &periphs,
	Peripheral **p_selected_periph, Axis **p_selected_axis)
{
	Peripheral	*selected_periph = *p_selected_periph;
	Axis		*selected_axis = *p_selected_axis;

	std::string_view preview = (selected_periph ? selected_periph->name.c_str() : "No device found");
	if (ImGui::BeginCombo("Device", preview.data()))
	{
		for (auto& periph : periphs)
		{
			if (ImGui::Selectable(periph.name.c_str(), selected_periph == &periph))
			{
				selected_periph = &periph;
				selected_axis = get_default_selected_axis(selected_periph);
			}
			if (selected_periph == &periph)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	*p_selected_periph = selected_periph;
	*p_selected_axis = selected_axis;
}

static void component_axis_combo(std::vector<Peripheral>& periphs,
	Peripheral** p_selected_periph, Axis** p_selected_axis)
{
	Peripheral* selected_periph = *p_selected_periph;
	Axis* selected_axis = *p_selected_axis;

	std::string preview = selected_axis ? selected_axis->name.c_str() : "No axies found";
	if (ImGui::BeginCombo("Axis", preview.data()))
	{
		for (auto& axis : selected_periph->axies)
		{
			if (ImGui::Selectable(axis.name.c_str()))
				selected_axis = &axis;
			if (selected_axis == &axis)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	*p_selected_periph = selected_periph;
	*p_selected_axis = selected_axis;
}

/*
* The main component. Displays two Combos that contain the list of devices and the list of
* axies for currently selected device.
* Returns the axis position in range [0, UINT16_MAX], and whether axis is OK or not.
*/
std::pair<uint16_t, bool>	peripheral_and_axis_combo()
{
	static std::vector<Peripheral>	periphs = get_peripherals();
	static Peripheral	*selected_periph	= periphs.size() ? &periphs[0] : nullptr;
	static Axis			*selected_axis		= get_default_selected_axis(selected_periph);

	auto refresh_all = [&] {
		periphs = get_peripherals();
		selected_periph = periphs.size() ? &periphs[0] : nullptr;
		selected_axis = get_default_selected_axis(selected_periph);
		};

	if (ImGui::Button("Refresh device list"))
		refresh_all();
	ImGui::SameLine();
	component_help_marker("If you don't find your device in this list, make sure it's plugged in \
your computer correctly, and then click this button.");

	component_peripheral_combo(periphs, &selected_periph, &selected_axis);
	if (!selected_periph)
		return { UINT16_MAX, false };

	component_axis_combo(periphs, &selected_periph, &selected_axis);

	DIJOYSTATE state;
	if (selected_periph->di_device->GetDeviceState(sizeof(state), &state) != DI_OK)
	{
		refresh_all();
		return { UINT16_MAX, false };
	}
	else
	{
		if (selected_axis)
			return { selected_axis->get_value(state), true };
		else
			return { UINT16_MAX, false };
	}

}
