/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#include "Simulation.h"
#include "FileReader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <sys/types.h>



static Planet* find_planet(Planet** planets, size_t count, const char* name)
{
    for (size_t i=0;i<count;++i)
        if (strcmp(planet_get_name(planets[i]), name)==0)
            return planets[i];
    return NULL;
}
static ssize_t find_ship_index(SpaceShip** ships,size_t cnt,const char* n)
{
    for (size_t i=0;i<cnt;++i)
        if (strcmp(spaceship_get_name(ships[i]), n)==0) return (ssize_t)i;
    return -1;
}

/* ---------- konsol ---------- */
static void clear_console(void)
{
#ifdef _WIN32
    system("cls");
#else
    printf("\033[H\033[2J");
    fflush(stdout);
#endif
}

/* ---------- bitiş kontrolü ---------- */
static int is_finished(const Simulation* s)
{
    for (size_t i=0;i<s->ship_count;++i) {
        ShipStatus st = spaceship_get_status(s->ships[i]);
        if (st != SHIP_VARDI && st != SHIP_IMHA) return 0;
    }
    return 1;
}

/* ---------- nüfusun yeniden hesaplanması ---------- */
static void update_populations(Simulation* sim)
{
    for (size_t i=0;i<sim->planet_count;++i)
        planet_set_population(sim->planets[i], 0);

    for (size_t i=0;i<sim->ship_count;++i) {
        SpaceShip* sp = sim->ships[i];
        size_t alive = 0;
        for (size_t j=0;j<sp->passenger_count;++j)
            if (person_is_alive(sp->passengers[j])) ++alive;

        if (sp->status == SHIP_BEKLIYOR) {
            Planet* p = find_planet(sim->planets,sim->planet_count,sp->origin);
            if (p) planet_add_population(p, alive);
        } else if (sp->status == SHIP_VARDI) {
            Planet* p = find_planet(sim->planets,sim->planet_count,sp->destination);
            if (p) planet_add_population(p, alive);
        }
    }
}

/* ---------- simülasyon başlatma ---------- */


static void print_left(int width, const char* text) {
    printf("%-*s", width, text);
}

static void print_centered(int width, const char* text) {
    int len = (int)strlen(text);
    int left = (width - len) / 2;
    if (left < 0) left = 0;
    int right = width - len - left;
    printf("%*s%s%*s", left, "", text, right, "");
}

/* ---------- display_status ---------- */
static const char* status_to_string(ShipStatus s) {
    return (s == SHIP_BEKLIYOR) ? "BEKLIYOR"
         : (s == SHIP_YOLDA)    ? "YOLDA"
         : (s == SHIP_VARDI)    ? "VARDI"
                                : "IMHA";
}

