#include "simlib.h"
#include <map>
#include "EnergyConsumer.h"
#include "EnergyGenerator.h"
#include "Children.h"
#include "Parent.h"

#define SIM_TIME 31536000
#define BATTERY_CAPACITY 20000*3600
#define PANEL_COUNT 4

int main(int argc, char const *argv[])
{
    Init(0, SIM_TIME);
    std::map<std::string, EnergyConsumer*> consumers;
    std::shared_ptr<EnergyStore> battery = std::make_shared<EnergyStore>(BATTERY_CAPACITY);
    battery->AddEnergy(BATTERY_CAPACITY);

    EnergyGenerator* panels = new EnergyGenerator("data.csv", PANEL_COUNT, battery);

    panels->Activate();    

    /* Definice spotřebičů */
    
    (consumers["Fridge"] = new EnergyConsumer(battery,17,false))->Start(SIM_TIME); // Nstartování lednice
    (consumers["Others"] = new EnergyConsumer(battery,10,false))->Start(SIM_TIME);
    consumers["Kettle"] = new EnergyConsumer(battery,2000);
    consumers["Microwave"] = new EnergyConsumer(battery,1200);
    consumers["Vacuum"] = new EnergyConsumer(battery,700);
    consumers["WashingMachine"] = new EnergyConsumer(battery,230);
    consumers["Dishwasher"] = new EnergyConsumer(battery,370);
    consumers["Cooker"] = new EnergyConsumer(battery,2000);        // 2 plotynky
    consumers["Oven"] = new EnergyConsumer(battery,900);
    consumers["Bulb"] = new EnergyConsumer(battery,8.5);          // 1 zarovka
    consumers["Notebook1"] = new EnergyConsumer(battery,45);
    consumers["Notebook2"] = new EnergyConsumer(battery,45);
    consumers["Notebook3"] = new EnergyConsumer(battery,45);
    consumers["Notebook4"] = new EnergyConsumer(battery,45);
    consumers["Boiler"] = new EnergyConsumer(battery,2200);
    (new Children(consumers))->Activate(24300); 
    (new Parent(consumers))->Activate(21600);   

    

    Run();

    std::cout << battery->GetOverflowed() << "\n";
    std::cout << battery->GetUnderflowed() << "\n";


    return 0;
}