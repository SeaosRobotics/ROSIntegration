#pragma once

#include <functional> // std::function
#include <iostream>

#include <CoreMinimal.h>
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "IPAddress.h"
#include <Sockets.h>
#include <SocketSubsystem.h>

#include "itransport_layer.h"
#include "types.h"

#include "rapidjson/document.h"
using json = rapidjson::Document;

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

// Must be a FRunnable to make it multi-threaded.
class TCPConnection : public FRunnable, public rosbridge2cpp::ITransportLayer {
public:
	TCPConnection(FString ip_addr, int port, bool bson_only);
	~TCPConnection();

	FString _ip_addr;
	int _port;

	// FRunnable
	virtual uint32 Run() override;
	virtual void Exit() override;
	virtual void Stop() override;

	bool IsHealthy() const;
	bool SendMessage(std::string data);
	bool SendMessage(const uint8_t *data, unsigned int length);
	int ReceiverThreadFunction(TSharedRef<FInternetAddr> addr);
	void RegisterIncomingMessageCallback(std::function<void(json&)> fun);
	void RegisterIncomingMessageCallback(std::function<void(bson_t&)> fun);
	void RegisterErrorCallback(std::function<void(rosbridge2cpp::TransportError)> fun);
	void ReportError(rosbridge2cpp::TransportError err);
	void SetTransportMode(rosbridge2cpp::ITransportLayer::TransportMode);

private:

	FRunnableThread * Thread;
	FSocket *_sock;
	bool bson_mode, running;
	std::function<void(json&)> _incoming_message_callback;
	std::function<void(bson_t&)> incoming_message_callback_bson_;
	std::function<void(rosbridge2cpp::TransportError)> _error_callback;
};
