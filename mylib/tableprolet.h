#ifndef TABLEPROLET_H
#define TABLEPROLET_H

#include <time.h>

#include <vector>
#include <string>

namespace proletRF {
/*!
   * \brief Набор состояний спутника
   */
enum class SATELLITE_TASK {
    WAIT = 0, ///< Ожидание (ничего не делаем)
    SHOOTING = 1, ///< Съемка (фотографирование)
    UPLOAD = 2,   ///< Сброс данных
};

struct TimeZoneRF{
    struct tm tm_start = {}; ///< время начала пролета над РФ без мили секунд
    int milisecs_start; ///< миллисекунды начала пролета над РФ
    struct tm tm_end = {}; ///< время конца пролета над РФ без мили секунд
    int milisecs_end; ///< миллисекунды начала пролета над РФ
    int satellite; ///< КА
    double duration; ///< продолжительность
    SATELLITE_TASK task; ///< задача
};

/*!
   * \brief Набор типов спутников
   */
enum class SATELLITE_TYPE {
    KINOSPUTNIK = 0, ///< Киноспутник
    ZORKIY = 1, ///< Зоркий
};

struct Satellite{
    int satellite; ///< КА
    SATELLITE_TYPE type; ///< тип КА
    double filled_inf; ///< объем заполненной памяти в Гбит
    double filled_inf_percent; ///< объем заполненной памяти в %
    double tank; ///< размер памяти в Гбит
    double bitrate; ///< скорость передечи в Гбит/с
    double shooting_speed = 0.5; ///< скорость заполенеия изображениями в Гбайт/с
    TimeZoneRF last_prolet; ///< пролет
};

class TableProletRF
{
public:
    TableProletRF();

    std::vector<TimeZoneRF> SortTable(std::vector<TimeZoneRF> ProletRF);

    double get_bitrate(SATELLITE_TYPE type);
    double get_tank_size(SATELLITE_TYPE type);

private:
    static bool Comparator(const TimeZoneRF &zone1,const TimeZoneRF &zone2);
    std::vector <TimeZoneRF> m_prolety_nad_RF;
};
}

namespace proletZRV {
struct ZRV{
    struct tm tm_start = {}; ///< время начала ЗРВ без мили секунд
    int milisecs_start; ///< миллисекунды начала ЗРВ
    struct tm tm_end = {}; ///< время конца ЗРВ без мили секунд
    int milisecs_end; ///< миллисекунды начала ЗРВ
    int satellite; ///< КА
    std::string ppi; ///< пункт приема информации (ППИ/НС)
    double duration; ///< продолжительность
};


struct AnswerData{
    struct tm tm_start = {}; ///< время начала ЗРВ без мили секунд
    int milisecs_start; ///< миллисекунды начала ЗРВ
    struct tm tm_end = {}; ///< время конца ЗРВ без мили секунд
    int milisecs_end; ///< миллисекунды начала ЗРВ
    double duration; ///< продолжительность сброса
    int satellite; ///< КА
    std::string ppi; ///< пункт приема информации (ППИ/НС)
    double transfered_inf; ///< объем переданной памяти в Гбит
    double tank_balance; ///< остаток в баке в %
};

class TableZRV
{
public:
    TableZRV(std::string upload_path, std::string shoot_path, int check_upload, int check_shoot);
    static bool ZRVComporator(const proletZRV::ZRV &zone1,const proletZRV::ZRV &zone2);
    static bool AnswerComporator(const proletZRV::AnswerData &zone1,const proletZRV::AnswerData &zone2);
    std::vector<proletZRV::ZRV> SortZRV(std::vector<proletZRV::ZRV> tableZRV);
    std::vector<proletZRV::AnswerData> SortAnswer(std::vector<proletZRV::AnswerData> answer);

    void AnalyzeTask(std::vector<proletRF::TimeZoneRF> &proletyRF, std::vector<proletZRV::ZRV> &zrv_list , std::vector<proletRF::Satellite> &satellites, std::vector <proletZRV::AnswerData> &answer);
private:
    proletRF::TimeZoneRF find_before(std::vector<proletRF::Satellite> satellites, int curr_sat);
    std::vector<proletZRV::ZRV> find_ZRV_between_2_prolet(std::vector<proletZRV::ZRV> TableZRV, const proletRF::TimeZoneRF prolet1, const proletRF::TimeZoneRF &prolet2, int flag);
    double shooting(proletRF::TimeZoneRF prolet, double duration, std::vector<proletRF::Satellite> &satellites, proletRF::Satellite &cur_sat);
    double upload(proletRF::TimeZoneRF prolet, proletZRV::ZRV zrv, std::vector<proletRF::Satellite> &satellites, std::vector<AnswerData> &answer);
    double get_current_tank_size(int sat_number, std::vector<proletRF::Satellite> satellites);

    bool cross_zrv_check(std::vector<proletRF::Satellite> satellites, std::vector<proletZRV::ZRV>, std::vector<proletZRV::ZRV> table_zrv, proletZRV::ZRV &using_zrv);
    void analyze_after_prolet(std::vector<proletZRV::ZRV> &zrv_list, std::vector<proletRF::Satellite> &satellites, std::vector <proletZRV::AnswerData> &answer);
    void delete_ZRV_after_upload(proletZRV::ZRV cur_zrv, std::vector<ZRV> &table_zrv);
    void delete_1_ZRV_upload(proletZRV::ZRV cur_zrv, std::vector<ZRV> &table_zrv);
    void delete_ZRV_after_prolet(proletRF::Satellite satellite, std::vector<ZRV> &table_zrv);
    void makeResult_for_upload(int pos);
    void makeResult_for_shoot(int pos);
    std::string makeOutputStringMsec(int msec);

    double m_total_upload = 0.0;
    int m_check_pos_upload;
    int m_check_pos_shoot;
    std::string m_shoot_file;
    bool isMakeShootFile =false;
    std::string m_upload_file;
    bool isMakeUploadFile = false;
    std::string m_live_result_file;
    std::vector<proletZRV::AnswerData> m_shoot_data;
    std::vector<proletZRV::AnswerData> m_upload_data;
};
}
#endif // TABLEPROLET_H
