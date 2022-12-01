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
    bool _matka;

public:
    Parent(std::map<std::string, EnergyConsumer*> consumers, bool matka, Boiler* boiler){
        _consumers = consumers;
        _matka = matka;
        _boiler = boiler;
    }

    void Behavior(){
        while(true){
            if(_matka){ std::cout<< "Matka vstava\n";}
            else{ std::cout<< "Otec vstava\n";}

            auto today = getDay();

            // sprcha
            //_consumers["Boiler"]->Start(60);

            // Uvaří si čaj
            auto consumer = _consumers["Kettle"];
            Enter(consumer->TurnOnPrivilegy);
            consumer->Start(60); // 2 min/l -> 0.5 l            
            Wait(Normal(20,2));

            // matka uvaří snídani
            if(_matka){
                consumer = _consumers["Cooker"];
                Enter(consumer->TurnOnPrivilegy);
                auto jobTime = Normal(MinsToSec(20),MinsToSec(2));
                consumer->Start(jobTime);    // cca 20 min vaří snídani       
                Wait(jobTime);
            }

            if(today % 7 < 5){  // všední den
                
                // Návrat z Práce 16:30
                Activate(GetTime(today,16,30,0));


                // otec vysaje
                if(!_matka && Random() <= 0.2){    // 20%
                    consumer = _consumers["Vacuum"];
                    Enter(consumer->TurnOnPrivilegy);
                    auto jobTime = Normal(MinsToSec(10),MinsToSec(1));
                    consumer->Start(jobTime);    // cca 10 min vysává      
                    Wait(jobTime);
                }

                // matka vypere
                if(_matka && Random() <= 0.2){    // 20%
                    consumer = _consumers["WashingMachine"];
                    Enter(consumer->TurnOnPrivilegy);
                    consumer->Start(HoursToSec(2));    
                }

                Wait(Uniform(MinsToSec(10), MinsToSec(60)));

                // matka zapne myčku
                if(_matka && Random() <= 0.5){    // 50%
                    consumer = _consumers["Dishwasher"];
                    Enter(consumer->TurnOnPrivilegy);
                    consumer->Start(HoursToSec(2));    
                }

                Wait(Uniform(MinsToSec(10), MinsToSec(60)));

                // Zapne si počítač
                if(_matka){
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

                // Vysaje
                if(!_matka && Random() <= 0.5){    // 50% - pouze Otec
                    consumer = _consumers["Vacuum"];
                    Enter(consumer->TurnOnPrivilegy);
                    auto jobTime = Normal(MinsToSec(10),MinsToSec(1));
                    consumer->Start(jobTime);    // cca 10 min vysává      
                    Wait(jobTime);
                }

                // Vypere
                if(_matka && Random() <= 0.5){    // 50% - pouze Matka
                    consumer = _consumers["WashingMachine"];
                    Enter(consumer->TurnOnPrivilegy);
                    consumer->Start(HoursToSec(2));    
                }

                Wait(Uniform(MinsToSec(10), MinsToSec(60)));

                // Zapne si počítač
                if(_matka){
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
                if(!_matka){
                    consumer = _consumers["Cooker"];
                    Enter(consumer->TurnOnPrivilegy);
                    auto jobTime = Normal(HoursToSec(1),MinsToSec(10));
                    consumer->Start(jobTime);    // cca 1 hodinu vaří     
                    Wait(jobTime);
                }

                Wait(Normal(MinsToSec(20), MinsToSec(4)));      // obědvá

                // matka zapne myčku
                if(_matka){
                    consumer = _consumers["Dishwasher"];
                    Enter(consumer->TurnOnPrivilegy);
                    consumer->Start(HoursToSec(2));
                }

                Wait(Uniform(MinsToSec(10), MinsToSec(40)));

                // matka upeče buchtu
                if(_matka && Random() < 0.5){
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
                if(_matka){
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

            Activate(GetTime(today+1,6,0,0));

        }
    }

};