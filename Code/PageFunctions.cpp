
#include <string>

#include "PageFunctions.h"
#include "Alarm.h"

#include "Utils.h"
using BC::Utils;

#include "DateTime.h"
using BC::DateTime;

using std::string;

namespace BC
{
    namespace Web
    {
        namespace Modules
        {
            bool PageFunctions::moduleRegister(HttpHandler * const handler)
            {
                // Register functions
                handler->functionRegister(this, "include", include);
                handler->functionRegister(this, "getLight", getLight);
                handler->functionRegister(this, "getTemperature", getTemperature);
                handler->functionRegister(this, "getIpAddress", getIpAddress);
                handler->functionRegister(this, "getAlarms", getAlarms);
                handler->functionRegister(this, "getFieldTime", getFieldTime);
                handler->functionRegister(this, "getServices", getServices);
                handler->functionRegister(this, "getUptime", getUptime);
                handler->functionRegister(this, "getProcessTime", getProcessTime);
                
                return true;
            }
            string PageFunctions::include(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower)
            {
                string s;
                if(handler->getDiskCache()->fetchLoad(arguments, s))
                    return s;
                else
                    return string("[ERROR: Could not include file '") + arguments + string("'!]");
            }
            string PageFunctions::getTemperature(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower)
            {
                Sensors *sensors = handler->getService_Sensors();
                return Utils::round(std::to_string(sensors == 0 ? 0 : sensors->getTemperature()), 2);
            }
            string PageFunctions::getLight(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower)
            {
                Sensors *sensors = handler->getService_Sensors();
                return Utils::round(std::to_string(sensors == 0 ? 0 : sensors->getLight()), 2);
            }
            string PageFunctions::getIpAddress(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower)
            {
                return string(client->ipAddress);
            }
            string PageFunctions::getAlarms(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower)
            {
                // Load template required to build table rows
                string templateRow;
                if(!handler->getDiskCache()->fetchLoad("admin_alarm_item.bct", templateRow))
                    return "Failed to load admin_alarm_item.bct template!";
                // Build table rows
                stringstream ss;
                Alarm *alarm = handler->getService_Alarm();
                if(alarm == 0)
                    return "Alarms service is offline!";
                alarm->getMutex()->lock();
                string row;
                for(int i = 0; i < alarm->count(); i++)
                {
                    DateTime at = alarm->getAlarm(i);
                    row = templateRow;
                    if(at.day < 10)
                        Utils::replace(row, "%DAY%", "0" + std::to_string(at.day));
                    else
                        Utils::replace(row, "%DAY%", std::to_string(at.day));
                    if(at.month < 10)
                        Utils::replace(row, "%MONTH%", "0" + std::to_string(at.month));
                    else
                        Utils::replace(row, "%MONTH%", std::to_string(at.month));
                    Utils::replace(row, "%YEAR%", std::to_string(at.year));
                    if(at.hour < 10)
                        Utils::replace(row, "%HOUR%", "0" + std::to_string(at.hour));
                    else
                        Utils::replace(row, "%HOUR%", std::to_string(at.hour));
                    if(at.minute < 10)
                        Utils::replace(row, "%MINUTE%", "0" + std::to_string(at.minute));
                    else
                        Utils::replace(row, "%MINUTE%", std::to_string(at.minute));
                    if(at.second < 10)
                        Utils::replace(row, "%SECOND%", "0" + std::to_string(at.second));
                    else
                        Utils::replace(row, "%SECOND%", std::to_string(at.second));
                    ss << row;
                }
                alarm->getMutex()->unlock();
                return ss.str();
            }
            string PageFunctions::getFieldTime(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower)
            {
                if(arguments == "year")
                    return (request->formData["year"].length() > 0) ? request->formData["year"] : std::to_string(Utils::getDateTime().year);
                else if(arguments == "month")
                    return (request->formData["month"].length() > 0) ? request->formData["month"] : std::to_string(Utils::getDateTime().month);
                else if(arguments == "day")
                    return (request->formData["day"].length() > 0) ? request->formData["day"] : std::to_string(Utils::getDateTime().day);
                else if(arguments == "hour")
                    return (request->formData["hour"].length() > 0) ? request->formData["hour"] : std::to_string(Utils::getDateTime().hour);
                else if(arguments == "minute")
                    return (request->formData["minute"].length() > 0) ? request->formData["minute"] : std::to_string(Utils::getDateTime().minute);
                else if(arguments == "second")
                    return (request->formData["second"].length() > 0) ? request->formData["second"] : std::to_string(Utils::getDateTime().second);
                else
                    return "Invalid getFieldTime argument!";
            }
            string PageFunctions::getServices(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower)
            {
                // Load item template
                string temp;
                if(!handler->getDiskCache()->fetchLoad("admin_services_item.bct", temp))
                    return "Failed to load service item template!";
                // Build list of services
                stringstream ss;
                string t;
                IService *tS;
                map<string, IService*> services = handler->getService_WebHttp()->getController()->getServices();
                for(map<string, IService*>::iterator it = services.begin(); it != services.end(); it++)
                {
                    tS = (*it).second;
                    t = temp;
                    string titleEscaped = tS->getTitle();
                    Utils::replace(titleEscaped, " ", "_");
                    Utils::replace(t, "%TITLE%", tS->getTitle());
                    Utils::replace(t, "%NAME%", titleEscaped);
                    Utils::replace(t, "%STATUS%", tS->isRunning() ? "ONLINE" : "OFFLINE");
                    ss << t;
                }
                return ss.str();
            }
            string PageFunctions::getUptime(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower)
            {
                long long uptime = Utils::getEpochTimeMs() - handler->getEpochStartTimeMs();
                return Utils::getSecondsAsString(uptime / 1000LL);
            }
            string PageFunctions::getProcessTime(HttpHandler * const handler, IModule *module, Client *client, HttpRequest *request, HttpResponse *response, string arguments, string &pageUpper, string &pageLower)
            {
                return std::to_string(Utils::getEpochTimeMs() - request->getTimeStart()) + string(" m/s");
            }
        }
    }
}