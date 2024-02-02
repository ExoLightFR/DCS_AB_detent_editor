#include "DCS_AB_detent_editor.h"

struct Slopes {
	float	mil_slope = 0;
	float	ab_slope = 0;
	float	mil_int = 0;
	float	ab_int = 0;
	bool	inverted = false;
};

static axis_100	axis_value_to_normed_floating(uint16_t axis_val, bool invert)
{
	float		axis_val_f	= axis_val;
	axis_1		pos_0_1		= invert ? (axis_val_f / UINT16_MAX) : (1 - axis_val_f / UINT16_MAX);
	axis_100	pos_0_100	= std::clamp(pos_0_1 * 100, 0.0f, 100.0f);
	return pos_0_100;
}

inline float	get_mil_slope(float module_detent, axis_100 throttle, bool invert)
{
	if (invert)
		return -(100 - module_detent) / throttle;
	else
		return (100 - module_detent) / (100 - throttle);
}

inline float	get_afterburner_slope(float module_detent, axis_100 throttle, bool invert)
{
	if (invert)
		return -module_detent / (100 - throttle);
	else
		return module_detent / throttle;
}

static float	get_curve_point_setting(axis_100 point, axis_100 HOTAS_detent, Slopes const& slopes)
{
	if (slopes.inverted)
	{
		if (point < HOTAS_detent)
			return point * slopes.mil_slope + slopes.mil_int;
		else
			return point * slopes.ab_slope + slopes.ab_int;
	}
	else
	{
		if (point < HOTAS_detent)
			return point * slopes.ab_slope + slopes.ab_int;
		else
			return point * slopes.mil_slope + slopes.mil_int;
	}
}

std::array<float, 11>	get_custom_curves(float module_detent, axis_100 throttle, bool invert)
{
	// axis_100	throttle	= axis_value_to_normed_floating(axis_val, invert);
	float		mil_slope	= get_mil_slope(module_detent, throttle, invert);
	float		ab_slope	= get_afterburner_slope(module_detent, throttle, invert);
	Slopes		slopes = {
		.mil_slope	= mil_slope,
		.ab_slope	= ab_slope,
		.mil_int	= invert ? 100 : -(mil_slope - 1) * 100,	// What do these "int" mean? Too sleep deprived rn
		.ab_int		= invert ? -ab_slope * 100 : 0,
		.inverted	= invert
	};

	std::array<float, 11>	curves = {};
	for (int i = 0; i <= 10; ++i)
	{
		curves[i] = get_curve_point_setting((axis_100)i * 10, throttle, slopes);
	}
	return curves;
}
