#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Camera>
#include <osg/CameraView>
#include <osg/io_utils>
#include "RocketAnimationPath.h"


void RocketAnimationPath::insert(double time, const ControlPoint& controlPoint)
{
    _timeControlPointMap[time] = controlPoint;
}

bool RocketAnimationPath::getInterpolatedControlPoint(double time, ControlPoint& controlPoint) const
{
    if (_timeControlPointMap.empty()) return false;

    switch (_loopMode)
    {
    case(SWING) :
    {
        double modulated_time = (time - getFirstTime()) / (getPeriod()*2.0);
        double fraction_part = modulated_time - floor(modulated_time);
        if (fraction_part>0.5) fraction_part = 1.0 - fraction_part;

        time = getFirstTime() + (fraction_part*2.0) * getPeriod();
        break;
    }
    case(LOOP) :
    {
        double modulated_time = (time - getFirstTime()) / getPeriod();
        double fraction_part = modulated_time - floor(modulated_time);
        time = getFirstTime() + fraction_part * getPeriod();
        break;
    }
    case(NO_LOOPING) :
        // no need to modulate the time.
        break;
    }



    TimeControlPointMap::const_iterator second = _timeControlPointMap.lower_bound(time);
    if (second == _timeControlPointMap.begin())
    {
        controlPoint = second->second;
    }
    else if (second != _timeControlPointMap.end())
    {
        TimeControlPointMap::const_iterator first = second;
        --first;

        // we have both a lower bound and the next item.

        // delta_time = second.time - first.time
        double delta_time = second->first - first->first;

        if (delta_time == 0.0)
            controlPoint = first->second;
        else
        {
            controlPoint.interpolate((time - first->first) / delta_time,
                first->second,
                second->second);
        }
    }
    else // (second==_timeControlPointMap.end())
    {
        controlPoint = _timeControlPointMap.rbegin()->second;
    }
    return true;
}


void RocketAnimationPath::read(std::istream& in)
{
    while (!in.eof())
    {
        double time;
        osg::Vec3d position;
        osg::Quat rotation;
        in >> time >> position.x() >> position.y() >> position.z() >> rotation.x() >> rotation.y() >> rotation.z() >> rotation.w();
        if (!in.eof())
            insert(time, RocketAnimationPath::ControlPoint(position, rotation));
    }
}

void RocketAnimationPath::write(TimeControlPointMap::const_iterator itr, std::ostream& fout) const
{
    const ControlPoint& cp = itr->second;
    fout << itr->first << " " << cp.getPosition() << " " << cp.getRotation() << std::endl;
}

void RocketAnimationPath::write(std::ostream& fout) const
{
    int prec = fout.precision();
    fout.precision(15);

    const TimeControlPointMap& tcpm = getTimeControlPointMap();
    for (TimeControlPointMap::const_iterator tcpmitr = tcpm.begin();
        tcpmitr != tcpm.end();
        ++tcpmitr)
    {
        write(tcpmitr, fout);
    }

    fout.precision(prec);
}
