
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

#include <string>
#include <pqxx/pqxx>
#include <vector>

#include "Definitions.h"
#include "IService.h"
#include "ServiceController.h"
#include "IPAddress.h"
#include "Utils.h"
#include "ClientSocket.h"
#include "Configurator.h"
#include "Pages.h"

using std::string;
using std::vector;
using namespace pqxx;

using BC::Utils;
using BC::Services::IPAddress;
using BC::Web::Core::ClientSocket;

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

