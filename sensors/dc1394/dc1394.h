// This file is part of snark, a generic and flexible library for robotics research
// Copyright (c) 2011 The University of Sydney
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of the University of Sydney nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
// GRANTED BY THIS LICENSE.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
// HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
// IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#ifndef SNARK_SENSORS_DC1394_H_
#define SNARK_SENSORS_DC1394_H_

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/optional.hpp>
#include <dc1394/dc1394.h>

#include <opencv2/core/core.hpp>
#include <opencv2/core/core_c.h>
#include <comma/io/select.h>
#include <comma/visiting/traits.h>
#include <snark/sensors/dc1394/types.h>

namespace snark { namespace camera {

/// image acquisition from dc1394 camera
class dc1394
{
public:

    struct config
    {
        config();
        
        dc1394video_mode_t  video_mode;
        dc1394operation_mode_t operation_mode;
        dc1394speed_t iso_speed;
        // TODO framerate is not used in format7, as the way to set the framerate is different.
        // see http://damien.douxchamps.net/ieee1394/libdc1394/v2.x/faq/#How_do_I_set_the_frame_rate
        dc1394framerate_t frame_rate;
        unsigned int relative_shutter;
        unsigned int relative_gain;
        float shutter; // 0 means do not change
        float gain;
        unsigned int exposure;
        uint64_t guid;

        unsigned int format7_left;
        unsigned int format7_top;
        unsigned int format7_width;
        unsigned int format7_height;
        unsigned int format7_packet_size;
        dc1394color_coding_t format7_color_coding;

        bool deinterlace;
    };

    
    dc1394( const config& config = config() );
    ~dc1394();

    const cv::Mat& read();
    boost::posix_time::ptime time() const { return m_time; }
    bool poll();
    static void list_cameras();
    void list_attributes();

private:
    void init_camera();
    void setup_camera();
    void setup_camera_format7();

    void set_absolute_shutter_gain( float shutter, float gain );
    void set_relative_shutter_gain( unsigned int shutter, unsigned int gain );
    void set_exposure( unsigned int exposure );

    config m_config;
    
    dc1394camera_t* m_camera;
    dc1394video_frame_t* m_frame;
    
    dc1394operation_mode_t m_operation_mode;
    dc1394speed_t m_iso_speed;
    dc1394video_mode_t m_video_mode;
    dc1394framerate_t m_framerate;
    dc1394color_coding_t m_color_coding;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_top;
    unsigned int m_left;
    unsigned int m_packet_size;
    
    cv::Mat m_image;
    const boost::posix_time::ptime m_epoch;
    boost::posix_time::ptime m_time;
    int m_fd;
    comma::io::select m_select;
    boost::posix_time::time_duration m_frame_duration;
};

} } // namespace snark { namespace camera {

namespace comma { namespace visiting {

template <> struct traits< snark::camera::dc1394::config >
{
    template < typename Key, class Visitor >
    static void visit( const Key&, snark::camera::dc1394::config& c, Visitor& v )
    {
        std::string video_mode;
        std::string operation_mode;
        std::string iso_speed;
        std::string frame_rate;
        std::string color_coding="DC1394_COLOR_CODING_MONO8";
        v.apply( "video-mode", video_mode );
        v.apply( "operation-mode", operation_mode );
        v.apply( "iso-speed", iso_speed );
        v.apply( "frame-rate", frame_rate );
        v.apply( "color-coding", color_coding );
        v.apply( "left", c.format7_left );
        v.apply( "top", c.format7_top );
        v.apply( "width", c.format7_width );
        v.apply( "height", c.format7_height );
        v.apply( "packet-size", c.format7_packet_size );

        c.video_mode = snark::camera::video_mode_from_string( video_mode );
        c.operation_mode = snark::camera::operation_mode_from_string( operation_mode );
        c.iso_speed = snark::camera::iso_speed_from_string( iso_speed );
        c.frame_rate = snark::camera::frame_rate_from_string( frame_rate );
        c.format7_color_coding = snark::camera::color_coding_from_string( color_coding );

        v.apply( "relative-shutter", c.relative_shutter );
        v.apply( "relative-gain", c.relative_gain );
        v.apply( "shutter", c.shutter );
        v.apply( "gain", c.gain );
        v.apply( "exposure", c.exposure );
        v.apply( "guid", c.guid );
        v.apply( "deinterlace", c.deinterlace );
    }

    template < typename Key, class Visitor >
    static void visit( const Key&, const snark::camera::dc1394::config& c, Visitor& v )
    {
        std::string video_mode = snark::camera::video_mode_to_string( c.video_mode );
        std::string operation_mode = snark::camera::operation_mode_to_string( c.operation_mode );
        std::string iso_speed = snark::camera::iso_speed_to_string( c.iso_speed );
        std::string frame_rate = snark::camera::frame_rate_to_string( c.frame_rate );
        std::string color_coding = snark::camera::color_coding_to_string( c.format7_color_coding );

        v.apply( "video-mode", video_mode );
        v.apply( "operation-mode", operation_mode );
        v.apply( "iso-speed", iso_speed );
        v.apply( "frame-rate", frame_rate );
        v.apply( "color-coding", color_coding );
        v.apply( "left", c.format7_left );
        v.apply( "top", c.format7_top );
        v.apply( "width", c.format7_width );
        v.apply( "height", c.format7_height );
        v.apply( "packet-size", c.format7_packet_size );

        v.apply( "relative-shutter", c.relative_shutter );
        v.apply( "relative-gain", c.relative_gain );
        v.apply( "shutter", c.shutter );
        v.apply( "gain", c.gain );
        v.apply( "exposure", c.exposure );
        v.apply( "guid", c.guid );
        v.apply( "deinterlace", c.deinterlace );
    }
};

} }

#endif // SNARK_SENSORS_DC1394_H_
