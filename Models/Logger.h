#pragma once
#include <fstream>
#include <iostream>

#include "simlib.h"

template <class C, typename T, typename T2>
class Logger : public Event {
   private:
    double _step;
    double _startTime;
    double _stopTime;
    C* _object;
    std::ofstream file;

    T (C::*_getter)
    ();

   public:
    Logger(double step, C* object, T (C::*getter)(), double startTime, double stopTime, const std::string& path) {
        _step = step;
        _startTime = startTime;
        _stopTime = stopTime;
        file.open(path);
        _getter = getter;
        _object = object;
    }
    void Activate() {
        Event::Activate(_startTime);
    }

    ~Logger() {
        file.close();
    }
    void Behavior() {
        file << (int64_t)Time << ";" << (T2)(_object->*_getter)() << "\n";
        if (Time < _stopTime) {
            Event::Activate(Time + _step);
        }
    }
};