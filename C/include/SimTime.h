/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#ifndef SIMTIME_H
#define SIMTIME_H

#include <stddef.h>

/**
 * SimTime yapısı:
 * @day           : Gün bilgisi
 * @month         : Ay bilgisi
 * @year          : Yıl bilgisi
 * @hour          : Saat bilgisi
 * @hours_per_day : Bir günün kaç saat sürdüğü
 */
typedef struct {
    int    day;
    int    month;
    int    year;
    int    hour;
    int    hours_per_day;
} SimTime;

/**
 * simtime_init: SimTime nesnesini başlatır.
 */
void simtime_init(SimTime* t,
                  int day,
                  int month,
                  int year,
                  int hours_per_day);

/**
 * simtime_advance_hours: Belirtilen saat kadar SimTime üzerinde ileri/geri hareket eder.
 */
void simtime_advance_hours(SimTime* t, int hours);

/**
 * simtime_compare: İki SimTime'ı karşılaştırır.
 * @return Negatif: a < b, 0: eşit, Pozitif: a > b
 */
int simtime_compare(const SimTime* a, const SimTime* b);

/**
 * simtime_to_string: "gg.aa.yyyy ss:00" formatında string döner.
 */
char* simtime_to_string(const SimTime* t);

#endif