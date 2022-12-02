#pragma once
#include <iostream>
#include <memory>
class EnergyStore {
   private:
    double _capacity = 0, buffer = 0, _overflowed = 0, _undeflowed = 0, _value = 0, _efficiency = 0.8;

   public:
    double GetValue() {
        return _value;
    }
    double GetUnderflowed() {
        return _undeflowed;
    }
    double GetOverflowed() {
        return _overflowed;
    }
    EnergyStore(double capacity) {
        _capacity = capacity;
    }
    void AddEnergy(double energy) {

        auto lastenergy= buffer;
        buffer = energy;
        //Započítáváme okamžitý příkon s příhlédnutím na předcházející
        lastenergy /=_efficiency;

        // Dobíjení energie
        if ((_value + lastenergy) > _capacity) {
            _overflowed += (lastenergy - (_capacity - _value)) *_efficiency;
            _value = _capacity;
        } else {
            _value += lastenergy;
        }
    }
    void RemoveEnergy(double energy) {

        if(buffer > energy){
            buffer-=energy;
            energy=0;
        }else{
            energy-=buffer;
            buffer=0;            
        }

        // Baterie
        if ((_value - energy) < 0) {
            _undeflowed += energy - _value;
            _value = 0;
        } else {
            _value -= energy;
        }
    }
};