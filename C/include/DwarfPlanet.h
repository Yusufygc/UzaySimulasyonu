/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#ifndef DWARF_PLANET_H
#define DWARF_PLANET_H

#include "Planet.h"


typedef struct DwarfPlanet DwarfPlanet;

/* --------------------------------------------------
 *   Cüce gezegen oluşturucu:
 *   - name: gezegen adı (heap’te kopyalanır)
 *   - hours_per_day: günün kaç saat süreceği
 *   - day, month, year: başlangıç tarihi
 *   Dönecek değer: başarı → Planet*, başarısızlık → NULL
 * -------------------------------------------------- */
Planet* dwarf_planet_create(const char* name,int hours_per_day,int day, int month, int year);
                            
                            

                           
 /* --------------------------------------------------
 *   Cüce gezegen imha edici:
 *   - p: heap’te oluşturulmuş Planet* göstergesi
 *   Tüm bağlı bellek bloklarını (isim, struct) temizler.
 * -------------------------------------------------- */
void    dwarf_planet_destroy(Planet* p);

#endif
