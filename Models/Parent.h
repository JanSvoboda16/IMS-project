#pragma once
#include "simlib.h"
#include "EnergyConsumer.h"
#include <map>
#include "TimeTransfer.h"
#include "Boiler.h"

class Parent : public Process{
private:
    std::map<std::string, EnergyConsumer*> _consumers;
    Boiler* _boiler;
    int _id;
    int _firstDayOfSim;

public:
    Parent(std::map<std::string, EnergyConsumer*> consumers, int id, Boiler* boiler, int firstDayOfSim){
        _consumers = consumers;
        _boiler = boiler;
        _id = id;
        _firstDayOfSim = firstDayOfSim;
    }

    void Behavior(){
        while(true){
            if(_id == 1){ std::cout<< "Matka vstava\n";}
            else{ std::cout<< "Otec vstava\n";}

            auto today = getDay();
            auto dayInYear = today + _firstDayOfSim;
            bool summerTime = false;

           if (dayInYear > 90 && dayInYear < 304) {
                summerTime = true;                
            }else{
                Wait(HoursToSec(1));
            }

            // sprcha
            //_consumers["Boiler"]->Start(60);

            // Uvaří si čaj
            auto consumer = _consumers["Kettle"];
            Enter(consumer->TurnOnPrivilegy);
            consumer->Start(60); // 2 min/l -> 0.5 l            
            Wait(Normal(20,2));

            // matka uvaří snídani
            if(_id == 1){
                consumer = _consumers["Cooker"];
                Enter(consumer->TurnOnPrivilegy);
                auto jobTime = Normal(MinsToSec(20),MinsToSec(2));
                consumer->Start(jobTime);    // cca 20 min vaří snídani       
                Wait(jobTime);
            }

            Activate(GetTime(today,7-summerTime,30,0));

            if(today % 7 < 5){  // všední den
                
                // Návrat z Práce 16:30
                Activate(GetTime(today,16-summerTime,30,0));


                // otec vysaje
                if(_id == 2 && Random() <= 0.2){    // 20%
                    consumer = _consumers["Vacuum"];
                    Enter(consumer->TurnOnPrivilegy);
                    auto jobTime = Normal(MinsToSec(10),MinsToSec(1));
                    consumer->Start(jobTime);    // cca 10 min vysává      
                    Wait(jobTime);
                }

                // matka vypere
                if(_id == 1 && Random() <= 0.2){    // 20%
                    consumer = _consumers["WashingMachine"];
                    Enter(consumer->TurnOnPrivilegy);
                    consumer->Start(HoursToSec(2));    
                }

                Wait(Uniform(MinsToSec(10), MinsToSec(60)));

                // matka zapne myčku
                if(_id == 1 && Random() <= 0.5){    // 50%
                    consumer = _consumers["Dishwasher"];
                    Enter(consumer->TurnOnPrivilegy);
                    consumer->Start(HoursToSec(2));    
                }

                Wait(Uniform(MinsToSec(10), MinsToSec(60)));

                // Zapne si počítač
                if(_id == 1){
                    consumer = _consumers["Notebook3"]; 
                }
                else{
                    consumer = _consumers["Notebook4"];
                }   
                Enter(consumer->TurnOnPrivilegy);
                auto timeOnComputer = Uniform(HoursToSec(0.5), HoursToSec(3));
                consumer->Start(timeOnComputer); // Rodič stráví u počítače 0.5 - 3 hodiny
                Wait(timeOnComputer);



            }
            else{   // víkend

                // otec Vysaje
                if(_id == 2 && Random() <= 0.5){    // 50%
                    consumer = _consumers["Vacuum"];
                    Enter(consumer->TurnOnPrivilegy);
                    auto jobTime = Normal(MinsToSec(10),MinsToSec(1));
                    consumer->Start(jobTime);    // cca 10 min vysává      
                    Wait(jobTime);
                }

                // matka Vypere
                if(_id == 1 && Random() <= 0.5){    // 50%
                    consumer = _consumers["WashingMachine"];
                    Enter(consumer->TurnOnPrivilegy);
                    consumer->Start(HoursToSec(2));    
                }

                Wait(Uniform(MinsToSec(10), MinsToSec(60)));

                // Zapne si počítač
                if(_id == 1){
                    consumer = _consumers["Notebook3"]; 
                }
                else{
                    consumer = _consumers["Notebook4"];
                }
                Enter(consumer->TurnOnPrivilegy);
                auto timeOnComputer = Uniform(HoursToSec(0.5), HoursToSec(1.5));
                consumer->Start(timeOnComputer); // Rodič stráví u počítače 0.5 - 1.5 hodiny
                Wait(timeOnComputer);

                // Otec uvaří oběd
                if(_id == 2){
                    consumer = _consumers["Cooker"];
                    Enter(consumer->TurnOnPrivilegy);
                    auto jobTime = Normal(HoursToSec(1),MinsToSec(10));
                    consumer->Start(jobTime);    // cca 1 hodinu vaří     
                    Wait(jobTime);
                }

                Wait(Normal(MinsToSec(20), MinsToSec(4)));      // obědvá

                // matka zapne myčku
                if(_id == 1){
                    consumer = _consumers["Dishwasher"];
                    Enter(consumer->TurnOnPrivilegy);
                    consumer->Start(HoursToSec(2));
                }

                Wait(Uniform(MinsToSec(10), MinsToSec(40)));

                // matka upeče buchtu
                if(_id == 1 && Random() < 0.5){
                    consumer = _consumers["Oven"];
                    Enter(consumer->TurnOnPrivilegy);
                    auto jobTime = Normal(HoursToSec(1.5),MinsToSec(15));
                    consumer->Start(jobTime);    // cca 1.5 hodiny peče     
                    Wait(jobTime);
                }

                Wait(Uniform(MinsToSec(30), MinsToSec(60)));

                // Uvaří si čaj
                consumer = _consumers["Kettle"];
                Enter(consumer->TurnOnPrivilegy);
                consumer->Start(60); // 2 min/l -> 0.5 l            
                Wait(Normal(20,2));

                // Zapne si počítač
                if(_id == 1){
                    consumer = _consumers["Notebook3"]; 
                }
                else{
                    consumer = _consumers["Notebook4"];
                }                
                Enter(consumer->TurnOnPrivilegy);
                timeOnComputer = Uniform(HoursToSec(1.5), HoursToSec(3));
                consumer->Start(timeOnComputer); // Rodič stráví u počítače 1.5 - 3 hodiny
                Wait(timeOnComputer);
            }


            Wait(HoursToSec(Uniform(0.5,2)));
            Seize(_boiler->ShowerFacility);
            double showerTime = MinsToSec(Normal(10,2.5));
            _boiler->RemoveWater(showerTime * 0.14, 40); // 0.14 l/s => naměřeno
            Wait(showerTime);
            Release(_boiler->ShowerFacility);

            Activate(GetTime(today+1,6-1,0,0));

        }
    }

};