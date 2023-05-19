#include "mylib.h"
#include <time.h>
#include <iomanip>
#include <sstream>

#include <QTextStream>
#include <QFileInfo>

#include "inputfileshandler.h"
#include "tableprolet.h"

using namespace proletRF;

Mylib::Mylib()
{

}

bool Mylib::readInputData(string dirRF, string dirZRV){
    vector <proletRF::TimeZoneRF> proletyRF;
    vector <proletZRV::ZRV> proletyZRV;
    vector<Satellite> sattelites_list;

    InputFileRFHandler in;
    bool rf = in.make_RF_trace_list(dirRF, proletyRF, sattelites_list);
    bool zrv = in.make_ZRV_trace_list(dirZRV, proletyZRV);

    if (rf && zrv) {
        return true;
    } else
        return false;
}


    //ниже все старые функции. Пока их не удаляю
vector <Mylib::Ppi> Mylib::makePpi(string file) {
    QString fileSrt = QString::fromStdString(file);

    QFile restrictionsFile(fileSrt);
    if (!restrictionsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout << "File restrictions open error" << endl;
    }

    int i = 0;
    QTextStream in(&restrictionsFile);

    while (!in.atEnd()){
        int ppi2=0;
        int tmp;

        string tmpLn = in.readLine().toStdString();
        if (tmpLn.find("#") != std::string::npos) { //добавил в файл ограничений комментарий, используя символ #. Поэтому такие строки пропускаем.
            continue;
        }

        istringstream l(tmpLn);
        l >> tmp;

        if (ppi2 == tmp){
            int t;
            l >> t;
            m_kp[i-1].kp.push_back(t);
            vector <int> temp;

            while (l >> t){
                temp.push_back(t);
            }

            m_kp[i-1].agreed.push_back(temp);

            for(int j = 0; j < i; j++){
                m_kp[i-1].isbusy.push_back("0000-00-00 00:00:00");
                m_kp[i-1].dopelganger.push_back(make_pair("0000-00-00 00:00:00",0));
            }
            //kp[i-1].isbusy.resize(kp[i-1].kp.size());
        } else {
            i++;
            int t;
            l >> t;

            Mylib::Ppi v;
            v.ppi=tmp;
            v.kp.push_back(t);

            vector <int> temp;

            while(l >> t){
                temp.push_back(t);
            }

            v.agreed.push_back(temp);
            v.isbusy.push_back("0000-00-00 00:00:00"); // add by EAA
            v.dopelganger.push_back(make_pair("0000-00-00 00:00:00", 0));
            m_kp.push_back(v);
        }
        ppi2 = tmp;
    }

    restrictionsFile.close();

    return m_kp;
}

vector <Mylib::Time> Mylib::readZRV(vector <string> files){
    if (!files.empty()) {
        for (int i = 0; i < files.size(); i++) {
            QFile dataFile(QString::fromStdString(files.at(i)));
            if (dataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream inStrTime(&dataFile);
                QFileInfo fileInfo(dataFile.fileName());
                QString fileName = fileInfo.fileName(); //файлы с ЗРВ должны называться по номеру ППИ
                int ppi = fileName.toInt();

                while (!inStrTime.atEnd()) {
                    string tmpLn = inStrTime.readLine().toStdString();
                    if (tmpLn.find("#") != std::string::npos) { //добавил в файл ограничений комментарий, используя символ #. Поэтому такие строки пропускаем.
                        continue;
                    }
                    string start_day, end_day, start_time, end_time;
                    int satellite,transmission,bandwidth;

                    istringstream iss(tmpLn);
                    iss >> satellite >> start_day >> start_time >> end_day >> end_time >> bandwidth >> transmission;

                    string start = start_day + " " + start_time;
                    string end = end_day + " " + end_time;
                    int work = transmission/(bandwidth);
                    struct tm tm = {};
                    istringstream inTime(start);
                    inTime >> get_time(&tm,"%Y-%m-%d %H:%M:%S");
                    time_t mytime = mktime(&tm) + work;
                    localtime_r(&mytime, &tm);
                    string tmp_end_time;
                    ostringstream oss;
                    oss << put_time(&tm,"%Y-%m-%d %H:%M:%S");
                    tmp_end_time = oss.str();
                    oss.clear();
                    oss.str("");
                    inTime=istringstream(end);
                    inTime>> get_time(&tm,"%Y-%m-%d %H:%M:%S");
                    mytime = mktime(&tm) - work;
                    localtime_r(&mytime, &tm);
                    oss << put_time(&tm,"%Y-%m-%d %H:%M:%S");
                    string start_max=oss.str();
                    if(tmp_end_time<=end){ /// проверка условия на то, будет ли время передача всей информации больше, времени свзи, если меньше, то добавляем данные
                        //cout<<tmp_end_time<<" "<<end<<" "<<start<<" "<<work<< "\n";
                        m_segments.push_back({start, end, start_max, tmp_end_time, satellite, ppi, bandwidth, transmission, work});
                    }
                }

                dataFile.close();
            } else {
                cout << "File <" << files.at(i) << "> open error!" << endl;
                cout << dataFile.errorString().toStdString() << endl;
            }
        }
    } else {
        cout << "Data folder is empty." << endl;
    }

    return m_segments;
}

