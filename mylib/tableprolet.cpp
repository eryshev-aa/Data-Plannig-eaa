#include "tableprolet.h"
#include <time.h>
#include <algorithm>
#include <iostream>

#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>

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

double TableZRV::TimeDifferenceAAA(proletZRV::AnswerData zone) {
    time_t first = mktime(&(zone.tm_end)) * 1000 + zone.milisecs_end;
    time_t second = mktime(&(zone.tm_start)) * 1000 + zone.milisecs_start;
    double differ = (first - second);

    return differ / 1000;
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
            if (satellites.at(i).filled_inf_percent == 1.0) {
                return 1.0;
            }
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

    double time_spent = 0.0;
    clock_t begin = clock();


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
    std::copy_if(table_zrv.begin(), table_zrv.end(), std::back_inserter(result),[prolet1,prolet2,prolet1_start,prolet1_end,prolet2_start,prolet2_end,flag](proletZRV::ZRV tmpZRV){
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
            if (flag==1) return zrv_start > prolet1_end && zrv_start < prolet2_start && zrv_end < prolet2_start;
            //входит от конца первого пролета и начало ЗРВ начинается раньше конца второго пролета
            if (flag==2) return zrv_start > prolet1_end && zrv_start < prolet2_end;

        }
        return false;
    });

    /*for(const auto& tmpZRV: table_zrv){
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
    }*/
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    // вычислить прошедшее время, найдя разницу (end - begin)
    std::cout << std::setprecision(9) << "find_ZRV_between_2_prolet time is " << time_spent << " sec. "
              << std::fixed << "zrv_count = " << result.size() << std::endl;
    return result;
}

void TableZRV::analyze_task(std::vector<proletRF::TimeZoneRF> &proletyRF, std::vector<proletZRV::ZRV> &zrv_list , std::vector<proletRF::Satellite> &satellites,std::vector<proletZRV::AnswerData>& answer){
    int length = answer.size();
    int counterRF = 0;
    double time_spent = 0.0;
    clock_t begin = clock();
    for(auto cur_sat: proletyRF){
        if (get_current_tank_size(cur_sat.satellite, satellites) > 0.60) {
            proletRF::TimeZoneRF before = find_before(satellites, cur_sat.satellite);
            std::vector<proletZRV::ZRV> zrv = find_ZRV_between_2_prolet(zrv_list, before, cur_sat, 1); //вектор всех возможных подходящих ЗРВ между пролетами этого КА
            if (!zrv.empty()) {
                for (auto zrv_between: zrv) { // для каждой ЗРВ между пролетами проверяем пересечения с другими КА на этом ППИ
                    if (cross_zrv_check(satellites, zrv_between, zrv_list)) {
                        upload(cur_sat, zrv_between, satellites, answer);
                        break; //если нашли ЗРВ, которая не пересекается или у нас самый заполненный бак, то сбрасываем на этой ЗРВ и выходим из пролета.
                    }
                }
                //не нашли подходящей ЗРВ (потому что все пересекаются с более заполнеными КА), расширяем интервал поика, пробуем найти там
                zrv = find_ZRV_between_2_prolet(zrv_list, before, cur_sat, 2); //вектор всех возможных подходящих ЗРВ в расширенном интервале
                if (!zrv.empty()) { //если нашли ЗРВ
                    for (auto zrv_between: zrv) { // для каждой найденной ЗРВ
                        if (cross_zrv_check(satellites, zrv_between, zrv_list)) { // нашли ЗРВ, на которой будем сбрасывать
                            upload(cur_sat, zrv_between, satellites, answer);
                            break;
                        }
                    }
                }
            } else {
                zrv = find_ZRV_between_2_prolet(zrv_list, before, cur_sat, 2); //вектор всех возможных подходящих ЗРВ в расширенном интервал
                if (!zrv.empty()) { //если нашли ЗРВ
                    for (auto zrv_between: zrv) { // для каждой найденной ЗРВ
                        if (cross_zrv_check(satellites, zrv_between, zrv_list)) {
                            upload(cur_sat, zrv_between, satellites, answer);
                            break;
                        }
                    }
                }
            }
        } else {
            shooting(cur_sat, cur_sat.duration, satellites);
        }
        if (answer.size()>length){
            makeResultFile(answer,answer.size(), counterRF);
            length=answer.size();
        }
        counterRF++;
    }
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    std::cout << std::setprecision(9) << "analyze_task time is " << time_spent << " sec. "
              << std::fixed << "counterRF = " << counterRF << std::endl;
}

