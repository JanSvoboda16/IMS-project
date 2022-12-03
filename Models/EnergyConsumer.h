#pragma once
#include "simlib.h"
#include "EnergyStore.h"
#include <iostream>
class EnergyConsumer: public Process{
    private:
        double _consumption;
        double _duration;
        double _timeStart;
        int _step = 60;  
        double _lastAdd;
        bool _usePrivilegy;
        std::shared_ptr<EnergyStore> _energyStore;

    public:
        // Zamezuje spuštění před dokončením činnosti
        Store TurnOnPrivilegy;
        EnergyConsumer(const std::shared_ptr<EnergyStore> &energyStore, double consumption, bool usePrivilegy =true): TurnOnPrivilegy(1) {
            _usePrivilegy = usePrivilegy;
            _energyStore=energyStore;
            _consumption=consumption;
        }
        virtual void Start(double duration, double defer=0){
            _duration = duration;
            Activate(Time+defer);                    
        }
        void Behavior(){
            while(true){
                _timeStart = Time;
                auto timeEnd = _timeStart + _duration;
                while(true){
                    if(Time+_step>timeEnd){
                        auto _step2 = timeEnd-Time;
                        _lastAdd = Time;
                        _energyStore->RemoveEnergy(_step2*_consumption);
                        Wait(_step2);
                        break;                    
                    }else{
                        _lastAdd = Time;
                        _energyStore->RemoveEnergy(_step*_consumption);
                        Wait(_step);
                    }                
                }
                if(_usePrivilegy){
                    Leave(TurnOnPrivilegy);
                }
                Passivate(); 
            }
        }
};