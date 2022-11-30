#pragma once
#include "simlib.h"
#include "EnergyConsumer.h"
#include <map>

class Children : public Process{
private:
    std::map<std::string, EnergyConsumer*> _consumers;

    int getDay(){
        return (int)Time/24/60/60;
    }
    int GetTime(int day, int hour, int minute, int second ){
        return ((day*24+hour)*60 + minute)*60 + second;
    }
public:
    Children(std::map<std::string, EnergyConsumer*> consumers){
        _consumers = consumers;
    }
    void Behavior(){

        while(true){
            std::cout<< "Dite vstava\n";
            auto today = getDay();
            //_consumers["Boiler"]->Start(60);
            
            // Uvaří si čaj
            auto consumer = _consumers["Kettle"];
            Enter(consumer->TurnOnPrivilegy);
            consumer->Start(60); // 2 min/l -> 0.5 l            
            Wait(20);

            consumer =_consumers["Microwave"];
            Enter(consumer->TurnOnPrivilegy);
            consumer->Start(90); // 1.5 min -> snídaně

            if(today%7<5){ // Všední den

                // Návrat ze školy 15:00
                Activate(GetTime(today,15,0,0));
                
                // Po příchodu za 0 - 3 h
                Wait(Uniform(0,3*60*60)); 

                // Zapne si počítač
                consumer = _consumers["Notebook1"]; 
                if(consumer->TurnOnPrivilegy.Empty()){
                    consumer = _consumers["Notebook2"];
                }    

                Enter(consumer->TurnOnPrivilegy);
                auto timeOnComputer = Uniform(30*60, 4*60*60);
                consumer->Start(timeOnComputer); // Dítě stráví u počítače 0.5 - 4 hodiny
                Wait(timeOnComputer);



                
            }else{

            }
             

            

            //_consumers["Boiler"]->Start(70); 
            Activate(GetTime(today+1,6,0,0));
        }    
    }
};