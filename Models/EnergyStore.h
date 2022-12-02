#pragma once
#include <iostream>
#include <memory>
class EnergyStore {
   private:
    double _capacity = 0, lastAddedTime=0, lastPower= 0, _overflowed = 0, _undeflowed = 0, _value = 0, _efficiency = 0.8;

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

        //Započítáváme okamžitý příkon s příhlédnutím na předcházející
        if(Time - lastAddedTime > 0.01){
            lastPower = energy/(Time-lastAddedTime);
            lastAddedTime = Time;
        }

        // Dobíjení energie
        if ((_value + energy) > _capacity) {
            _overflowed += energy - (_capacity - _value);
            _value = _capacity;
        } else {
            _value += energy;
        }
    }
    void RemoveEnergy(double energy, double step) {

        if(energy/step > lastPower){
            //Připočítáme ztráty k energii získané z baterie
            energy += (energy - (lastPower*step))*(1-_efficiency);
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