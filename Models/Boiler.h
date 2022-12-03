#pragma once
#include "EnergyConsumer.h"
#include "TimeTransfer.h"

class Boiler : public Process {
    double _tempToHeat;
    double _litresToHeat = 0;
    double _inputTemp;
    double _power;
    double _maxStep = 60;
    std::shared_ptr<EnergyStore> _battery;
    bool isActive = false;
    bool _delayedStartup;

   public:
    Facility ShowerFacility;
    Boiler(double tempToHeat, double inputTemp, double power, std::shared_ptr<EnergyStore> battery, bool delayedStartup = true) {
        _tempToHeat = tempToHeat;
        _inputTemp = inputTemp;
        _power = power;
        _battery = battery;
        _delayedStartup = delayedStartup;
        if(_delayedStartup){
            Activate(HoursToSec(10));
        }
    }

    // Odebírá horkou vodu z bojleru a míchá ji se studenou tak, aby měla požadovanou teplotu
    void RemoveWater(double litres, double temperature) {
        _litresToHeat += litres * (temperature - _inputTemp) / (_tempToHeat - _inputTemp);
        if (!isActive && !_delayedStartup) {
            Activate();
        }
    }

    void Behavior() {
        while (true) {
            isActive = true;
            auto today = GetDay();
            while (_litresToHeat > 0) {
                double step = _maxStep;
                double energy = _power * _maxStep;

                double deltaT = _tempToHeat - _inputTemp;
                double heatedLitres = energy / (4200 * (deltaT)) * 0.85;
                _litresToHeat -= heatedLitres;

                // Vrácení energie za přebytečnou vodu
                if (_litresToHeat < 0) {
                    double overflowed = 0 - _litresToHeat;
                    energy -= overflowed * 4200 * deltaT / 0.85;
                    step = energy / _power;
                    _litresToHeat = 0;
                }

                _battery->RemoveEnergy(energy);
                Wait(step);
            }
            isActive = false;

            // Opožděné spuštění boileru
            if(_delayedStartup){
                Activate(GetTime(today+1,10,0,0));
            }else{
                Passivate();
            }
        }
    }
};