void TableZRV::analyze_task_new(std::vector<proletRF::TimeZoneRF> &proletyRF, std::vector<proletZRV::ZRV> &zrv_list , std::vector<proletRF::Satellite> &satellites,std::vector<proletZRV::AnswerData>& answer){
    int length = answer.size();
    int counterRF = 0;

    double time_spent = 0.0;
    clock_t begin = clock();

    bool finish_checks = false;

    for(auto cur_prolet: proletyRF){
        if (counterRF == 802) { //после этого пролета крашится
            int a = 0;
        }
        finish_checks = false;
        if (get_current_tank_size(cur_prolet.satellite, satellites) > 0.60) {
            while (get_current_tank_size(cur_prolet.satellite, satellites) > 0.60) { //пока в баке более 60% пытаемся найти ЗРВ для сброса и сбросить
                if (finish_checks) break;
                proletRF::TimeZoneRF before = find_before(satellites, cur_prolet.satellite); // находим время окончания прошлого пролета или время окончаня прошлого сброса
                std::vector<proletZRV::ZRV> zrv = find_ZRV_between_2_prolet(zrv_list, before, cur_prolet, 1); //вектор всех возможных подходящих ЗРВ между пролетами этого КА
                if (!zrv.empty()) {
                    for (auto zrv_between: zrv) { // для каждой ЗРВ между пролетами проверяем пересечения с другими КА на этом ППИ
                        if (cross_zrv_check(satellites, zrv_between, zrv_list)) {
                            upload(cur_prolet, zrv_between, satellites, answer);
                            if (get_current_tank_size(cur_prolet.satellite, satellites) <= 0.60) {
                                shooting(cur_prolet, cur_prolet.duration, satellites);
                            }
                            finish_checks = true;
                            break; //если нашли ЗРВ, которая не пересекается или у нас самый заполненный бак, то сбрасываем на этой ЗРВ и выходим из пролета.
                        } else {
                            zrv = find_ZRV_between_2_prolet(zrv_list, before, cur_prolet, 2); //вектор всех возможных подходящих ЗРВ в расширенном интервал
                            if (!zrv.empty()) { //если нашли ЗРВ
                                for (auto zrv_between: zrv) { // для каждой найденной ЗРВ
                                    if (cross_zrv_check(satellites, zrv_between, zrv_list)) {
                                        upload(cur_prolet, zrv_between, satellites, answer);                                        
                                        finish_checks = true;
                                        break;
                                    } else {
                                        shooting(cur_prolet, cur_prolet.duration, satellites);
                                        finish_checks = true;
                                        break;
                                    }
                                }
                            } else {
                                shooting(cur_prolet, cur_prolet.duration, satellites);
                                finish_checks = true;
                                break;
                            }
                        }
                    }
                } else {
                    zrv = find_ZRV_between_2_prolet(zrv_list, before, cur_prolet, 2); //вектор всех возможных подходящих ЗРВ в расширенном интервал
                    if (!zrv.empty()) { //если нашли ЗРВ
                        for (auto zrv_between: zrv) { // для каждой найденной ЗРВ // if full tank
                            if (get_current_tank_size(cur_prolet.satellite, satellites) == 1.0) {
                                upload(cur_prolet, zrv_between, satellites, answer);
                                finish_checks = true;
                                break;
                            } else {
                                if (cross_zrv_check(satellites, zrv_between, zrv_list)) {
                                    upload(cur_prolet, zrv_between, satellites, answer);
                                    finish_checks = true;
                                    break;
                                } else {
                                    shooting(cur_prolet, cur_prolet.duration, satellites);
                                    finish_checks = true;
                                    break;
                                }
                            }
                        }
                    } else {
                        shooting(cur_prolet, cur_prolet.duration, satellites);
                        finish_checks = true;
                        break;
                    }
                }
            }
        } else {
            shooting(cur_prolet, cur_prolet.duration, satellites);
            //int a = find_ZRV_for_delete(cur_prolet, zrv_list);
        }

        if (answer.size()>length){
            makeResultFile(answer,answer.size(), counterRF);
            length=answer.size();
        }
        counterRF++;
    }
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    std::cout << std::setprecision(9) << "analyze_task_new time is " << time_spent << " sec. "
              << std::fixed << "counterRF = " << counterRF << std::endl;
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

double TableZRV::upload(proletRF::TimeZoneRF prolet, proletZRV::ZRV zrv, std::vector<proletRF::Satellite> &satellites, std::vector<proletZRV::AnswerData> &answer){
    double res = 1.0;
    int i = 0;

    double time_spent = 0.0;
    clock_t begin = clock();

    for(auto sat: satellites){
        if (sat.satellite == prolet.satellite) {
            double to_send = zrv.duration * sat.bitrate;
            if(to_send > satellites.at(i).filled_inf){
                to_send = to_send - (to_send - satellites.at(i).filled_inf);
            }
            satellites.at(i).filled_inf -= to_send;
            satellites.at(i).filled_inf_percent = satellites.at(i).filled_inf / satellites.at(i).tank;
            satellites.at(i).last_prolet = prolet;
            satellites.at(i).last_prolet.milisecs_end = zrv.milisecs_end;
            satellites.at(i).last_prolet.tm_end = zrv.tm_end;
            satellites.at(i).last_prolet.task = SATELLITE_TASK::UPLOAD;
            res = satellites.at(i).filled_inf_percent;
            AnswerData ans;
            ans.milisecs_end = zrv.milisecs_end;
            ans.milisecs_start = zrv.milisecs_start;
            ans.ppi = zrv.ppi;
            ans.satellite = sat.satellite;
            ans.duration = zrv.duration;
            ans.tm_end = zrv.tm_end;
            ans.tm_start = zrv.tm_start;
            ans.transfered_inf = to_send;
            answer.push_back(ans);
            break;
        }
        i++;
    }
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    // вычислить прошедшее время, найдя разницу (end - begin)
    std::cout << std::setprecision(9) << "upload time is " << time_spent << " sec. " << std::endl;
    return res;
}

int TableZRV::find_ZRV_for_delete(proletRF::TimeZoneRF prolet, std::vector<proletZRV::ZRV> &table_zrv) {
    double time_spent =0.0;
    clock_t begin = clock();
    char prolet_end1[80];
    strftime(prolet_end1,80,"%d.%m.%Y %H:%M:%S.", &prolet.tm_end);
    std::string prolet_end(prolet_end1);
    prolet_end+=std::to_string(prolet.milisecs_end);
    //1.поиск итератора на последний ЗРВ у которого tm_end<table_zrv
    table_zrv.erase(std::remove_if(table_zrv.begin(),table_zrv.end(),[&](const proletZRV::ZRV&zrv){
        char tmpZRV_end1[80];
        strftime(tmpZRV_end1,80, "%d.%m.%Y %H:%M:%S.", &zrv.tm_end);
        std::string tmpZRV_end(tmpZRV_end1);
        tmpZRV_end+=std::to_string(zrv.milisecs_end);
        return(prolet.satellite==zrv.satellite)&&(tmpZRV_end < prolet_end);
    }),table_zrv.end());
    //2.удалить все ЗРВ prolet.satellite до итератора last_before
//    if(last_before!=table_zrv.rend()){
//        auto aaa = last_before.base();
//        table_zrv.erase(table_zrv.begin(),last_before.base());
//    }


//    auto last_before=std::find_if(table_zrv.rbegin(), table_zrv.rend(),[&](const proletZRV::ZRV&zrv){
//        char tmpZRV_end1[80];
//        strftime(tmpZRV_end1,80, "%d.%m.%Y %H:%M:%S", &zrv.tm_end);
//        std::string tmpZRV_end(tmpZRV_end1);
//        tmpZRV_end+=std::to_string(zrv.milisecs_end);
//        return(prolet.satellite==zrv.satellite)&&(tmpZRV_end < prolet_end);
//    });
//    //2.удалить все ЗРВ prolet.satellite до итератора last_before
//    if(last_before!=table_zrv.rend()){
//        last_before=last_before.base;
//        table_zrv.erase(std::remove_if(table_zrv.begin,last_before.base(), [&](const proletZRV::ZRV&zrv){
//            return zrv.satellite==prolet.satellite;
//        }),last_before);
//        //table_zrv.erase(table_zrv.begin(),last_before.base());
//    }

    clock_t end =clock();
    time_spent+=(double)(end-begin)/CLOCKS_PER_SEC;
    std::cout<<std::setprecision(9)<<"find_ZRV_for_delete time is "<<time_spent << " sec. "
            << std::fixed << "table_zrv.size = " << table_zrv.size() << std::endl;
    return 1;
}

bool TableZRV::cross_zrv_check(std::vector<proletRF::Satellite> satellites, proletZRV::ZRV target_zrv, std::vector<ZRV> table_zrv) {
    int result = 0;
    std::vector<proletZRV::ZRV> cross_zrv_list;
    cross_zrv_list.reserve(1);

    double time_spent = 0.0;
    clock_t begin = clock();

    char target_zrv_start1 [80];
    char target_zrv_end1 [80];
    strftime (target_zrv_start1, 80, "%d.%m.%Y %H:%M:%S.", &target_zrv.tm_start);
    strftime (target_zrv_end1, 80, "%d.%m.%Y %H:%M:%S.", &target_zrv.tm_end);
    std::string target_zrv_start(target_zrv_start1);
    std::string target_zrv_end(target_zrv_end1);
    target_zrv_start += std::to_string(target_zrv.milisecs_start);
    target_zrv_end += std::to_string(target_zrv.milisecs_end);

    std::copy_if(table_zrv.begin(), table_zrv.end(), std::back_inserter(cross_zrv_list),[target_zrv, target_zrv_start1,target_zrv_end1,target_zrv_start,target_zrv_end](proletZRV::ZRV tmpZRV){
        char tmpZRV_start1 [80];
        char tmpZRV_end1 [80];
        strftime (tmpZRV_start1, 80, "%d.%m.%Y %H:%M:%S.", &tmpZRV.tm_start);
        strftime (tmpZRV_end1, 80, "%d.%m.%Y %H:%M:%S.", &tmpZRV.tm_end);
        std::string tmpZRV_start(tmpZRV_start1);
        std::string tmpZRV_end(tmpZRV_end1);
        tmpZRV_start += std::to_string(tmpZRV.milisecs_start);
        tmpZRV_end += std::to_string(tmpZRV.milisecs_end);

        if ((tmpZRV.ppi == target_zrv.ppi) && (tmpZRV.satellite != target_zrv.satellite)) {
            return !(tmpZRV_end < target_zrv_start || tmpZRV_start > target_zrv_end); //заканчивается раньше начала или начинается позже конца, т.е. не пересекается совсем
        }
        return false;
    });

    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    // вычислить прошедшее время, найдя разницу (end - begin)
    std::cout << std::setprecision(9) << "cross_zrv_check time is " << time_spent << " sec. "
              << std::fixed << "zrv_count = " << table_zrv.size() << std::endl;

    if (!cross_zrv_list.empty()) {
        for(const auto& tmpZRV: cross_zrv_list){
            double target_ka_tank = get_current_tank_size(target_zrv.satellite, satellites);
            double tmp_ka_tank = get_current_tank_size(tmpZRV.satellite, satellites);
            if (target_ka_tank == 1.0) {
                return true;
            } else if (target_ka_tank >= tmp_ka_tank) {
                result ++;
            } else { // если у другого КА бак более заполнен
                return false;
            }
        }
    } else {
        return true;
    }

    if (result > 0 ) {
        return true;
    } else {
        return false;
    }
}

std::string TableZRV::makeOutputStringMsec(int msec) {
    std::string  res;

    if (msec == 0) {
        res = "000";
    } else if (msec > 0 && msec < 10) {
        res = "00" + std::to_string(msec);
    } else if (msec > 9 && msec < 100) {
        res = "0" + std::to_string(msec);
    } else {
        res = std::to_string(msec);
    }

    return res;
}

void TableZRV::makeResultFile(std::vector <proletZRV::AnswerData> answerData, int pos, int counterRF){
    int  access = pos;
    std::ofstream fout;

    fout.open("/home/user/ProfIT-Data-Plannig/result.txt", std::fstream::out | std::fstream::app);
//    fout.open("/home/anton/ProfIT-Data-Plannig/result.txt", std::fstream::out | std::fstream::app);
    time_t t;
    char start [80];
    char end [80];

//    fout << " Access  *  Start Time(UTCG)       *   Stop Time(UTCG)       * dur(s)    *sat_n * NS * Data(Gbit) * Duration" << std::endl;
//    fout << "------------------------------------------------------------------------------------------------------------" << std::endl;
//    fout << std::flush;

    for (auto i=pos-1;i<answerData.size();i++)
    {
        time (&t);
        strftime (start, 80, "%d.%m.%Y %H:%M:%S.", &answerData[i].tm_start);
        strftime (end, 80, "%d.%m.%Y %H:%M:%S.", &answerData[i].tm_end);

        fout << std::setw(6) << std::setprecision(3) << std::right;
        fout << access << "   "
             << start << makeOutputStringMsec(answerData[i].milisecs_start) << "   "
             << end << makeOutputStringMsec(answerData[i].milisecs_end) << "   "
             << std::setw(7) << std::fixed << std::right << answerData[i].duration << "   "
             << std::setw(7) << answerData[i].satellite << "   "
             << std::setw(12) << answerData[i].ppi << "   "
             << std::right << answerData[i].transfered_inf << "   "
             << std::setw(7) << std::right << counterRF
             << std::endl;

        access ++;
    }

    fout.close();
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



