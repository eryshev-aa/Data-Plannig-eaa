#include "inputfileshandler.h"

#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <algorithm>

InputFileRFHandler::InputFileRFHandler()
{

}

vector <string> InputFileRFHandler::getFileNamesInDir_FS(string dirName){
    vector <string> result;

    for (const auto & entry : std::filesystem::directory_iterator(dirName)) {
        result.push_back(entry.path());
    }

    return result;
}

vector <string> InputFileRFHandler::getFileNamesInDir_QDir(string dirName){
    vector <string> result;

    QFileInfoList filesList;
    QDir filesDir(QString::fromStdString(dirName));
    filesDir.setFilter(QDir::Files);
    filesList = filesDir.entryInfoList();
    if (filesList.count() > 0) {
        for (int i = 0; i < filesList.size(); ++i) {
            QFileInfo fileInfo = filesList.at(i);
            result.push_back(fileInfo.absolutePath().toStdString() + "/" + fileInfo.fileName().toStdString());
        }
    } else {
        cout << "Data folder " << dirName << " is empty!" << endl;
        result.clear();
        return result;
    }

    return result;
}

bool InputFileRFHandler::make_RF_trace_list(string dirPath, vector<proletRF::TimeZoneRF> &rf_trace_list, vector<Satellite> &sattelites_list) {
    //return true;
    //vector <string> files = getFileNamesInDir_FS(dirPath); //gcc: filesistem
    vector <string> files = getFileNamesInDir_QDir(dirPath); //gcc+MinGW: QDir
    string line;
//    int vitok = 0;
    proletRF::TimeZoneRF tz_current/*, tz_previos*/;
    proletRF::Satellite satellite;

    for (const auto& entry : files) {
        //std::cout << "!========== " << entry.path() << std::endl;
        ifstream in_RF_file(entry);

        if (in_RF_file.is_open())
        {
            getline(in_RF_file, line);
            //std::vector<TimeZoneRF> rf_trace_vitok_list;
            TableProletRF prolet;
            while (std::getline(in_RF_file, line))
            {
                if ((line.find("Civil Air Patrol Use Only") != std::string::npos) ||
                    (line.find("AreaTarget-Russia-To-Satellite") != std::string::npos ) ||
                    (line.find("------") != std::string::npos) ||
                    (line.find("Access") != std::string::npos) ||
                    (line.find("Min Duration") != std::string::npos) ||             // В нашем алгоритме
                    (line.find("Max Duration") != std::string::npos) ||             // эти значения
                    (line.find("Mean Duration") != std::string::npos) ||            // не используются.
                    (line.find("Total Duration") != std::string::npos) ||           // Но может их не просто так дали...?
                    (line.find("Global Statistics") != std::string::npos) ||
                    (line[0] == '\0')) {
//                    if (vitok != 0) {
//                        prolet.IsUpload(rf_trace_vitok_list);
//                        for (auto vitok: rf_trace_vitok_list) {
//                            rf_trace_list.push_back(vitok);
//                        }
//                        rf_trace_vitok_list.clear();
//                    }
                    continue;
                } else if (line.find("Russia-To-KinoSat_") != std::string::npos) {
                    int sat_number = std::stoi(line.substr(18,6));  // получаем номер КА
                    satellite.satellite = sat_number;
                    satellite.filled_inf = 0;
                    satellite.filled_inf_percent = 0.0;
                    satellite.type = SATELLITE_TYPE::KINOSPUTNIK;
                    satellite.bitrate = prolet.get_bitrate(SATELLITE_TYPE::KINOSPUTNIK);
                    satellite.tank = prolet.get_tank_size(SATELLITE_TYPE::KINOSPUTNIK);
                    sattelites_list.push_back(satellite);
                    tz_current.satellite = sat_number;
//                    if (vitok != 0) {
//                        prolet.IsUpload(rf_trace_vitok_list);
//                        for (auto vitok: rf_trace_vitok_list) {
//                            rf_trace_list.push_back(vitok);
//                        }
//                        rf_trace_vitok_list.clear();
//                    }
//                    vitok = 0;
                } else {
                    string rf_trace_start_time = line.substr(28,24);
                    string start = rf_trace_start_time.substr(0,20);
                    if (start[0] == ' ') {
                        start[0] = '0';
                    }
                    struct tm tm = {};
                    istringstream inTime1(start);
                    inTime1 >> get_time(&tm,"%d %b %Y %H:%M:%S");
                    tz_current.tm_start = tm;
                    tz_current.milisecs_start = std::stoi(rf_trace_start_time.substr(21,3));

                    string rf_trace_end_time   = line.substr(56,24);
                    string end = rf_trace_end_time.substr(0,20);
                    if (end[0] == ' ') {
                        end[0] = '0';
                    }
                    istringstream inTime2(end);
                    inTime2 >> get_time(&tm,"%d %b %Y %H:%M:%S");
                    tz_current.tm_end = tm;
                    tz_current.milisecs_end = std::stoi(rf_trace_end_time.substr(21,3));

                    string rf_trace_duration_str = line.substr(91,7);
                    size_t ptr = -1;
                    double rf_trace_duration = std::stod(rf_trace_duration_str, &ptr);
                    if (ptr = 3) {
                        std::replace(rf_trace_duration_str.begin(), rf_trace_duration_str.end(), '.', ','); // у меня в Линукс почему-то работает на "," а не на "."
                        rf_trace_duration = std::stod(rf_trace_duration_str);
                    }
                    tz_current.duration = rf_trace_duration;

                    if ((tz_current.tm_end.tm_hour >= 18 && tz_current.tm_end.tm_min >= 0 && tz_current.tm_end.tm_sec >= 0) ||
                        (tz_current.tm_start.tm_hour < 9 && tz_current.tm_start.tm_min >= 0 && tz_current.tm_start.tm_sec >= 0)) {
                        tz_current.task = SATELLITE_TASK::UPLOAD;
                    } else {
                        tz_current.task = SATELLITE_TASK::WAIT;
                    }

                    rf_trace_list.push_back(tz_current);
//                    if (vitok != 0) {
//                        // нужно сравнить с концом предыдущего пролета
//                        double dif = prolet.TimeDifference30(tz_current,tz_previos);
//                        if (dif > 1800.0) {
//                            prolet.IsUpload(rf_trace_vitok_list);
//                            for (auto vitok: rf_trace_vitok_list) {
//                                rf_trace_list.push_back(vitok);
//                            }
//                            vitok++;
//                            tz_current.vitok = vitok;
//                            rf_trace_vitok_list.clear();
//                            rf_trace_vitok_list.push_back(tz_current);
//                        } else {
//                            tz_current.vitok = vitok;
//                            rf_trace_vitok_list.push_back(tz_current);
//                        }
//                        //

//                        tz_previos = tz_current;
//                    } else { //vitok = 0
//                        vitok ++;
//                        tz_current.vitok = vitok;
//                        rf_trace_vitok_list.push_back(tz_current);
//                        tz_previos = tz_current;
//                    }
                }
            }
        } else
        {
            return false;
        }
        in_RF_file.close();
    }

    return true;
}

