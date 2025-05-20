/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <stddef.h>
#include "SimTime.h"
#include "Person.h"
#include "Planet.h"


/* -----------------------------------------------------------
 *   Uzay aracı durumu
 * ----------------------------------------------------------- */
typedef enum {
    SHIP_BEKLIYOR,
    SHIP_YOLDA,
    SHIP_VARDI,
    SHIP_IMHA
} ShipStatus;

/* -----------------------------------------------------------
 *   Uzay aracı yapısı
 *   - name: uzay aracının adı 
 *   - origin: uzay aracının kalkış yeri
 *   - destination: uzay aracının hedefi 
 *   - departure_time: kalkış zamanı
 *   - distance_hours: hedefe ulaşmak için gereken saat
 *   - status: uzay aracının durumu
 *   - hours_to_target: hedefe kalan saat
 *   - arrival_time: varış zamanı
 *   - has_arrived: varış durumu (0 veya 1)
 *   - passengers: yolcu dizisi (Person* dizisi)
 * ----------------------------------------------------------- */
typedef struct SpaceShip {
    char*        name;
    char*        origin;
    char*        destination;
    SimTime      departure_time;
    int          distance_hours;

    ShipStatus   status;
    int          hours_to_target;
    SimTime      arrival_time;
    int          has_arrived;

    Person**     passengers;
    size_t       passenger_count;
    size_t       passenger_capacity;
} SpaceShip;


/* -----------------------------------------------------------
 *   Uzay aracı oluşturucu ve yok edici
 * ----------------------------------------------------------- */
SpaceShip* spaceship_create(const char* name,
                            const char* origin,
                            const char* destination,
                            const SimTime* departure_time,
                            int distance_hours);
void       spaceship_destroy(SpaceShip* ship);

/* -----------------------------------------------------------
 *   Getter ve Setter
 * ----------------------------------------------------------- */
const char* spaceship_get_name(const SpaceShip* ship);
ShipStatus  spaceship_get_status(const SpaceShip* ship);
int         spaceship_get_hours_to_target(const SpaceShip* ship);
char*       spaceship_get_formatted_arrival(const SpaceShip* ship);

// -----------------------------------------------------------
//     Uzay aracı davranışları 
//-----------------------------------------------------------
int spaceship_add_passenger(SpaceShip* ship, Person* p);
void spaceship_update_status(SpaceShip* ship, Planet** planets, size_t planet_count);

// Bilgiyi okunabilir stringe dönüştürme
char* spaceship_to_string(const SpaceShip* ship);

#endif