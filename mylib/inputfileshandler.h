#ifndef INPUTFILESHANDLER_H
#define INPUTFILESHANDLER_H

#include <iostream>
#include <vector>

#include "tableprolet.h"

using namespace std;
using namespace proletRF;

class InputFileRFHandler
{
public:
    InputFileRFHandler();

    bool make_RF_trace_list(string dirPath, vector<TimeZoneRF> &rf_trace_list);
    bool make_ZRV_trace_list(string dirPath);

private:
    vector <string> getFileNamesInDir_FS(string dirName);
};

#endif // INPUTFILESHANDLER_H
