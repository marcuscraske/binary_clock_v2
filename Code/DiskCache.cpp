
#include <algorithm>

#include "DiskCache.h"
#include "Utils.h"
#include "Configurator.h"

namespace BC
{
    namespace Web
    {
        namespace Http
        {
            DiskCache::DiskCache(ServiceController *con, string basePath) : basePath(basePath)
            {
                reload(con);
            }
            void DiskCache::dispose()
            {
                unique_lock<mutex> lock(mlock);
                cout << "Disk Cache - disposing cache items..." << endl;
                // Dispose all of the items in the cache
                for(map<string, DiskCacheItem*>::iterator it = items.begin(); it != items.end(); it++)
                    delete (*it).second;
                items.clear();
                cout << "Disk Cache - disposed items." << endl;
            }
            void DiskCache::reload(ServiceController *con)
            {
                dispose();
                unique_lock<mutex> lock(mlock);
                cout << "Disk Cache - reloading cache..." << endl;
                // Read the extensions to be cached
                Configurator *config = static_cast<Configurator*>(con->getServiceByName(SERVICETITLE_CONFIGURATOR));
                cachedExtensions = Utils::split(config->get(CONFIG__HTTP_DISKCACHE_EXTENSIONS, HTTP_DISKCACHE_DEFAULT_EXTENSIONS), ',');
                for(vector<string>::iterator it = cachedExtensions.begin(); it != cachedExtensions.end(); it++)
                    cout << "Disk Cache - caching extension '" << *it << "'" << endl;
                if(basePath.length() == 0)
                    cerr << "Disk Cache - critical failure: unable to determine base-path of program!" << endl;
                else
                {
                    cout << "Disk Cache - locating items at '" << basePath << "'..." << endl;
                    int lengthRemoved = basePath.length()+1;
                    // Search the base-directory for items
                    vector<FileInfo> dirs;
                    vector<FileInfo> files;
                    Utils::getRecursiveFilesAndDirectories(basePath, dirs, files);
                    // Cache all the items with an extension in cachedExtensions
                    cout << "Disk Cache - processing items to be cached..." << endl;
                    string relativePath;
                    DiskCacheItem *item = 0;
                    long long totalBytes = 0;
                    for(vector<FileInfo>::iterator i = files.begin(); i != files.end(); i++)
                    {
                        item = 0;
                        relativePath = (*i).path.substr(lengthRemoved);
                        if(isCacheAppropriate(relativePath) && (item = load(relativePath)) != 0)
                        {
                            items[relativePath] = item;
                            cout << "Disk Cache - cached item at '" << basePath << "/" << relativePath << "' ('" << relativePath << "') - " << item->length << " bytes" << endl;
                            totalBytes += item->length;
                        }
                        else if(item != 0)
                            delete item;
                    }
                    cout << "Disk Cache - cached a total of " << totalBytes << " bytes!" << endl;
                }
            }
            DiskCache::~DiskCache()
            {
                dispose();
            }
            DiskCacheItem* DiskCache::fetch(string relativePath)
            {
                unique_lock<mutex> lock(mlock);
                // Locate the item in the cache
                map<string, DiskCacheItem*>::iterator it = items.find(relativePath);
                if(it == items.end())
                {
                    // Attempt to load it from disk
                    DiskCacheItem *item = load(relativePath);
                    if(item != 0 && isCacheAppropriate(relativePath))
                    {
                        // Cache the item
                        items[relativePath] = item;
                        cout << "Disk Cache - cached item at '" << basePath << "/" << relativePath << "' ('" << relativePath << "')." << endl;
                    }
                    return item;
                }
                else
                    return (*it).second;
            }
            string DiskCache::fetch(string relativePath, string errorMessage)
            {
                DiskCacheItem *item = fetch(relativePath);
                return item == 0 ? errorMessage : string(item->data, item->length);
            }
            bool DiskCache::fetchLoad(string relativePath, string &dest)
            {
                DiskCacheItem *item = fetch(relativePath);
                if(item != 0)
                    dest = string(item->data, item->length);
                return item != 0;
            }
            DiskCacheItem* DiskCache::load(string relativePath)
            {
                ifstream file(basePath + "/" + relativePath, std::ios::binary | std::ios::ate);
                if(file.is_open())
                {
                    // Read data into a new cache item
                    DiskCacheItem *item = new DiskCacheItem();
                    item->length = file.tellg();
                    file.seekg(0, std::ios::beg);
                    // Copy data across
                    item->data = new char[item->length];
                    file.read(item->data, item->length);
                    // Close file
                    file.close();
                    return item;
                }
                return 0;
            }
            bool DiskCache::isCacheAppropriate(string path)
            {
                int exti = path.find_last_of('.');
                return exti != string::npos && exti != path.length()-1 && std::find(cachedExtensions.begin(), cachedExtensions.end(), path.substr(exti)) != cachedExtensions.end();
            }
        }
    }
}