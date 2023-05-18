#ifndef TABLE_PROLET_H
#define TABLE_PROLET_H
#include <time.h>

class table_prolet
{
public:
    table_prolet();

    struct TimeZone{
        struct tm tm_start{};/// время начала без мили секунд
        int milisecs_start;/// миллисекунды начала
        int satellite;///КА
        int turn;///виток
        double duration;///продолжительность
        int problem;///задача
    };

    double TimeDifference(TimeZone zone1, TimeZone zone2);
};

#endif // TABLE_PROLET_H
