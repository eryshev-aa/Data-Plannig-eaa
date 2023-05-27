#include "tableprolet.h"
#include <time.h>
#include <algorithm>
#include <iostream>

#include <cmath>

#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace proletRF;
using namespace proletZRV;

TableProletRF::TableProletRF()
{

}

TableZRV::TableZRV(std::string upload_path, std::string shoot_path, int check_upload, int check_shoot)
{
    m_upload_file = upload_path;
    m_check_pos_upload = check_upload;
    m_shoot_file = shoot_path;
    m_check_pos_shoot = check_shoot;
    m_shoot_data.reserve(1);
    m_upload_data.reserve(1);
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

//функция в которой происходит сортировка таблицы
std::vector<TimeZoneRF> TableProletRF::SortTable(std::vector<TimeZoneRF> ProletRF){
    std::sort(ProletRF.begin(), ProletRF.end(), &TableProletRF::Comparator);
    // добавить в однук сортировку не получилось иначе происходит "каша" с данными
    //std::sort(ProletRF.begin(), ProletRF.end(), &TableProletRF::ZoneComporator);
    return ProletRF;
}

double TableProletRF::get_bitrate(SATELLITE_TYPE type){
    if (type == SATELLITE_TYPE::KINOSPUTNIK) {
        return 0.125;//1.0; //
    } else if (type == SATELLITE_TYPE::ZORKIY) {
        return 0.03125; //0.25;//
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

std::vector<proletZRV::ZRV> TableZRV::SortZRV(std::vector<proletZRV::ZRV> tableZRV) {
    std::sort(tableZRV.begin(), tableZRV.end(), &TableZRV::ZRVComporator);
    return tableZRV;
}

std::vector<proletZRV::AnswerData> TableZRV::SortAnswer(std::vector<proletZRV::AnswerData> answer) {
    std::sort(answer.begin(), answer.end(), &TableZRV::AnswerComporator);
    return answer;
}

bool TableZRV::AnswerComporator(const proletZRV::AnswerData &zone1,const proletZRV::AnswerData &zone2) {
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

double TableZRV::shooting(proletRF::TimeZoneRF prolet, double duration, std::vector<proletRF::Satellite> &satellites, proletRF::Satellite & cur_sat) {
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
            cur_sat = satellites.at(i);
            // заполнение данных для вывода промежуточных в файл
            proletZRV::AnswerData tmp;
            tmp.duration=duration;
            tmp.milisecs_end=prolet.milisecs_end;
            tmp.milisecs_start=prolet.milisecs_start;
            tmp.tm_end=prolet.tm_end;
            tmp.tm_start=prolet.tm_start;
            tmp.transfered_inf=satellites.at(i).filled_inf;
            tmp.satellite=sat.satellite;
            m_shoot_data.push_back(tmp);
            break;
        }
        i++;
    }

    if (m_shoot_data.size()%m_check_pos_shoot == 0) {
        makeResult_for_shoot(m_shoot_data.size());
    }

    return res;
}
//поиск между двух зрв если flag==1 , то входит начало пограничного ЗРВ(строго) а если
// flag==2 то входит от конца первого зрв до начала второго зрв(строго)
std::vector<proletZRV::ZRV> TableZRV::find_ZRV_between_2_prolet(std::vector<proletZRV::ZRV> table_zrv, const proletRF::TimeZoneRF prolet1, const proletRF::TimeZoneRF &prolet2, int flag){
    std::vector<proletZRV::ZRV> result;
    result.reserve(1);

    //double time_spent = 0.0;
    //clock_t begin = clock();

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
    std::copy_if(table_zrv.begin(), table_zrv.end(), std::back_inserter(result),[prolet2,prolet1_start,prolet1_end,prolet2_start,prolet2_end,flag](proletZRV::ZRV tmpZRV){
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
            if (flag==1) {
                return zrv_start > prolet1_end && zrv_start < prolet2_start && zrv_end < prolet2_start;
            }
            //входит от конца первого пролета и начало ЗРВ начинается раньше конца второго пролета
            if (flag==2) {
                return zrv_start > prolet1_end && zrv_start < prolet2_end;
            }

        }
        return false;
    });

    return result;
}

void TableZRV::AnalyzeTask(std::vector<proletRF::TimeZoneRF> &proletyRF, std::vector<proletZRV::ZRV> &zrv_list , std::vector<proletRF::Satellite> &satellites,std::vector<proletZRV::AnswerData>& answer){
    int counterRF = 0;
    int totalCountRF = proletyRF.size();

    bool finish_checks = false;

    for(auto cur_prolet: proletyRF){
        if (counterRF%50 == 0) {
            std::cout << "Prolet = " << counterRF << "/" << totalCountRF << ", ZRV total count = " << zrv_list.size() << std::endl;
        }
        finish_checks = false;
        proletRF::Satellite sat;
        if (get_current_tank_size(cur_prolet.satellite, satellites) > 0.60) {
            while (get_current_tank_size(cur_prolet.satellite, satellites) > 0.60) { //пока в баке более 60% пытаемся найти ЗРВ для сброса и сбросить
                if (finish_checks) break;
                proletRF::TimeZoneRF before = find_before(satellites, cur_prolet.satellite); // находим время окончания прошлого пролета или время окончаня прошлого сброса
                std::vector<proletZRV::ZRV> zrv = find_ZRV_between_2_prolet(zrv_list, before, cur_prolet, 1); //вектор всех возможных подходящих ЗРВ строго между пролетами этого КА
                if (!zrv.empty()) {
                    proletZRV::ZRV using_zrv;
                    if (get_current_tank_size(cur_prolet.satellite, satellites) == 1.0) { // если у текущего КА полный бак, то забираем эту ЗРВ под сброс
                        upload(cur_prolet, zrv.at(0), satellites, answer);
                        delete_ZRV_after_upload(zrv.at(0), zrv_list);
                        if (get_current_tank_size(cur_prolet.satellite, satellites) <= 0.60) { // если после сброса высвободилось достаточно (<60), то еще и снимаем
                            shooting(cur_prolet, cur_prolet.duration, satellites, sat);
                        }
                        continue;
                    }
                    if (cross_zrv_check(satellites, zrv, zrv_list, using_zrv)) { // если бак не полный, смотрим у кого бак более заполнен
                        upload(cur_prolet, using_zrv, satellites, answer); // если у нас или вообще нет пересечений с другими КА, то мы збрасываем
                        delete_ZRV_after_upload(using_zrv, zrv_list);
                        if (get_current_tank_size(cur_prolet.satellite, satellites) <= 0.60) { // если после сброса высвободилось достаточно (<60), то еще и снимаем
                            shooting(cur_prolet, cur_prolet.duration, satellites, sat);
                        }
                        finish_checks = true;
                        break;
                    } else { // если не удалось найти ЗРВ, т.к. на всех пересечениях у дгурих КА больше данных в баке
                        zrv = find_ZRV_between_2_prolet(zrv_list, before, cur_prolet, 2); //вектор всех возможных подходящих ЗРВ в расширенном интервал
                        if (!zrv.empty()) { //если нашли ЗРВ
                            if (cross_zrv_check(satellites, zrv, zrv_list, using_zrv)) {
                                upload(cur_prolet, using_zrv, satellites, answer);
                                delete_ZRV_after_upload(using_zrv, zrv_list);
                                break;
                            } else { // если и в расширенном интервале все занято, то просто снимаем
                                shooting(cur_prolet, cur_prolet.duration, satellites, sat);
                                finish_checks = true;
                                break;
                            }
                        } else {
                            shooting(cur_prolet, cur_prolet.duration, satellites, sat);
                            finish_checks = true;
                            break;
                        }
                    }
                } else { //если не нашли, то расширяем интервао до коца пролета
                    zrv = find_ZRV_between_2_prolet(zrv_list, before, cur_prolet, 2); //вектор всех возможных подходящих ЗРВ в расширенном интервале
                    if (!zrv.empty()) { //если нашли ЗРВ
                        proletZRV::ZRV using_zrv;
                        if (get_current_tank_size(cur_prolet.satellite, satellites) == 1.0) {
                            upload(cur_prolet, zrv.at(0), satellites, answer);
                            delete_ZRV_after_upload(zrv.at(0), zrv_list);
                            finish_checks = true;
                            break;
                        }
                        if (cross_zrv_check(satellites, zrv, zrv_list, using_zrv)) {
                            upload(cur_prolet, using_zrv, satellites, answer);
                            delete_ZRV_after_upload(using_zrv, zrv_list);
                            finish_checks = true;
                            break;
                        } else {
                            shooting(cur_prolet, cur_prolet.duration, satellites, sat);
                            finish_checks = true;
                            break;
                        }
                    } else {
                        shooting(cur_prolet, cur_prolet.duration, satellites, sat);
                        finish_checks = true;
                        break;
                    }
                    delete_ZRV_after_prolet(sat, zrv_list);
                }
            }
        } else {
            shooting(cur_prolet, cur_prolet.duration, satellites, sat);
            delete_ZRV_after_prolet(sat, zrv_list);
        }

        counterRF++;
    }

    proletyRF.clear(); // удаляем все пролеты
    std::cout << "Finish main analyze." << std::endl;
    analyze_after_prolet(zrv_list, satellites, answer);
}

void TableZRV::analyze_after_prolet(std::vector<proletZRV::ZRV> &zrv_list, std::vector<proletRF::Satellite> &satellites, std::vector <proletZRV::AnswerData> &answer) {
    std::cout << "Start analyze after all prolety." << std::endl;
    int counterSAT = 1;
    for(auto sat: satellites){
        std::cout << "Current satellite = " << counterSAT << "/" << satellites.size() << std::endl;
        if (sat.filled_inf_percent > 0.0) { //если в баке что-то осталось после всех пролетов
            delete_ZRV_after_prolet(sat, zrv_list);
            proletRF::TimeZoneRF zrv_prolet;// = find_before(satellites, sat.satellite);

            while (get_current_tank_size(sat.satellite, satellites) > 0.0 && zrv_list.size() != 0) {
                zrv_prolet.tm_end = zrv_list.at(zrv_list.size() - 1).tm_end;
                zrv_prolet.milisecs_end = zrv_list.at(zrv_list.size() - 1).milisecs_end;
                zrv_prolet.satellite = sat.satellite;//zrv_list.at(zrv_list.size() - 1).satellite;
                std::vector<proletZRV::ZRV> zrv = find_ZRV_between_2_prolet(zrv_list, sat.last_prolet, zrv_prolet, 2); //вектор всех возможных ЗРВ после последнего пролета.
                if (zrv_list.size() == 0 || zrv.size() == 0){
                    sat.last_prolet.tm_end = zrv_list.at(zrv_list.size() - 1).tm_end;
                    sat.last_prolet.milisecs_end = zrv_list.at(zrv_list.size() - 1).milisecs_end;
                    delete_ZRV_after_prolet(sat, zrv_list);
                    break;
                }
                zrv_prolet = find_before(satellites, sat.satellite);

                proletZRV::ZRV using_zrv;
                if (get_current_tank_size(sat.satellite, satellites) == 1.0) { // если у текущего КА полный бак, то забираем эту ЗРВ под сброс
                    upload(zrv_prolet, zrv.at(0), satellites, answer);
                    delete_ZRV_after_upload(zrv.at(0), zrv_list);
                    if (get_current_tank_size(sat.satellite, satellites) == 0.0) {
                        sat.last_prolet.tm_end = zrv_list.at(zrv_list.size() - 1).tm_end;
                        sat.last_prolet.milisecs_end = zrv_list.at(zrv_list.size() - 1).milisecs_end;
                        delete_ZRV_after_prolet(sat, zrv_list);
                        break;
                    }
                    continue;
                }
                if (cross_zrv_check(satellites, zrv, zrv_list, using_zrv)) { // если бак не полный, смотрим у кого бак более заполнен
                    upload(zrv_prolet, using_zrv, satellites, answer); // если у нас или вообще нет пересечений с другими КА, то мы cбрасываем
                    //if(m_shoot_data.size()%100==0){makeResult_for_shoot(m_shoot_data%100);}
                    delete_ZRV_after_upload(using_zrv, zrv_list);
                    if (get_current_tank_size(sat.satellite, satellites) == 0.0) {
                        sat.last_prolet.tm_end = zrv_list.at(zrv_list.size() - 1).tm_end;
                        sat.last_prolet.milisecs_end = zrv_list.at(zrv_list.size() - 1).milisecs_end;
                        delete_ZRV_after_prolet(sat, zrv_list);
                        break;
                    }
                } else {  // если не наш приоритет, то просто удаляем эту ЗРВ
                    delete_1_ZRV_upload(zrv.at(0), zrv_list);
                }
            }
        } else {
            sat.last_prolet.tm_end = zrv_list.at(zrv_list.size() - 1).tm_end;
            sat.last_prolet.milisecs_end = zrv_list.at(zrv_list.size() - 1).milisecs_end;
            delete_ZRV_after_prolet(sat, zrv_list);
        }
        counterSAT++;
    }
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
            ans.tank_balance = satellites.at(i).filled_inf_percent;
            answer.push_back(ans);
            // для вывода в файл
            m_upload_data.push_back(ans);
            m_total_upload += to_send;
            break;
        }
        i++;
    }

    if (m_upload_data.size()%m_check_pos_upload == 0) {
        makeResult_for_upload(m_upload_data.size());
    }

    return res;
}

