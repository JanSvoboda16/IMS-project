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

            double lightTime;
            if(today < 65){
                lightTime = MinsToSec(-1.9*today + 455 - 330);       // y = -1.9*x + 455   
            }
            else if(today >= 65 && today < 222){
                lightTime = 0;
            }
            else if(today >= 222 && today < 320){
                lightTime = MinsToSec(1.48*today + 2 - 330); // y = 1.48*x + 2
            }
            else{
                lightTime = MinsToSec(475 - 330);
            }

            if(lightTime > HoursToSec(1.5) && today % 7 < 5){   // mělo by svítit déle než do 7:30 a je všední den
                lightTime = HoursToSec(1.5);
            }  

            std::cout<< "Svetla: ON\n";
            // 6:00 Zapnout
            auto consumer = _consumers["BulbSmall"];
            Enter(consumer->TurnOnPrivilegy);
            consumer->Start(lightTime);
            Wait(lightTime);           
            // 7:30 Vypnout (o víkendu lze i později)
            std::cout<< "Day: " << today << "\n";
            std::cout<< "lightTime: " << lightTime/60 << "\tSvetla: OFF\n";


            double startTime;
            if(today < 124){
                startTime = 1.56*today + 1015 - 30;  // y = 1.56*x + 1015
            }
            else if(today >= 124 && today < 165){
                startTime = 1208 - 30;
            }
            else if(today >= 165 && today < 295){
                startTime = -1.9*today + 1520 - 30; // y = -1.9*x + 1520
            }
            else if(today >= 295 && today < 331){
                startTime = 960 - 30;
            }
            else{
                startTime = 1.56*today + 445 - 30; // y = 1.56*x + 445
            }
            lightTime = HoursToSec(23) - MinsToSec(startTime);

            Activate(GetTime(today,0,0,0) + MinsToSec(startTime));

            std::cout<< "Time: "<< startTime << "\tSvetla: ON\n";
            // Zapnout
            consumer = _consumers["BulbBig"];
            Enter(consumer->TurnOnPrivilegy);
            consumer->Start(lightTime);
            Wait(lightTime);           
            // 23:00 Vypnout
            std::cout<< "Svetla: OFF\n";

            Activate(GetTime(today+1,6,0,1));
        }
    }
};