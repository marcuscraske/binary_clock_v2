
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