bool Mylib::compareTimes(const Mylib::Time& timeA, const Mylib::Time& timeB){
    if (timeA.start_time != timeB.start_time){
        return timeA.start_time < timeB.start_time;
    } else if (timeA.end_time != timeB.end_time){
        return (timeA.end_time < timeB.end_time) ;
    } else {
        return false;
    }
}

bool Mylib::compareInTransmission(const Mylib::Time& timeA, const Mylib::Time& timeB){
    if(timeA.start_time == timeB.start_time && timeA.ppi==timeB.ppi){
        return timeA.transmission > timeB.transmission;
    }
    return false;
}

bool Mylib::compareAnsTimes(const Mylib::AnsVector& vecA,const Mylib::AnsVector& vecB){
    if (vecA.start_time != vecB.start_time){
        return vecA.start_time < vecB.start_time;
    } else if (vecA.end_time != vecB.end_time){
        return vecA.end_time < vecB.end_time;
    } else {
        return false;
    }
}

vector <Mylib::AnsVector> Mylib::planning(vector<Mylib::Time>segments, vector<Mylib::Ppi>kp){
    vector <Mylib::AnsVector> answer;
    answer.reserve(1);
    int flag;

    std::sort(segments.begin(),segments.end(), &Mylib::compareTimes);
    std::sort(segments.begin(),segments.end(), &Mylib::compareInTransmission);

    DodrySatellite(segments, kp, answer);
    Mylib::AnsVector t;
    for(const auto &segment:segments){
        flag = 0;
        for( auto &kpi: kp){
            if(segment.ppi == kpi.ppi){
                for (auto i = 0; i < kpi.agreed.size(); i++){
                    for(auto j = 0; j < kpi.agreed[i].size(); j++){
                        if((kpi.agreed[i][j] == segment.satellite) && (segment.max_start_time > kpi.isbusy[i])){
                            if(kpi.isbusy[i] != "0000-00-00 00:00:00"){
                                if(segment.start_time + " " + segment.end_time == kpi.dopelganger[i].first){
                                    struct tm tm = {};
                                    istringstream inTime(kpi.isbusy[i]);
                                    inTime >> get_time(&tm,"%Y-%m-%d %H:%M:%S");
                                    time_t mytime_start = mktime(&tm) + 1;
                                    time_t mytime_end = mktime(&tm) + segment.work_time + 1;
                                    localtime_r(&mytime_end, &tm);
                                    ostringstream oss;
                                    oss << put_time(&tm,"%Y-%m-%d %H:%M:%S");
                                    t.end_time = oss.str();
                                    kpi.isbusy[i] = oss.str();
                                    oss.clear();
                                    oss.str("");
                                    localtime_r(&mytime_start, &tm);
                                    oss << put_time(&tm,"%Y-%m-%d %H:%M:%S");
                                    t.start_time = oss.str();
                                    kpi.dopelganger[i].second = 1;
                                }else{
                                    struct tm tm = {};
                                    istringstream inTime(kpi.isbusy[i]);
                                    inTime >> get_time(&tm,"%Y-%m-%d %H:%M:%S");
                                    time_t mytime_fend = mktime(&tm);
                                    istringstream inTime1(segment.start_time);
                                    inTime1 >> get_time(&tm,"%Y-%m-%d %H:%M:%S");
                                    time_t mytime_start = mktime(&tm);
                                    if(mytime_start - mytime_fend == 0){
                                        mytime_start += 1;
                                        localtime_r(&mytime_start, &tm);
                                        ostringstream oss;
                                        oss << put_time(&tm,"%Y-%m-%d %H:%M:%S");
                                        t.start_time = oss.str();
                                        oss.clear();
                                        oss.str("");
                                        mytime_start += segment.work_time;
                                        localtime_r(&mytime_start, &tm);
                                        oss << put_time(&tm,"%Y-%m-%d %H:%M:%S");
                                        t.end_time = oss.str();
                                    }else{
                                        t.end_time = segment.end_transfer;
                                        t.start_time = segment.start_time;
                                    }
                                    kpi.isbusy[i] = segment.end_transfer;
                                }
                            }else{
                                kpi.dopelganger[i] = make_pair(segment.start_time + " " + segment.end_time, 0);
                                t.end_time = segment.end_transfer;
                                t.start_time = segment.start_time;
                                kpi.isbusy[i] = segment.end_transfer;
                            }
                            t.satellite = segment.satellite;
                            t.bandwidth = segment.bandwidth;
                            t.kp = kpi.kp[i];
                            t.ppi = kpi.ppi;
                            answer.push_back(t);
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 1) break;
                }
                if (flag == 1){
                    break;
                }
            }
        }
    }
    std::sort(answer.begin(),answer.end(), &Mylib::compareAnsTimes);

    return answer;
}

//приданный(Dodry) КА
void Mylib::DodrySatellite(vector<Mylib::Time>& sec, vector<Mylib::Ppi>& kp,vector <Mylib::AnsVector>& answer){
    int delited = 0;
    Mylib::AnsVector t;
    vector<Mylib::Time> tmp = sec;
    for(auto &kpi: kp){
        for(int i = 0; i < kpi.agreed.size(); i++){
            if(kpi.agreed[i].size() == 1 ){
                for(int j = 0; j < tmp.size(); j++){
                    if(tmp[j].satellite == kpi.agreed[i][0] && tmp[j].ppi == kpi.ppi){
                        if(kpi.isbusy[i] < tmp[j].max_start_time){
                            //std::cout<<kpi.isbusy[i]<<std::endl;
                            if(kpi.isbusy[i]=="0000-00-00 00:00:00"){
                                t.end_time = tmp[j].end_transfer;
                                t.start_time = tmp[j].start_time;
                                kpi.isbusy[i] = tmp[j].end_transfer;
                            }else{
                                struct tm tm = {};
                                istringstream inTime(kpi.isbusy[i]);
                                inTime >> get_time(&tm,"%Y-%m-%d %H:%M:%S");
                                time_t mytime_start = mktime(&tm) + 1;
                                time_t mytime_end = mktime(&tm) + tmp[j].work_time+1;
                                localtime_r(&mytime_end, &tm);
                                ostringstream oss;
                                oss << put_time(&tm,"%Y-%m-%d %H:%M:%S");
                                t.end_time = oss.str();
                                kpi.isbusy[i] = oss.str();
                                oss.clear();
                                oss.str("");
                                localtime_r(&mytime_start, &tm);
                                oss << put_time(&tm,"%Y-%m-%d %H:%M:%S");
                                t.start_time = oss.str();
                            }
                            t.satellite = tmp[j].satellite;
                            t.bandwidth=tmp[j].bandwidth;
                            t.kp = kpi.kp[i];
                            t.ppi = kpi.ppi;
                            answer.push_back(t);

                            if(sec.size()-1>delited){
                            sec.erase(sec.begin()+j-delited);
                            }else{
                            sec.erase(sec.begin());
                            }
                            delited++;
                        }
                    }
                }
            }
        }
    }
}

//обслуживание КА
vector<pair<Mylib::Time, Mylib::Ppi>> Mylib::ServiceSatellite(vector<Mylib::Time>segments, vector<Mylib::Ppi>kp){
    vector<pair<Mylib::Time, Mylib::Ppi>> myres;
    for(int i = 0; i < kp.size(); i++){
        for(int j = 0; j < segments.size(); j++){
            for(const auto &agree : kp[i].agreed){
                for(int k = 0; k < agree.size();k++){
                    if(agree[k] == segments[j].satellite && kp[i].ppi == segments[j].ppi){
                        myres.push_back(make_pair(segments[j],kp[i]));
                    }
                }
            }
        }
    }
    return myres;
}

bool Mylib::sendReady(string outFileName, vector<Mylib::AnsVector> answer){
    QFile outfile(QString::fromStdString(outFileName));

    if (!outfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        cout << "Output file open error" << endl;
        return false;
    } else {
        QTextStream out(&outfile);
        out << "Result output " << "\n";
        for(const auto& segment:answer){

            out << segment.ppi << " " << segment.kp << " " << segment.satellite << " "
                << QString::fromStdString(segment.start_time)<< " " << QString::fromStdString(segment.end_time) <<"\n";
        }
        outfile.close();
        return true;
    }
}
