
#ifndef UTILS_H
#define	UTILS_H

#include <ctime>
#include <string>
#include <math.h>
#include <unistd.h>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "FileItemType.h"
#include "FileInfo.h"

using std::string;
using std::vector;

using BC::FileItemType;
using BC::FileInfo;

#include "DateTime.h"
using BC::DateTime;

#include "Time.h"
namespace BC
{
    class Time;
}
using BC::Time;

#include <sstream>
using std::stringstream;

#include <cstdarg> // Used for va_list.

namespace BC
{
    static const char base64map[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'};
    class Utils
    {
    public:
        inline static long long getEpochTimeMs()
        {
            struct timeval t;
            gettimeofday(&t, 0);
            // tv_sec represents seconds, tv_usec represents the microseconds
            // 1 mill microseconds = secs
            return ((long long)t.tv_sec * 1000LL) + ((long long)t.tv_usec / 1000LL);
        }
        static DateTime getDateTime();
        static Time getTime();
        static string getSecondsAsString(long long seconds)
        {
            long long days = seconds / 86400LL;
            long long hours = (seconds % 86400LL) / 3600LL;
            long long mins = ((seconds % 86400LL) % 3600LL) / 60LL;
            long long secs = ((seconds % 86400LL) % 3600LL) % 60LL;
            return std::to_string(days) + " days " + std::to_string(hours) + " hours " + std::to_string(mins) + " minutes " + std::to_string(secs) + " seconds";
        }
        /**
         * Returns the chrono millisecond for the time now and an offset.
         * @param ms The offset to be added.
         * @return The time now with the offset applied.
         */
        inline static void conditionVariableSleep(std::unique_lock<std::mutex> &cl, std::condition_variable &cv, int ms)
        {
            auto n = std::chrono::system_clock::now();
            cv.wait_until(cl, n + std::chrono::milliseconds(ms));
        }
        /**
         * Removes leading and trailing white-space.
         */
        static string trim(string value)
        {
            int start = 0, end = value.length() - 1;
            while(start < end+1 && (value[start] == ' ' || value[start] == '\r' || value[start] == '\n' || value[start] == '\t'))
                start++;
            while(end > 0 && (value[end] == ' ' || value[end] == '\r' || value[end] == '\n' || value[end] == '\t'))
                end--;
            if(start > end || end < start)
                return "";
            else
                return value.substr(start, (end - start)+1);
        }
        /**
         * Attempts to parse a string as a numeric data-type; returns
         * -1 if unsuccessful (should only be used for positive integers).
         */
        static bool tryParse(string data, int &dest)
        {
            if(data.length() == 0) return false;
            int i = 0;
            dest = 0;
            while(i < data.length() && data[i] >= '0' && data[i] <= '9')
            {
                dest = dest * 10 + (data[i] - '0');
                i++;
            }
            return i == data.length();
        }
        static string round(string number, int decimalPlaces)
        {
            if(decimalPlaces <= 0)
                return number;
            int ind = number.find_first_of('.');
            if(ind == string::npos || ind+1+decimalPlaces > number.length())
                return number;
            else
                return number.substr(0, ind+1+decimalPlaces);
        }
        static void replace(string &input, string replaceOld, string replaceNew)
        {
            if(input.length() == 0 || input.length() < replaceOld.length() || replaceOld.length() == 0)
                return;
            int pos = 0;
            while(pos < (input.length() - replaceOld.length())+1)
            {
                // Compare each char; attempt to disprove a match
                bool match = true;
                for(int i = 0; match && i < replaceOld.length(); i++)
                    if(input[pos+i] != replaceOld[i])
                        match = false;
                // If we found a match, split and insert
                if(match)
                {
                    string t;
                    if(pos > 0)
                        t = input.substr(0, pos) + replaceNew;
                    else
                        t = replaceNew;
                    if(pos+replaceOld.length() < input.length())
                        t += input.substr(pos+replaceOld.length());
                    input = t;
                    pos += replaceNew.length();
                }
                else
                    pos++;
            }
        }
        static string base64Decode(string text)
        {
            int totalchars = text.length();
            // Ignore padding characters
            while(text[totalchars-1] == '=')
                totalchars--;
            // Translate characters into numeric values
            for(int i = 0; i < totalchars; i++)
            {
                int n = 0;
                char c = text[i];
                while(base64map[n] != c && n < 64)
                    n++;
                text[i] = n;
            }
            // Decode binary
            int charoffset = 0;
            int charbitoffset = 0;
            int resultoffset = 0;
            int resultbitoffset = 0;
            int resultchars = (int)ceil((double)(totalchars * 6) / 8.0);
            char *result = new char[resultchars + 1]; // +1 for null/terminate
            // Encode each character
            while(charoffset < totalchars)
            {
                if((text[charoffset] & (1 << (5 - charbitoffset))) > 0)
                    result[resultoffset] |= 1 << (7 - resultbitoffset);
                if(++charbitoffset > 5)
                {
                    charbitoffset = 0;
                    charoffset++;
                }
                if(++resultbitoffset > 7)
                {
                    resultbitoffset = 0;
                    resultoffset++;
                }
            }
            return result;

        }
        static string base64Encode(string text)
        {
            int totalchars = text.length();
            int charoffset = 0;
            int charbitoffset = 0;
            int resultoffset = 0;
            int resultbitoffset = 0;
            int resultchars = (int)ceil((double)(totalchars * 8) / 6.0);
            int paddchars = (resultchars % 4);
            if(paddchars > 0)
                paddchars = 4 - paddchars;
            char *result = new char[resultchars + paddchars + 1]; // +1 for null/terminate
            // Encode each character
            while(charoffset < totalchars)
            {
                if((text[charoffset] & (1 << (7 - charbitoffset))) > 0)
                    result[resultoffset] |= 1 << (5 - resultbitoffset);
                if(++charbitoffset > 7)
                {
                    charbitoffset = 0;
                    charoffset++;
                }
                if(++resultbitoffset > 5)
                {
                    resultbitoffset = 0;
                    resultoffset++;
                }
            }
            // Encode result into ASCII
            for(int i = 0; i < resultchars; i++)
                result[i] = base64map[result[i]];
            // Add padding
            for(int i = 0; i < paddchars; i++)
                result[resultchars + i] = '=';
            return std::string(result);
        }
        static inline void sleep(long milliseconds)
        {
            usleep(1000 * milliseconds);
        }
        static inline string intToString(int number)
        {
            return std::to_string(number);
        }
        static inline string longToString(long number)
        {
            return std::to_string(number);
        }
        static inline string longlongToString(long long number)
        {
            return std::to_string(number);
        }
        static inline string doubleToString(double number)
        {
            return std::to_string(number);
        }
        // -- Calculates the absolute value of the specified value.
        template<class T>
        static inline T abs(T val)
        {
            return val >= 0 ? val : val *-1;
        }
        // -- Splits a string by a delimiter and returns a vector of tokens.
        // -- Blank tokens are ignored.
        static vector<string> split(string source, char delimiter)
        {
            vector<string> tokens;
            string ct; // The current token
            char c;
            for(int i = 0; i < source.length(); i++)
            {
                c = source[i];
                if(c != delimiter)
                    ct += c;
                else if(ct.length() > 0)
                {
                    tokens.push_back(ct);
                    ct = "";
                }
            }
            if(ct.length() > 0)
                tokens.push_back(ct);
            return tokens;
        }
        static inline bool endsWith(string source, string ending)
        {
            return source.substr(source.length() - (ending.length() + 1)) == ending;
        }
        // -- Gets the base-path of the current application; returns an
        // -- empty-string if an error occurred.
        static string getBasePath()
        {
            char path[1024];
            if(getcwd(path, sizeof(path)))
                return string(path);
            else
                return "";
        }
        // -- Returns information about the item at the specified path.
        static FileInfo getPathInfo(string path)
        {
            FileInfo cinfo;
            cinfo.path = path;
            // Fetch info about the item at the path
            struct stat info;
            // Determine the type using bitmask of st_mode
            if(stat(path.c_str(), &info) != 0)
                cinfo.type = FileItemType::Unknown;
            else if(info.st_mode & S_IFDIR)
                cinfo.type = FileItemType::Directory;
            else if(info.st_mode & S_IFREG)
            {
                cinfo.type = FileItemType::File;
                cinfo.size = info.st_size;
            }
            else
                cinfo.type = FileItemType::Unknown;
            return cinfo;
        }
        static bool getFilesDirectories(string path, vector<FileInfo> &directories, vector<FileInfo> &files)
        {
            DIR *dir = opendir(path.c_str());
            // Check we managed to get a handle to the directory
            if(!dir)
                return false;
            // Iterate through each item
            struct dirent *ent;
            FileInfo fi;
            string p;
            while(ent = readdir(dir))
            {
                p = string(ent->d_name);
                if(p != "." && p != "..")
                {
                    p = path + "/" + p;
                    fi = getPathInfo(p);
                    switch(fi.type)
                    {
                        case FileItemType::File:
                            files.push_back(fi);
                            break;
                        case FileItemType::Directory:
                            directories.push_back(fi);
                            break;
                    }
                }
            }
            closedir(dir);
        }
    private:
        static inline void _getRecursiveFilesAndDirectories(string path, vector<FileInfo> &directories, vector<FileInfo> &files)
        {
            // Fetch all the files and folders at the current path
            vector<FileInfo> tdi;
            getFilesDirectories(path, tdi, files);
            // Recursively process each directory
            FileInfo f;
            for(vector<FileInfo>::iterator it = tdi.begin(); it != tdi.end(); it++)
            {
                f = *it;
                directories.push_back(f);
                _getRecursiveFilesAndDirectories(f.path, directories, files);
            }
        }
    public:
        // -- Recursively adds all the files and folders at a path; base path
        // -- of path's are removed.
        static void getRecursiveFilesAndDirectories(string path, vector<FileInfo> &directories, vector<FileInfo> &files)
        {
            _getRecursiveFilesAndDirectories(path, directories, files);
        }
        // -- Clones the source array into the destination array; destination
        // -- array must be allocated.
        template<class T>
        static void arrayClone(T* source, T* dest, int size)
        {
            for(int i = 0; i < size; i++)
                dest[i] = source[i];
        }
        static string htmlEncode(string input)
        {
            Utils::replace(input, "<", "&lt;");
            Utils::replace(input, ">", "&gt;");
            Utils::replace(input, "&", "&amp;");
            Utils::replace(input, "%", "&#38;");
            return input;
        }
        static string urlDecodeBasic(string input)
        {
            Utils::replace(input, "%2C", ",");
            Utils::replace(input, "%A3", "£");
            Utils::replace(input, "%24", "$");
            Utils::replace(input, "%21", "!");
            Utils::replace(input, "%20", " ");
            Utils::replace(input, "+", " ");
            Utils::replace(input, "%2B", "+");
            Utils::replace(input, "%3F", "?");
            Utils::replace(input, "%27", "'");
            Utils::replace(input, "%60", "`");
            return input;
        }
        // -- Returns the human-readable representation of the bytes e.g.
        // -- 2.43 MB etc.
        static string bytesToHumanString(long long bytes)
        {
            // Quick check just in-case
            if(bytes == 0)
                return "0 B";
            else if(bytes < 0)
                return "Invalid number of bytes!";
            
            double v = (double)bytes;
            
            if(bytes > 1099511627776LL) // Terrabytes
            {
                return std::to_string(v / 1099511627776.0) + " TB";
            }
            else if(bytes > 1073741824LL) // Gigabytes
            {
                return std::to_string(v / 1073741824.0) + " GB";
            }
            else if(bytes > 1048576LL) // Megabytes
            {
                return std::to_string(v / 1048576.0) + " MB";
            }
            else if(bytes > 1024LL) // Kilobytes
            {
                return std::to_string(v / 1024.0) + " KB";
            }
            else
                return std::to_string(v) + " B";
        }
        static int random(int min, int max)
        {
            return min + (rand() % (max - min + 1));
        }
        static void randomSetSeed()
        {
            srand(time(0));
        }
        static string getTimeDateGBString();
        static string concatChars(int totalChars, char chars, ...)
        {
            stringstream ss;
            va_list l;
            va_start(l, chars);
            char t;
            for(int i = 0; i < totalChars; i++)
            {
                t = va_arg(l, int);
                ss << t;
            }
            va_end(l);
            return ss.str();
        }
    };
}

#endif	/* UTILS_H */
