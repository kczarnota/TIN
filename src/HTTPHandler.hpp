#ifndef DNS_CHECKER_HTTPHANDLER_H
#define DNS_CHECKER_HTTPHANDLER_H


#include <string>
#include <json/json.h>
#include <json/writer.h>

class HTTPHandler;

class HTTPHandlerException
{
public:
    enum Type
    {
        REQUEST_SYNTAX_ERROR,
        REQUEST_NO_COMMAND,
        REQUEST_NO_DOMAINS
    };
    const std::string getDescription() const {
        return this->description;
    }
    void setDescription(const std::string description) {
        this->description = description;
    }
private:
    std::string description;
    friend class HTTPHandler;
    HTTPHandlerException(HTTPHandlerException::Type type)
    {
        this->type = type;
    }
    Type type;
};


class HTTPHandler
{
public:
    enum MessageStatus {OK, NO_IN_DATABASE, ALREADY_IN_DATABASE, UNREACHABLE, UNKNOWN};
    std::string testAction(std::string json)
    {
        chooseAction(json);
        //Json::StyledWriter writer;
        //std::string response = writer.write(this->response);
        std::cout << this->response << std::endl;
        return "cos";
    }

private:
    const std::string addAction = "add";
    const std::string removeAction = "remove";
    const std::string queryAction = "query";

    std::string command;
    std::vector<std::string> domains;
    Json::Value response;
    Json::Value vecOfDomains;
    Json::Value pair;
    Json::Value vecOfDomainStatusPairs;

    std::string chooseAction(std::string json);
    void parse(std::string json);
    std::string statusToString(HTTPHandler::MessageStatus s);
    void prepareResponse();

};


#endif //DNS_CHECKER_HTTPHANDLER_H
