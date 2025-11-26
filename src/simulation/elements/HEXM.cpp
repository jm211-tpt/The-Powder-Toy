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
	// bool foundSAWD = false;
	// bool foundOIL = false;
	// int sawdID = -1;
	// int oilID = -1;
	for (auto rx = -1; rx <= 1; rx++)
	{
		for (auto ry = -1; ry <= 1; ry++) 
		{
			if (rx == 0 && ry == 0) {continue;}
			auto r = pmap[y+ry][x+rx];
			if (r != 0)
			{
				if (TYP(r) == PT_LO2 && parts[i].life < 10)
				{
					sim->kill_part(ID(r));
					parts[i].life += 1;
				}
				else if (TYP(r) == PT_ACID) // && sim->rng.chance(1, 2))
				{
					// sim->kill_part(ID(r));
					sim->part_change_type(ID(r), x, y, PT_NITR);
					sim->part_change_type(i, x, y, PT_NITR);
				}
			}
			else
			{
				if (parts[i].life > 0 && sim->pv[y / CELL][x / CELL] <= 20 && sim->rng.chance(parts[i].life, static_cast<unsigned int>(266 + sim->pv[y / CELL][x / CELL])))
				{
					int np = sim->create_part(-1, x + rx, y + ry, PT_FIRE);
					if (np > -1)
					{
						parts[i].life -= 1;
						sim->pv[y / CELL][x / CELL] += 0.5;
						parts[np].temp = std::min(3773.15f + sim->rng.between(0, 200), parts[i].temp + 200.0f);
					}
				}
				else if (sim->pv[y / CELL][x / CELL] <= -200 && parts[i].temp >= 553.15 && sim->rng.chance(1, 20) && parts[i].life == 0)
				{
					int np = sim->create_part(-1, x + rx, y + ry, PT_GAS);
					if (np > -1)
					{
						sim->part_change_type(i, x, y, PT_BASE);
						parts[i].life = 1;
						parts[np].temp = parts[i].temp;
						sim->pv[y / CELL][x / CELL] += 2.5;
					}
				}

			}
			// else if (TYP(r) == PT_SAWD)
			// {
			// 	foundSAWD = true;
			// 	sawdID = ID(r);
			// }
			// else if (TYP(r) == PT_OIL)
			// {
			// 	foundOIL = true;
			// 	oilID = ID(r);
			// }
		}
	}
	auto r2x = sim->rng.between(-2, 2);
	auto r2y = sim->rng.between(-2, 2); // i only want this to run once per update, so no for loop
	auto r = pmap[y+r2y][x+r2x];
	if (TYP(r) == PT_HEXM && (r2x || r2y) && (parts[i].life > parts[ID(r)].life) && sim->rng.chance(1, 5) ) { // diffuse
		parts[i].life -= 1;
		parts[ID(r)].life += 1;
	}
	// if (foundSAWD && foundOIL && sim->rng.chance(1,5))
	// {
	// 		float newT = (parts[i].temp + parts[sawdID].temp + parts[oilID].temp)/2; // average temps and then multiply by 1.5
	// 		sim->kill_part(sawdID);
	// 		sim->kill_part(oilID);
	// 		sim->part_change_type(i, x, y, PT_GEL);
	// 		parts[i].temp = newT;
	// }
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS) {
	*colr = 0xA0;
	*colg = 0xC0;
	float unclamped_colb = 0xA0 + (std::pow((float)cpart->life, 1.978f));
	float clamped_colb = std::max(0.0f, std::min(255.0f, unclamped_colb));
	clamped_colb = unclamped_colb;
	*colb = static_cast<int>(std::floorf(clamped_colb));
	return 0;
}
