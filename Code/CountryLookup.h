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

// Defintiions: the IP block lists of the five IP registrants.
// -- Heavy uses:
// -- http://www.apnic.net/publications/media-library/documents/resource-guidelines/rir-statistics-exchange-format
#define COUNTRYLOOKUP_LIST_RIPENCC              "iplist_ripencc"
#define COUNTRYLOOKUP_LIST_RIPENCC_URL          "ftp.arin.net/pub/stats/ripencc/delegated-ripencc-latest"
#define COUNTRYLOOKUP_LIST_LACNIC               "iplist_lacnic"
#define COUNTRYLOOKUP_LIST_LACNIC_URL           "ftp.arin.net/pub/stats/lacnic/delegated-lacnic-latest"
#define COUNTRYLOOKUO_LIST_ARIN                 "iplist_arin"
#define COUNTRYLOOKUP_LIST_ARIN_URL             "ftp.arin.net/pub/stats/arin/delegated-arin-latest"
#define COUNTRYLOOKUP_LIST_APNIC                "iplist_apnic"
#define COUNTRYLOOKUP_LIST_APNIC_URL            "ftp.arin.net/pub/stats/apnic/delegated-apnic-latest"
#define COUNTRYLOOKUP_LIST_AFRNIC               "iplist_afrinic"
#define COUNTRYLOOKUP_LIST_AFRINIC_URL          "ftp.arin.net/pub/stats/afrinic/delegated-afrinic-latest"

#define COUNTRYLOOKUP_LIST_DEFAULT_UPDATE_INTERVAL      86400000LL // Default: 1 day

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