static void display_status(const Simulation* sim) {
    clear_console();
    printf("Gezegenler:\n\n");

    // Konsol genişliğini al (varsayılan olarak 80 karakter kullan)
    int console_width = 80;
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            console_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        }
    #else
        struct winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
            console_width = w.ws_col;
        }
    #endif

    // Minimum sütun genişliği
    const int min_col_width = 15;
    // Gezegen başına sütun genişliği (tarih ve nüfus için yeterli alan sağlamak üzere)
    const int col_width = min_col_width;
    // Satır başına maksimum gezegen sayısını dinamik olarak hesapladık
    int per_line = (console_width - 2) / (col_width + 2); // 2: sütun arası boşluk
    if (per_line < 1) per_line = 1;
    if (per_line > 8) per_line = 8; // Maksimum 8 gezegen bir satırda

    for (size_t start = 0; start < sim->planet_count; start += per_line) {
        size_t end = start + per_line;
        if (end > sim->planet_count) end = sim->planet_count;

        /* --- Gezegen İsimleri --- */
        printf("  ");
        print_centered(col_width, "");
        for (size_t i = start; i < end; ++i) {
            printf("  ");
            print_centered(col_width, planet_get_name(sim->planets[i]));
        }
        printf("\n");

        /* --- Tarih Satırı  --- */
        printf("  ");
        print_left(col_width, "Tarih");
        for (size_t i = start; i < end; ++i) {
            char* ts = simtime_to_string(planet_get_current_time(sim->planets[i]));
            char date[11];
            strncpy(date, ts, 10);
            date[10] = '\0';
            free(ts);

            printf("  ");
            print_centered(col_width, date);
        }
        printf("\n");

        /* --- Nüfus Satırı --- */
        printf("  ");
        print_left(col_width, "Nüfus");
        for (size_t i = start; i < end; ++i) {
            char pop_str[32];
            snprintf(pop_str, sizeof(pop_str), "%zu",
                     planet_get_population(sim->planets[i]));

            printf("  ");
            print_centered(col_width, pop_str);
        }
        printf("\n\n");
    }

    /* ---------- Uzay Araçları ---------- */
    printf("Uzay Araçlari:\n\n");
    // Sütun genişliklerini konsol genişliğine göre ayarladık
    int ship_cols[] = {10, 9, 6, 6, 18, 18};
    const char* headers[] = {
        "Gemi", "Durum", "Çıkış", "Varış",
        "Hedefe Kalan Saat", "Hedefe Varacağı Tarih"
    };

    // Toplam sütun genişliğini hesapladık
    int total_width = 0;
    for (int k = 0; k < 6; ++k) {
        total_width += ship_cols[k];
        if (k > 0) total_width += 2; // Sütun arası boşluk
    }

    //toplam genişlik konsol genişliğinden büyükse, sütun genişliklerini ayarladık
    if (total_width > console_width) {
        // Öncelikle en büyük sütunları küçülttük
        ship_cols[4] = 12; // "Hedefe Kalan Saat"
        ship_cols[5] = 12; // "Hedefe Varacağı Tarih"
        total_width = 0;
        for (int k = 0; k < 6; ++k) {
            total_width += ship_cols[k];
            if (k > 0) total_width += 2;
        }
    }

    /* Başlık */
    for (int k = 0; k < 6; ++k) {
        if (k) printf("  ");
        print_centered(ship_cols[k], headers[k]);
    }
    printf("\n");

    /* Veriler */
    for (size_t i = 0; i < sim->ship_count; ++i) {
        SpaceShip* sp = sim->ships[i];
        char* full = spaceship_get_formatted_arrival(sp);
        char date[11];
        if (!full || strcmp(full, "--") == 0) {
            strcpy(date, "--");
        } else {
            strncpy(date, full, 10);
            date[10] = '\0';
        }
        free(full);

        char hours_str[16];
        snprintf(hours_str, sizeof(hours_str), "%zu",
                 (size_t)spaceship_get_hours_to_target(sp));

        const char* contents[6] = {
            spaceship_get_name(sp),
            status_to_string(spaceship_get_status(sp)),
            sp->origin,
            sp->destination,
            hours_str,
            date
        };
        for (int k = 0; k < 6; ++k) {
            if (k) printf("  ");
            print_centered(ship_cols[k], contents[k]);
        }
        printf("\n");
    }
    printf("\n");
}


/* ---------- özet raporlar ---------- */
static void print_invalid_passenger_summary(const Simulation* sim)
{
    if (sim->invalid_person_count == 0) return;
    printf("Tanımsız araca atanan yolcu sayısı: %zu\n", sim->invalid_person_count);
    printf("İlgili yolcular: ");
    for (size_t i = 0; i < sim->invalid_person_count; ++i) {
        printf("%s", person_get_name(sim->invalid_persons[i]));
        if (i+1 < sim->invalid_person_count) printf(", ");
    }
    printf("\n\n");
}


static void print_invalid_destination_summary(const Simulation* sim)
{
    if (sim->invalid_destination_count == 0) return;
    printf("Tanımsız hedef gezegene giden araç sayısı: %zu\n", sim->invalid_destination_count);
    printf("İlgili araçlar: ");
    for (size_t i = 0; i < sim->invalid_destination_count; ++i) {
        printf("%s", spaceship_get_name(sim->invalid_destination_ships[i]));
        if (i+1 < sim->invalid_destination_count) printf(", ");
    }
    printf("\n\n");
}


