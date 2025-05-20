/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#ifndef ROCKY_PLANET_H
#define ROCKY_PLANET_H

#include "Planet.h"

/* --------------------------------------------------
 *   Türetilmiş gezegen tanımı
 * -------------------------------------------------- */
typedef struct RockyPlanet RockyPlanet;

/* --------------------------------------------------
 *   Kayaç gezegen oluşturucu (factory)
 *   - name           : gezegen adı (heap’te kopyalanır)
 *   - hours_per_day  : gezegende bir günün uzunluğu
 *   - day,month,year : başlangıç tarihi (gg,aa,yyyy)
 *   Başarılı → Planet* (up-cast edilmiş RockyPlanet*),
 *   Başarısız → NULL
 * -------------------------------------------------- */
Planet* rocky_planet_create(const char* name, int hours_per_day,int day, int month, int year);
                                     
                                       
/* --------------------------------------------------
 *   Kayaç gezegen imha edici (destructor)
 *   - p: heap’te oluşturulmuş Planet* göstergesi
 *   Bu fonksiyon, hem yapı içindeki name stringini
 *   hem de struct belleğini serbest bırakır.
 * -------------------------------------------------- */
void rocky_planet_destroy(Planet* rp);

#endif 
