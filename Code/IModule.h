
#ifndef IMODULE_H
#define	IMODULE_H

#include "HttpHandler.h"

// Forward declarations
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

