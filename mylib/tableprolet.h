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
    int vitok; ///< виток
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
    int shooting_speed = 4; ///< скорость заполенеия изображениями в Гбит/с
    TimeZoneRF last_prolet; ///< пролет
};

class TableProletRF
{
public:
    TableProletRF();

    double TimeDifference(TimeZoneRF zone);
    double TimeDifference30(TimeZoneRF zone1, TimeZoneRF zone2);
    void IsUpload(std::vector<TimeZoneRF> &rf_trace_vitok_list);

    std::vector<TimeZoneRF> SortTable(std::vector<TimeZoneRF> ProletRF);
    double get_bitrate(SATELLITE_TYPE type);
    double get_tank_size(SATELLITE_TYPE type);



private:
    static bool Comparator(const TimeZoneRF &zone1,const TimeZoneRF &zone2);
    static bool ZoneComporator(const TimeZoneRF &zone1,const TimeZoneRF &zone2);

    //std::vector<TimeZoneRF> proletyNaVitke(std::vector<TimeZoneRF> &ProletRF, int vitok);
    //proletRF::TimeZoneRF makeTZforVitok(std::vector<TimeZoneRF> ProletRF, int sat, int vitok);

    std::vector <TimeZoneRF> m_prolety_nad_RF;
};
}

namespace proletZRV {
struct ZRV{ // КА, НС, время начала, время конца, продолжительность.
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
    int satellite; ///< КА
    std::string ppi; ///< пункт приема информации (ППИ/НС)
    double transfered_inf; ///< объем переданной памяти в Гбит
};

class TableZRV
{
public:
    TableZRV();
    static bool ZRVComporator(const proletZRV::ZRV& zone1,const proletZRV::ZRV& zone2);
    std::vector<proletZRV::ZRV> SortZRV(std::vector<proletZRV::ZRV> tableZRV);

    void analyze_task(std::vector<proletRF::TimeZoneRF> &proletyRF, std::vector<proletZRV::ZRV> &zrv_list , std::vector<proletRF::Satellite> &satellites);

private:
    proletRF::TimeZoneRF find_before(std::vector<proletRF::Satellite> satellites, int curr_sat);
    std::vector<proletZRV::ZRV> find_ZRV_between_2_prolet(std::vector<proletZRV::ZRV> TableZRV, const proletRF::TimeZoneRF &prolet1, const proletRF::TimeZoneRF &prolet2, int flag);
    double shooting(proletRF::TimeZoneRF prolet, double duration, std::vector<proletRF::Satellite> &satellites);
    void upload(std::vector<proletRF::TimeZoneRF> &ProletRF);
    double get_current_tank_size(int sat_number, std::vector<proletRF::Satellite> satellites);
    bool cross_zrv_check(std::vector<proletRF::Satellite> satellites, proletZRV::ZRV target_zrv, std::vector<proletZRV::ZRV> table_zrv);
};
}
#endif // TABLEPROLET_H
