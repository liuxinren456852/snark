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


#include "./rotation_matrix.h"

namespace snark {

/// constructor from rotation matrix    
rotation_matrix::rotation_matrix( const Eigen::Matrix3d& rotation ):
    m_rotation( rotation )
{

}

/// constructor from quaternion
rotation_matrix::rotation_matrix(const Eigen::Quaterniond& quaternion):
    m_rotation( quaternion.normalized() )
{

}

/// constructor from roll pitch yaw angles
rotation_matrix::rotation_matrix( const Eigen::Vector3d& rpy )
    : m_rotation( rotation( rpy ) )
{
}

/// get rotation matrix
const Eigen::Matrix3d& rotation_matrix::rotation() const
{
    return m_rotation;
}

/// convert to quaternion
Eigen::Quaterniond rotation_matrix::quaternion() const
{
    return Eigen::Quaterniond( m_rotation );
}

/// convert to roll pitch yaw
Eigen::Vector3d rotation_matrix::roll_pitch_yaw() const
{
    return roll_pitch_yaw( m_rotation );
}

Eigen::Vector3d rotation_matrix::roll_pitch_yaw( const ::Eigen::Matrix3d& m )
{
    Eigen::Vector3d rpy;
    double roll;
    double pitch=std::asin( -m(2,0) );
    double yaw;
    if( m(2,0)==1 || m(2,0)==-1 )
    {
        roll=0;
        yaw=std::atan2( m(1,2), m(0,2) );
    }
    else
    {
        roll=std::atan2( m(2,1), m(2,2) );
        yaw=std::atan2( m(1,0), m(0,0) );
    }
    rpy << roll,
           pitch,
           yaw;
    return rpy;
}

::Eigen::Matrix3d rotation_matrix::rotation( const ::Eigen::Vector3d& rpy )
{
    return rotation( rpy.x(), rpy.y(), rpy.z() );
}

::Eigen::Matrix3d rotation_matrix::rotation( double roll, double pitch, double yaw )
{
    ::Eigen::Matrix3d m;
    const double sr = std::sin( roll );
    const double cr = std::cos( roll );
    const double sp = std::sin( pitch );
    const double cp = std::cos( pitch );
    const double sy = std::sin( yaw );
    const double cy = std::cos( yaw );
    const double spcy = sp*cy;
    const double spsy = sp*sy;
    m << cp*cy, -cr*sy+sr*spcy,  sr*sy+cr*spcy,
         cp*sy,  cr*cy+sr*spsy, -sr*cy+cr*spsy,
           -sp,          sr*cp,          cr*cp;
    return m;
}

// template< typename Output >
// Output rotation_matrix::convert() const
// {
//     SNARK_THROW( Exception, " conversion to this type not implemented " );
// }

template<>
Eigen::Quaterniond rotation_matrix::convert< Eigen::Quaterniond >() const
{
    return quaternion();
}

template<>
Eigen::Vector3d rotation_matrix::convert< Eigen::Vector3d >() const
{
    return roll_pitch_yaw();
}

} // namespace snark {

