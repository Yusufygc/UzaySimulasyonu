/**
*
* @author Muhammed Yusuf Yağcı  mail = B211210017
* @since 23/03/2025-26/04/2025
* <p>
* 1. Öğretim C
* </p>
*/

package simulation.io;

import simulation.model.Person;
import simulation.model.Planet;
import simulation.model.SpaceShip;
import simulation.model.Time;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * Dosyadan veri okuyup model nesnelerine dönüştüren sınıf.
 */
public class FileReader {

    /**
     * Kisiler.txt dosyasından Person listesi okur.
     */
    public static List<Person> readPersons(String filePath) throws IOException {
        List<Person> persons = new ArrayList<>();
        for (String line : Files.readAllLines(Paths.get(filePath))) {
            if (line.isBlank()) continue;
            String[] parts = line.split("#");
            if (parts.length != 4) {
                throw new IllegalArgumentException("Kisiler.txt satır formatı hatalı: " + line);
            }
            String name            = parts[0];
            int age                = Integer.parseInt(parts[1]);
            int remainingLifeHours = Integer.parseInt(parts[2]);
            String shipName        = parts[3];
            persons.add(new Person(name, age, remainingLifeHours, shipName));
        }
        return persons;
    }

    /**
     * Gezegenler.txt dosyasından Planet listesi okur.
     */
    public static List<Planet> readPlanets(String filePath) throws IOException {
        List<Planet> planets = new ArrayList<>();
        for (String line : Files.readAllLines(Paths.get(filePath))) {
            if (line.isBlank()) continue;
            String[] parts = line.split("#");
            if (parts.length != 3) {
                throw new IllegalArgumentException("Gezegenler.txt satır formatı hatalı: " + line);
            }
            String name       = parts[0];
            int hoursPerDay   = Integer.parseInt(parts[1]);

            String[] dp = parts[2].split("\\.");
            int day   = Integer.parseInt(dp[0]);
            int month = Integer.parseInt(dp[1]);
            int year  = Integer.parseInt(dp[2]);
            LocalDate date    = LocalDate.of(year, month, day);

            Time time = new Time(date, hoursPerDay);
            planets.add(new Planet(name, time));
        }
        return planets;
    }

    /**
     * Araclar.txt dosyasından SpaceShip listesi okur.
     */
    public static List<SpaceShip> readSpaceShips(
            String filePath,
            Map<String, Planet> planetMap
    ) throws IOException {
        List<SpaceShip> ships = new ArrayList<>();
        for (String line : Files.readAllLines(Paths.get(filePath))) {
            if (line.isBlank()) continue;
            String[] parts = line.split("#");
            if (parts.length != 5) {
                throw new IllegalArgumentException("Araclar.txt satır formatı hatalı: " + line);
            }
            String name        = parts[0];
            String origin      = parts[1];
            String destination = parts[2];

            String[] dp = parts[3].split("\\.");
            int day   = Integer.parseInt(dp[0]);
            int month = Integer.parseInt(dp[1]);
            int year  = Integer.parseInt(dp[2]);

            Planet originPlanet = planetMap.get(origin);
            if (originPlanet == null) {
                throw new IllegalArgumentException("Bilinmeyen gezegen: " + origin);
            }
            int hoursPerDay      = originPlanet.getCurrentTime().getHoursPerDay();
            LocalDate date       = LocalDate.of(year, month, day);
            Time departureTime   = new Time(date, hoursPerDay);

            int distanceHours    = Integer.parseInt(parts[4]);
            ships.add(new SpaceShip(name, origin, destination, departureTime, distanceHours));
        }
        return ships;
    }
}