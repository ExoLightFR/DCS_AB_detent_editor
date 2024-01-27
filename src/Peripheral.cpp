#include <functional>
#include <string>
#include <map>
#include <cassert>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Peripheral.hpp"
#include "InteropString.hpp"

using std::map, std::string, std::function, std::vector;

static bool	operator<(GUID const& a, GUID const& b)
{
	int less_than = a.Data1 < b.Data1
		|| a.Data2 < b.Data2
		|| a.Data3 < b.Data3;
	for (unsigned int i = 0; i < 8; ++i)
	{
		less_than |= (a.Data4[i] < b.Data4[i]);
	}
	return less_than;
}

static uint16_t	get_x(DIJOYSTATE& state)	{ return (uint16_t)state.lX; }
static uint16_t	get_y(DIJOYSTATE& state)	{ return (uint16_t)state.lY; }
static uint16_t	get_z(DIJOYSTATE& state)	{ return (uint16_t)state.lZ; }
static uint16_t	get_rx(DIJOYSTATE& state)	{ return (uint16_t)state.lRx; }
static uint16_t	get_ry(DIJOYSTATE& state)	{ return (uint16_t)state.lRy; }
static uint16_t	get_rz(DIJOYSTATE& state)	{ return (uint16_t)state.lRz; }
static uint16_t	get_s1(DIJOYSTATE& state)	{ return (uint16_t)state.rglSlider[0]; }
static uint16_t	get_s2(DIJOYSTATE& state)	{ return (uint16_t)state.rglSlider[1]; }

void	Peripheral::insert_Axis_getter_functor(LPCDIDEVICEOBJECTINSTANCE device_object)
{
	static const map<GUID, std::function<uint16_t(DIJOYSTATE&)>>	functors = {
		{GUID_XAxis, get_x},
		{GUID_YAxis, get_y},
		{GUID_ZAxis, get_z},
		{GUID_RxAxis, get_rx},
		{GUID_RyAxis, get_ry},
		{GUID_RzAxis, get_rz},
		{GUID_Slider, get_s1},
	};

	auto tgt = functors.find(device_object->guidType);
	if (tgt == functors.end())
		return;

	auto& [guid, fn] = *tgt;
	string name = InteropString::wcs_to_mbs(device_object->tszName);

	bool slider_already_present = false;
	for (auto const& axis : axies)
	{
		if (axis.type == GUID_Slider)
			slider_already_present = true;
	}

	// GUID can't differenciate between slider 1 & 2, have to check if it's already there
	if (guid == GUID_Slider && slider_already_present)
		axies.push_back(Axis{ name, guid, get_s2 });
	else
		axies.push_back(Axis{ name, guid, fn });
}
