
#ifndef RELAYCONDITION_H
#define	RELAYCONDITION_H

#include <string>
using std::string;

#include "ServiceController.h"
using BC::ServiceController;

#include <functional>

namespace BC
{
    namespace Services
    {
        struct RelayCondition
        {
            // Type-Definitions ----------------------------------------------->
            typedef std::function<bool(ServiceController *controller, string param1, string param2)> ConditionFunction;
            // Fields ----------------------------------------------------->
            string                  conditionName;          // The key/name of the function used by the mapper in RelayBoard.h.
            ConditionFunction       func;                   // The function to invoke to determine if the condition is true/false.
            string                  param1, param2;         // The function parameters.
            // Member Functions - Constructors ---------------------------->
            RelayCondition(string conditionName, ConditionFunction func, string param1, string param2)
            : conditionName(conditionName), func(func), param1(param1), param2(param2)
            {}
            // Member Functions ------------------------------------------->
            ////////////////////////////////////////////////////////////////
            // Tests and returns the condition by invoking the underlying
            // function with the parameters.
            ////////////////////////////////////////////////////////////////
            inline bool test(ServiceController *controller)
            {
                return func(controller, param1, param2);
            }
        };
    }
}

#endif	/* RELAYCONDITION_H */

