#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Windows.h>

// Deleter struct for RAII with smart pointers
struct Deleter
{
	void	operator()(IDirectInputDevice8* device)	{ device->Release(); }
	void	operator()(IDirectInput8* dinput)		{ dinput->Release(); }
};

struct Axis
{
	std::string	name;
	GUID		type;
	std::function<uint16_t(DIJOYSTATE&)> get_value;
};

class Peripheral
{
public:
	using unique_ptr_DIDevice = std::unique_ptr<IDirectInputDevice8, Deleter>;

// private:
	unique_ptr_DIDevice	di_device;
	GUID				guid;
	std::vector<Axis>	axies;
	void	insert_Axis_getter_functor(LPCDIDEVICEOBJECTINSTANCE device_object);

public:
	std::string			name;

	Peripheral(IDirectInputDevice8* _dev, GUID _guid, std::string _name)
		: di_device(_dev, Deleter{}), guid(_guid), name(_name)
	{
		auto add_axis = [](LPCDIDEVICEOBJECTINSTANCE device_object, LPVOID user_data) -> BOOL
			{
				Peripheral* periph = reinterpret_cast<Peripheral*>(user_data);
				periph->insert_Axis_getter_functor(device_object);
				return TRUE;
			};
		di_device->EnumObjects(add_axis, (void*)this, DIDFT_AXIS);
	}
};
