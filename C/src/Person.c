/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#include "Person.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * duplicate_string:
 *   - Gelen C-string'i heap'e kopyalar.
 *   - NULL kontrolü yapar.
 */
static char* duplicate_string(const char* s)
{
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char* d  = (char*)malloc(n);
    if (d) memcpy(d, s, n);
    return d;
}

/* ---------------------------------- ctor / dtor ---------------------------------- */
/**
 * person_create:
 *   - Yeni Person nesnesi heap üzerinde allocate edilir.
 *   - name ve ship_name parametreleri kopyalanır.
 *   - Hata durumunda bütün kaynaklar temizlenir.
 */
Person* person_create(const char* name,
                      size_t age,
                      double remaining_life_hours,
                      const char* ship_name)
{
    Person* p = (Person*)malloc(sizeof *p);
    if (!p) return NULL;

    p->name      = duplicate_string(name);
    p->age       = age;
    p->remaining_life_hours = remaining_life_hours;
    p->ship_name = duplicate_string(ship_name);

    if ((name && !p->name) || (ship_name && !p->ship_name)) {
        person_destroy(p);
        return NULL;
    }
    return p;
}

/**
 * person_destroy:
 *   - Person nesnesini yok eder.
 *   - İsim ve gemi adını serbest bırakır.
 */
void person_destroy(Person* p)
{
    if (!p) return;
    free(p->name);
    free(p->ship_name);
    free(p);
}

/* ---------------------------------- getter ---------------------------------- */
const char* person_get_name(const Person* p)                 { return p ? p->name : NULL; }
size_t      person_get_age(const Person* p)                  { return p ? p->age : 0; }
double      person_get_remaining_life_hours(const Person* p) { return p ? p->remaining_life_hours : 0.0; }
const char* person_get_ship_name(const Person* p)            { return p ? p->ship_name : NULL; }

/* ---------------------------------- setter ---------------------------------- */
void person_set_name(Person* p, const char* name)
{
    if (!p) return;
    char* tmp = duplicate_string(name);
    if (tmp) { free(p->name); p->name = tmp; }
}

void person_set_age(Person* p, size_t age)                         { if (p) p->age = age; }
void person_set_remaining_life_hours(Person* p, double h)          { if (p) p->remaining_life_hours = h; }

/**
 * person_set_ship_name:
 *   - Kişinin gemi adını günceller.
 *   - Eski gemi adını serbest bırakır.
 */
void person_set_ship_name(Person* p, const char* sn)
{
    if (!p) return;
    char* tmp = duplicate_string(sn);
    if (tmp) { free(p->ship_name); p->ship_name = tmp; }
}

/* ---------------------------------- davranış ---------------------------------- */
void person_decrement_life(Person* p, double hours)
{
    if (!p || hours <= 0.0) return;
    p->remaining_life_hours = (hours >= p->remaining_life_hours)
                                ? 0.0
                                : p->remaining_life_hours - hours;
}

bool person_is_alive(const Person* p)
{
    return p && p->remaining_life_hours > 0.0;
}


char* person_to_string(const Person* p)
{
    if (!p) return NULL;
    const char* ship = p->ship_name ? p->ship_name : "-";
    size_t len = strlen(p->name) + strlen(ship) + 128;
    char* buf = (char*)malloc(len);
    if (buf)
        snprintf(buf, len,
                 "%s (Age: %zu, Life: %.1f h, Ship: %s)",
                 p->name, p->age, p->remaining_life_hours, ship);
    return buf;
}
