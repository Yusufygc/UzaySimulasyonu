/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#ifndef FILEREADER_H
#define FILEREADER_H

#include <stddef.h>
#include "Person.h"
#include "Planet.h"
#include "SpaceShip.h"

/**
 * Kişi listesini oku:
 *  path           : Kisiler.txt yolu
 *  persons_out    : döndürülecek Person* dizisine işaretçi
 *  count_out      : dizi uzunluğunu yazacak değişken
 */
int filereader_read_persons (const char* path,
                             Person***    persons_out,
                             size_t*      count_out);
/**
 * Gezegen listesini oku:
 *  path            : Gezegenler.txt yolu
 *  planets_out     : döndürülecek Planet* dizisine işaretçi
 *  count_out       : dizi uzunluğunu yazacak değişken
 *  (oluşturulan her Planet*, caller’da destroy edilmeli)
 */
int filereader_read_planets(const char* path,
                             Planet***   planets_out,
                             size_t*     count_out);
/**
 * Uzay araçları listesini oku:
 *  path            : Araclar.txt yolu
 *  ships_out       : döndürülecek SpaceShip* dizisine işaretçi
 *  count_out       : dizi uzunluğunu yazacak değişken
 *  planets         : önceden oluşturulmuş Planet* dizisi
 *  planet_count    : gezegen sayısı (içinde adı geçen gezegen bulunmalı)
 *  (bulunmayan hedef → o satır atlanır)
 */
int filereader_read_spaceships(const char* path,
                               SpaceShip*** ships_out,
                               size_t*      count_out,
                               Planet**     planets,
                               size_t       planet_count);

#endif 
