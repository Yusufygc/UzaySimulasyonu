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
 * Kişi sınıfı, bireylerin ad, yaş ve kalan ömür bilgilerini tutar.
 */
public class Person {
    private String name;
    private int age;
    private int remainingLifeHours;
    private String shipName;

    /**
     * Person constructor.
     */
    public Person(String name, int age, int remainingLifeHours, String shipName) {
        this.name = name;
        this.age = age;
        this.remainingLifeHours = remainingLifeHours;
        this.shipName = shipName;
    }

    public String getName() {
        return name;
    }

    public int getAge() {
        return age;
    }

    public int getRemainingLifeHours() {
        return remainingLifeHours;
    }

    public String getShipName() {
        return shipName;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public void setRemainingLifeHours(int remainingLifeHours) {
        this.remainingLifeHours = remainingLifeHours;
    }

    public void setShipName(String shipName) {
        this.shipName = shipName;
    }

    /**
     * Kişinin kalan ömrünü verilen saat kadar azaltır.
     */
    public void decrementLife(int hours) {
        this.remainingLifeHours = Math.max(0, this.remainingLifeHours - hours);
    }

    /**
     * Kişinin hayatta olup olmadığını döner.
     */
    public boolean isAlive() {
        return remainingLifeHours > 0;
    }

    @Override
    public String toString() {
        return String.format(
            "%s (Yaş: %d, Kalan Ömür: %d saat, Gemisi: %s)",
            name,
            age,
            remainingLifeHours,
            shipName
        );
    }
}
