#ifndef GX2FCALLBACK_SZ_H
#define GX2FCALLBACK_SZ_H

#include "RocketAnimationPath.h"
#include <osg/NodeCallback>
#include <osg/MatrixTransform>
#include <osg/ref_ptr>
#include <iostream>




class  CZ2FCallback_sz : public osg::NodeCallback
{
public:
	CZ2FCallback_sz(osg::Node* root);

	//传送时间码
	void setStep(int step);

	void setTimeOffset(double offset) { _timeOffset = offset; }
	
	
	double getTimeOffset() const { return _timeOffset; }

	void setTimeMultiplier(double multiplier) { _timeMultiplier = multiplier; }
	double getTimeMultiplier() const { return _timeMultiplier; }

	virtual void reset();

	void setPause(bool pause);
	bool getPause() const { return _pause; }

	/** Get the animation time that is used to specify the position along
	* the AnimationPath. Animation time is computed from the formula:
	*   ((_latestTime-_firstTime)-_timeOffset)*_timeMultiplier.*/
	virtual double getAnimationTime() const;

	virtual double getAnimationTime(double referenceTime) const;

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

protected:
	void initIDNodes();
	void initPaths();
	void update();


private:
	osg::Node* _model;
	std::map<int, osg::Node*> _IDNodes;
	std::map<int, osg::Matrix> _IDMatrix;

	osg::ref_ptr<RocketAnimationPath> _SPath;
	osg::ref_ptr<RocketAnimationPath> _SPath_up;
	osg::ref_ptr<RocketAnimationPath> _STPath;

	//整流罩
	osg::ref_ptr<RocketAnimationPath> _STRPath_F1;
	osg::ref_ptr<RocketAnimationPath> _STRPath_F2;


	osg::ref_ptr<RocketAnimationPath> _STRPath_L1;
	osg::ref_ptr<RocketAnimationPath> _STRPath_L2;
	osg::ref_ptr<RocketAnimationPath> _STRPath_L3;
	osg::ref_ptr<RocketAnimationPath> _STRPath_L4;
	osg::ref_ptr<RocketAnimationPath> _STRPath_L5;

	osg::ref_ptr<RocketAnimationPath> _STRPath_R1;
	osg::ref_ptr<RocketAnimationPath> _STRPath_R2;
	osg::ref_ptr<RocketAnimationPath> _STRPath_R3;
	osg::ref_ptr<RocketAnimationPath> _STRPath_R4;
	osg::ref_ptr<RocketAnimationPath> _STRPath_R5;

	osg::Vec3d _pivotPoint;


	int _step;

public:
	double                  _timeOffset;
	double                  _timeMultiplier;
	double                  _firstTime;
	double                  _latestTime;
	bool                    _pause;
	double                  _pauseTime;

	//各阶段的相对时间
	double _timeID5;
	double _timeID10;
	double _timeID6;
	double _timeID11;
	double _timeID4;
	double _timeID8;
	double _timeID12;
	double _timeID7;
	double _timeID13;

	//TG
	double _timeID14;
	double _timeID73;

	//SZ
	double _timeID9;


};

#endif
