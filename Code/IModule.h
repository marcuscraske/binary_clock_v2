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
 *      File:           IModule.h
 *      Path:           BC/Web/Modules/IModule.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * The base-class for modules; modules are used as plugins on the web-server for
 * back-end functions (at present). This is similar, in idea, to modules with
 * Apache (web server - Apache Foundation).
 * *****************************************************************************
 */
#ifndef IMODULE_H
#define	IMODULE_H

// Forward declarations
#include "HttpHandler.h"
namespace BC
{
    namespace Web
    {
        namespace Http
        {
            class HttpHandler;
        }
    }
}
using BC::Web::Http::HttpHandler;

namespace BC
{
    namespace Web
    {
        namespace Modules
        {
            class IModule
            {
            public:
                // Invoked when the HTTP handler starts; this should be used to
                // register functions with the HTTP handler, load configuration,
                // etc. Should return true if the module has successfully been
                // registered; if not, the module will be destroyed.
                virtual bool moduleRegister(HttpHandler * const handler)
                {
                    return false;
                }
            };
        }
    }
}

#endif	/* IMODULEHANDLER_H */

