/**
 * @brief Implements the PowerFactory methods.
 *
 * @author Dariusz Scharsig
 *
 * @date 23.09.2022
 */
#include "powerfactory.h"
#include "powerinfo.h"

IPower * PowerFactory::getPowerInterface()
{
    return new PowerInfo;
}
