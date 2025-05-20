/**
*
* @author Muhammed Yusuf Yağcı  mail = B211210017
* @since 23/03/2025-26/04/2025
* <p>
* 1. Öğretim C
* </p>
*/

package simulation.model;

/**
 * Gezegen sınıfı, her gezegende zamanı ve nüfusu tutar.
 */
public class Planet {
    private String name;
    private Time currentTime;
    private int population;

    /**
     * Gezegen adı ve başlangıç zamanını vererek oluşturur. Başlangıçta nüfus 0.
     */
    public Planet(String name, Time currentTime) {
        this.name = name;
        this.currentTime = currentTime;
        this.population = 0;
    }

    /**
     * Gezegen adını döner.
     */
    public String getName() {
        return name;
    }

    /**
     * Gezegenin mevcut zaman nesnesini döner.
     */
    public Time getCurrentTime() {
        return currentTime;
    }

    /**
     * Gezegenin nüfusunu döner.
     */
    public int getPopulation() {
        return population;
    }

    /**
     * Gezegen nüfusunu sıfırlar veya dışarıdan ayarlar.
     */
    public void setPopulation(int population) {
        this.population = population;
    }

    /**
     * Gezegen saatini belirtilen kadar ilerletir.
     */
    public void advanceHours(int hours) {
        currentTime.advanceHours(hours);
    }

    /**
     * Gezegene canlı yolcu ekler.
     */
    public void addPopulation(int count) {
        this.population += count;
    }

    @Override
    public String toString() {
        return String.format(
            "%s - Zaman: %s - Nüfus: %d",
            name,
            currentTime.toString(),
            population
        );
    }
}
