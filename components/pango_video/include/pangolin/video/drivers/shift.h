/* This file is part of the Pangolin Project.
 * http://github.com/stevenlovegrove/Pangolin
 *
 * Copyright (c) 2014 Steven Lovegrove
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <pangolin/video/video_interface.h>
#include <set>

namespace pangolin
{

// Video class that debayers its video input using the given method.
class PANGOLIN_EXPORT ShiftVideo : public VideoInterface, public VideoFilterInterface
{
public:
    ShiftVideo(std::unique_ptr<VideoInterface>& videoin,
               const std::map<size_t, int>& shift_right_bits,
               const std::map<size_t, uint32_t>& masks);
    ~ShiftVideo();

    //! Implement VideoInput::Start()
    void Start();

    //! Implement VideoInput::Stop()
    void Stop();

    //! Implement VideoInput::SizeBytes()
    size_t SizeBytes() const;

    //! Implement VideoInput::Streams()
    const std::vector<StreamInfo>& Streams() const;

    //! Implement VideoInput::GrabNext()
    bool GrabNext( uint8_t* image, bool wait = true );

    //! Implement VideoInput::GrabNewest()
    bool GrabNewest( uint8_t* image, bool wait = true );

    std::vector<VideoInterface*>& InputStreams();

protected:
    void Process(uint8_t* buffer_out, const uint8_t* buffer_in);

    std::unique_ptr<VideoInterface> src;
    std::vector<VideoInterface*> videoin;
    std::vector<StreamInfo> streams;
    size_t size_bytes;
    std::unique_ptr<uint8_t[]> buffer;
    const std::map<size_t, int> shift_right_bits;
    const std::map<size_t, uint32_t> masks;
    std::set<std::string> formats_supported;
};

}