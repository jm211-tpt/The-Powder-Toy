#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_GAS()
{
	Identifier = "DEFAULT_PT_GAS";
	Name = "GAS";
	Colour = 0xE0FF20_rgb;
	MenuVisible = 1;
	MenuSection = SC_GAS;
	Enabled = 1;

	Advection = 1.0f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.30f;
	Collision = -0.1f;
	Gravity = 0.0f;
	Diffusion = 0.75f;
	HotAir = 0.001f	* CFDS;
	Falldown = 0;

	Flammable = 600;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 1;

	DefaultProperties.temp = R_TEMP + 2.0f + 273.15f;
	HeatConduct = 42;
	Description = "Diffuses quickly and is flammable. Liquefies into OIL under pressure.";

	Properties = TYPE_GAS | PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = 6.0f;
	HighPressureTransition = PT_OIL;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 573.0f;
	HighTemperatureTransition = PT_FIRE;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS) {
	if (surround_space == 8) {return 0;}
	
	for (auto offset_x = -1; offset_x <= 1; offset_x++)
	{
		for (auto offset_y = -1; offset_y <= 1; offset_y++) {
			if (offset_x == 0 && offset_y == 0) {continue;}

			auto r = pmap[y+offset_y][x+offset_x]; // i would have called this n_pmap, why is it r lol
			
			if (TYP(r) == PT_BASE && (parts[ID(r)].life > 3)) {

				if ((sim->rng.chance(1, parts[ID(r)].life + 10)) && parts[ID(r)].life > 3) { // decrease the BASE's life by 3 and convert into HEXM when in contact with BASE
					sim->part_change_type(i, x, y, PT_HEXM);
					parts[ID(r)].life -= 3;  
				}
			}
			
		}
	}
	
	return 0;
}
