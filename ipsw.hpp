//
// Created by kat on 9/26/23.
//

#ifndef STRATA_IPSW_IPSW_HPP
#define STRATA_IPSW_IPSW_HPP
#include "json.hpp"
using json = nlohmann::json;

extern "C" {
extern char c_internal_download_iphonewiki_GetWikiIPSWs(char* configJson, uint64_t configJsonLen, char* proxy, int proxyLen, char insecure,
                                                        char** outputJson, int* outputJsonLen);

extern char c_internal_download_ipsw_me_GetDevice(char* identifier, size_t identifierLen, char** outJson, size_t* outJsonLen, char** err, size_t* errLen);
extern char c_internal_download_ipsw_me_GetDeviceIPSWs(char* identifier, size_t identifierLen, char** outJson, size_t* outJsonLen, char** err, size_t* errLen);

extern char c_pkg_xcode_xcode_GetDevices(char** outJson, size_t* outJsonLen, char** err, size_t* errLen);
}
namespace Pkg {
    namespace XCode {
        class XCode {
        public:
            struct Device {
                std::string target;
                std::string targetType;
                std::string targetVariant;
                std::string platform;
                std::string productType;
                std::string productDescription;
                std::string compatibleDeviceFallback;
            };
            static std::vector<Device> GetDevices() {
                std::vector<Device> devices;

                char* outputData = NULL;
                size_t outputDataLen;
                char* errData = NULL;
                size_t errDataLen = 0;
                c_pkg_xcode_xcode_GetDevices(&outputData, &outputDataLen, &errData, &errDataLen);
                std::string outString(outputData);
                free(outputData);
                if (errDataLen != 0)
                {
                    std::cerr << std::string(errData) << std::endl;
                    free(errData);
                    abort();
                }
                for (auto device : json::parse(outString))
                {
                    devices.push_back({device.contains("target") ? device["target"] : "",
                                       device.contains("target_type") ? device["target_type"] : "",
                                       device.contains("target_variant") ? device["target_variant"] : "",
                                       device.contains("platform") ? device["platform"] : "",
                                       device.contains("product_type") ? device["product_type"] : "",
                                       device.contains("product_description") ? device["product_description"] : "",
                                       device.contains("compatible_device_fallback") ? device["compatible_device_fallback"] : "",});
                }

                return devices;
            }
        };
    }
}
namespace Internal {
    namespace Download {
        class iPhoneWiki {
        public:
            struct WikiConfig {
                std::string device;
                std::string version;
                std::string build;
                bool ipsw;
                bool ota;
                bool beta;
            };
            static bool GetWikiIPSWs(WikiConfig cfg, std::string proxy, bool insecure) {
                // TODO
                return false;
            }
        };
        class IPSW_Me {
        public:
            struct IPSW {
                std::string buildid;
                uint64_t filesize;
                std::string identifier;
                std::string md5sum;
                std::string releaseDate;
                std::string sha1sum;
                std::string uploadDate;
                std::string url;
                std::string version;
            };
            static std::vector<IPSW> GetDeviceIPSWs(const std::string& identifier)
            {
                std::vector<IPSW> ipsws;

                char* outputData = NULL;
                size_t outputDataLen;
                char* errData = NULL;
                size_t errDataLen = 0;
                char* deviceID = new char[identifier.length() + 1];
                std::strcpy(deviceID, identifier.c_str());
                c_internal_download_ipsw_me_GetDeviceIPSWs(deviceID, strlen(deviceID), &outputData, &outputDataLen, &errData, &errDataLen);
                std::string outString(outputData);
                free(outputData);
                if (errDataLen != 0)
                {
                    std::cerr << std::string(errData) << std::endl;
                    free(errData);
                    abort();
                }
                for (auto ipsw : json::parse(outString))
                {
                    ipsws.push_back({ipsw.contains("buildid") ? ipsw["buildid"] : "",
                                     ipsw.contains("filesize") ? ipsw["filesize"] : "",
                                     ipsw.contains("identifier") ? ipsw["identifier"] : "",
                                     ipsw.contains("md5sum") ? ipsw["md5sum"] : "",
                                     ipsw.contains("release_date") ? ipsw["release_date"] : "",
                                     ipsw.contains("sha1sum") ? ipsw["sha1sum"] : "",
                                     ipsw.contains("upload_date") ? ipsw["upload_date"] : "",
                                     ipsw.contains("url") ? ipsw["url"] : "",
                                     ipsw.contains("version") ? ipsw["version"] : ""});
                }

                return ipsws;
            }
        };
    }
}

#endif //STRATA_IPSW_IPSW_HPP
