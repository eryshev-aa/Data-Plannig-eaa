#ifndef TABLEPROLET_H
#define TABLEPROLET_H
#include <time.h>

#include <vector>

namespace proletRF {

struct  BULLETIN_B {
   int final;       ///< block of Final values
   int preliminary; ///< block of Preliminary extension
};

/*!
   * \brief Набор навигационных систем
   */
enum class SATELLITE_TASK {
    WAIT = 0,
    SHOOTING = 1, ///< Съемка (фотографирование)
    UPLOAD = 2,   ///< Сброс данных
};

struct TimeZone{
    struct tm tm_start = {}; ///< время начала без мили секунд
    int milisecs_start; ///< миллисекунды начала
    struct tm tm_end = {}; ///< время конца без мили секунд
    int milisecs_end; ///< миллисекунды начала
    int satellite; ///< КА
    int vitok; ///< виток
    double duration; ///< продолжительность
    SATELLITE_TASK task; ///< задача
};

class TableProlet
{
public:
    TableProlet();

    double TimeDifference(TimeZone zone1, TimeZone zone2);
    double TimeDifference30(TimeZone zone1, TimeZone zone2);
    void IsUpload(std::vector<TimeZone> &rf_trace_vitok_list);
    bool IsUploadFor1Vitok(TimeZone rf_trace_vitok);

private:
    std::vector <TimeZone> m_prolety_nad_RF;
};
}
#endif // TABLEPROLET_H
