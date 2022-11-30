#include "simlib.h"
#include <map>
#include "EnergyConsumer.h"
#include "EnergyGenerator.h"

int main(int argc, char const *argv[])
{
    Init(0, 31536000);
    std::map<std::string, EnergyConsumer*> consumers;
    std::shared_ptr<EnergyStore> battery = std::make_shared<EnergyStore>(72000000);
    battery->AddEnergy(72000000);

    EnergyGenerator* panels = new EnergyGenerator("data.csv", 18, battery);

    panels->Activate();    

    /* Definice spotřebičů */
    (consumers["Fridge"] = new EnergyConsumer(battery,17))->Start(100000000); // Nstartování lednice

    

    Run();

    std::cout << battery->GetOverflowed() << "\n";
    std::cout << battery->GetUnderflowed() << "\n";


    return 0;
}