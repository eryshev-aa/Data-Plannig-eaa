#include "tableprolet.h"
#include <time.h>
#include <algorithm>
#include <iostream>
using namespace proletRF;
using namespace proletZRV;

TableProletRF::TableProletRF()
{

}

TableZRV::TableZRV()
{

}

double TableProletRF::TimeDifference(TimeZoneRF zone) {
    time_t first = mktime(&(zone.tm_end)) * 1000 + zone.milisecs_end;
    time_t second = mktime(&(zone.tm_start)) * 1000 + zone.milisecs_start;
    double differ = (first - second);

    return differ;
}

//добавил себе отдельно функцию, потому что мне нужно вычитать из начала конец.
double TableProletRF::TimeDifference30(TimeZoneRF zone1, TimeZoneRF zone2) {
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
bool TableProletRF::Comparator(const TimeZoneRF &zone1, const  TimeZoneRF &zone2) {
    char start1 [80];
    char end1 [80];
    strftime (start1, 80, "%d.%m.%Y %H:%M:%S.", &zone1.tm_start);
    strftime (end1, 80, "%d.%m.%Y %H:%M:%S.", &zone1.tm_end);
    std::string buff1_start(start1);
    std::string buff1_end(end1);
    buff1_start += std::to_string(zone1.milisecs_start);
    buff1_end += std::to_string(zone1.milisecs_end);

    char start2 [80];
    char end2 [80];
    strftime (start2, 80, "%d.%m.%Y %H:%M:%S.", &zone2.tm_start);
    strftime (end2, 80, "%d.%m.%Y %H:%M:%S.", &zone2.tm_end);
    std::string buff2_start(start2);
    std::string buff2_end(end2);
    buff2_start += std::to_string(zone2.milisecs_start);
    buff2_end += std::to_string(zone2.milisecs_end);

    if (buff1_start != buff2_start){
        return buff1_start < buff2_start;
    } else if(buff1_end != buff2_end){
        return buff1_end < buff2_end;
    } else{
        return false;
    }
}

// компоратор для сортировки по витку
bool TableProletRF::ZoneComporator(const TimeZoneRF& zone1, const TimeZoneRF& zone2){
    return zone1.vitok < zone2.vitok;
}

//функция в которой происходит сортировка таблицы
std::vector<TimeZoneRF> TableProletRF::SortTable(std::vector<TimeZoneRF> ProletRF){
    std::sort(ProletRF.begin(), ProletRF.end(), &TableProletRF::Comparator);
    // добавить в однук сортировку не получилось иначе происходит "каша" с данными
    //std::sort(ProletRF.begin(), ProletRF.end(), &TableProletRF::ZoneComporator);
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

// компоратор для сравнения двух структур таблицы ZRV
bool TableZRV::ZRVComporator(const proletZRV::ZRV& zone1,const proletZRV::ZRV& zone2){
    //return zone1.duration > zone2.duration;
    char start1 [80];
    char end1 [80];
    strftime (start1, 80, "%d.%m.%Y %H:%M:%S.", &zone1.tm_start);
    strftime (end1, 80, "%d.%m.%Y %H:%M:%S.", &zone1.tm_end);
    std::string buff1_start(start1);
    std::string buff1_end(end1);
    buff1_start += std::to_string(zone1.milisecs_start);
    buff1_end += std::to_string(zone1.milisecs_end);

    char start2 [80];
    char end2 [80];
    strftime (start2, 80, "%d.%m.%Y %H:%M:%S.", &zone2.tm_start);
    strftime (end2, 80, "%d.%m.%Y %H:%M:%S.", &zone2.tm_end);
    std::string buff2_start(start2);
    std::string buff2_end(end2);
    buff2_start += std::to_string(zone2.milisecs_start);
    buff2_end += std::to_string(zone2.milisecs_end);

    if (buff1_start != buff2_start){
        return buff1_start < buff2_start;
    } else if(buff1_end != buff2_end){
        return buff1_end < buff2_end;
    } else{
        return false;
    }
}

std::vector<proletZRV::ZRV> TableZRV::SortZRV(std::vector<proletZRV::ZRV> tableZRV){
    std::sort(tableZRV.begin(), tableZRV.end(), &TableZRV::ZRVComporator);
    return tableZRV;
}

double TableZRV::get_current_tank_size(int sat_number, std::vector<proletRF::Satellite> satellites) {
    double res = -1.0;
    for(auto sat: satellites){
        if (sat.satellite == sat_number) {
            res = sat.filled_inf_percent;
        }
    }
    return res;
}
//поиск между двух зрв если flag==1 , то входит начало пограничного ЗРВ(строго) а если
// flag==2 то входит от конца первого зрв до начала второго зрв(строго)
std::vector<proletZRV::ZRV> TableZRV::find_between_two(std::vector<proletZRV::ZRV>table_zrv, const proletZRV::ZRV zone1, const proletZRV::ZRV zone2, int flag){
    std::vector<proletZRV::ZRV> result;
    result.reserve(1);
    char start1 [80];
    char end1 [80];
    strftime (start1, 80, "%d.%m.%Y %H:%M:%S.", &zone1.tm_start);
    strftime (end1, 80, "%d.%m.%Y %H:%M:%S.", &zone1.tm_end);
    std::string buff1_start(start1);
    std::string buff1_end(end1);
    buff1_start += std::to_string(zone1.milisecs_start);
    buff1_end += std::to_string(zone1.milisecs_end);
    char start2 [80];
    char end2 [80];
    strftime (start2, 80, "%d.%m.%Y %H:%M:%S.", &zone2.tm_start);
    strftime (end2, 80, "%d.%m.%Y %H:%M:%S.", &zone2.tm_end);
    std::string buff2_start(start2);
    std::string buff2_end(end2);
    buff2_start += std::to_string(zone2.milisecs_start);
    buff2_end += std::to_string(zone2.milisecs_end);
    for(const auto& tmpZRV:table_zrv){
        char start3 [80];
        char end3 [80];
        strftime (start3, 80, "%d.%m.%Y %H:%M:%S.", &tmpZRV.tm_start);
        strftime (end3, 80, "%d.%m.%Y %H:%M:%S.", &tmpZRV.tm_end);
        std::string buff3_start(start3);
        std::string buff3_end(end3);
        buff3_start += std::to_string(tmpZRV.milisecs_start);
        buff3_end += std::to_string(tmpZRV.milisecs_end);
        //входит начало пограничного ЗРВ
        if(buff3_start>buff1_end && buff3_start<buff2_end && buff3_end<buff2_end && flag==2){
            result.push_back(tmpZRV);
        }
        //входит от конца первого зрв до начала второго зрв
        if(buff3_start>buff1_end && buff3_start<buff2_start && buff3_end<buff2_start && flag==1){
            result.push_back(tmpZRV);
        }
    }
    return result;
}

void TableZRV::analyze_task(std::vector<proletRF::TimeZoneRF> &proletyRF, std::vector<ZRV> &zrv_list , std::vector<proletRF::Satellite> &satelllites){
    for(auto cur_sat: proletyRF){
        if (get_current_tank_size(cur_sat.satellite, satelllites) > 60.0) {

        }
    }
//    }
}

//std::vector<TimeZoneRF> TableProletRF::proletyNaVitke(std::vector<TimeZoneRF> &ProletRF, int vitok) {
//    std::vector<TimeZoneRF> res;

//    for(auto cur_prolet: ProletRF){
//        if (cur_prolet.vitok == vitok)
//        {
//            res.push_back(cur_prolet);
//        }
//    }

//    return res;
//}

//если на витке несколько пролетов, то сделать начало от перовго, а конец от последнего.
proletRF::TimeZoneRF TableProletRF::makeTZforVitok(std::vector<TimeZoneRF> ProletRF, int sat, int vitok){
    int id;
    int tmp=0;
    TimeZoneRF resTZ;

    for(auto cur_prolet: ProletRF){
        auto pred = [cur_prolet, sat](const TimeZoneRF & item) {
            return (cur_prolet.satellite == sat) ;
        };

        std::cout<< cur_prolet.satellite << " " << sat << " tmp=" << tmp << std::endl;
        auto it = std::find_if(std::begin(ProletRF) + tmp, std::end(ProletRF), pred);
        if (it != std::end(ProletRF)) {

            //if ()
            id = std::distance(std::begin(ProletRF) + tmp, it);
            //std::cout<< id << std::endl;
            tmp = tmp + id;
            if(id==0){
                tmp+=1;
            }
            std::cout<<ProletRF.at(id).duration<< std::endl;
        } else {
            //std::cout<<"h"<<std::endl;
            int b = 0;
        }
    }
    return resTZ;
}


void TableProletRF::upload(std::vector<TimeZoneRF> &ProletRF){

}
