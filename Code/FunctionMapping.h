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
 *      File:           FunctionMapping.h
 *      Path:           BC/Web/Http/FunctionMapping.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * Used to represent a function mapping for the custom template system
 * implemented for the web server.
 * *****************************************************************************
 */
#ifndef FUNCTIONMAPPING_H
#define	FUNCTIONMAPPING_H

#include <functional>

#include "IModule.h"
using BC::Web::Modules::IModule;

#include "HttpRequest.h"
using BC::Web::Http::HttpRequest;

#include "HttpResponse.h"
using BC::Web::Http::HttpResponse;

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

