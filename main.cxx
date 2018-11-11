#include <iostream>
#include <vector>
#include "Utilities/Utility.h"
#include "udp/UdpSender.h"
#include "udp/UdpReceiver.h"

#define PORT 5000
#define PACKET_COUNT 1000000.0f

int main(int argc, char **argv)
{
	UTIL.setLogLevel(4);

	std::vector<std::string> args(argv + 1, argv + argc);

	if (args.size() < 1)
	{
		UTIL.log(ERROR_LOG, "USAGE:\n"
				"\t ./UdpQueue send 10.0.0.100\n"
				"\t ./UdpQueue recv");
		return ERROR;
	}

	if (args[0] == "send")
	{
		UdpSender sender;
		sender.init(args[1], PORT);

		// create a fixed size packet
		std::vector<unsigned char> packet;
		int packetSize = 32;
		for (int ii = 0; ii < packetSize; ii++)
		{
			packet.push_back(0x01);
		}

		// Send packets
		int counter = (int) PACKET_COUNT;
		UTIL.log(INFO_LOG, "Sending %d packets...", counter);
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		while (counter--)
		{
			sender.send(packet);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
		auto averageTime = duration / PACKET_COUNT;
		auto avgPacketsPerSecond = 1.0 / (averageTime * 1e-6); // microseconds to seconds

		UTIL.log(INFO_LOG, "Send duration: %f", duration);
		UTIL.log(INFO_LOG, "Average packets sent per second: %f", avgPacketsPerSecond);
	}
	else if (args[0].compare("recv") == OK)
	{
		UdpReceiver udpReceiver("RECV");
		udpReceiver.init(PORT);

		std::vector<unsigned char> packet;
		int counter = PACKET_COUNT;
		UTIL.log(INFO_LOG, "Receiving packets...");
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		while (counter--)
		{
			udpReceiver.receive(packet);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
		UTIL.log(INFO_LOG, "Receive duration: %f", duration);
	}
	else
	{
		UTIL.log(ERROR_LOG, "Invalid run type!");
		return ERROR;
	}

	return OK;
}