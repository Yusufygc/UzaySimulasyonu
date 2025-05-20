/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#include "RockyPlanet.h"
#include <stdlib.h>


struct RockyPlanet {
    Planet super;  
};

/* ----- sanal metodlar ----- */

static void rocky_advance_hours(Planet* p, int hours)
{
    planet_base_advance_hours(p, hours);
}

static double rocky_aging_factor(const Planet* p)
{
    return planet_base_aging_factor(p);
}

static void rocky_add_population(Planet* p, size_t cnt)
{
    planet_base_add_population(p, cnt);
}

/* ----- v-tablo ----- */

static const PlanetVTable rocky_vtbl = {
    .advance_hours  = rocky_advance_hours,
    .aging_factor   = rocky_aging_factor,
    .add_population = rocky_add_population,
    .destroy        = NULL
};

/* ----- ctor / dtor ----- */

Planet* rocky_planet_create(const char* name,
                            int hours_per_day,
                            int day, int month, int year)
{
    
    RockyPlanet* rp = malloc(sizeof *rp);
    if (!rp) return NULL;

    planet_init(&rp->super, &rocky_vtbl,
                name, hours_per_day, day, month, year);
    return &rp->super;
}

void rocky_planet_destroy(Planet* p)
{
    if (!p) return;
    planet_destroy(p);
}
