/**
*
* @author Muhammed Yusuf Yağcı  mail = B211210017
* @since 23/03/2025-26/04/2025
* <p> 1. Öğretim C </p>
*/

package simulation;

import simulation.model.Person;
import simulation.model.Planet;
import simulation.model.SpaceShip;
import simulation.io.FileReader;

import java.io.IOException;
import java.util.*;
import java.util.stream.Collectors;

/**
 * Simülasyon sınıfı – uygulamanın giriş noktası ve ana döngüsü.
 */
public class Simulation {

    private List<Person>    persons;
    private List<Planet>    planets;
    private List<SpaceShip> ships;

    /* Tanımsız gemiye binmeye çalışan yolcular              */
    private final List<Person>   invalidPersons           = new ArrayList<>();
    /* Tanımsız hedef gezegene gitmeye çalışan gemiler       */
    private final List<SpaceShip> invalidDestinationShips = new ArrayList<>();

    /* ─────────────────── Veri okuma & ilişkilendirme ─────────────────── */
    public void initialize() throws IOException {

        persons = FileReader.readPersons ("files/Kisiler.txt");
        planets = FileReader.readPlanets ("files/Gezegenler.txt");

        Map<String,Planet> planetMap = planets.stream()
                .collect(Collectors.toMap(Planet::getName, p -> p));

        /* Gemileri oku – origin gezegeni zaten doğrulanıyor */
        ships = FileReader.readSpaceShips("files/Araclar.txt", planetMap);

        /* ---- Tanımsız hedef gezegen kontrolü ---- */
        Iterator<SpaceShip> it = ships.iterator();
        while (it.hasNext()) {
            SpaceShip s = it.next();
            if (!planetMap.containsKey(s.getDestinationPlanet())) {
                invalidDestinationShips.add(s); // raporla
                it.remove();                    // simülasyondan çıkar
            }
        }

        /* ---- Tanımsız gemi kontrolü (yolcu tarafı) ---- */
        Map<String,SpaceShip> shipMap = ships.stream()
                .collect(Collectors.toMap(SpaceShip::getName, sh -> sh));

        for (Person p : persons) {
            SpaceShip s = shipMap.get(p.getShipName());
            if (s == null) {                      // gemi yok → yolcu atla
                invalidPersons.add(p);
            } else {
                s.addPassenger(p);
            }
        }
    }

    /* ───────────────────── Ana döngü ───────────────────── */
    public void run() throws InterruptedException {

        updatePopulations();
        displayStatus();

        /* 1 iterasyon = 1 saat */
        while (!isFinished()) {

            /* Gezegen saatlerini +1 ilerlet */
            for (Planet p : planets) p.advanceHours(1);

            /* ── Tanımsız gemiye/gezegene bağlı yolcuları da yaşlandır ── */
            for (Person p : invalidPersons) p.decrementLife(1);

            /* Gemileri güncelle */
            Map<String,Planet> planetMap = planets.stream()
                    .collect(Collectors.toMap(Planet::getName, p -> p));
            for (SpaceShip s : ships) s.updateStatus(planetMap);

            updatePopulations();
            displayStatus();

            Thread.sleep(200);
        }

        /* Simülasyon bitti → son durum + özetler */
        displayStatus();
        printInvalidPassengerSummary();
        printInvalidDestinationSummary();
        printDeathSummary();
    }

    /* ───────────────── Yardımcılar ───────────────── */

    /** Tüm gemiler VARDI veya IMHA olduğunda biter. */
    private boolean isFinished() {
        return ships.stream().allMatch(s ->
                s.getStatus() == SpaceShip.ShipStatus.VARDI ||
                s.getStatus() == SpaceShip.ShipStatus.IMHA);
    }

    /** Gezegen nüfuslarını yeniden hesapla (yalnızca canlılar). */
    private void updatePopulations() {

        planets.forEach(p -> p.setPopulation(0));

        Map<String,Planet> map = planets.stream()
                .collect(Collectors.toMap(Planet::getName, p -> p));

        for (SpaceShip s : ships) {
            int alive = s.getPassengers().stream()
                         .mapToInt(pp -> pp.isAlive() ? 1 : 0)
                         .sum();

            if (s.getStatus() == SpaceShip.ShipStatus.BEKLIYOR) {
                map.get(s.getOriginPlanet()).addPopulation(alive);
            } else if (s.getStatus() == SpaceShip.ShipStatus.VARDI) {
                map.get(s.getDestinationPlanet()).addPopulation(alive);
            }
        }
    }

