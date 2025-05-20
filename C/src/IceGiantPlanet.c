/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#include "IceGiantPlanet.h"
#include <stdlib.h>


struct IceGiantPlanet {
    Planet super;  
};

/* -------- sanal metodlar -------- */

static void ig_advance_hours(Planet* p, int hrs)
{
    planet_base_advance_hours(p, hrs);
}

/* Buz Devi → kişi 2 kat daha uzun yaşar → yaşlanma ×0.5 */
static double ig_aging_factor(const Planet* p)
{
    return planet_base_aging_factor(p) * 0.5;
}

static void ig_add_population(Planet* p, size_t cnt)
{
    planet_base_add_population(p, cnt);
}

/* -------- v-tablo -------- */

static const PlanetVTable ig_vtbl = {
    .advance_hours  = ig_advance_hours,
    .aging_factor   = ig_aging_factor,
    .add_population = ig_add_population,
    .destroy        = NULL  
};

/* -------- ctor / dtor -------- */

Planet* ice_giant_planet_create(const char* name,
                                int hours_per_day,
                                int day, int month, int year)
{
    IceGiantPlanet* ig = malloc(sizeof *ig);
    if (!ig) return NULL;

    planet_init(&ig->super,
                &ig_vtbl,
                name,
                hours_per_day,
                day, month, year);
    return &ig->super;
}

void ice_giant_planet_destroy(Planet* p)
{
    if (!p) return;
    planet_destroy(p);
}
