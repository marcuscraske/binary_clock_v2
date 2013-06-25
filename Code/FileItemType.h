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
 *      File:           FileItemType.h
 *      Path:           BC/FileItemType.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * An enum for the type of file; platform should be irrelevant.
 * *****************************************************************************
 */
#ifndef FILEITEMTYPE_H
#define	FILEITEMTYPE_H

namespace BC
{
    // -- The type of file; unknown - type could not be determined, possibly
    // -- does not exist or cannot be accessed.
    enum FileItemType
    {
        Directory,
        File,
        Unknown
    };
}

#endif	/* FILEITEMTYPE_H */

