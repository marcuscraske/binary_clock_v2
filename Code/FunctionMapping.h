
#ifndef FUNCTIONMAPPING_H
#define	FUNCTIONMAPPING_H

#include <functional>

#include "IModule.h"

using BC::Web::Modules::IModule;

namespace BC
{
    namespace Web
    {
        namespace Http
        {
            struct FunctionMapping
            {
                // -- The type definition for our actual function mapping (func).
                // -- -- Returns string.
                typedef std::function<string(HttpHandler *const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower)> func;
                // -- The function of the mapping.
                func            function;
                // -- The instance of the module responsible for the mapping.
                IModule         *module;
                
                FunctionMapping(IModule *module, func function) : module(module), function(function)
                {}
            };
        }
    }
}

#endif	/* FUNCTIONMAPPING_H */

