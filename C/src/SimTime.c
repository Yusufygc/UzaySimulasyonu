/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#include "SimTime.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>



/**
 * is_leap:
 *   - Artık yıl kontrolü:
 *   - 4’e bölünebilen, 100’e bölünemeyen veya 400’e bölünebilen yıllar.
 */
static inline bool is_leap(int y)
{
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

/**
 * days_in_month:
 *   - Yıla ve aya göre o ayda kaç gün var döner.
 *   - Hatalı ay indeksi gelirse 30 gün varsayar.
 */
static int days_in_month(int year, int month)
{
    static const int mdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month < 1 || month > 12) return 30;           
    return (month == 2 && is_leap(year)) ? 29 : mdays[month - 1];
}

/**
 * add_one_day:
 *   - SimTime’da bir gün ileri alır.
 *   - Ay ve yıl geçişlerini günün sonunda yönetir.
 */
static void add_one_day(SimTime* t)
{
    t->day++;
    if (t->day > days_in_month(t->year, t->month)) {
        t->day = 1;
        t->month++;
        if (t->month > 12) { t->month = 1; t->year++; }
    }
}

/**
 * sub_one_day:
 *   - SimTime’da bir gün geri alır.
 *   - Ay ve yıl başı geçişlerini yönetir.
 */
static void sub_one_day(SimTime* t)
{
    t->day--;
    if (t->day < 1) {
        t->month--;
        if (t->month < 1) { t->month = 12; t->year--; }
        t->day = days_in_month(t->year, t->month);
    }
}


/**
 * simtime_init:
 *   - SimTime struct’ını başlatır.
 *   - Saat başlangıcı 0, gün-ay-yıl ve günün saat uzunluğu ayarlanır.
 */
void simtime_init(SimTime* t,
                  int day,
                  int month,
                  int year,
                  int hours_per_day)
{
    t->day           = day;
    t->month         = month;
    t->year          = year;
    t->hour          = 0;
    t->hours_per_day = hours_per_day;
}

/**
 * simtime_advance_hours:
 *   - "hours" kadar SimTime’da ileri veya geri hareket.
 *   - Çok büyük saat atlamaları için önce toplu gün atlaması,
 *     sonra kalan saat işlemi yapar (performans optimizasyonu).
 */
void simtime_advance_hours(SimTime* t, int hours)
{
    if (hours == 0) return;

    /* 1) doğrudan gün atlayabileceğimiz kadar atla */
    long long total = (long long)t->hour + hours;
    long long hpd   = t->hours_per_day;

    /* negatif mod’u doğru yönetmek için long long kullanıyoruz */
    long long fullDays = total / hpd;
    if (total < 0 && (total % hpd)) fullDays--;   

    /*  günleri topluca ilerlet */
    while (fullDays > 0) { add_one_day(t);  fullDays--; }
    while (fullDays < 0) { sub_one_day(t);  fullDays++; }

     /* Kalan saat hesapla (mod hpd) */
    t->hour = (int)(total - ( (total / hpd) * hpd ));
    if (t->hour < 0) t->hour += hpd;  
}

/**
 * simtime_compare:
 *   - İki SimTime’ı karşılaştırır.
 *   - a < b → negatif, a == b → 0, a > b → pozitif döner.
 */
int simtime_compare(const SimTime* a, const SimTime* b)
{
    if (a->year  != b->year ) return a->year  - b->year;
    if (a->month != b->month) return a->month - b->month;
    if (a->day   != b->day  ) return a->day   - b->day;
    return a->hour - b->hour;
}

char* simtime_to_string(const SimTime* t)
{
    
    char* buf = (char*)malloc(18);
    if (buf)
        snprintf(buf, 18, "%02d.%02d.%04d %02d:00",
                 t->day, t->month, t->year, t->hour);
    return buf;
}
