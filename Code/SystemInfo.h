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
 *      File:           SystemInfo.h
 *      Path:           BC/Web/Modules/SystemInfo.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * Used to retrieve information about the current system the binary clock
 * application is operating upon.
 * *****************************************************************************
 */
#ifndef SYSTEMINFO_H
#define	SYSTEMINFO_H

#include <sys/sysinfo.h>

#include <iostream>
#include <fstream>
using std::ifstream;

#include "Utils.h"
using BC::Utils;

namespace BC
{
    namespace Web
    {
        namespace Modules
        {
            struct SystemInfo
            {
                // Fields
                // -- CPU usage
                double          cpuUsageAll,
                                cpuUsageUser,
                                cpuUsageNice,
                                cpuUsageSystem,
                                cpuUsageIdle;
                // -- Memory usage
                long long       memoryTotal,
                                memoryFree,
                                memoryShared,
                                memorySelfSize,
                                memorySelfPeak;
                // -- Misc
                int             miscProcs,
                                miscPid;
                DateTime        miscTime;
                long            miscUptime;
                // Constructor
                SystemInfo()
                {
                    // CPU usage
                    ifstream iff("/proc/stat");
                    if(iff.is_open())
                    {
                        long user, nice, system, idle;
                        string t;
                        iff >> t; // cpu
                        iff >> user;
                        iff >> nice;
                        iff >> system;
                        iff >> idle;
                        double capacity = user+nice+system+idle;
                        cpuUsageAll = ((user+nice+system)/capacity)*100.0;
                        cpuUsageUser = (user/capacity)*100.0;
                        cpuUsageNice = (nice/capacity)*100.0;
                        cpuUsageSystem = (system/capacity)*100.0;
                        cpuUsageIdle = (idle/capacity)*100.0;
                        iff.close();
                    }
                    else
                    {
                        cpuUsageAll = 0;
                        cpuUsageUser = 0;
                        cpuUsageNice = 0;
                        cpuUsageSystem = 0;
                        cpuUsageIdle = 0;
                    }
                    // Memory usage
                    struct sysinfo info;
                    sysinfo(&info);
                    memoryTotal = info.totalram;
                    memoryFree = info.freeram;
                    memoryShared = info.sharedram;
                    ifstream ifm("/proc/self/status");
                    if(ifm.is_open())
                    {
                        string line, t;
                        while(getline(ifm, line, '\n'))
                        {
                            stringstream ss;
                            ss << line;
                            ss >> t;
                            if(t == "VmPeak:")
                            {
                                ss >> memorySelfPeak;
                                memorySelfPeak *= 1024;
                            }
                            else if(t == "VmSize:")
                            {
                                ss >> memorySelfSize;
                                memorySelfSize *= 1024;
                            }
                        }
                        ifm.close();
                    }
                    // Misc
                    miscPid = getpid();
                    miscTime = Utils::getDateTime();
                    miscProcs = info.procs;
                    miscUptime = info.uptime;
                }
            };
        }
    }
}

#endif	/* SYSTEMINFO_H */

