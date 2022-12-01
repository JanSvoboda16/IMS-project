#pragma once
#include "EnergyConsumer.h"

class Boiler : public Process {
    double _tempToHeat;
    double _litresToHeat = 0;
    double _inputTemp;
    double _power;
    double _maxStep = 60;
    std::shared_ptr<EnergyStore> _battery;
    bool isActive = false;
public:

    Boiler(double tempToHeat, double inputTemp, double power, std::shared_ptr<EnergyStore> battery){
        _tempToHeat = tempToHeat;
        _inputTemp = inputTemp;
        _power = power;
        _battery = battery;
    }
    
    // Odebírá horkou vodu z bojleru a míchá ji se studenou tak, aby měla požadovanou teplotu
    void RemoveWater(double litres, double temperature){
        _litresToHeat += litres * (temperature-_inputTemp)/(_tempToHeat-_inputTemp);
        if(!isActive){
            Activate();
        }
    }

    void Behavior(){
        while(true){
            isActive = true;
            while (_litresToHeat >0)
            {
                double step = _maxStep;
                double energy = _power * _maxStep;
                _battery->RemoveEnergy(energy);
                double deltaT =_tempToHeat - _inputTemp;
                double heatedLitres = energy/(4200*(deltaT));    
                _litresToHeat -= heatedLitres;

                // Vrácení energie za přebytečnou vodu
                if (_litresToHeat < 0){
                    double overflowed = 0 - _litresToHeat;
                    _battery->AddEnergy(overflowed * 4200 * deltaT);
                    _litresToHeat = 0;
                }
                Wait(_maxStep);
            }
            isActive = false;
            Passivate();            
        }
    }
};