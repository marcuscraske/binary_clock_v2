/*                       ____               ____________
 *                      |    |             |            |
 *                      |    |             |    ________|
 *                      |    |             |   |
 *                      |    |             |   |    
 *                      |    |             |   |    ____
 *                      |    |             |   |   |    |
 *                      |    |_______      |   |___|    |
 *                      |            |  _  |            |
 *                      |____________| |_| |____________|
 *                        
 *      Author(s):      limpygnome (Marcus Craske)              limpygnome@gmail.com
 * 
 *      License:        Creative Commons Attribution-ShareAlike 3.0 Unported
 *                      http://creativecommons.org/licenses/by-sa/3.0/
 * 
 *      File:           RelayCondition.h
 *      Path:           BC/Services/RelayCondition.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * A structure to hold the condition function and parameters of a relay
 * condition.
 * *****************************************************************************
 */
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
            int                     uniqueID;               // The unique ID for this session of the program, for e.g. removing the condition; this is not saved. 
            string                  conditionName;          // The key/name of the function used by the mapper in RelayBoard.h.
            ConditionFunction       func;                   // The function to invoke to determine if the condition is true/false.
            string                  param1, param2;         // The function parameters.
            // Member Functions - Constructors -------------------------------->
            RelayCondition(){}
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

