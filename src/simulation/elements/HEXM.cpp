#include "simulation/ElementCommon.h"
#include <cmath>

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_HEXM()
{
	Identifier = "DEFAULT_PT_HEXM";
	Name = "HEXM";
	Colour = 0xA0C0A0_rgb;
	MenuVisible = 1;
	MenuSection = SC_EXPLOSIVE;
	Enabled = 1;

	Advection = 0.7f;
	AirDrag = 0.02f * CFDS;
	AirLoss = 0.96f;
	Loss = 0.80f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = 15;

	HeatConduct = 70;
	Description = "Hexamine. Powdery substance produced when dilute BASE and GAS collide. Absorbs LOXY and becomes more explosive.";

	Properties = TYPE_PART;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
	Graphics = &graphics;
}

static int update(UPDATE_FUNC_ARGS) {
	if (surround_space == 8) {return 0;}
	
	for (auto offset_x = -1; offset_x <= 1; offset_x++)
	{
		for (auto offset_y = -1; offset_y <= 1; offset_y++) {
			if (offset_x == 0 && offset_y == 0) {continue;}
			auto r = pmap[y+offset_y][x+offset_x];

			if (TYP(r) == PT_LO2 && parts[i].life < 10) { 
				sim->kill_part(ID(r));
				parts[i].life += 1;
			}		
		}
	}
	auto offset_x = sim->rng.between(-2, 2);
	auto offset_y = sim->rng.between(-2, 2); // i only want this to run once per update, so no for loop
	auto r = pmap[y+offset_y][x+offset_x];
	if (TYP(r) == PT_HEXM && (offset_x || offset_y) && (parts[i].life > parts[ID(r)].life) && sim->rng.chance(1, 5) ) { // diffuse
		parts[i].life -= 1;
		parts[ID(r)].life += 1;
	}
	
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS) {
	*colr = 0xA0;
	*colg = 0xC0;
	int clamped_colb; // i don't know c++
	auto unclamped_colb = 0xA0 + (std::pow((float)cpart->life, 1.978f));
	if (unclamped_colb > 255) {clamped_colb = 255;}
	else if (unclamped_colb < 0) {clamped_colb = 0;}
	else {clamped_colb = unclamped_colb;}
	*colb = std::floorf(clamped_colb);
	return 0;
}
