/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#include "SpaceShip.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* --------------------------------------------------
 *   Başlangıç yolcu kapasitesi: 
 *   - İlk malloc bu kadar yer ayırır
 *   - Dolduğunda çarpanlı büyütme yapmak için kullanılır
 * -------------------------------------------------- */
#define INITIAL_PASSENGER_CAPACITY 16


static char* duplicate_string(const char* s)
{
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char* d  = malloc(n);
    if (d) memcpy(d, s, n);
    return d;
}

/* --------------------------------------------------
 *adı verilen gezegeni listeden bulur
 * -------------------------------------------------- */
static Planet* find_planet(Planet** planets, size_t count, const char* name)
{
    for (size_t i = 0; i < count; ++i)
        if (strcmp(planet_get_name(planets[i]), name) == 0)
            return planets[i];
    return NULL;
}

/* --------------------------------------------------
 *   Yardımcı: nüfusa ekle/çıkar
 *   sign > 0 → ekle, sign < 0 → çıkar
 * -------------------------------------------------- */
static void transfer_population(Planet* pl, size_t cnt, int sign)
{
    if (!pl) return;
    if (sign > 0)      planet_add_population(pl, cnt);
    else if (sign < 0) planet_add_population(pl, (size_t)-(long long)cnt);
}

/* --------------------------------------------------
 *   Yardımcı: varış işlemleri
 *   - status = VARDI, kalan saat = 0
 *   - varışta gezegen nüfusuna yolcular eklenir
 * -------------------------------------------------- */
static void arrive(SpaceShip* ship, Planet** planets, size_t planet_count)
{
    ship->status          = SHIP_VARDI;
    ship->hours_to_target = 0;

    Planet* dest = find_planet(planets, planet_count, ship->destination);
    if (dest) {
        ship->arrival_time = *planet_get_current_time(dest);
        ship->has_arrived  = 1;
        /* yolcuları nüfusa ekle */
        transfer_population(dest, ship->passenger_count, +1);
    }
}

/* --------------------------------------------------
 *   - Temel bilgiler ve yolcu dizisi ayrılır
 *   - INITIAL_PASSENGER_CAPACITY uzunluğunda başlar
 * -------------------------------------------------- */
SpaceShip* spaceship_create(const char* name,
                            const char* origin,
                            const char* destination,
                            const SimTime* departure_time,
                            int distance_hours)
{
    SpaceShip* ship = malloc(sizeof *ship);
    if (!ship) return NULL;

    ship->name         = duplicate_string(name);
    ship->origin       = duplicate_string(origin);
    ship->destination  = duplicate_string(destination);
    ship->departure_time = *departure_time;
    ship->distance_hours = distance_hours;
    ship->status         = SHIP_BEKLIYOR;
    ship->hours_to_target = distance_hours;
    ship->has_arrived     = 0;

    ship->passenger_count    = 0;
    ship->passenger_capacity = INITIAL_PASSENGER_CAPACITY;
    ship->passengers         = malloc(sizeof(Person*) * ship->passenger_capacity);

    if (!ship->name || !ship->origin || !ship->destination || !ship->passengers) {
        spaceship_destroy(ship);
        return NULL;
    }
    return ship;
}

/* --------------------------------------------------
 *   - Yolcu gösterge dizisi ve stringler free edilir
 * -------------------------------------------------- */
void spaceship_destroy(SpaceShip* ship)
{
    if (!ship) return;

    free(ship->passengers);
    free(ship->name);
    free(ship->origin);
    free(ship->destination);
    free(ship);
}


const char* spaceship_get_name(const SpaceShip* s)          { return s ? s->name : NULL; }
ShipStatus  spaceship_get_status(const SpaceShip* s)        { return s ? s->status : SHIP_IMHA; }
int         spaceship_get_hours_to_target(const SpaceShip* s)
{
    return (!s || s->status == SHIP_IMHA) ? 0 : s->hours_to_target;
}
char* spaceship_get_formatted_arrival(const SpaceShip* s)
{
    if (!s || !s->has_arrived) return duplicate_string("--");
    return simtime_to_string(&s->arrival_time);
}

