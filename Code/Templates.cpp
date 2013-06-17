#include "Templates.h"

namespace BC
{
    namespace Web
    {
        namespace Http
        {
            string Templates::format(HttpHandler * const handler, string html, Client *client, HttpRequest *request, HttpResponse *response)
            {   
                if(html.length() >= 10)
                {
                    int t, startTag = 0, funcLength, argsLength;
                    while(startTag < html.length() - 10)
                    {
                        //  <!--[function name](arguments)-->
                        funcLength = argsLength = 0;
                        // <---
                        while(startTag < html.length() - 7 && !(html[startTag] == '<' && html[startTag+1] == '!' && html[startTag+2] == '-' && html[startTag+3] == '-'))
                            startTag++;
                        while(((t = startTag + 4 + funcLength + 1) < html.length() - 5) && ( (html[t] >= 'a' && html[t] <= 'z' ) || (html[t] >= 'A' && html[t] <= 'Z') || (html[t] >= '0' && html[t] <= '9') ) )
                            funcLength++;
                        if(funcLength > 0 && html[t] == '(')
                        {
                            // ( ... ) - we'll loop until we find closing )
                            // [start tag <---][function name]([argLength]
                            while((t = startTag + 4 + funcLength + 1 + argsLength + 1) < html.length() && html[t] != ')')
                                argsLength++;
                            // Check for ending ')-->'
                            t = startTag + 4 + funcLength + 1 + argsLength;
                            if(html[t+1] == ')' && html[t+2] == '-' && html[t+3] == '-' && html[t+4] == '>')
                            {
                                // Success - we found a valid func call!
                                // Check if the function exists
                                string function = html.substr(startTag + 4, funcLength+1);
                                string args = html.substr(startTag + 4 + funcLength + 2, argsLength);
                                string content;
                                FunctionMapping *fm = handler->functionGet(function);
                                // Split content
                                string upper = html.substr(0, startTag);
                                string lower = html.substr(startTag + 4 + funcLength + 1 + argsLength + 5);
                                // Get function content
                                if(fm != 0)
                                    content = fm->function(handler, fm->module, client, request, response, args, upper, lower);
                                else
                                    content = string("[ERROR: Function '") + function + string("' is not mapped!]");
                                // Replace markup with content
                                html = upper + content + lower;
                            }
                        }
                        startTag++; // If not, the first while-loop will repeat the same condition (infinite-loop)
                    }
                }
                return html;
            }
        }
    }
}