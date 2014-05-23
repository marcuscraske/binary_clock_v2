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
 *      File:           CountryLookup.h
 *      Path:           BC/Services/CountryLookup.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * The country look-up service. Used to download five IP block allocation lists,
 * which are used to determine the location of IPs for the ip table in the
 * database (used by the guest-book etc).
 * *****************************************************************************
 */
#ifndef COUNTRYLOOKUP_H
#define	COUNTRYLOOKUP_H

#include "Definitions.h"

#include <string>
using std::string;

#include <pqxx/pqxx>
using namespace pqxx;

#include <vector>
using std::vector;

#include "IService.h"
using BC::Services::IService;

#include "ServiceController.h"
using BC::ServiceController;

#include "IPAddress.h"
using BC::Services::IPAddress;

#include "Utils.h"
using BC::Utils;

#include "ClientSocket.h"
using BC::Web::Core::ClientSocket;

#include "Configurator.h"
using BC::Services::Configurator;

#include "Pages.h" // Access to defines only.

namespace BC
{
    namespace Services
    {
        class CountryLookup : public IService
        {
        private:
            // Fields
            string qs;
            map<string, string> registrantLists; // File URL,HTTP url
            long long   listsLastUpdated,
                        listsUpdatePeriod;
        public:
            CountryLookup(ServiceController *const controller) : IService(controller) {}
            void prerun();
            void run();
        private:
            bool findIPCountry(string ip, string &country);
            void updateLists();
            connection* createDbConnection();
        public:
            string getTitle() { return SERVICETITLE_COUNTRYLOOKUP; }
        };
    }
}

#endif	/* COUNTRYLOOKUP_H */

