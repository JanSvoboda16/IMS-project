#pragma once
#include <iostream>
#include <memory>
class EnergyStore {
   private:
    double _capacity = 0, buffer = 0, _overflowed = 0, _undeflowed = 0, _value = 0, _efficiency = 0.8;

   public:
    double GetValue() { return _value; }
    double GetUnderflowed() { return _undeflowed; }
    double GetOverflowed() { return _overflowed; }
    EnergyStore(double capacity) {
        _capacity = capacity;
    }
    void AddEnergy(double energy) {
        
        // Energie je po dobu jednoho vzorku nejdříve uložena 
        // do pomocné proměnné, která simuluje okamžitý doběr
        auto lastenergy = buffer;
        buffer = energy;
        
        // Zbylá energie z minulého vzorku je se ztrátou uloží do baterie
        lastenergy *= _efficiency;

        // 
        if ((_value + lastenergy) > _capacity) {
            // Množství, které se do baterie nevleze odešle do sítě (musíme odebrat ztráty)
            _overflowed += (lastenergy - (_capacity - _value)) / _efficiency;
            _value = _capacity;
        } else {
            _value += lastenergy;
        }
    }
    void RemoveEnergy(double energy) {
        
        // Získání dostupné energie přímo z panelů ->
        // bez ztrát na baterii
        if (buffer > energy) {
            buffer -= energy;
            energy = 0;
        } else {
            energy -= buffer;
            buffer = 0;
        }

        // Získání energie z baterie
        if (_value < energy) {
            _undeflowed += energy - _value;
            _value = 0;
        } else {
            _value -= energy;
        }
    }
};