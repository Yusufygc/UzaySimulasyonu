/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#ifndef SIMULATION_H
#define SIMULATION_H

#include <stddef.h>
#include "Person.h"
#include "Planet.h"
#include "SpaceShip.h"

/* -----------------------------------------------------------
 *   Simülasyon yapısı
 *   - persons: Kişi dizisi 
 *   - planets: Gezegen dizisi 
 *   - ships: Uzay aracı dizisi 
 *   - invalid_persons: Geçersiz kişi dizisi 
 *   - invalid_destination_ships: Geçersiz hedefe sahip uzay aracı dizisi 
 * ----------------------------------------------------------- */
typedef struct {
    Person**     persons;
    size_t       person_count;

    Planet**     planets;
    size_t       planet_count;

    SpaceShip**  ships;
    size_t       ship_count;

    Person**     invalid_persons;
    size_t       invalid_person_count;

    SpaceShip**  invalid_destination_ships;
    size_t       invalid_destination_count;
} Simulation;

/* -----------------------------------------------------------
 *   Simülasyon başlatma ve yok etme
 *   - simulation_initialize: Simülasyonu başlatır
 *   - simulation_run: Simülasyonu çalıştırır
 *   - simulation_destroy: Simülasyonu yok eder
 * ----------------------------------------------------------- */
int  simulation_initialize(Simulation* sim);
void simulation_run       (Simulation* sim);
void simulation_destroy   (Simulation* sim);

#endif