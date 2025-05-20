/**
*
* @author Muhammed Yusuf Yağcı  mail = B211210017
* @since 23/03/2025-26/04/2025
* <p>
* 1. Öğretim C
* </p>
*/

package simulation.model;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;

/**
 * Uzay aracı sınıfı – geminin durumunu, rotasını ve yolcularını yönetir.
 */
public class SpaceShip {

    public enum ShipStatus { BEKLIYOR, YOLDA, VARDI, IMHA }

    private final String name;
    private final String originPlanetName;
    private final String destinationPlanetName;
    private final Time   departureTime;
    private final int    distanceHours;

    private ShipStatus status;
    private int  hoursToTarget;
    private Time arrivalTime;           // “VARDI” olduktan sonra atanır → tarih ekranda sabit kalır

    /* Yolcu listesi */
    private final List<Person> passengers = new ArrayList<>();

    /* ──────────────────── Kurucu ──────────────────── */
    public SpaceShip(String name, String origin, String destination,
                     Time departureTime, int distanceHours) {

        this.name                 = name;
        this.originPlanetName     = origin;
        this.destinationPlanetName= destination;
        this.departureTime        = departureTime;
        this.distanceHours        = distanceHours;

        this.status        = ShipStatus.BEKLIYOR;
        this.hoursToTarget = distanceHours;
    }

    /* ──────────────────── Getter’lar ──────────────── */
    public String     getName()              { return name; }
    public String     getOriginPlanet()      { return originPlanetName; }
    public String     getDestinationPlanet() { return destinationPlanetName; }
    public ShipStatus getStatus()            { return status; }

    /** Yolda kaldıkça düşer; IMHA ise daima 0 döner. */
    public int getHoursToTarget() {
        return status == ShipStatus.IMHA ? 0 : Math.max(0, hoursToTarget);
    }

    /** Varış tarihi: – geminin asla varmadığı senaryoda “--” döner. */
    public String getFormattedArrival() {
        return (arrivalTime == null) ? "--"
                                     : arrivalTime.toString();
    }

    /** Yolcu listesine yalnızca okunur erişim. */
    public List<Person> getPassengers() {
        return Collections.unmodifiableList(passengers);
    }

    /* ──────────────────── Yolcu işlemleri ────────── */
    public void addPassenger(Person p) { passengers.add(p); }

    private int alivePassengerCount() {
        int c = 0;
        for (Person p : passengers) if (p.isAlive()) c++;
        return c;
    }

    /* ──────────────────── saatlik update ──────── */
    public void updateStatus(Map<String, Planet> planets) {

        /* IMHA dışındaki her statüde yolcuların ömrü 1 saat azalır */
        if (status != ShipStatus.IMHA) {
            for (Person p : passengers) p.decrementLife(1);
        }

        /* Tüm yolcular öldüyse gemi IMHA (nerede olursa olsun) */
        if (status != ShipStatus.IMHA && alivePassengerCount() == 0) {
            status        = ShipStatus.IMHA;
            hoursToTarget = 0;
            return;
        }

        /* ─── BEKLIYOR → kalkış zamanı geldiyse YOLDA ─── */
        if (status == ShipStatus.BEKLIYOR) {
            Planet origin = planets.get(originPlanetName);
            if (origin.getCurrentTime().compareTo(departureTime) >= 0) {
                status        = ShipStatus.YOLDA;
                hoursToTarget = distanceHours;

                if (hoursToTarget <= 0) {         // mesafe 0 ise anında varış
                    arrive(planets);
                }
            }
        }
        /* ─── YOLDA → sayaç –1, 0’a inince varış ─── */
        else if (status == ShipStatus.YOLDA) {
            hoursToTarget--;
            if (hoursToTarget <= 0) {
                arrive(planets);
            }
        }
        /* VARDI: yalnızca yaşam takibi yapıldı, ek işlem yok */
    }

    /* ─────────────── Varış işlemlerini tek noktada yap ────────────── */
    private void arrive(Map<String, Planet> planets) {
        status        = ShipStatus.VARDI;
        hoursToTarget = 0;

        Planet dest    = planets.get(destinationPlanetName);
        Time   destNow = dest.getCurrentTime();
        arrivalTime    = new Time(destNow.getDate(),
                                  destNow.getHour(),
                                  destNow.getHoursPerDay());
    }

    
    @Override
    public String toString() {
        return String.format("%s [%s] %s→%s (%d saat, varış: %s)",
                             name, status,
                             originPlanetName, destinationPlanetName,
                             getHoursToTarget(), getFormattedArrival());
    }
}
