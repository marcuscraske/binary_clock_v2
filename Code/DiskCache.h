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
 *      File:           DiskCache.h
 *      Path:           BC/Web/Http/DiskCache.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * Disk caching used for the web-server, although this could be used for other
 * things since it's independent. This is also a thread-safe collection.
 * Disk caching is used to store files in primary memory, as opposed to
 * secondary memory, to speed-up requests by reducing I/O time.
 * *****************************************************************************
 */
#ifndef DISKCACHE_H
#define	DISKCACHE_H

#define HTTP_DISKCACHE_DEFAULT_EXTENSIONS       ".bc,.bct,.css,.png,.jpg"

#include "Definitions.h"

#include <map>
using std::map;

#include <mutex>
using std::mutex;
using std::unique_lock;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <iostream>
#include <fstream>
using std::iostream;
using std::ifstream;

#include "ServiceController.h"
using BC::ServiceController;

#include "DiskCacheItem.h"
using BC::Web::Http::DiskCacheItem;

#include "Utils.h"
using BC::Utils;

#include "Configurator.h"
using BC::Services::Configurator;

// Forward declarations
namespace BC
{
    namespace Web
    {
        namespace Http
        {
            class DiskCacheItem;
        }
    }
}
using BC::Web::Http::DiskCacheItem;

namespace BC
{
    namespace Web
    {
        namespace Http
        {
            class DiskCache
            {
            private:
                string basePath;                        // The base path for files.
                mutex mlock;                            // Used for locking mutual access to the cache.
                map<string, DiskCacheItem*> items;      // Cached items.
                vector<string> cachedExtensions;        // The extensions to be cached.
            public:
                DiskCache(ServiceController *con, string basePath);
                ~DiskCache();
                // -- Fetches an item from cache; returns zero if not found.
                // -- If the item is not in the cache, it'll check the
                // -- file-system and cache the item if its extension is
                // -- to be cached.
                DiskCacheItem* fetch(string relativePath);
                // -- Fetches an item as a string; returns errorMessage if
                // -- the item could not be loaded.
                string fetch(string relativePath, string errorMessage);
                // -- Fetches an item and places it in dest if found; returns
                // -- true if found, false if not found.
                bool fetchLoad(string relativePath, string &dest);
                // -- Indicates if the item at the path ends with an
                // -- extension which is in the cache list.
                bool isCacheAppropriate(string path);
                // -- Wipes and reloads the cache with items from the local
                // -- file-system.
                void reload(ServiceController *con);
                // -- Disposes all of the cached items.
                void dispose();
                // -- Constant access to cache items; not thread-safe, use
                // -- getMutex to lock the collection.
                inline map<string, DiskCacheItem*> const* getItems()
                {
                    return &items;
                }
                inline mutex* getMutex()
                {
                    return &mlock;
                }
                inline string getBasePath()
                {
                    return basePath;
                }
            private:
                // -- Attempts to load an item at the specified path; returns
                // -- 0 if it could not be loaded. This is an internal
                // -- function. Does not load items into the cache, but
                // -- rather the invoking function should add it to the cache.
                DiskCacheItem* load(string relativePath);
            };
        }
    }
}

#endif	/* DISKCACHE_H */

