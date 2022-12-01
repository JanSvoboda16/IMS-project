#pragma once
#include "simlib.h"
#include "EnergyConsumer.h"
#include <map>
#include "TimeTransfer.h"
#include "Boiler.h"

class Children : public Process{
private:
    std::map<std::string, EnergyConsumer*> _consumers;
    int _id;
    Boiler *_boiler;

public:
    Children(std::map<std::string, EnergyConsumer*> consumers, int id, Boiler* boiler){
        _consumers = consumers;
        _id = id;
        _boiler = boiler;
    }
    void Behavior(){

        while(true){
            std::cout<< "Dite " << _id << " vstava\n";
            auto today = getDay();
            //_consumers["Boiler"]->Start(60);
            
            // Uvaří si čaj
            auto consumer = _consumers["Kettle"];
            Enter(consumer->TurnOnPrivilegy);
            consumer->Start(60); // 2 min/l -> 0.5 l            
            Wait(20);

            // ohřeje si snídani
            consumer =_consumers["Microwave"];
            Enter(consumer->TurnOnPrivilegy);
            consumer->Start(90); // 1.5 min -> snídaně

            if(today%7<5){ // Všední den

                // Návrat ze školy 15:00
                Activate(GetTime(today,15,0,0));
                
                // Po příchodu za 0 - 3 h
                Wait(Uniform(0,HoursToSec(3))); 

                // Vysaje si pokoj
                if(_id == 1 && Random() <= 0.2){    // 20% - pouze dítě 1
                    consumer = _consumers["Vacuum"];
                    Enter(consumer->TurnOnPrivilegy);
                    auto jobTime = Normal(MinsToSec(5),MinsToSec(1));
                    consumer->Start(jobTime);    // cca 5 min vysává      
                    Wait(jobTime);
                }

                // Zapne si počítač
                if(_id == 1){
                    consumer = _consumers["Notebook1"]; 
                }
                else{
                    consumer = _consumers["Notebook2"];
                }
                Enter(consumer->TurnOnPrivilegy);
                auto timeOnComputer = Uniform(MinsToSec(30), HoursToSec(4));
                consumer->Start(timeOnComputer); // Dítě stráví u počítače 0.5 - 4 hodiny
                Wait(timeOnComputer);
                
            }else{

                // Po ránu za 0 - 2 h
                Wait(Uniform(0,HoursToSec(2))); 

                // Vysaje si pokoj
                if(_id == 1 && Random() <= 0.5){    // 50% - pouze Dítě 1
                    consumer = _consumers["Vacuum"];
                    Enter(consumer->TurnOnPrivilegy);
                    auto jobTime = Normal(MinsToSec(5),MinsToSec(1));
                    consumer->Start(jobTime);    // cca 5 min vysává      
                    Wait(jobTime);
                }

                // Zapne si počítač
                if(_id == 1){
                    consumer = _consumers["Notebook1"]; 
                }
                else{
                    consumer = _consumers["Notebook2"];
                }    
                Enter(consumer->TurnOnPrivilegy);
                auto timeOnComputer = Uniform(HoursToSec(2), HoursToSec(8));
                consumer->Start(timeOnComputer); // Dítě stráví u počítače 2 - 8 hodin
                Wait(timeOnComputer);
            }
             
            
            // sprcha
            //_consumers["Boiler"]->Start(70); 
            

            Wait(HoursToSec(Uniform(0.5,2)));
            Seize(_boiler->ShowerFacility);
            double showerTime = MinsToSec(Normal(10,2.5));
            _boiler->RemoveWater(showerTime * 0.14, 40); // 0.14 l/s => naměřeno
            Release(_boiler->ShowerFacility);

            Activate(GetTime(today+1,6,45,0));
        }    
    }
};