void TableZRV::delete_1_ZRV_upload(proletZRV::ZRV cur_zrv, std::vector<ZRV> &table_zrv) {
    char cur_zrv_start1[80];
    char cur_zrv_end1[80];
    strftime(cur_zrv_start1,80,"%d.%m.%Y %H:%M:%S.", &cur_zrv.tm_start);
    strftime(cur_zrv_end1,80,"%d.%m.%Y %H:%M:%S.", &cur_zrv.tm_end);
    std::string cur_zrv_start(cur_zrv_start1);
    std::string cur_zrv_end(cur_zrv_end1);
    cur_zrv_start += std::to_string(cur_zrv.milisecs_start);
    cur_zrv_end += std::to_string(cur_zrv.milisecs_end);

    table_zrv.erase(std::remove_if(table_zrv.begin(),table_zrv.end(),[&](const proletZRV::ZRV&zrv){
                        char tmpZRV_start1[80];
                        char tmpZRV_end1[80];
                        strftime(tmpZRV_start1,80, "%d.%m.%Y %H:%M:%S.", &zrv.tm_start);
                        strftime(tmpZRV_end1,80, "%d.%m.%Y %H:%M:%S.", &zrv.tm_end);
                        std::string tmpZRV_start(tmpZRV_start1);
                        std::string tmpZRV_end(tmpZRV_end1);
                        tmpZRV_start += std::to_string(zrv.milisecs_start);
                        tmpZRV_end += std::to_string(zrv.milisecs_end);
                        bool isDelete = false;

                        if (cur_zrv.satellite != zrv.satellite) {
                            return isDelete;
                        }

                        if (cur_zrv.ppi != zrv.ppi) {
                            return isDelete;
                        }
                        if (tmpZRV_end < cur_zrv_start || tmpZRV_start > cur_zrv_end) { //если не пересекается, то и не удаляем
                            return isDelete;
                        }
                        return true;;
                    }),table_zrv.end());
}

