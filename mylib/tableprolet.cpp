#include "tableprolet.h"
#include <time.h>
#include <algorithm>
using namespace proletRF;
using namespace proletZRV;

TableProletRF::TableProletRF()
{

}

TableZRV::TableZRV()
{

}

double TableProletRF::TimeDifference(TimeZoneRF zone1, TimeZoneRF zone2){
    time_t first = mktime(&(zone1.tm_start)) * 1000 + zone1.milisecs_start;
    time_t second = mktime(&(zone2.tm_start)) * 1000 + zone2.milisecs_start;
    double differ = (first - second) / 1000;

    return differ;
}

//добавил себе отдельно функцию, потому что мне нужно вычитать из начала конец.
double TableProletRF::TimeDifference30(TimeZoneRF zone1, TimeZoneRF zone2){
    time_t first = mktime(&zone1.tm_start) * 1000 + zone1.milisecs_start;
    time_t second = mktime(&zone2.tm_end) * 1000 + zone2.milisecs_end;
    double differ = (first - second);

    return differ / 1000;
}

// если пролет (на ВИТКЕ) с 18:00 до 9:00, то это Сброс (2)
void TableProletRF::IsUpload(std::vector<TimeZoneRF> &rf_trace_vitok_list) {
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

// компоратор для сравнения двух структур таблицы "пролет"
bool TableProletRF::Comparator(const TimeZoneRF& zone1,const  TimeZoneRF& zone2){
    tm zone1_start=zone1.tm_start;
    tm zone2_start=zone2.tm_start;
    tm zone1_end=zone1.tm_end;
    tm zone2_end=zone1.tm_end;
    time_t first_start = mktime(&zone1_start) * 1000 + zone1.milisecs_start;
    time_t second_start = mktime(&zone2_start) * 1000 + zone2.milisecs_start;
    time_t first_end = mktime(&zone1_end) * 1000 + zone1.milisecs_end;
    time_t second_end = mktime(&zone2_end) * 1000 + zone2.milisecs_end;
    if(first_start!=second_start){
        return first_start<second_start;
    } else if(first_end!=second_end){
        return first_end<second_end;
    }else{
        return false;
    }
}

// компоратор для сортировки по витку
bool TableProletRF::ZoneComporator(const TimeZoneRF& zone1,const TimeZoneRF& zone2){
    return zone1.vitok<zone2.vitok;
}

//функция в которой происходит сортировка таблицы
std::vector<TimeZoneRF> TableProletRF::SortTable(std::vector<TimeZoneRF> ProletRF){
    std::sort(ProletRF.begin(), ProletRF.end(), &TableProletRF::Comparator);
    // добавить в однук сортировку не получилось иначе происходит "каша" с данными
    std::sort(ProletRF.begin(), ProletRF.end(), &TableProletRF::ZoneComporator);
    return ProletRF;
}

double TableProletRF::get_bitrate(SATELLITE_TYPE type){
    if (type == SATELLITE_TYPE::KINOSPUTNIK) {
        return 1.0;
    } else if (type == SATELLITE_TYPE::ZORKIY) {
        return 0.25;
    } else
        return 0.0;
}

double TableProletRF::get_tank_size(SATELLITE_TYPE type){
    if (type == SATELLITE_TYPE::KINOSPUTNIK) {
        return 1024.0;
    } else if (type == SATELLITE_TYPE::ZORKIY) {
        return 512.0;
    } else
        return 0.0;
}
