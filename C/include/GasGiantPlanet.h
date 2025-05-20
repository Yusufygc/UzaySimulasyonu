/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#ifndef GAS_GIANT_PLANET_H
#define GAS_GIANT_PLANET_H

#include "Planet.h"


typedef struct GasGiantPlanet GasGiantPlanet;

/* --------------------------------------------------
 *   Gaz Devi gezegen oluşturucu:
 *   - name: gezegen adı (heap’te kopyalanır)
 *   - hours_per_day: gezegende bir günün kaç saat olduğu
 *   - day, month, year: gezegenin başlangıç tarihi
 *   Başarılı → Planet* (up-cast edilmiş), Başarısız → NULL
 * -------------------------------------------------- */
Planet* gas_giant_planet_create(const char* name,
                                int hours_per_day,
                                int day, int month, int year);
/* --------------------------------------------------
 *   Gaz Devi gezegen imha edici:
 *   - p: heap’te oluşturulmuş Planet* göstergesi
 *   Bu fonksiyon, ad ve struct alanlarını temizler.
 * -------------------------------------------------- */
void    gas_giant_planet_destroy(Planet* p);

#endif 
