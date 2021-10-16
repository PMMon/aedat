#include <csignal>
#include <string>
#include <sys/types.h>

#include "CLI11.hpp"

#include "aedat.hpp"
#include "usb.hpp"

int main(int argc, char *argv[]) {
  CLI::App app{"Streams DVS data from a USB camera or AEDAT file to a file or "
               "UDP socket"};

  // Define CLI arguments
  std::string camera = "davis";
  std::uint16_t deviceId;
  std::uint16_t deviceAddress;
  std::int64_t maxPackets = -1;
  std::uint16_t packetSize = 128;
  std::string port = "3333";           // Port number
  std::string ipAddress = "localhost"; // IP Adress - if NULL, use own IP.
  std::uint32_t bufferSize = 1024;
  std::string filename = "None"; // TODO: Implement file output with subgroups

  app.add_option("id", deviceId, "Hardware ID")->required();
  app.add_option("address", deviceAddress, "Hardware address")->required();
  app.add_option("camera", camera, "Type of camera; davis or dvx")->required();
  app.add_option("destination", ipAddress,
                 "Destination IP. Defaults to localhost");
  app.add_option("port", port, "Destination port. Defaults to 3333");
  app.add_option("--max-packets", maxPackets,
                 "Maximum number of packets to read before sending. Defaults "
                 "to -1 (infinite).");
  app.add_option("--packet-size", packetSize,
                 "Number of events in a single UDP packet. Defaults to 128");
  app.add_option("--buffer-size", bufferSize,
                 "UDP buffer size. Defaults to 1024");

  CLI11_PARSE(app, argc, argv);

  try {
    Generator<AEDAT::PolarityEvent> generator =
        usb_event_generator(camera, deviceId, deviceAddress);

    int count = 0;
    for (AEDAT::PolarityEvent event : generator) {
      printf("Test %dx%d", event.x, event.y);
    }
  } catch (const std::exception &e) {
    std::cout << "Failure while streaming events: " << e.what() << "\n";
  }
}