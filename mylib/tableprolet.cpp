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
            break;
        }
    }
    return res;
}

double TableZRV::shooting(proletRF::TimeZoneRF prolet, double duration, std::vector<proletRF::Satellite> &satellites) {
    double res = -1.0;
    int i = 0;
    for(auto sat: satellites){
        if (sat.satellite == prolet.satellite) {
            satellites.at(i).filled_inf += duration * sat.shooting_speed;
            satellites.at(i).filled_inf_percent = satellites.at(i).filled_inf / satellites.at(i).tank;
            if (satellites.at(i).filled_inf_percent > 1.0) { //если переполнили при фото, то выставляем ровно полный бак
                TableProletRF t;
                satellites.at(i).filled_inf = t.get_tank_size(satellites.at(i).type);
                satellites.at(i).filled_inf_percent = 1.0;
            }
            satellites.at(i).last_prolet = prolet;
            satellites.at(i).last_prolet.task = SATELLITE_TASK::SHOOTING;
            res = satellites.at(i).filled_inf_percent;
            break;
        }
        i++;
    }
    return res;
}
//поиск между двух зрв если flag==1 , то входит начало пограничного ЗРВ(строго) а если
// flag==2 то входит от конца первого зрв до начала второго зрв(строго)
std::vector<proletZRV::ZRV> TableZRV::find_ZRV_between_2_prolet(std::vector<proletZRV::ZRV> table_zrv, const proletRF::TimeZoneRF &prolet1, const proletRF::TimeZoneRF &prolet2, int flag){
    std::vector<proletZRV::ZRV> result;
    result.reserve(1);

    char start1 [80];
    char end1 [80];
    strftime (start1, 80, "%d.%m.%Y %H:%M:%S.", &prolet1.tm_start);
    strftime (end1, 80, "%d.%m.%Y %H:%M:%S.", &prolet1.tm_end);
    std::string prolet1_start(start1);
    std::string prolet1_end(end1);
    prolet1_start += std::to_string(prolet1.milisecs_start);
    prolet1_end += std::to_string(prolet1.milisecs_end);

    char start2 [80];
    char end2 [80];
    strftime (start2, 80, "%d.%m.%Y %H:%M:%S.", &prolet2.tm_start);
    strftime (end2, 80, "%d.%m.%Y %H:%M:%S.", &prolet2.tm_end);
    std::string prolet2_start(start2);
    std::string prolet2_end(end2);
    prolet2_start += std::to_string(prolet2.milisecs_start);
    prolet2_end += std::to_string(prolet2.milisecs_end);

    for(const auto& tmpZRV: table_zrv){
        char start3 [80];
        char end3 [80];
        strftime (start3, 80, "%d.%m.%Y %H:%M:%S.", &tmpZRV.tm_start);
        strftime (end3, 80, "%d.%m.%Y %H:%M:%S.", &tmpZRV.tm_end);
        std::string zrv_start(start3);
        std::string zrv_end(end3);
        zrv_start += std::to_string(tmpZRV.milisecs_start);
        zrv_end += std::to_string(tmpZRV.milisecs_end);
        if (prolet2.satellite == tmpZRV.satellite) {
            //входит от конца первого пролета до начала второго пролета
            if(zrv_start > prolet1_end && zrv_start < prolet2_start && zrv_end < prolet2_start && flag==1){
                result.push_back(tmpZRV);
            }
            //входит от конца первого пролета и начало ЗРВ раньше конца 2-го пролета
            if(zrv_start > prolet1_end && zrv_start < prolet2_end && flag==2){
                result.push_back(tmpZRV);
            }
        }
    }
    return result;
}

void TableZRV::analyze_task(std::vector<proletRF::TimeZoneRF> &proletyRF, std::vector<proletZRV::ZRV> &zrv_list , std::vector<proletRF::Satellite> &satellites){
    for(auto cur_sat: proletyRF){
        if (get_current_tank_size(cur_sat.satellite, satellites) > 0.60) {
            proletRF::TimeZoneRF before = find_before(satellites, cur_sat.satellite);
            std::vector<proletZRV::ZRV> zrv = find_ZRV_between_2_prolet(zrv_list, before, cur_sat, 1); //вектор всех возможных подходящих ЗРВ
            if (zrv.empty()) {
                zrv = find_ZRV_between_2_prolet(zrv_list, before, cur_sat, 2); //вектор всех возможных подходящих ЗРВ
                if (!zrv.empty()) { //если нашли ЗРВ
                    for (auto zrv_between: zrv) { // для каждой найденной ЗРВ
                        bool cross = cross_zrv_check(satellites, zrv_between, zrv_list);
                        int x = 0;
                    }
                    int a = 0;
                }
                int c = 0;
            } else {
                for (auto cross_zrv: zrv) {
                    bool cross = cross_zrv_check(satellites, cross_zrv, zrv_list);
                }
                int b = 0;
            }
        } else {
            int b = 0;
            std::cout  << "sat#" << cur_sat.satellite << " photo="<< shooting(cur_sat, cur_sat.duration, satellites) << std::endl;
        }
    }
//    }
}

proletRF::TimeZoneRF TableZRV::find_before(std::vector<Satellite> satellites, int curr_sat){
    proletRF::TimeZoneRF res;
    for(auto sat: satellites){
        if (sat.satellite == curr_sat) {
            res = sat.last_prolet;
            break;
        }
    }
    return res;
}

