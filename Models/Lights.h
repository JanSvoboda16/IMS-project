#pragma once
#include "simlib.h"
#include "EnergyConsumer.h"
#include <map>
#include "TimeTransfer.h"

class Lights : public Process{
private:
    std::map<std::string, EnergyConsumer*> _consumers;

public:
    Lights(std::map<std::string, EnergyConsumer*> consumers){
        _consumers = consumers;
    }

    void Behavior(){
        while(true){
            auto today = getDay();

            //std::cout<< "Svetla: ON\n";
            // 6:00 Zapnout
            auto consumer = _consumers["BulbSmall"];
            Enter(consumer->TurnOnPrivilegy);
            consumer->Start(HoursToSec(1.5));
            Wait(HoursToSec(1.5));           
            // 7:30 Vypnout
           // std::cout<< "Svetla: OFF\n";

            Activate(GetTime(today,18,0,0));

            //std::cout<< "Svetla: ON\n";
            // 18:00 Zapnout
            consumer = _consumers["BulbBig"];
            Enter(consumer->TurnOnPrivilegy);
            consumer->Start(HoursToSec(5));
            Wait(HoursToSec(5));           
            // 23:00 Vypnout
            //std::cout<< "Svetla: OFF\n";

            Activate(GetTime(today+1,6,0,1));
        }
    }
};