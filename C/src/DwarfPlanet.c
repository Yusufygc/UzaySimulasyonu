/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#include "DwarfPlanet.h"
#include <stdlib.h>

// "Planet" üst sınıfından kalıtım 
struct DwarfPlanet {
    Planet super;  
};

/* -------- sanal metodlar -------- */
static void dp_advance_hours(Planet* p, int hrs)
{
    planet_base_advance_hours(p, hrs);
}

/* Cüce gezegen → kişi 100× daha uzun yaşar → yaşlanma ×0.01 */
static double dp_aging_factor(const Planet* p)
{
    return planet_base_aging_factor(p) * 0.01;
}

/**
 * dp_add_population:
 *   - Gezegene nüfus ekleme/çıkarma işlemi
 *   - "count" kadar nüfus artışı
 */
static void dp_add_population(Planet* p, size_t cnt)
{
    planet_base_add_population(p, cnt);
}

/* -------- v-tablo -------- */
static const PlanetVTable dp_vtbl = {
    .advance_hours  = dp_advance_hours,
    .aging_factor   = dp_aging_factor,
    .add_population = dp_add_population,
    .destroy        = NULL   
};

/* -------- ctor / dtor -------- */
/**
 * dwarf_planet_create:
 *   - heap üzerinde yeni DwarfPlanet oluşturur
 *   - "Planet" üst sınıfını doğru v-tablo ile başlatır
 *   - Başarısız malloc durumunda NULL döner
 */
Planet* dwarf_planet_create(const char* name,
                            int hours_per_day,
                            int day, int month, int year)
{
    DwarfPlanet* dp = malloc(sizeof *dp);
    if (!dp) return NULL;

    planet_init(&dp->super,
                &dp_vtbl,
                name,
                hours_per_day,
                day, month, year);
    return &dp->super;
}

/**
 * dwarf_planet_destroy:
 *   - heap üzerinde oluşturulmuş Planet*'i serbest bırakır
 *   - Planet içindeki kaynaklar (name) de temizlenir
 */
void dwarf_planet_destroy(Planet* p)
{
    if (!p) return;
    planet_destroy(p);
}
