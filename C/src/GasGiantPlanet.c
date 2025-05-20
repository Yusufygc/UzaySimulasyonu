/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#include "GasGiantPlanet.h"
#include <stdlib.h>


struct GasGiantPlanet {
    Planet super; 
};

/* -------- sanal metodlar -------- */
static void gg_advance_hours(Planet* p, int hrs)
{
    planet_base_advance_hours(p, hrs);
}

/* Gaz Devi → kişi 10× daha uzun yaşar → yaşlanma ×0.1 */
static double gg_aging_factor(const Planet* p)
{
    return planet_base_aging_factor(p) * 0.1;
}

static void gg_add_population(Planet* p, size_t cnt)
{
    planet_base_add_population(p, cnt);
}

/* -------- v-tablo -------- */
static const PlanetVTable gg_vtbl = {
    .advance_hours  = gg_advance_hours,
    .aging_factor   = gg_aging_factor,
    .add_population = gg_add_population,
    .destroy        = NULL   
};

/* -------- ctor / dtor -------- */
Planet* gas_giant_planet_create(const char* name,
                                int hours_per_day,
                                int day, int month, int year)
{
    GasGiantPlanet* gg = malloc(sizeof *gg);
    if (!gg) return NULL;

    planet_init(&gg->super,
                &gg_vtbl,
                name,
                hours_per_day,
                day, month, year);
    return &gg->super;
}

void gas_giant_planet_destroy(Planet* p)
{
    if (!p) return;
    planet_destroy(p);
}