void TableZRV::delete_ZRV_after_upload(proletZRV::ZRV cur_zrv, std::vector<ZRV> &table_zrv) {
    char cur_zrv_start1[80];
    char cur_zrv_end1[80];
    strftime(cur_zrv_start1,80,"%d.%m.%Y %H:%M:%S.", &cur_zrv.tm_start);
    strftime(cur_zrv_end1,80,"%d.%m.%Y %H:%M:%S.", &cur_zrv.tm_end);
    std::string cur_zrv_start(cur_zrv_start1);
    std::string cur_zrv_end(cur_zrv_end1);
    cur_zrv_start += std::to_string(cur_zrv.milisecs_start);
    cur_zrv_end += std::to_string(cur_zrv.milisecs_end);

    table_zrv.erase(std::remove_if(table_zrv.begin(),table_zrv.end(),[&](const proletZRV::ZRV&zrv){
        char tmpZRV_start1[80];
        char tmpZRV_end1[80];
        strftime(tmpZRV_start1,80, "%d.%m.%Y %H:%M:%S.", &zrv.tm_start);
        strftime(tmpZRV_end1,80, "%d.%m.%Y %H:%M:%S.", &zrv.tm_end);
        std::string tmpZRV_start(tmpZRV_start1);
        std::string tmpZRV_end(tmpZRV_end1);
        tmpZRV_start += std::to_string(zrv.milisecs_start);
        tmpZRV_end += std::to_string(zrv.milisecs_end);
        bool isDelete = false;

        if (cur_zrv.ppi != zrv.ppi) {
            return isDelete;
        }
        if (tmpZRV_end < cur_zrv_start || tmpZRV_start > cur_zrv_end) { //если не пересекается, то и не удаляем
            return isDelete;
        }
        return true;;
    }),table_zrv.end());
}

