/**
* @author Muhammed Yusuf Yağcı / B211210017
* @since 01/05/2025-18/05/2025
* 1.Öğretim C grubu
*/
#ifdef _WIN32
#include <stdlib.h>  
#endif

#include "Simulation.h"

int main(void)
{
#ifdef _WIN32
    system("chcp 65001 > nul");   /* UTF-8 konsol */
#endif

    Simulation sim;
    if (simulation_initialize(&sim) != 0) {
        return 1;
    }

    simulation_run(&sim);

    simulation_destroy(&sim);     /* ← bellek temizliği */

    return 0;
}
