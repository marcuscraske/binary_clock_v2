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
 *      File:           DiskCacheItem.h
 *      Path:           BC/Web/Http/DiskCacheItem.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * A data-type used to represent an item stored in the disk cache.
 * *****************************************************************************
 */
#ifndef DISKCACHEITEM_H
#define	DISKCACHEITEM_H

#include "DiskCache.h"
// Forward declaration
namespace BC
{
    namespace Web
    {
        namespace Http
        {
            class DiskCache;
        }
    }
}
using BC::Web::Http::DiskCache;

namespace BC
{
    namespace Web
    {
        namespace Http
        {
            class DiskCacheItem
            {
                friend DiskCache;
            private:
                // Fields ----------------------------------------------------->
                long length;
                char *data;
            public:
                // Constructors ----------------------------------------------->
                DiskCacheItem() {}
                // Destructors ------------------------------------------------>
                ~DiskCacheItem()
                {
                    delete[] data;
                }
                // Member Functions - Accessors ------------------------------->
                inline long getLength() const
                {
                    return this->length;
                }
                inline char* getData() const
                {
                    return this->data;
                }
            };
        }
    }
}

#endif	/* DISKCACHEITEM_H */