double TableZRV::upload(proletRF::TimeZoneRF prolet, proletZRV::ZRV zrv, std::vector<proletRF::Satellite> &satellites, std::pair<std::vector<proletZRV::AnswerData>, double>&answer){
    double res = 1.0;
    int i = 0;
    for(auto sat: satellites){
        if (sat.satellite == prolet.satellite) {
            double to_send = zrv.duration * sat.bitrate;
            if(to_send > satellites.at(i).filled_inf){
                to_send = to_send - (to_send - satellites.at(i).filled_inf);
            }
            satellites.at(i).filled_inf -= zrv.duration * sat.bitrate;
            answer.second+=to_send;
            satellites.at(i).filled_inf_percent = satellites.at(i).filled_inf / satellites.at(i).tank;
//            if (satellites.at(i).filled_inf_percent < 0.0) { //если ушли в минус при сбросе, то выставляем полностью пустой бак
//                TableProletRF t;
//                satellites.at(i).filled_inf = t.get_tank_size(satellites.at(i).type);
//                satellites.at(i).filled_inf_percent = 1.0;
//            } кажись убежал от этого
            satellites.at(i).last_prolet = prolet;
            satellites.at(i).last_prolet.task = SATELLITE_TASK::UPLOAD;
            res = satellites.at(i).filled_inf_percent;
            AnswerData ans;
            ans.milisecs_end=zrv.milisecs_end;
            ans.milisecs_start=zrv.milisecs_start;
            ans.ppi=zrv.ppi;
            ans.satellite=sat.satellite;
            ans.tm_end=zrv.tm_end;
            ans.tm_start=zrv.tm_start;
            ans.transfered_inf= to_send;
            answer.first.push_back(ans);
            break;
        }
        i++;
    }
    return res;
}

bool TableZRV::cross_zrv_check(std::vector<proletRF::Satellite> satellites, proletZRV::ZRV target_zrv, std::vector<ZRV> table_zrv) {
    std::vector<proletZRV::ZRV> result;
    result.reserve(1);

    char target_zrv_start1 [80];
    char target_zrv_end1 [80];
    strftime (target_zrv_start1, 80, "%d.%m.%Y %H:%M:%S.", &target_zrv.tm_start);
    strftime (target_zrv_end1, 80, "%d.%m.%Y %H:%M:%S.", &target_zrv.tm_end);
    std::string target_zrv_start(target_zrv_start1);
    std::string target_zrv_end(target_zrv_end1);
    target_zrv_start += std::to_string(target_zrv.milisecs_start);
    target_zrv_end += std::to_string(target_zrv.milisecs_end);

    for(const auto& tmpZRV: table_zrv){
        char tmpZRV_start1 [80];
        char tmpZRV_end1 [80];
        strftime (tmpZRV_start1, 80, "%d.%m.%Y %H:%M:%S.", &tmpZRV.tm_start);
        strftime (tmpZRV_end1, 80, "%d.%m.%Y %H:%M:%S.", &tmpZRV.tm_end);
        std::string tmpZRV_start(tmpZRV_start1);
        std::string tmpZRV_end(tmpZRV_end1);
        tmpZRV_start += std::to_string(tmpZRV.milisecs_start);
        tmpZRV_end += std::to_string(tmpZRV.milisecs_end);

        if (tmpZRV.ppi == target_zrv.ppi) { // ЗРВ для текущего ППИ
            if (tmpZRV_end < target_zrv_start || tmpZRV_start > target_zrv_end) { //заканчивается раньше начала или начинается позже конца, т.е. не пересекается совсем
                continue;
            } else {
                if (tmpZRV.satellite != target_zrv.satellite) {
                    result.push_back(tmpZRV);
                }
            }
        }
    }

    if (!result.empty()) {
        for(const auto& tmpZRV: result){
            double target_ka_tank = get_current_tank_size(target_zrv.satellite, satellites);
            double tmp_ka_tank = get_current_tank_size(tmpZRV.satellite, satellites);
            int a = 0;
        }
    } else {
        return true;
    }
    return true;
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
//proletRF::TimeZoneRF TableProletRF::makeTZforVitok(std::vector<TimeZoneRF> ProletRF, int sat, int vitok){
//    int id;
//    int tmp=0;
//    TimeZoneRF resTZ;

//    for(auto cur_prolet: ProletRF){
//        auto pred = [cur_prolet, sat](const TimeZoneRF & item) {
//            return (cur_prolet.satellite == sat) ;
//        };

//        std::cout<< cur_prolet.satellite << " " << sat << " tmp=" << tmp << std::endl;
//        auto it = std::find_if(std::begin(ProletRF) + tmp, std::end(ProletRF), pred);
//        if (it != std::end(ProletRF)) {

//            //if ()
//            id = std::distance(std::begin(ProletRF) + tmp, it);
//            //std::cout<< id << std::endl;
//            tmp = tmp + id;
//            if(id==0){
//                tmp+=1;
//            }
//            std::cout<<ProletRF.at(id).duration<< std::endl;
//        } else {
//            //std::cout<<"h"<<std::endl;
//            int b = 0;
//        }
//    }
//    return resTZ;
//}



