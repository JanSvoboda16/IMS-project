#include <map>

#include "Children.h"
#include "EnergyConsumer.h"
#include "EnergyGenerator.h"
#include "Lights.h"
#include "Logger.h"
#include "Parent.h"
#include "simlib.h"

#define SIM_LENGTH 31536000
#define FIRST_DAY_OF_SIM 31

int main(int argc, char const* argv[]) {
    
    if (argc < 4) {
        return 1;
    }

    /**
     * PARAMETRY PROGRAMU
     * 1) počet panelů
     * 2) velikost baterie (kWh)
     * 3) krok logování hodnot baterie(s)
     * 4) začátek logování hodnot baterie od startu simulace(s) [volitelné]
     * 5) doba logování hodnot baterie (s) [volitelné]
    */
    int panelCount = stoi(argv[1]);
    double batteryCapacity = stod(argv[2]) * 3600000;  // kwh -> J
    double logStep = stod(argv[3]);
    double logFrom = 0 ;
    if (argc >4){
       logFrom = stod(argv[4]);
    }
    double duration = SIM_LENGTH - logFrom;
    if (argc >5){
        duration = stod(argv[5]);
    }

    std::cout << "SIMULACE \n";
    std::cout << "Počet panelů: " << panelCount << "\n";
    std::cout << "Kapacita baterie: " << batteryCapacity/3600000 << " kWh\n";
    std::cout << "Informace o hodnotách baterie pro vybraný interval budou uloženy do souborů battery_<typ>.csv\n";
    std::cout << "STATISTICKÉ VÝSLEDKY (ROK): \n";

    Init(0, SIM_LENGTH);
    std::map<std::string, EnergyConsumer*> consumers;
    std::shared_ptr<EnergyStore> battery = std::make_shared<EnergyStore>(batteryCapacity);
    battery->AddEnergy(batteryCapacity / 2); // Inicializace baterie na polovinu kapacity

    EnergyGenerator* panels = new EnergyGenerator("data.csv", panelCount, battery);

    panels->Activate();

    /* Definice spotřebičů */
    (consumers["Fridge"] = new EnergyConsumer(battery, 17, false))->Start(SIM_LENGTH);  // Spuštění lednice
    (consumers["Others"] = new EnergyConsumer(battery, 10, false))->Start(SIM_LENGTH);
    consumers["Kettle"] = new EnergyConsumer(battery, 2000);
    consumers["Microwave"] = new EnergyConsumer(battery, 1200);
    consumers["Vacuum"] = new EnergyConsumer(battery, 700);
    consumers["WashingMachine"] = new EnergyConsumer(battery, 230);
    consumers["Dishwasher"] = new EnergyConsumer(battery, 370);
    consumers["Cooker"] = new EnergyConsumer(battery, 2000);  // 2 plotynky
    consumers["Oven"] = new EnergyConsumer(battery, 900);
    consumers["BulbSmall"] = new EnergyConsumer(battery, 4 * 8.5);  // 4 zarovky
    consumers["BulbBig"] = new EnergyConsumer(battery, 7 * 8.5);    // 7 zarovek
    consumers["Notebook1"] = new EnergyConsumer(battery, 45);
    consumers["Notebook2"] = new EnergyConsumer(battery, 45);
    consumers["Notebook3"] = new EnergyConsumer(battery, 45);
    consumers["Notebook4"] = new EnergyConsumer(battery, 45);

    Boiler* boiler = new Boiler(60, 15, 2000, battery);
    
    // Spuštění dětí a rodičů je naplánováno na ráno
    (new Children(consumers, 1, boiler, FIRST_DAY_OF_SIM))->Activate(24300);
    (new Children(consumers, 2, boiler, FIRST_DAY_OF_SIM))->Activate(24300);
    (new Parent(consumers, 1, boiler, FIRST_DAY_OF_SIM))->Activate(21600);   // Matka
    (new Parent(consumers, 2, boiler, FIRST_DAY_OF_SIM))->Activate(21600);  // Otec
    (new Lights(consumers))->Activate(24301);

    // Výpis statistik do souborů
    auto logger = new Logger<EnergyStore, double, int>(logStep, battery.get(), &EnergyStore::GetValue, logFrom, logFrom + duration, "battery_value.csv");
    logger->Activate();
    auto logger2 = new Logger<EnergyStore, double, int64_t>(logStep, battery.get(), &EnergyStore::GetOverflowed, logFrom, logFrom + duration, "battery_overflowed.csv");
    logger2->Activate();
    auto logger3=new Logger<EnergyStore, double, int64_t>(logStep, battery.get(), &EnergyStore::GetUnderflowed, logFrom, logFrom + duration, "battery_underflowed.csv");
    logger3->Activate();

    Run();
    delete logger;
    delete logger2;
    delete logger3;

    std::cout << "Celkové množství prodané energie: " << battery->GetOverflowed() / 3600000 << " kWh \n";
    std::cout << "Celkové množství dokoupené energie: " << battery->GetUnderflowed() / 3600000 << " kWh \n\n";

    return 0;
}