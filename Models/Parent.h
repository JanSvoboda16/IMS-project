#pragma once
#include <map>

#include "Boiler.h"
#include "EnergyConsumer.h"
#include "TimeTransfer.h"
#include "simlib.h"

class Parent : public Process {
   private:
    std::map<std::string, EnergyConsumer*> _consumers;
    Boiler* _boiler;
    int _id;
    int _firstDayOfSim;

   public:
    Parent(std::map<std::string, EnergyConsumer*> consumers, int id, Boiler* boiler, int firstDayOfSim) {
        _consumers = consumers;
        _boiler = boiler;
        _id = id;
        _firstDayOfSim = firstDayOfSim;
    }

    void Behavior() {
        while (true) {
            auto today = GetDay();
            auto dayInYear = today + _firstDayOfSim;

            // Rozhodovani, zdali se jedna o zimni, nebo letni čas
            bool summerTime = false;
            if (dayInYear > 90 && dayInYear < 304) {
                summerTime = true;
            } else {
                Wait(HoursToSec(1));
            }

            // sprcha
            //_consumers["Boiler"]->Start(60);

            // Uvari si caj
            auto consumer = _consumers["Kettle"];
            Enter(consumer->TurnOnPrivilegy);
            consumer->Start(Normal(60, 5));  // 2 min/l -> 0.5 l
            Wait(Normal(60, 5));

            // matka uvari snidani
            if (_id == 1) {
                consumer = _consumers["Cooker"];
                Enter(consumer->TurnOnPrivilegy);
                auto jobTime = Normal(MinsToSec(20), MinsToSec(2));
                consumer->Start(jobTime);  // cca 20 min vari
                Wait(jobTime);
            }

            Activate(GetTime(today, 7 - summerTime, 30, 0));

            if (today % 7 < 5) {  // vsedni den

                // Návrat z prace 16:30
                Activate(GetTime(today, 16 - summerTime, 30, 0));

                // otec vysaje
                if (_id == 2 && Random() <= 0.2) {  // 20%
                    consumer = _consumers["Vacuum"];
                    Enter(consumer->TurnOnPrivilegy);
                    auto jobTime = Normal(MinsToSec(10), MinsToSec(1));
                    consumer->Start(jobTime);  // cca 10 min vysava
                    Wait(jobTime);
                }

                // matka vypere
                if (_id == 1 && Random() <= 0.2) {  // 20%
                    consumer = _consumers["WashingMachine"];
                    Enter(consumer->TurnOnPrivilegy);
                    consumer->Start(HoursToSec(2));
                }

                Wait(Uniform(MinsToSec(10), MinsToSec(60)));

                // matka zapne mycku
                if (_id == 1 && Random() <= 0.5) {  // 50%
                    consumer = _consumers["Dishwasher"];
                    Enter(consumer->TurnOnPrivilegy);
                    consumer->Start(HoursToSec(2));
                }

                Wait(Uniform(MinsToSec(10), MinsToSec(60)));

                // Zapne si pocitac
                if (_id == 1) {
                    consumer = _consumers["Notebook3"];
                } else {
                    consumer = _consumers["Notebook4"];
                }
                Enter(consumer->TurnOnPrivilegy);
                auto timeOnComputer = Uniform(HoursToSec(0.5), HoursToSec(3));
                consumer->Start(timeOnComputer);  // Rodic stravi u pocitace 0.5 - 3 hodiny
                Wait(timeOnComputer);
            } else {  // vikend

                // otec vysaje
                if (_id == 2 && Random() <= 0.5) {  // 50%
                    consumer = _consumers["Vacuum"];
                    Enter(consumer->TurnOnPrivilegy);
                    auto jobTime = Normal(MinsToSec(10), MinsToSec(1));
                    consumer->Start(jobTime);  // cca 10 min vysava
                    Wait(jobTime);
                }

                // matka Vypere
                if (_id == 1 && Random() <= 0.5) {  // 50%
                    consumer = _consumers["WashingMachine"];
                    Enter(consumer->TurnOnPrivilegy);
                    consumer->Start(HoursToSec(2));
                }

                Wait(Uniform(MinsToSec(10), MinsToSec(60)));

                // Zapne si pocitac
                if (_id == 1) {
                    consumer = _consumers["Notebook3"];
                } else {
                    consumer = _consumers["Notebook4"];
                }
                Enter(consumer->TurnOnPrivilegy);
                auto timeOnComputer = Uniform(HoursToSec(0.5), HoursToSec(1.5));
                consumer->Start(timeOnComputer);  // Rodič stráví u počítače 0.5 - 1.5 hodiny
                Wait(timeOnComputer);

                // Otec uvari obed
                if (_id == 2) {
                    consumer = _consumers["Cooker"];
                    Enter(consumer->TurnOnPrivilegy);
                    auto jobTime = Normal(HoursToSec(1), MinsToSec(10));
                    consumer->Start(jobTime);  // cca 1 hodinu vari
                    Wait(jobTime);
                }

                Wait(Normal(MinsToSec(20), MinsToSec(4)));  // obedva

                // matka zapne mycku
                if (_id == 1) {
                    consumer = _consumers["Dishwasher"];
                    Enter(consumer->TurnOnPrivilegy);
                    consumer->Start(HoursToSec(2));
                }

                Wait(Uniform(MinsToSec(10), MinsToSec(40)));

                // matka upece buchtu
                if (_id == 1 && Random() < 0.5) {
                    consumer = _consumers["Oven"];
                    Enter(consumer->TurnOnPrivilegy);
                    auto jobTime = Normal(HoursToSec(1.5), MinsToSec(15));
                    consumer->Start(jobTime);  // cca 1.5 hodiny pece
                    Wait(jobTime);
                }

                Wait(Uniform(MinsToSec(30), MinsToSec(60)));

                // Uvari si caj
                consumer = _consumers["Kettle"];
                Enter(consumer->TurnOnPrivilegy);
                consumer->Start(60);  // 2 min/l -> 0.5 l
                Wait(Normal(20, 2));

                // Zapne si pocitac
                if (_id == 1) {
                    consumer = _consumers["Notebook3"];
                } else {
                    consumer = _consumers["Notebook4"];
                }
                Enter(consumer->TurnOnPrivilegy);
                timeOnComputer = Uniform(HoursToSec(1.5), HoursToSec(3));
                consumer->Start(timeOnComputer);  // Rodic stravi u pocitace 1.5 - 3 hodiny
                Wait(timeOnComputer);
            }

            auto evening = GetTime(today, 20-summerTime,0,0);
            if(Time < evening){
                Activate(evening);
            }
            
            // Rodic se osprchuje
            Wait(HoursToSec(Uniform(0.5, 2)));
            Seize(_boiler->ShowerFacility);
            double showerTime = MinsToSec(Normal(10, 2.5));
            _boiler->RemoveWater(showerTime * 0.14, 40);  // 0.14 l/s => namereno
            Wait(showerTime);
            Release(_boiler->ShowerFacility);

            Activate(GetTime(today + 1, 6 - 1, 0, 0));
        }
    }
};