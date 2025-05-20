/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#ifndef PERSON_H
#define PERSON_H

#include <stddef.h>
#include <stdbool.h>

/* ----------------------------------------------------------
 *   Kişi Yapısı
 *   - name: heap’te saklanan isim stringi
 *   - age : kişinin yaşı (yıl)
 *   - remaining_life_hours: kalan ömür (saat, ondalıklı)
 *   - ship_name: hangi gemide (NULL ise henüz bir gemiye binmemiş)
 * ---------------------------------------------------------- */
typedef struct {
    char*   name;                    
    size_t  age;                    
    double  remaining_life_hours;    
    char*   ship_name;               
} Person;

/* ----------------------------------------------------------
 *   Oluştur / Yok Et
 *   person_create  : yeni Person* malloc, isim ve gemi adını kopyalar
 *   person_destroy : adı ve gemi adını free, struct’ı free eder
 * ---------------------------------------------------------- */
Person* person_create (const char* name,
                       size_t age,
                       double remaining_life_hours,
                       const char* ship_name /* NULL serbest */);

void    person_destroy(Person* p);

/* ----------------------------------------------------------
 *   Getter / Setter
 *   - person_get_*  : güvenli okuma (NULL kontrolü içerebilir)
 *   - person_set_*  : var olan değerleri güncelle, eski stringi free edip yenisini kopyala
 * ---------------------------------------------------------- */
const char* person_get_name   (const Person* p);
size_t      person_get_age    (const Person* p);
double      person_get_remaining_life_hours(const Person* p);
const char* person_get_ship_name(const Person* p);

void  person_set_name (Person* p, const char* name);
void  person_set_age  (Person* p, size_t age);
void  person_set_remaining_life_hours(Person* p, double hours);
void  person_set_ship_name(Person* p, const char* ship_name);

/* ----------------------------------------------------------
 *   Davranış
 *   - person_decrement_life: kalan ömürü saat cinsinden azaltır
 *   - person_is_alive       : kalan ömür > 0 ise true
 * ---------------------------------------------------------- */
void  person_decrement_life(Person* p, double hours); 
bool  person_is_alive(const Person* p);

/* ----------------------------------------------------------
 *   - person_to_string: “Name (Age: X, Life: Y h, Ship: Z)” formatlı malloc dönen string
 * ---------------------------------------------------------- */
char* person_to_string(const Person* p);

#endif 
