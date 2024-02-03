#pragma once

#include <pangolin/video/video_interface.h>
#include <cstdint>

namespace pangolin
{

class AsyncSourceReader;
class PANGOLIN_EXPORT UvcMediaFoundationVideo
    : public pangolin::VideoInterface, public pangolin::VideoUvcInterface, public pangolin::VideoPropertiesInterface
{
  public:
    UvcMediaFoundationVideo(int vendorId, int productId, const std::string& instanceId, size_t width, size_t height, int fps) {};
    ~UvcMediaFoundationVideo() {};

    //! Implement VideoInput::Start()
    void Start() {};

    //! Implement VideoInput::Stop()
    void Stop() {};

    //! Implement VideoInput::SizeBytes()
    size_t SizeBytes() const { return 0; };

    //! Implement VideoInput::Streams()
    const std::vector<pangolin::StreamInfo>& Streams() const { return {}; };

    //! Implement VideoInput::GrabNext()
    bool GrabNext(unsigned char* image, bool wait = true) { return true; };

    //! Implement VideoInput::GrabNewest()
    bool GrabNewest(unsigned char* image, bool wait = true) { return true; };

    //! Implement VideoUvcInterface::GetCtrl()
    int IoCtrl(uint8_t unit, uint8_t ctrl, unsigned char* data, int len, pangolin::UvcRequestCode req_code) { return 0; };

    //! Implement VideoUvcInterface::GetExposure()
    bool GetExposure(int& exp_us) { return true; };

    //! Implement VideoUvcInterface::SetExposure()
    bool SetExposure(int exp_us) { return true; };

    //! Implement VideoUvcInterface::GetGain()
    bool GetGain(float& gain) { return true; };

    //! Implement VideoUvcInterface::SetGain()
    bool SetGain(float gain) { return true; };

    //! Access JSON properties of device
    const picojson::value& DeviceProperties() const { return {}; };

    //! Access JSON properties of most recently captured frame
    const picojson::value& FrameProperties() const { return {}; };

  protected:
    void InitDevice(size_t width, size_t height, bool async);

    IMFMediaSource* mediaSource;
    AsyncSourceReader* asyncSourceReader;

    int64_t expected_fps;
};
}