/* --------------------------------------------------
 *   Yolcu ekleme:
 *   - Kapasite dolduğunda *2 katına çıkar
 * -------------------------------------------------- */
int spaceship_add_passenger(SpaceShip* ship, Person* p)
{
    if (!ship || !p) return -1;

    if (ship->passenger_count >= ship->passenger_capacity) {
        size_t new_cap = ship->passenger_capacity * 2;
        Person** tmp = realloc(ship->passengers, sizeof(Person*) * new_cap);
        if (!tmp) return -1;
        ship->passengers         = tmp;
        ship->passenger_capacity = new_cap;
    }
    ship->passengers[ship->passenger_count++] = p;
    return 0;
}

/* --------------------------------------------------
 *   Yaşayan yolcu sayısı
 * -------------------------------------------------- */
static size_t alive_count(const SpaceShip* s)
{
    size_t c = 0;
    for (size_t i = 0; i < s->passenger_count; ++i)
        if (person_is_alive(s->passengers[i])) ++c;
    return c;
}

/* --------------------------------------------------
 *   Her saat güncellemesi:
 *   1) Yaşlandırma   (hem bekleyen hem varan yolcu)
 *   2) Tüm öldüyse IMHA
 *   3) Kalkış/Zamanlama
 * -------------------------------------------------- */
void spaceship_update_status(SpaceShip* ship,Planet** planets, size_t planet_count)                          
{
    if (!ship) return;

    /* 1) Yolcuları yaşlandır */
    double factor = 1.0;
    if (ship->status == SHIP_BEKLIYOR) {
        Planet* origin = find_planet(planets, planet_count, ship->origin);
        if (origin) factor = planet_get_aging_factor(origin);
    } else if (ship->status == SHIP_VARDI) {
        Planet* dest = find_planet(planets, planet_count, ship->destination);
        if (dest) factor = planet_get_aging_factor(dest);
    }
    for (size_t i = 0; i < ship->passenger_count; ++i)
        if (person_is_alive(ship->passengers[i]))
            person_decrement_life(ship->passengers[i], factor);

    /* 2) Tüm yolcular öldüyse IMHA */
    if (alive_count(ship) == 0) {
        /* nüfustan düşür */
        Planet* pl = NULL;
        if (ship->status == SHIP_BEKLIYOR)
            pl = find_planet(planets, planet_count, ship->origin);
        else if (ship->status == SHIP_VARDI)
            pl = find_planet(planets, planet_count, ship->destination);

        transfer_population(pl, ship->passenger_count, -1);
        ship->status          = SHIP_IMHA;
        ship->hours_to_target = 0;
        return;
    }

    /* 3) Durum değişimleri */
    if (ship->status == SHIP_BEKLIYOR) {
        Planet* origin = find_planet(planets, planet_count, ship->origin);
        if (origin && simtime_compare(planet_get_current_time(origin),
                                      &ship->departure_time) >= 0) {
            /* kalkış → yolcuları gezegen nüfusundan çıkar */
            transfer_population(origin, ship->passenger_count, -1);
            ship->status = SHIP_YOLDA;
        }
    } else if (ship->status == SHIP_YOLDA) {
        if (--ship->hours_to_target <= 0)
            arrive(ship, planets, planet_count);
    }
}


char* spaceship_to_string(const SpaceShip* s)
{
    if (!s) return NULL;
    const char* st =
        (s->status == SHIP_BEKLIYOR) ? "BEKLIYOR" :
        (s->status == SHIP_YOLDA)    ? "YOLDA"    :
        (s->status == SHIP_VARDI)    ? "VARDI"    : "IMHA";

    char* arrival = spaceship_get_formatted_arrival(s);
    size_t len = strlen(s->name) + strlen(st) + strlen(s->origin) +
                 strlen(s->destination) + strlen(arrival) + 128;
    char* buf = malloc(len);
    if (buf)
        snprintf(buf, len, "%s [%s] %s→%s (%d h, ETA: %s)",
                 s->name, st, s->origin, s->destination,
                 spaceship_get_hours_to_target(s), arrival);

    free(arrival);
    return buf;
}
