#pragma once
#include <map>

#include "Boiler.h"
#include "EnergyConsumer.h"
#include "TimeTransfer.h"
#include "simlib.h"

class Children : public Process {
   private:
    std::map<std::string, EnergyConsumer*> _consumers;
    int _id;
    Boiler* _boiler;
    int _firstDayOfSim;

   public:
    Children(std::map<std::string, EnergyConsumer*> consumers, int id, Boiler* boiler, int firstDayOfSim) {
        _consumers = consumers;
        _id = id;
        _boiler = boiler;
        _firstDayOfSim = firstDayOfSim;
    }
    void Behavior() {
        while (true) {
            auto today = GetDay();
            auto dayInYear = today + _firstDayOfSim;
            bool summerTime = false;

            if (dayInYear > 90 && dayInYear < 304) {
                summerTime = true;                
            }else{
                Wait(HoursToSec(1));
            }

            // Uvari si caj
            auto consumer = _consumers["Kettle"];
            Enter(consumer->TurnOnPrivilegy);
            consumer->Start(60);  // 2 min/l -> 0.5 l
            Wait(20);

            // Ohreje si snidani
            consumer = _consumers["Microwave"];
            Enter(consumer->TurnOnPrivilegy);
            consumer->Start(90);  // 1.5 min -> snidane

            if (today % 7 < 5) {  // Vsední den

                // Navrat ze skoly 15:00
                Activate(GetTime(today, 15 - summerTime, 0, 0));

                // Po prichodu za 0 - 3 h
                Wait(Uniform(0, HoursToSec(3)));

                // Vysaje si pokoj
                if (Random() <= 0.2) {  // 20%
                    consumer = _consumers["Vacuum"];
                    Enter(consumer->TurnOnPrivilegy);
                    auto jobTime = Normal(MinsToSec(5), MinsToSec(1));
                    consumer->Start(jobTime);  // cca 5 min vysava
                    Wait(jobTime);
                }

                // Zapne si pocitac
                if (_id == 1) {
                    consumer = _consumers["Notebook1"];
                } else {
                    consumer = _consumers["Notebook2"];
                }
                Enter(consumer->TurnOnPrivilegy);
                auto timeOnComputer = Uniform(MinsToSec(30), HoursToSec(4));
                consumer->Start(timeOnComputer);  // Dite stravi u pocitace 0.5 - 4 hodiny
                Wait(timeOnComputer);

            } else {
                // Po ranu za 0 - 2 h
                Wait(Uniform(0, HoursToSec(2)));

                // Vysaje si pokoj
                if (Random() <= 0.5) {  // 50%
                    consumer = _consumers["Vacuum"];
                    Enter(consumer->TurnOnPrivilegy);
                    auto jobTime = Normal(MinsToSec(5), MinsToSec(1));
                    consumer->Start(jobTime);  // cca 5 min vysava
                    Wait(jobTime);
                }

                // Zapne si pocitac
                if (_id == 1) {
                    consumer = _consumers["Notebook1"];
                } else {
                    consumer = _consumers["Notebook2"];
                }
                Enter(consumer->TurnOnPrivilegy);
                auto timeOnComputer = Uniform(HoursToSec(2), HoursToSec(8));
                consumer->Start(timeOnComputer);  // Dite stravi u pocitace 2 - 8 hodin
                Wait(timeOnComputer);
            }


            auto evening = GetTime(today, 20-summerTime,0,0);
            if(Time < evening){
                Activate(evening);
            }
            
            Wait(HoursToSec(Uniform(0.5, 2)));

            // sprcha
            Seize(_boiler->ShowerFacility);
            double showerTime = MinsToSec(Normal(10, 2.5));
            _boiler->RemoveWater(showerTime * 0.14, 40);  // 0.14 l/s => namereno
            Release(_boiler->ShowerFacility);

            Activate(GetTime(today + 1, 6-1, 45, 0));
        }
    }
};