
#ifndef IPADDRESS_H
#define	IPADDRESS_H

#include <string>
#include <sstream>
#include "Utils.h"

using std::string;
using std::stringstream;
using BC::Utils;

namespace BC
{
    namespace Services
    {
        class IPAddress
        {
            enum Type
            {
                IPv4,
                IPv6,
                Invalid
            };
        private:
            Type type;
            int *segments;
            int totalSegments;
        public:
            IPAddress(string ipAddress)
            {
                segments = 0;
                // Determine type by finding either ':' for IPv6 or '.' for IPv4 (since IPv6 can have zeroes ommitted - size is therefore a poor indicator)
                // -- We do not allow zero-padded addresses.
                if(ipAddress.find_first_of('.') != string::npos)
                {
                    // IPv4
                    if(!splitIntoSegments(ipAddress, '.', 255) || totalSegments != 4)
                        type = Type::Invalid;
                    else
                        type = Type::IPv4;
                }
                else if(ipAddress.find_first_of(':') != string::npos)
                {
                    // IPv6
                    if(!splitIntoSegments(ipAddress, ':', 65535) || totalSegments != 8)
                        type = Type::Invalid;
                    else
                        type = Type::IPv6;
                }
                else
                    type = Type::Invalid;
            }
            IPAddress(const IPAddress &original)
            {
                type = original.type;
                if(type != Type::Invalid)
                {
                    totalSegments = original.totalSegments;
                    segments = new int[totalSegments];
                    for(int i = 0; i < totalSegments; i++)
                        segments[i] = original.segments[i];
                }
            }
        private:
            IPAddress(){}
        public:
            ~IPAddress()
            {
                if(segments != 0)
                    delete[] segments;
            }
            inline Type getType()
            {
                return type;
            }
            inline int getSegment(int index)
            {
                return segments[index];
            }
            inline int segmentCount()
            {
                return totalSegments;
            }
            bool operator<(const IPAddress &addr2)
            {
                // Check we are comparing the same types
                if(type == Type::Invalid || type != addr2.type || totalSegments != addr2.totalSegments)
                    return false;
                // Continue while segments are equal
                for(int i = 0; i < totalSegments; i++)
                {
                    if(segments[i] != addr2.segments[i])
                        return segments[i] < addr2.segments[i];
                }
                return false;
            }
            bool operator>(const IPAddress &addr2)
            {
                // Check we are comparing the same types
                if(type == Type::Invalid || type != addr2.type || totalSegments != addr2.totalSegments)
                    return false;
                // Continue while segments are equal
                for(int i = 0; i < totalSegments; i++)
                {
                    if(segments[i] != addr2.segments[i])
                        return segments[i] > addr2.segments[i];
                }
                return false;
            }
            bool operator<=(const IPAddress &addr2)
            {
                // Check we are comparing the same types
                if(type == Type::Invalid || type != addr2.type || totalSegments != addr2.totalSegments)
                    return false;
                // Continue while segments are equal
                for(int i = 0; i < totalSegments; i++)
                {
                    if(segments[i] != addr2.segments[i])
                        return segments[i] < addr2.segments[i];
                }
                return true;
            }
            bool operator>=(const IPAddress &addr2)
            {
                // Check we are comparing the same types
                if(type == Type::Invalid || type != addr2.type || totalSegments != addr2.totalSegments)
                    return false;
                // Continue while segments are equal
                for(int i = 0; i < totalSegments; i++)
                {
                    if(segments[i] != addr2.segments[i])
                        return segments[i] > addr2.segments[i];
                }
                return true;
            }
            // Only works for IPv4 at present.
            static IPAddress getAddressOffset(const IPAddress &addr, long long offset)
            {
                IPAddress clone(addr);
                if(clone.type == Type::Invalid)
                    return clone;
                // Offset the segments
                if(offset >= 16777216) // 256^3
                {
                    clone.segments[0] += offset / 16777216;
                    offset = offset % 16777216;
                }
                if(offset >= 65536) // 256^2
                {
                    clone.segments[1] += offset / 65536;
                    offset = offset % 65536;
                }
                if(offset >= 256) // 256^1
                {
                    clone.segments[2] += offset / 256;
                    offset = offset % 256;
                }
                clone.segments[3] += offset;
                return clone;
            }
            string toString()
            {
                char c;
                if(type == Invalid)
                    return "Invalid IP";
                else if(type == Type::IPv4)
                    c = '.';
                else if(type = Type::IPv6)
                    c = ':';
                else
                    return "Unknown type of IP";
                stringstream ss;
                for(int i = 0; i < totalSegments; i++)
                {
                    ss << segments[i];
                    if(i < totalSegments -1)
                        ss << c;
                }
                return ss.str();
            }
        private:
            bool splitIntoSegments(string ipAddress, char splitChar, int segmentInclusiveLimit)
            {
                vector<string> rawSegs = Utils::split(ipAddress, splitChar);
                // Convert each segment into an integer
                totalSegments = rawSegs.size();
                segments = new int[totalSegments];
                int v;
                for(int i = 0; i < totalSegments; i++)
                {
                    if(Utils::tryParse(rawSegs.at(i), v) && v >= 0 && v <= segmentInclusiveLimit)
                        segments[i] = v;
                    else
                    {
                        // Invalid segment found - delete our progress and return false!
                        totalSegments = 0;
                        delete[] segments;
                        return false;
                    }
                }
                return true;
            }
        };
    }
}

#endif	/* IPADDRESS_H */

