
#ifndef FILEINFO_H
#define	FILEINFO_H

#include <string>
using std::string;

namespace BC
{
    struct FileInfo
    {
        FileItemType type;  // The type of item at the path.
        long size;          // Only set if the type is file.
        string path;        // The path of the item.
    };
}

#endif	/* FILEINFO_H */

