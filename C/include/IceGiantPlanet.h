/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#ifndef ICE_GIANT_PLANET_H
#define ICE_GIANT_PLANET_H

#include "Planet.h"


typedef struct IceGiantPlanet IceGiantPlanet;

/* --------------------------------------------------
 *   Buz Devi gezegen oluşturucu:
 *   - name           : gezegen adı (heap’te kopyalanıp saklanır)
 *   - hours_per_day  : gezegende bir günün kaç saat sürdüğü
 *   - day, month, year : gezegenin başlangıç tarihi
 *   Başarılı → Planet* (up-cast edilmiş), Başarısız → NULL
 * -------------------------------------------------- */
Planet* ice_giant_planet_create(const char* name,
                                int hours_per_day,
                                int day, int month, int year);
/* --------------------------------------------------
 *   Buz Devi gezegen imha edici:
 *   - p : heap’te oluşturulmuş Planet* göstergesi
 *   Bu fonksiyon gezegen adını ve struct alanlarını serbest bırakır.
 * -------------------------------------------------- */
void    ice_giant_planet_destroy(Planet* p);

#endif 