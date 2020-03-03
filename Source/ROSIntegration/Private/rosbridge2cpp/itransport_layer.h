#pragma once

#include "types.h"

/*
 * This class provides an interfaces for generic Transportlayers that can be used by the ROSBridge.
 * Since this library has been developed for different plattforms,
 * we abstract from the actual transport layer, since this varies from plattforms (and frameworks) a lot.
 *
 * Please see client/socket_tcp_connection.h for an example implementation that uses UNIX Sockets
 * to connect to a ROSBridge server
 */
namespace rosbridge2cpp {
	class ITransportLayer {
	public:
		enum TransportMode { JSON, BSON };

		// Status of the transport layer's socket connection & rx thread.
		virtual bool IsHealthy() const;

		// Send a string over the underlying transport mechanism to the rosbridge server
		virtual bool SendMessage(std::string data) = 0;

		// Send a string over the underlying transport mechanism to the rosbridge server
		virtual bool SendMessage(const uint8_t *data, int32 length) = 0;

		// Register a std::function that will be called whenever a new data packet has been received by this TransportLayer.
		virtual void RegisterIncomingMessageCallback(std::function<void(json&)>) = 0;

		// Register a std::function that will be called whenever a new data packet has been received by this TransportLayer.
		virtual void RegisterIncomingMessageCallback(std::function<void(bson_t&)>) = 0;

		// Register a std::function that will be called when errors occur.
		virtual void RegisterErrorCallback(std::function<void(TransportError)>) = 0;

		// Report an error to the registered ErrorCallback (see RegisterErrorCallback)
		virtual void ReportError(TransportError) = 0;

		// Report an error to the registered ErrorCallback (see RegisterErrorCallback)
		virtual void SetTransportMode(TransportMode) = 0;
	private:
		/* data */
	};
}
