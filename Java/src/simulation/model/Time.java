/**
*
* @author Muhammed Yusuf Yağcı  mail = B211210017
* @since 23/03/2025-26/04/2025
* <p>
* 1. Öğretim C
* </p>
*/

package simulation.model;

import java.time.LocalDate;
import java.time.format.DateTimeFormatter;

public class Time implements Comparable<Time> {
    private LocalDate date;
    private int hour;
    private final int hoursPerDay;
    private static final DateTimeFormatter DATE_FMT =
        DateTimeFormatter.ofPattern("dd.MM.yyyy");

    /** 
     * Gezegenin başlangıç tarihini ve gün uzunluğunu belirterek bir Time nesnesi oluşturur.
     * Saat başlangıçta 0 kabul edilir.
     */
    public Time(LocalDate date, int hoursPerDay) {
        this.date = date;
        this.hoursPerDay = hoursPerDay;
        this.hour = 0;
    }

    /**
     * Belirli bir saate başlatmak gerektiğinde kullanılır.
     */
    public Time(LocalDate date, int hour, int hoursPerDay) {
        if (hour < 0 || hour >= hoursPerDay) {
            throw new IllegalArgumentException(
                "Saat 0 ≤ hour < " + hoursPerDay + " olmalı");
        }
        this.date = date;
        this.hour = hour;
        this.hoursPerDay = hoursPerDay;
    }

    /**
     * Zamanı verilen saat kadar ilerletir; gün aşımı olursa tarihi bir gün artırır.
     */
    public void advanceHours(int h) {
        hour += h;
        while (hour >= hoursPerDay) {
            hour -= hoursPerDay;
            date = date.plusDays(1);
        }
    }

    /**
     * Saat ve tarihe göre karşılaştırma yapar.
     */
    @Override
    public int compareTo(Time other) {
        int cmp = this.date.compareTo(other.date);
        return (cmp != 0) ? cmp : Integer.compare(this.hour, other.hour);
    }

    @Override
    public String toString() {
        return date.format(DATE_FMT) + " " + String.format("%02d:00", hour);
    }


    public LocalDate getDate() {
        return date;
    }

    public void setDate(LocalDate date) {
        this.date = date;
    }

    public int getHour() {
        return hour;
    }

    public void setHour(int hour) {
        if (hour < 0 || hour >= hoursPerDay) {
            throw new IllegalArgumentException(
                "Saat 0 ≤ hour < " + hoursPerDay + " olmalı");
        }
        this.hour = hour;
    }

    public int getHoursPerDay() {
        return hoursPerDay;
    }
}
