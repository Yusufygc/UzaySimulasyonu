/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#ifndef PLANET_H
#define PLANET_H

#include "SimTime.h"
#include <stddef.h>   /* size_t */

/* -----------------------------------------------------------
 *   Soyut Gezegeni temsil eden yapı ve sanal metot tablosu
 * ----------------------------------------------------------- */

/* İleriye yönelik bildirim: Gerçek struct, kaynak dosyada tanımlanır */
typedef struct Planet Planet;

/* Sanal tablo: tüm gezegen türleri bu fonksiyonları override edebilir */
typedef struct {
    void   (*advance_hours)(Planet* self, int hours);      /* zamanı ilerlet */
    double (*aging_factor) (const Planet* self);           /* yaşlanma çarpanı */
    void   (*add_population)(Planet* self, size_t count);  /* nüfusa ekle/çıkar */
    void   (*destroy)      (Planet* self);                 /* türev temizleme opsiyonel */
} PlanetVTable;

/* -----------------------------------------------------------
 *   Planet (soyut üst sınıf)
 * ----------------------------------------------------------- */
struct Planet {
    const PlanetVTable* vptr;    /* sanal metod tablosu işaretçisi */
    char*               name;    /* gezegen adı (heap’te kopya) */
    SimTime             current_time; /* gezegenin kendi takvimi */
    size_t              population;   /* anlık nüfus */
};

/* -----------------------------------------------------------
 *   Başlatma ve yok etme (ctor / dtor benzetimi)
 * ----------------------------------------------------------- */
/**
 * planet_init:
 *   - p           : Planet nesnesi (önceden malloc veya stack)
 *   - vtable      : ilgili gezegen türünün metod tablosu
 *   - name        : gezegen adı (kopyalanır)
 *   - hours_per_day: gezegende bir günün uzunluğu
 *   - day,month,year : başlangıç tarihi
 */
void   planet_init(Planet* p,
                   const PlanetVTable* vtable,
                   const char* name,
                   int hours_per_day,
                   int day, int month, int year);

/**
 * planet_destroy:
 *   - p: heap’te oluşturulmuş Planet* — tüm iç kaynakları free eder
 *   (stack’te oluşturulmuş p için undefined behavior)
 */
void   planet_destroy(Planet* p);

/* -----------------------------------------------------------
 *  erişim fonksiyonları (getter / setter)
 * ----------------------------------------------------------- */
const char*     planet_get_name        (const Planet* p);
const SimTime*  planet_get_current_time(const Planet* p);
size_t          planet_get_population  (const Planet* p);
void            planet_set_population  (Planet* p, size_t pop);

/* -----------------------------------------------------------
 *   Polimorfik arayüz (sanal metot çağrıları)
 * ----------------------------------------------------------- */
/* Zamanı türüne göre ilerletir (override ederek faktörlü ilerleme) */
void   planet_advance_hours(Planet* p, int hours);
/* Gezegen türüne göre yaşlanma çarpanı döner (1.0,0.1,0.5,0.01) */
double planet_get_aging_factor(const Planet* p);
/* Nüfusa kişi ekle/çıkar (negatif count ile de çıkarmak mümkün) */
void   planet_add_population(Planet* p, size_t count);


/* Zamanı SimTime API’siyle düz ileri iterasyon */
void   planet_base_advance_hours(Planet* p, int hours);
/* Yaşlanma çarpanı: kayaç gezegende 1.0 (diğerleri override eder) */
double planet_base_aging_factor (const Planet* p);
/* Nüfusa doğrudan ekleme (negatif değeri cast ederek çıkarma) */
void   planet_base_add_population(Planet* p, size_t count);

#endif 
