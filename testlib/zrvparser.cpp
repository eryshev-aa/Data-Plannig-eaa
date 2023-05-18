#include "zrvparser.h"

#include <sstream>
#include <iomanip>
#include <time.h>

ZRVparser::ZRVparser()
{

}

vector <Mylib::Time> ZRVparser::result(vector <QString> files){
    if (!files.empty()) {
        for (int i = 0; i < files.size(); i++) {
            QFile dataFile(files.at(i));
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
                cout << "File <" << files.at(i).toStdString() << "> open error!" << endl;
                cout << dataFile.errorString().toStdString() << endl;
            }
        }
    } else {
        cout << "Data folder is empty." << endl;
    }

    return m_segments;
}


void ZRVparser::SendReady(QFile *outfile, vector<Mylib::AnsVector> answer){
    if (!outfile->open(QIODevice::WriteOnly | QIODevice::Text)) {
        cout << "Output file open error" << endl;
    } else {
        QTextStream out(outfile);
        out << "Result output " << "\n";
        for(const auto& segment:answer){

            out << segment.ppi << " " << segment.kp << " " << segment.satellite << " "
                << QString::fromStdString(segment.start_time)<< " " << QString::fromStdString(segment.end_time) <<"\n";
        }
        outfile->close();
    }
}
