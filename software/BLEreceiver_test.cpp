#include <SimpleBLE/SimpleBLE.h>
#include <iostream>

int main() {
    // Get the default BLE adapter
    SimpleBLE::Adapter adapter = SimpleBLE::Adapter::get_default();
    
    if (!adapter.is_valid()) {
        std::cerr << "No BLE adapter found!" << std::endl;
        return 1;
    }

    // Scan for BLE devices for 5 seconds
    adapter.scan_for(5);

    // Get the list of discovered devices
    auto devices = adapter.scan_get_results();
    std::cout << "Found " << devices.size() << " BLE devices:" << std::endl;

    for (const auto& device : devices) {
        std::cout << "- " << device.identifier() << " (" << device.address() << ")" << std::endl;
    }

    return 0;
}
