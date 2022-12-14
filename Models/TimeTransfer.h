#pragma once
#include "simlib.h"

int GetDay(){
    return (int)Time/24/60/60;
}

int GetTime(int day, int hour, int minute, int second ){
    return ((day * 24 + hour) * 60 + minute) * 60 + second;
}

int DaysToSec(int days){
    return days * 24 * 60 * 60;
}

int HoursToSec(double hours){
    return (int)(hours * 60 * 60);
}

int MinsToSec(double minutes){
    return (int)(minutes * 60);
}