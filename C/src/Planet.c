/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#include "Planet.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



static char* duplicate_string(const char* s)
{
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* copy = (char*)malloc(len);
    if (copy) memcpy(copy, s, len);
    return copy;
}



void planet_base_advance_hours(Planet* p, int hours)
{
    simtime_advance_hours(&p->current_time, hours);
}

double planet_base_aging_factor(const Planet* p)
{
    (void)p;
    return 1.0;
}

void planet_base_add_population(Planet* p, size_t count)
{
    p->population += count;
}



void planet_init(Planet* p,
                 const PlanetVTable* vtable,
                 const char* name,
                 int hours_per_day,
                 int day, int month, int year)
{
    if (!p || !vtable) {
        fprintf(stderr, "Planet init: null pointer!\n");
        return;
    }
    p->vptr = vtable;
    p->name = duplicate_string(name);
    if (!p->name) {
        fprintf(stderr, "Planet init: malloc failed for name\n");
        /* çağıran free ettiği sürece yapıyı bırakıyoruz */
    }
    simtime_init(&p->current_time, day, month, year, hours_per_day);
    p->population = 0;
}


void planet_destroy(Planet* p)
{
    if (!p) return;
    if (p->vptr && p->vptr->destroy && p->vptr->destroy != planet_destroy)
        p->vptr->destroy(p);          

    free(p->name);
    free(p);
}

/* -----------------------------------------------------------
 *   Getter / Setter
 * ----------------------------------------------------------- */

const char* planet_get_name(const Planet* p)               { return p->name; }
const SimTime* planet_get_current_time(const Planet* p)    { return &p->current_time; }
size_t planet_get_population(const Planet* p)              { return p->population; }
void   planet_set_population(Planet* p, size_t pop)        { p->population = pop; }



void planet_advance_hours(Planet* p, int hours)
{
    p->vptr->advance_hours(p, hours);
}

double planet_get_aging_factor(const Planet* p)
{
    return p->vptr->aging_factor(p);
}

void planet_add_population(Planet* p, size_t count)
{
    p->vptr->add_population(p, count);
}
