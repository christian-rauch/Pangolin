#ifndef NOMINMAX
#  define NOMINMAX
#endif

#include <mfidl.h>
#include <mfreadwrite.h>

#include <pangolin/factory/factory_registry.h>
#include <pangolin/video/drivers/uvc_mediafoundation.h>


namespace pangolin
{

class AsyncSourceReader : public IMFSourceReaderCallback
{
public:
    AsyncSourceReader(IMFMediaSource* mediaSource, uint64_t timeout_ms) : ref_(1)
    {

    }

    // IUnknown methods
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv)
    {
        // library for REFIID comparison operator?
      // if (iid != __uuidof(IMFSourceReaderCallback)) {
      //   return E_NOINTERFACE;
      // }
      return S_OK;
    }
    STDMETHODIMP_(ULONG) AddRef()
    {
        return InterlockedIncrement(&ref_);
    }
    STDMETHODIMP_(ULONG) Release()
    {
        return 1;
    }

    // IMFSourceReaderCallback methods
    STDMETHODIMP OnReadSample(HRESULT hrStatus, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample *pSample)
    {

        return S_OK;
    }

    STDMETHODIMP OnEvent(DWORD, IMFMediaEvent *)
    {
        return S_OK;
    }

    STDMETHODIMP OnFlush(DWORD)
    {
        return S_OK;
    }

    IMFSourceReader* AddRefReader()
    {

    }

    HRESULT ReadSample(DWORD dwStreamIndex, DWORD dwControlFlags, DWORD* pdwActualStreamIndex, DWORD* pdwStreamFlags, LONGLONG* pllTimestamp, IMFSample** ppSample)
    {

    }

private:

    LONG ref_;
};


void UvcMediaFoundationVideo::InitDevice(size_t width, size_t height, bool async)
{
    if (async)
    {
        int64_t timeout_ms = expected_fps ? 4 * 1000 / expected_fps : 500;
        asyncSourceReader = new AsyncSourceReader(mediaSource, timeout_ms);
    }
}


PANGOLIN_REGISTER_FACTORY(UvcMediaFoundationVideo)
{
    struct UvcMediaFoundationVideoFactory final : public TypedFactoryInterface<VideoInterface>
    {
        std::map<std::string,Precedence> Schemes() const override
        {
            return {{"uvc",10}};
        }
        const char* Description() const override
        {
            return "Use Windows Media Foundation to open UVC USB device.";
        }
        ParamSet Params() const override
        {
            return {{
                {"size","640x480","Image dimension"},
                {"fps","0","Frames per second (0:unspecified)"},
                {"period","0","Specify frame period in microseconds (0:unspecified)"},
                {"num","0","Open the nth device (no need for vid and pid)"},
            }};
        }
        std::unique_ptr<VideoInterface> Open(const Uri& uri) override
        {
            int vendorId = 0;
            int productId = 0;

            const std::string instanceId = uri.url.substr(uri.url.rfind("\\") + 1);
            std::istringstream(uri.url.substr(uri.url.find("vid_", 0) + 4, 4)) >> std::hex >> vendorId;
            std::istringstream(uri.url.substr(uri.url.find("pid_", 0) + 4, 4)) >> std::hex >> productId;
            // const ImageDim dim = uri.Get<ImageDim>("size", ImageDim(640, 480));
            unsigned int fps = uri.Get<unsigned int>("fps", 0);  // 0 means unspecified
            if (fps == 0 && uri.Contains("period")) {
              uint32_t period_us = uri.Get<uint32_t>("period", 0);
              fps = 1000000 / period_us;
            }

            return std::unique_ptr<VideoInterface>(new UvcMediaFoundationVideo(vendorId, productId, instanceId, 640, 480, fps));
        }
    };

    return FactoryRegistry::I()->RegisterFactory<VideoInterface>(std::make_shared<UvcMediaFoundationVideoFactory>());
}
}