    /** Windows / Unix için basit konsol temizleme. */
    private void clearConsole() {
        try {
            if (System.getProperty("os.name").toLowerCase().contains("windows")) {
                new ProcessBuilder("cmd","/c","cls").inheritIO().start().waitFor();
            } else {
                System.out.print("\033[H\033[2J"); System.out.flush();
            }
        } catch (IOException | InterruptedException e) {
            for (int i = 0; i < 50; i++) System.out.println();
        }
    }

    /** Gezegen & uzay aracı tablolarını basar. */
    private void displayStatus() {

        clearConsole();

        /* ── Gezegenler ── */
        System.out.println("Gezegenler:\n");
        System.out.printf("%-12s", "");
        for (Planet p : planets) System.out.printf("%-12s", "--- " + p.getName() + " ---");
        System.out.println();

        System.out.printf("%-12s","Tarih");
        for (Planet p : planets)
            System.out.printf("%-12s", p.getCurrentTime().toString().split(" ")[0]);
        System.out.println();

        System.out.printf("%-12s","Nüfus");
        for (Planet p : planets) System.out.printf("%-12d", p.getPopulation());
        System.out.println("\n");

        /* ── Uzay Araçları ── */
        System.out.println("Uzay Araçları:\n");
        System.out.printf("%-12s%-12s%-12s%-12s%-20s%-22s%n",
                "Araç Adı","Durum","Çıkış","Varış","Hedefe Kalan Saat","Varış Tarihi");

        for (SpaceShip s : ships) {
            System.out.printf("%-12s%-12s%-12s%-12s%-20s%-22s%n",
                    s.getName(), s.getStatus(),
                    s.getOriginPlanet(), s.getDestinationPlanet(),
                    s.getHoursToTarget(), s.getFormattedArrival());
        }
        System.out.println();
    }

    /* ───────────── Özetler ───────────── */

    /** Tanımsız gemiye binen yolcu */
    private void printInvalidPassengerSummary() {
        if (invalidPersons.isEmpty()) return;

        long dead  = invalidPersons.stream().filter(p -> !p.isAlive()).count();
        long alive = invalidPersons.size() - dead;

        System.out.println("Tanımsız gemiye ya da gezegene atanmış ve harekete geçemeyen yolcu sayısı : "
                + invalidPersons.size());
        System.out.println("Beklerken Ölenler : " + dead + ",  Beklerken Hayatta Kalanlar : " + alive);

        Set<String> ships = invalidPersons.stream()
                .map(Person::getShipName)
                .collect(Collectors.toCollection(TreeSet::new));

        System.out.println("Tanımsız gemiler: " + String.join(", ", ships) + "\n");
    }

    /** Tanımsız hedef gezegenlere gitmeye çalışan gemi  */
    private void printInvalidDestinationSummary() {
        if (invalidDestinationShips.isEmpty()) return;

        System.out.println("Tanımsız gezegene gitmeye çalışan araç sayısı: "
                + invalidDestinationShips.size());

        Set<String> planets = invalidDestinationShips.stream()
                .map(SpaceShip::getDestinationPlanet)
                .collect(Collectors.toCollection(TreeSet::new));

        System.out.println("Tanımsız gezegenler: " + String.join(", ", planets));
        System.out.println("İlgili araçlar : " +
                invalidDestinationShips.stream()
                        .map(SpaceShip::getName)
                        .collect(Collectors.joining(", "))
                + "\n");
    }

    /** Ölen yolcuların nerede öldüklerine dair  */
    private void printDeathSummary() {

        int diedWaiting  = 0;
        int diedTransit  = 0;
        int diedArrived  = 0;

        for (SpaceShip s : ships) {
            for (Person p : s.getPassengers()) {
                if (!p.isAlive()) {
                    switch (s.getStatus()) {
                        case BEKLIYOR -> diedWaiting++;
                        case YOLDA    -> diedTransit++;
                        case IMHA -> {
                            if (s.getFormattedArrival().equals("--"))
                                diedTransit++;     // yolda IMHA
                            else
                                diedArrived++;    // varmıştı sonra IMHA
                        }
                        case VARDI    -> diedArrived++;
                    }
                }
            }
        }
        //  tanımsız gemiye atanıp ölen yolcular:
        long deadInvalid = invalidPersons.stream().filter(p -> !p.isAlive()).count();
        diedWaiting += deadInvalid;   // onlar kalkış bekleyen senaryoda sayılır

        System.out.println("Ölen (bekleme)      : " + diedWaiting);
        System.out.println("Ölen (yolda)        : " + diedTransit);
        System.out.println("Ölen (varış sonrası): " + diedArrived + "\n");
    }

    /* ─────────────────── Program girişi ─────────────────── */
    public static void main(String[] args) {
        Simulation sim = new Simulation();
        try {
            sim.initialize();
            sim.run();
        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
    }
}
