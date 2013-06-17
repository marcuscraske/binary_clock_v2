
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

