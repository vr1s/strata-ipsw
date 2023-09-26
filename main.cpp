#include <iostream>
#include "ipsw.hpp"

int main()
{
    std::vector<Internal::Download::IPSW_Me::IPSW> ipsws;
    for (const auto& device : Pkg::XCode::XCode::GetDevices())
    {
        if (device.productType.find("iPhone") != std::string::npos)
        {
            const auto& ipswsForDevice = Internal::Download::IPSW_Me::GetDeviceIPSWs(device.productType);
            ipsws.insert( ipsws.end(), ipswsForDevice.begin(), ipswsForDevice.end() );
        }
    }
    for (const auto ipsw : ipsws)
    {
        // this has a sha and md5 field if you want to do something funky w/ that.
        std::cout << ipsw.url << std::endl;
    }
    // std::cout << "Got " << ipsws.size() << " Image URLs" << std::endl;
    return 0;
}
