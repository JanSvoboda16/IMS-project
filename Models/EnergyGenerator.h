#pragma once
#include "simlib.h"
#include <vector>
#include <memory>
#include "EnergyStore.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <ctime>

using namespace std;


class EnergyGenerator: public Event{
    private:
        double time, power;
        shared_ptr<EnergyStore> _output;
        int _counter = 0;
        int _multiplicator;
        bool end = false;
        fstream fileS;        
        
        void readNext(){
            string line;
            if(!end && getline(fileS, line)){
                auto delimPos = line.find(';');
                time = stod(line.substr(0, delimPos));
                auto valueStr = line.substr(delimPos+1);
                power = stod(valueStr);    
            }else{
                end = true;
                power =0;
                time = time+1;
            }        
        }

        
    public:
        EnergyGenerator(const string &path, int multiplicator, shared_ptr<EnergyStore> output):fileS(path){
            _output = output;
            _multiplicator = multiplicator;
            readNext();
            Activate(time);            
        }

        void Behavior(){            
            readNext();
            _output->AddEnergy(power*(time-Time)*230.0*_multiplicator);
            Activate(time);
        }
};