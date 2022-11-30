#pragma once
#include "simlib.h"
#include "EnergyStore.h"
class EnergyConsumer: public Process{
    private:
        double _consumption;
        double _duration;
        double _timeStart;
        int _step = 60;  
        double _lastAdd;
        std::shared_ptr<EnergyStore> _energyStore;
        Facility consumerFacility;
    public:
        Facility UserFacility; 
        EnergyConsumer(const std::shared_ptr<EnergyStore> &energyStore, double consumption){
            _energyStore=energyStore;
            _consumption=consumption;
        }
        void Start(double duration){
            _duration = duration;
            Activate();
        }
        void Stop(){
            _energyStore->AddEnergy((_step -(Time-_lastAdd))*_consumption);
            Passivate();
        }
        void Behavior(){
            while(true){
                Seize(consumerFacility);
                _timeStart = Time;
                auto timeEnd = _timeStart + _duration;
                while(true){
                    if(Time+_step>timeEnd){
                        _step = timeEnd-Time;
                        _lastAdd = Time;
                        _energyStore->RemoveEnergy(_step*_consumption);
                        Wait(_step);
                        break;                    
                    }else{
                        _lastAdd = Time;
                        _energyStore->RemoveEnergy(_step*_consumption);
                        Wait(_step);
                    }                
                }
                Release(consumerFacility);
                Passivate(); 
            }
        }
};