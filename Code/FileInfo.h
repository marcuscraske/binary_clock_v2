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
 *      File:           FileInfo.h
 *      Path:           BC/FileInfo.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * A structure for holding information about a file; platform should be
 * irrelevant.
 * *****************************************************************************
 */
#ifndef FILEINFO_H
#define	FILEINFO_H

#include <string>
using std::string;

namespace BC
{
    struct FileInfo
    {
        FileItemType    type;   // The type of item at the path.
        long            size;   // Only set if the type is file.
        string          path;   // The path of the item.
    };
}

#endif	/* FILEINFO_H */