void TableZRV::delete_ZRV_after_prolet(proletRF::Satellite satellite, std::vector<ZRV> &table_zrv) {
//  удалить для конкретного КА на всех НС у которых время начала ЗРВ меньше ТТТ
    char T_end[80];
    strftime(T_end,80,"%d.%m.%Y %H:%M:%S.", &satellite.last_prolet.tm_end);
    std::string T(T_end);
    T += std::to_string(satellite.last_prolet.milisecs_end);
    //1.поиск итератора на последний ЗРВ у которого tm_end<table_zrv
    table_zrv.erase(std::remove_if(table_zrv.begin(), table_zrv.end(), [&](const proletZRV::ZRV &zrv){
        char tmpZRV_start1[80];
        strftime(tmpZRV_start1,80, "%d.%m.%Y %H:%M:%S.", &zrv.tm_start);
        std::string tmpZRV_start(tmpZRV_start1);
        tmpZRV_start += std::to_string(zrv.milisecs_start);
        bool isDelete = (satellite.satellite == zrv.satellite) && (tmpZRV_start < T);
        return isDelete;
    }),table_zrv.end());
}

bool TableZRV::cross_zrv_check(std::vector<proletRF::Satellite> satellites, std::vector<proletZRV::ZRV> target_zrv, std::vector<ZRV> table_zrv, proletZRV::ZRV &using_zrv) {
    unsigned long long result = 0;
    std::vector<proletZRV::ZRV> cross_zrv_list;
    cross_zrv_list.reserve(1);

    for (const auto &cur_target_zrv: target_zrv) {
        char target_zrv_start1 [80];
        char target_zrv_end1 [80];
        strftime (target_zrv_start1, 80, "%d.%m.%Y %H:%M:%S.", &cur_target_zrv.tm_start);
        strftime (target_zrv_end1, 80, "%d.%m.%Y %H:%M:%S.", &cur_target_zrv.tm_end);
        std::string target_zrv_start(target_zrv_start1);
        std::string target_zrv_end(target_zrv_end1);
        target_zrv_start += std::to_string(cur_target_zrv.milisecs_start);
        target_zrv_end += std::to_string(cur_target_zrv.milisecs_end);

        cross_zrv_list.clear();
        std::copy_if(table_zrv.begin(), table_zrv.end(), std::back_inserter(cross_zrv_list),[cur_target_zrv, target_zrv_start, target_zrv_end](proletZRV::ZRV tmpZRV){
            char tmpZRV_start1 [80];
            char tmpZRV_end1 [80];
            strftime (tmpZRV_start1, 80, "%d.%m.%Y %H:%M:%S.", &tmpZRV.tm_start);
            strftime (tmpZRV_end1, 80, "%d.%m.%Y %H:%M:%S.", &tmpZRV.tm_end);
            std::string tmpZRV_start(tmpZRV_start1);
            std::string tmpZRV_end(tmpZRV_end1);
            tmpZRV_start += std::to_string(tmpZRV.milisecs_start);
            tmpZRV_end += std::to_string(tmpZRV.milisecs_end);

            if ((tmpZRV.ppi == cur_target_zrv.ppi) && (tmpZRV.satellite != cur_target_zrv.satellite)) {
                return !(tmpZRV_end < target_zrv_start || tmpZRV_start > target_zrv_end); //заканчивается раньше начала или начинается позже конца, т.е. не пересекается совсем
            }
            return false;
        });

        if (!cross_zrv_list.empty()) {
            for(const auto& tmpZRV: cross_zrv_list){
                double target_ka_tank = get_current_tank_size(cur_target_zrv.satellite, satellites);
                double tmp_ka_tank = get_current_tank_size(tmpZRV.satellite, satellites);
                if (target_ka_tank == 1.0) {
                    using_zrv = tmpZRV;
                    return true;
                } else if (target_ka_tank >= tmp_ka_tank) {
                    using_zrv = cur_target_zrv;
                    result ++;
                } else { // если у другого КА бак более заполнен
                    result = 0;
                    break;
                }
                if (result == cross_zrv_list.size()) {
                    return true;
                }
            }
        } else {
            using_zrv = cur_target_zrv;
            return true;
        }
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

void TableZRV::makeResult_for_shoot(int pos){
    int  access = pos==m_check_pos_shoot?1:pos-m_check_pos_shoot+1;

    std::ofstream fout;

    fout.open(m_shoot_file, std::fstream::app);

    time_t t;
    char start [80];
    char end [80];
    pos = pos==m_check_pos_shoot?0:pos-m_check_pos_shoot;

    for (auto i = pos; i < m_shoot_data.size(); i++)
    {
        time (&t);
        strftime (start, 80, "%d.%m.%Y %H:%M:%S.", &m_shoot_data[i].tm_start);
        strftime (end, 80, "%d.%m.%Y %H:%M:%S.", &m_shoot_data[i].tm_end);

        fout << std::setw(6) << std::setprecision(3) << std::right;
        fout << access << "   "
             << start << makeOutputStringMsec(m_shoot_data[i].milisecs_start) << "   "
             << end << makeOutputStringMsec(m_shoot_data[i].milisecs_end) << "   "
             << std::setw(7) << std::fixed << std::right << m_shoot_data[i].duration << "   "
             << std::setw(7) << m_shoot_data[i].satellite
             << std::setw(12)<< m_shoot_data[i].transfered_inf
             << std::endl;

        access ++;
    }

    fout.close();
}

void TableZRV::makeResult_for_upload(int pos){
    int  access = pos==m_check_pos_upload?1:pos-m_check_pos_upload+1;
    std::ofstream fout;

    fout.open(m_upload_file, std::fstream::app);
    time_t t;
    char start [80];
    char end [80];

    //m_total_upload += answerData.at(0).transfered_inf;

    pos = pos==m_check_pos_upload?0:pos-m_check_pos_upload;

    for (auto i = pos ; i < m_upload_data.size(); i++)
    {
        time (&t);
        strftime (start, 80, "%d.%m.%Y %H:%M:%S.", &m_upload_data[i].tm_start);
        strftime (end, 80, "%d.%m.%Y %H:%M:%S.", &m_upload_data[i].tm_end);

        fout << std::setw(6) << std::setprecision(3) << std::right;
        fout << access << "   "
             << start << makeOutputStringMsec(m_upload_data[i].milisecs_start) << "   "
             << end << makeOutputStringMsec(m_upload_data[i].milisecs_end) << "   "
             << std::setw(7) << std::fixed << std::right << m_upload_data[i].duration << "   "
             << std::setw(7) << m_upload_data[i].satellite
             << std::setw(12) << m_upload_data[i].ppi << "   "
             << std::right << m_upload_data[i].transfered_inf << "        "
             << std::setw(7) << m_upload_data[i].tank_balance * 100
             << std::endl;

        access ++;
    }
    fout<<"All uploaded data for now: "<<m_total_upload<<std::endl;
    fout.close();
}

proletZRV::ZRV TableZRV::Adding_zrv_duration(proletZRV::ZRV zone, double duration){
    proletZRV::ZRV tmp=zone;
    double secs;
    double millisecs = 0.0;
    millisecs = std::modf(duration, &secs) * 1000 + zone.milisecs_start;
    int millisecs_int;
    std::time_t tmp_start= std::mktime(&zone.tm_start)+static_cast<int>(secs);
    if(millisecs > 1000){
        tmp_start = tmp_start+(static_cast<int>(millisecs)/1000);
        millisecs_int=static_cast<int>(millisecs)%1000;
    }
    //tmp.tm_start=localtime(&tmp_start);
    tmp.milisecs_start=millisecs_int;

    return tmp;
}
