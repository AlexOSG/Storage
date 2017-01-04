#ifndef GXROCKETANIMATIONPATH_H
#define GXROCKETANIMATIONPATH_H
#include <map>
#include <istream>
#include <float.h>

#include <osg/Matrixf>
#include <osg/Matrixd>
#include <osg/Quat>
#include <osg/Callback>
#include <osg/CopyOp>


class  RocketAnimationPath : public virtual osg::Object
{
public:

    RocketAnimationPath() :_loopMode(LOOP) {}

    RocketAnimationPath(const RocketAnimationPath& ap, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY) :
        Object(ap, copyop),
        _timeControlPointMap(ap._timeControlPointMap),
        _loopMode(ap._loopMode) {}

    META_Object(osg, RocketAnimationPath);

    class ControlPoint
    {
    public:
        ControlPoint() :
            _scale(1.0, 1.0, 1.0) {}

        ControlPoint(const osg::Vec3d& scale):
            _scale(scale){}

        ControlPoint(const osg::Quat& rotation):
            _rotation(rotation),
            _scale(1.0, 1.0, 1.0) {}

        ControlPoint(const osg::Vec3d& position, const osg::Quat& rotation) :
            _position(position),
            _rotation(rotation),
            _scale(1.0, 1.0, 1.0) {}

        ControlPoint(const osg::Vec3d& position, const osg::Vec3d& scale) :
            _position(position),
            _rotation(),
            _scale(scale) {}

        ControlPoint(const osg::Vec3d& position, const osg::Quat& rotation, const osg::Vec3d& scale) :
            _position(position),
            _rotation(rotation),
            _scale(scale) {}

        void setPosition(const osg::Vec3d& position) { _position = position; }
        const osg::Vec3d& getPosition() const { return _position; }

        void setRotation(const osg::Quat& rotation) { _rotation = rotation; }
        const osg::Quat& getRotation() const { return _rotation; }

        void setScale(const osg::Vec3d& scale) { _scale = scale; }
        const osg::Vec3d& getScale() const { return _scale; }

        inline void interpolate(float ratio, const ControlPoint& first, const ControlPoint& second)
        {
            float one_minus_ratio = 1.0f - ratio;
            _position = first._position*one_minus_ratio + second._position*ratio;
            _rotation.slerp(ratio, first._rotation, second._rotation);
            _scale = first._scale*one_minus_ratio + second._scale*ratio;
        }

        inline void interpolate(double ratio, const ControlPoint& first, const ControlPoint& second)
        {
            double one_minus_ratio = 1.0 - ratio;
            _position = first._position*one_minus_ratio + second._position*ratio;
            _rotation.slerp(ratio, first._rotation, second._rotation);
            _scale = first._scale*one_minus_ratio + second._scale*ratio;
        }

        inline void getMatrix(osg::Matrixf& matrix) const
        {
            matrix.makeRotate(_rotation);
            matrix.preMultScale(_scale);
            matrix.postMultTranslate(_position);
        }

        inline void getMatrix(osg::Matrixd& matrix) const
        {
            matrix.makeRotate(_rotation);
            matrix.preMultScale(_scale);
            matrix.postMultTranslate(_position);
        }

        inline void getInverse(osg::Matrixf& matrix) const
        {
            matrix.makeRotate(_rotation.inverse());
            matrix.postMultScale(osg::Vec3d(1.0 / _scale.x(), 1.0 / _scale.y(), 1.0 / _scale.z()));
            matrix.preMultTranslate(-_position);
        }

        inline void getInverse(osg::Matrixd& matrix) const
        {
            matrix.makeRotate(_rotation.inverse());
            matrix.postMultScale(osg::Vec3d(1.0 / _scale.x(), 1.0 / _scale.y(), 1.0 / _scale.z()));
            matrix.preMultTranslate(-_position);
        }

    protected:

        osg::Vec3d _position;
        osg::Quat _rotation;
        osg::Vec3d _scale;

    };


    /** Given a specific time, return the transformation matrix for a point. */
    bool getMatrix(double time, osg::Matrixf& matrix) const
    {
        ControlPoint cp;
        if (!getInterpolatedControlPoint(time, cp)) return false;
        cp.getMatrix(matrix);
        return true;
    }

    /** Given a specific time, return the transformation matrix for a point. */
    bool getMatrix(double time, osg::Matrixd& matrix) const
    {
        ControlPoint cp;
        if (!getInterpolatedControlPoint(time, cp)) return false;
        cp.getMatrix(matrix);
        return true;
    }

    /** Given a specific time, return the inverse transformation matrix for a point. */
    bool getInverse(double time, osg::Matrixf& matrix) const
    {
        ControlPoint cp;
        if (!getInterpolatedControlPoint(time, cp)) return false;
        cp.getInverse(matrix);
        return true;
    }

    bool getInverse(double time, osg::Matrixd& matrix) const
    {
        ControlPoint cp;
        if (!getInterpolatedControlPoint(time, cp)) return false;
        cp.getInverse(matrix);
        return true;
    }

    /** Given a specific time, return the local ControlPoint frame for a point. */
    virtual bool getInterpolatedControlPoint(double time, ControlPoint& controlPoint) const;

    /** Insert a control point into the RocketAnimationPath.*/
    void insert(double time, const ControlPoint& controlPoint);

    double getFirstTime() const { if (!_timeControlPointMap.empty()) return _timeControlPointMap.begin()->first; else return 0.0; }
    double getLastTime() const { if (!_timeControlPointMap.empty()) return _timeControlPointMap.rbegin()->first; else return 0.0; }
    double getPeriod() const { return getLastTime() - getFirstTime(); }

    enum LoopMode
    {
        SWING,
        LOOP,
        NO_LOOPING
    };

    void setLoopMode(LoopMode lm) { _loopMode = lm; }

    LoopMode getLoopMode() const { return _loopMode; }


    typedef std::map<double, ControlPoint> TimeControlPointMap;

    void setTimeControlPointMap(TimeControlPointMap& tcpm) { _timeControlPointMap = tcpm; }

    TimeControlPointMap& getTimeControlPointMap() { return _timeControlPointMap; }

    const TimeControlPointMap& getTimeControlPointMap() const { return _timeControlPointMap; }

    bool empty() const { return _timeControlPointMap.empty(); }

    void clear() { _timeControlPointMap.clear(); }

    /** Read the animation path from a flat ASCII file stream. */
    void read(std::istream& in);

    /** Write the animation path to a flat ASCII file stream. */
    void write(std::ostream& out) const;

    /** Write the control point to a flat ASCII file stream. */
    void write(TimeControlPointMap::const_iterator itr, std::ostream& out) const;

protected:

    virtual ~RocketAnimationPath() {}

    TimeControlPointMap _timeControlPointMap;
    LoopMode            _loopMode;

};

#endif
