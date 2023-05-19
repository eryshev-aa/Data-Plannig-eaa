#include "tableprolet.h"
#include <time.h>

using namespace proletRF;

TableProletRF::TableProletRF()
{

}

double TableProletRF::TimeDifference(TimeZone zone1, TimeZone zone2){
    time_t first = mktime(&(zone1.tm_start)) * 1000 + zone1.milisecs_start;
    time_t second = mktime(&(zone2.tm_start)) * 1000 + zone2.milisecs_start;
    double differ = (first - second) / 1000;

    return differ;
}

//добавил себе отдельно функцию, потому что мне нужно вычитать из начала конец.
double TableProletRF::TimeDifference30(TimeZone zone1, TimeZone zone2){
    time_t first = mktime(&zone1.tm_start) * 1000 + zone1.milisecs_start;
    time_t second = mktime(&zone2.tm_end) * 1000 + zone2.milisecs_end;
    double differ = (first - second);

    return differ / 1000;
}

// если пролет (на ВИТКЕ) с 18:00 до 9:00, то это Сброс (2)
void TableProletRF::IsUpload(std::vector<TimeZone> &rf_trace_vitok_list) {
    int result = 0;
    for (const auto& vitok : rf_trace_vitok_list) {
        if ((vitok.tm_end.tm_hour >= 18 && vitok.tm_end.tm_min >= 0 && vitok.tm_end.tm_sec >= 0) || (vitok.tm_start.tm_hour < 9 && vitok.tm_start.tm_min >= 0 && vitok.tm_start.tm_sec >= 0)) {
            result++;
        }
    }
    if (result > 0) {
        for (auto& vitok : rf_trace_vitok_list) {
            vitok.task = SATELLITE_TASK::UPLOAD;
        }
    } else {
        for (auto& vitok : rf_trace_vitok_list) {
            vitok.task = SATELLITE_TASK::WAIT;
        }
    }
}
