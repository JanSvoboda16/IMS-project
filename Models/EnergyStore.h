#pragma once
#include <iostream>
#include <memory>
class EnergyStore {
   private:
    double _capacity = 0, _overflowed = 0, _undeflowed = 0, _value = 0;

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
        if ((_value + energy) > _capacity) {
            _overflowed += energy - (_capacity - _value);
            _value = _capacity;
        } else {
            _value += energy;
        }
    }
    void RemoveEnergy(double energy) {
        if ((_value - energy) < 0) {
            _undeflowed += energy - _value;
            _value = 0;
        } else {
            _value -= energy;
        }
    }
};