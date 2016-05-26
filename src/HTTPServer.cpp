#include "HTTPServer.hpp"
#include "HTTPPacket.hpp"
#include <thread>
#include <sstream>
#include <functional>


HTTPServer::HTTPServer()
{
	Configuration configuration = Configuration::getInstance();
	SocketAddress address;
	address.setAddress(configuration.getHttpServerAddress());
	address.setPort(configuration.getHttpServerPort());
    this->socket.setReuseAddress();
	this->socket.bind(address);
	this->socket.listen(configuration.getHttpServerMaxThreads());
}

void HTTPServer::listen()
{
	Logger::getInstance().logInfo("Server", "Starting listening...");
	while (1) 
	{
		int clientSocket = this->socket.accept();
		std::thread thread(&HTTPServer::response, this, clientSocket);
		thread.detach();
	}
}

void HTTPServer::response(int clientSocket)
{
	Logger::getInstance().logDebug("Server", "Getting data...");
	TCPSocket tcpSocket(clientSocket);
	tcpSocket.setTimeout(Configuration::getInstance().getHttpServerReadTimeout(),0);
	HTTPPacket packet;
	std::string json_response, response;
	std::string line, json;
	while(true){
		try {
			line = tcpSocket.readLine();
		}
		catch (TimeoutException &e){
			Logger::getInstance().logWarning("Server", "Timeout while reciving http packet headers from client");
			return;
		}
		if (line == "\r\n")
			break;
		packet.save_line(line);
	}
	if(packet.is_valid_request()){
		Logger::getInstance().logDebug("Server", "Valid request");
		try {
			json = tcpSocket.read_from_socket(packet.get_content_length());
		}
		catch (TimeoutException &e){
			Logger::getInstance().logWarning("Server", "Timeout while reciving http packet body from client");
			return;
		}
		packet.save_json(json);
		HTTPHandler handler;
		try {
            json_response = handler.getResponse(json);
            response = valid_request_function(json_response);
		}
		catch (HTTPHandlerException &e) {
			Logger::getInstance().logWarning("Server", "Invalid json: " + e.getDescription());
			response = invalid_request_function();
		}
	}
	else{
		Logger::getInstance().logWarning("Server", "Invalid request");
		response = invalid_request_function();
	}
	tcpSocket << response;
}


std::string HTTPServer::valid_request_function(std::string &response_json)
{
	std::string response(this->valid_request);
	size_t f = response.find("{0}");
	response.replace(f, std::string("{0}").length(), std::to_string(response_json.size()));
	response = response + "\r\n" + response_json;
	return response;
}

std::string HTTPServer::invalid_request_function()
{
	std::string response(this->invalid_request);
	return response;

}

