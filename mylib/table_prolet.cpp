#include "table_prolet.h"
#include <time.h>
table_prolet::table_prolet()
{

};
double table_prolet::TimeDifference(table_prolet::TimeZone zone1, table_prolet::TimeZone zone2){
    time_t first=mktime(&(zone1.tm_start))*1000+zone1.milisecs_start;
    time_t second=mktime(&(zone2.tm_start))*1000+zone2.milisecs_start;
    double differ= (first-second)/1000;
    return differ;
};
