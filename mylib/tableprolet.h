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

struct Satellite{
    int satellite; ///< КА
    int filled_inf; ///< объем заполненной памяти
    double filled_inf_percent; ///< объем заполненной памяти в %
};

class TableProletRF
{
public:
    TableProletRF();

    double TimeDifference(TimeZoneRF zone1, TimeZoneRF zone2);
    double TimeDifference30(TimeZoneRF zone1, TimeZoneRF zone2);
    void IsUpload(std::vector<TimeZoneRF> &rf_trace_vitok_list);
    static bool Comparator(const TimeZoneRF& zone1,const TimeZoneRF& zone2);
    std::vector<TimeZoneRF> SortTable(std::vector<TimeZoneRF> ProletRF);
private:
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


class TableZRV
{
public:
    TableZRV();

private:

};
}
#endif // TABLEPROLET_H