static void print_death_summary(const Simulation* sim)
{
    size_t dw=0, dt=0, da=0;
    for (size_t i=0;i<sim->ship_count;++i)
        for (size_t j=0;j<sim->ships[i]->passenger_count;++j)
            if (!person_is_alive(sim->ships[i]->passengers[j])){
                ShipStatus st = spaceship_get_status(sim->ships[i]);
                if (st==SHIP_BEKLIYOR) ++dw;
                else if (st==SHIP_YOLDA||st==SHIP_IMHA) ++dt;
                else ++da;
            }

    for (size_t i=0;i<sim->invalid_person_count;++i)
        if (!person_is_alive(sim->invalid_persons[i])) ++dw;

    printf("Ölen (beklerken ömrü bitenler): %zu\nÖlen (yoldayken ömrü bitenler): %zu\nÖlen (varıştan sonra ömrü bitenler): %zu\n", dw, dt, da);
    
    //printf("Araç gezegene varınca varış tarihi yazdırıldı ama içindeki yolcuların ömrü bittiyse ve herkes öldüyse arac imha oldu ama varış tarihi korundu.\n");

}

/* ---------- init ---------- */
int simulation_initialize(Simulation* sim)
{
    memset(sim,0,sizeof *sim);

    if (filereader_read_persons("files/Kisiler.txt",
            &sim->persons,&sim->person_count))            return -1;
    if (filereader_read_planets("files/Gezegenler.txt",
            &sim->planets,&sim->planet_count))            return -2;
    if (filereader_read_spaceships("files/Araclar.txt",
            &sim->ships,&sim->ship_count,
            sim->planets,sim->planet_count))              return -3;

    /* -- hedefi tanımsız gezegen olan gemileri ayıkla -- */
    for (size_t i=0;i<sim->ship_count; ){
        if (!find_planet(sim->planets,sim->planet_count,sim->ships[i]->destination)){
            sim->invalid_destination_ships = realloc(sim->invalid_destination_ships,
                sizeof(SpaceShip*)*(sim->invalid_destination_count+1));
            sim->invalid_destination_ships[sim->invalid_destination_count++] = sim->ships[i];
            sim->ships[i] = sim->ships[--sim->ship_count];
        } else ++i;
    }

    /* -- yolcuları gemilere dağıt; tanımsızları kaydet -- */
    for (size_t i=0;i<sim->person_count;++i){
        Person* p = sim->persons[i];
        ssize_t idx = find_ship_index(sim->ships,sim->ship_count,p->ship_name);
        if (idx<0){
            sim->invalid_persons = realloc(sim->invalid_persons,
                sizeof(Person*)*(sim->invalid_person_count+1));
            sim->invalid_persons[sim->invalid_person_count++] = p;
        } else if (spaceship_add_passenger(sim->ships[idx], p)) return -4;
    }
    return 0;
}

/* ---------- run ---------- */
void simulation_run(Simulation* sim)
{
    update_populations(sim); display_status(sim);

    while (!is_finished(sim)) {
        /* gezegen saat +1 */
        for (size_t i=0;i<sim->planet_count;++i)
            planet_advance_hours(sim->planets[i],1);

        /* tanımsız yolcular yaşlandır */
        for (size_t i=0;i<sim->invalid_person_count;++i)
            person_decrement_life(sim->invalid_persons[i],1.0);

        /* gemi güncellemeleri */
        for (size_t i=0;i<sim->ship_count;++i)
            spaceship_update_status(sim->ships[i],sim->planets,sim->planet_count);

        update_populations(sim);
        display_status(sim);
    }
    display_status(sim);

    print_invalid_passenger_summary(sim);
    print_invalid_destination_summary(sim);
    print_death_summary(sim);
}

/* ---------- destroy ---------- */
void simulation_destroy(Simulation* sim)
{
    for (size_t i=0;i<sim->person_count;++i) person_destroy(sim->persons[i]);
    free(sim->persons); free(sim->invalid_persons);

    for (size_t i=0;i<sim->planet_count;++i) planet_destroy(sim->planets[i]);
    free(sim->planets);

    for (size_t i=0;i<sim->ship_count;++i) spaceship_destroy(sim->ships[i]);
    free(sim->ships); free(sim->invalid_destination_ships);
}
