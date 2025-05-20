/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#define _GNU_SOURCE          /* getline */
#include "FileReader.h"
#include "RockyPlanet.h"
#include "GasGiantPlanet.h"
#include "IceGiantPlanet.h"
#include "DwarfPlanet.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>


static inline char* strip_newline(char* s)
{
    size_t n = strlen(s);
    while (n && (s[n-1] == '\n' || s[n-1] == '\r')) s[--n] = '\0';
    return s;
}

/* === 1.Kişiler ================================== */
int filereader_read_persons(const char* path,
                            Person*** persons_out,
                            size_t*   count_out)
{
    FILE* f = fopen(path, "r");
    if (!f) return errno;

    Person** arr = NULL;
    size_t cap = 0, cnt = 0;
    char*  line = NULL;
    size_t n    = 0;

    while (getline(&line, &n, f) != -1) {
        strip_newline(line);
        if (!*line) continue;

        
        char* parts[4]; char* p = line;
        for (int i = 0; i < 4; ++i) {
            parts[i] = p;
            p = strchr(p, '#');
            if (!p && i < 3) { fclose(f); free(line); return -1; }
            if (p) *p++ = '\0';
        }

        
        Person* per = person_create(parts[0],
                                    (size_t)strtoull(parts[1],NULL,10),
                                    strtod  (parts[2],NULL),
                                    parts[3]);

        if (!per) { fclose(f); free(line); return -2; }

        /* dinamik dizi büyüt */
        if (cnt >= cap) {
            size_t newcap = cap ? cap*2 : 1024;
            Person** tmp = realloc(arr, sizeof(Person*) * newcap);
            if (!tmp) { person_destroy(per); fclose(f); free(line); return -3; }
            arr = tmp; cap = newcap;
        }
        arr[cnt++] = per;
    }
    free(line); fclose(f);
    *persons_out = arr; *count_out = cnt;
    return 0;
}

/* === 2. Gezegenler ================================== */
int filereader_read_planets(const char* path,
                            Planet*** planets_out,
                            size_t*  count_out)
{
    FILE* f = fopen(path, "r");
    if (!f) return errno;

    Planet** arr = NULL;
    size_t cap = 0, cnt = 0;
    char*  line = NULL;
    size_t n    = 0;

    while (getline(&line, &n, f) != -1) {
        strip_newline(line); if (!*line) continue;

        char* parts[4]; char* p = line;
        for (int i = 0; i < 4; ++i) {
            parts[i] = p;
            p = strchr(p, '#');
            if (!p && i < 3) { fclose(f); free(line); return -1; }
            if (p) *p++ = '\0';
        }

        int d,m,y;
        if (sscanf(parts[3], "%d.%d.%d", &d,&m,&y) != 3) { fclose(f); free(line); return -2; }

        Planet* pl = NULL;
        int type   = atoi(parts[1]);
        int hpd    = atoi(parts[2]);          /* günün kaç saat olduğu */

        switch (type) {
            case 0: pl = rocky_planet_create (parts[0], hpd, d,m,y); break;
            case 1: pl = gas_giant_planet_create(parts[0], hpd, d,m,y); break;
            case 2: pl = ice_giant_planet_create(parts[0], hpd, d,m,y); break;
            case 3: pl = dwarf_planet_create  (parts[0], hpd, d,m,y); break;
            default: break;
        }
        if (!pl) { fclose(f); free(line); return -3; }

        if (cnt >= cap) {
            size_t newcap = cap ? cap*2 : 128;
            Planet** tmp  = realloc(arr, sizeof(Planet*) * newcap);
            if (!tmp) { planet_destroy(pl); fclose(f); free(line); return -4; }
            arr = tmp; cap = newcap;
        }
        arr[cnt++] = pl;
    }
    free(line); fclose(f);
    *planets_out = arr; *count_out = cnt;
    return 0;
}

/* === 3.Uzay Araçları ================================== */
int filereader_read_spaceships(const char* path,
                               SpaceShip*** ships_out,
                               size_t*      count_out,
                               Planet**     planets,
                               size_t       planet_count)
{
    FILE* f = fopen(path, "r");
    if (!f) return errno;

    SpaceShip** arr = NULL;
    size_t cap = 0, cnt = 0;
    char*  line = NULL;
    size_t n    = 0;

    while (getline(&line, &n, f) != -1) {
        strip_newline(line); if (!*line) continue;

        char* parts[5]; char* p = line;
        for (int i = 0; i < 5; ++i) {
            parts[i] = p;
            p = strchr(p, '#');
            if (!p && i < 4) { fclose(f); free(line); return -1; }
            if (p) *p++ = '\0';
        }

        int d,m,y;
        if (sscanf(parts[3], "%d.%d.%d", &d,&m,&y) != 3) { fclose(f); free(line); return -2; }

        Planet* origin = NULL;
        for (size_t i = 0; i < planet_count; ++i)
            if (strcmp(planet_get_name(planets[i]), parts[1]) == 0) {
                origin = planets[i]; break;
            }
        if (!origin) continue;                    /* hatalı satırı atla */

        const SimTime* otime = planet_get_current_time(origin);
        SimTime dep; simtime_init(&dep, d,m,y, otime->hours_per_day);

        SpaceShip* sp = spaceship_create(parts[0], parts[1], parts[2],
                                         &dep, atoi(parts[4]));
        if (!sp) { fclose(f); free(line); return -3; }

        if (cnt >= cap) {
            size_t newcap = cap ? cap*2 : 128;
            SpaceShip** tmp = realloc(arr, sizeof(SpaceShip*) * newcap);
            if (!tmp) { spaceship_destroy(sp); fclose(f); free(line); return -4; }
            arr = tmp; cap = newcap;
        }
        arr[cnt++] = sp;
    }
    free(line); fclose(f);
    *ships_out = arr; *count_out = cnt;
    return 0;
}