bool InputFileRFHandler::make_ZRV_trace_list(string dirPath, vector<ZRV> &zrv_trace_list) {
    //return true;
    vector <string> files = getFileNamesInDir_FS(dirPath);
    string line;
    string ppi;
    proletZRV::ZRV zrv_current;

    for (const auto& entry : files) {
        //std::cout << "!========== " << entry.path() << std::endl;
        ifstream in_ZRV_file(entry);

        if (in_ZRV_file.is_open())
        {
            getline(in_ZRV_file, line);
            getline(in_ZRV_file, line);
            while (std::getline(in_ZRV_file, line))
            {
                if (line.find("Facility-") != std::string::npos) {
                    ppi = line.substr(line.find('-') + 1,50);
                    ppi = ppi.substr(0,ppi.find('-'));
                    zrv_current.ppi = ppi;
                } else if (line.find(ppi + "-To-") != std::string::npos) {
                    int sat_number = std::stoi(line.substr(line.find('_') + 1,6));  // получаем номер КА
                    zrv_current.satellite = sat_number;
                } else if ((line.find("------") != std::string::npos) ||
                           (line.find("Access") != std::string::npos) ||
                           (line.find("Min Duration") != std::string::npos) ||             // В нашем алгоритме
                           (line.find("Max Duration") != std::string::npos) ||             // эти значения
                           (line.find("Mean Duration") != std::string::npos) ||            // не используются.
                           (line.find("Total Duration") != std::string::npos) ||          // Но может их не просто так дали...?
                           (line.find("Global Statistics") != std::string::npos) ||
                           (line[0] == '\0')) {
                                continue;
                } else {
                    string zrv_start_time = line.substr(28,24);
                    string start = zrv_start_time.substr(0,20);
                    if (start[0] == ' ') {
                        start[0] = '0';
                    }
                    struct tm tm = {};
                    istringstream inTime1(start);
                    inTime1 >> get_time(&tm,"%d %b %Y %H:%M:%S");
                    zrv_current.tm_start = tm;
                    zrv_current.milisecs_start = std::stoi(zrv_start_time.substr(21,3));

                    string zrv_end_time   = line.substr(56,24);
                    string end = zrv_end_time.substr(0,20);
                    if (end[0] == ' ') {
                        end[0] = '0';
                    }
                    istringstream inTime2(end);
                    inTime2 >> get_time(&tm,"%d %b %Y %H:%M:%S");
                    zrv_current.tm_end = tm;
                    zrv_current.milisecs_end = std::stoi(zrv_end_time.substr(21,3));

                    string zrv_duration_str = line.substr(91,7);
                    size_t ptr = -1;
                    double zrv_duration   = std::stod(zrv_duration_str, &ptr);
                    if (ptr = 3) {
                        std::replace(zrv_duration_str.begin(), zrv_duration_str.end(), '.', ','); // у меня в Линукс почему-то работает на "," а не на "."
                        zrv_duration = std::stod(zrv_duration_str);
                    }
                    zrv_current.duration = zrv_duration;

                    zrv_trace_list.push_back(zrv_current);
                    //std::cout << zrv_start_time << "==" << zrv_end_time << "==" << zrv_duration << std::endl;
                }
            }
        } else
        {
            return false;
        }
        in_ZRV_file.close();
    }

    return true;
}
