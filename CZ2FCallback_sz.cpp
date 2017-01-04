#include "CZ2FCallback_sz.h"
#include <osgDB/ReadFile>
#include <osg/NodeVisitor>
#include <osg/PositionAttitudeTransform>
#include <osg/BoundingSphere>
#include <iostream>


#if 1
std::vector<std::string> names_sz
{
	//tao yi ta
	"TYT",
	//助推器关机
	"BoosterFl",
	"Booster11",
	"BoosterF9",
	"BoosterF7", 
	//助推器分离
	"Booster1", 
	"Booster2",
	"Booster3", 
	"Booster4",
	//一级关机
	"_1stSta13",
	//一二级分离
	"_1stStage",
	"JJD",
	//二级点火
	"_2ndMainF",
	//整流罩分离
	"Faring129",
	"Faring128",
	"Fairing1",
	"Faring1_S",
	"Faring124",
	"Fairing00", 

	//二级主机关机
	//二级游机关机
	"_2ndYJF19",
	"_2ndYJFla",
	"_2ndYJF18",
	"Plane011",
	//船舰分离
	"_2ndStag",

	//帆板展开
	//最里面数字最大
	"SZSPR5",
	"SZSPR4",
	"SZSPR3",
	"SZSPR2",
	"SZSPR1",

	"SZ133",
	"SZ132", 
	"SZ129", 
	"SZ131", 
	"SZ130", 
};

std::map<std::string, int> nameAdapter_sz
{
	//tao yi ta
	{ "TYT",91 },
	//助推器关机
	{ "BoosterFl", 51 },
	{ "Booster11", 52 },
	{ "BoosterF9", 53 },
	{ "BoosterF7", 54 },
	//助推器分离
	{ "Booster1", 101 },
	{ "Booster2", 102 },
	{ "Booster3", 103 },
	{ "Booster4", 104 },
	//一级关机
	{ "_1stSta13", 61 },
	//一二级分离
	{ "_1stStage", 111 },
	{ "JJD", 112 },
	//二级点火
	{ "_2ndMainF", 41 },
	//整流罩分离
	{ "Faring129", 121 },
	{ "Faring128", 122 },
	{ "Fairing1",123 },
	{"Faring1_S",124},
	{"Faring124",125},
	{"Fairing00",126},

	//二级主机关机
	//二级游机关机
	{ "_2ndYJF19", 81 },
	{ "_2ndYJFla", 82 },
	{ "_2ndYJF18", 83 },
	{ "Plane011" ,84},
	//船舰分离
	{ "_2ndStag", 131 },

	//帆板展开
	//最里面数字最大
	{ "SZSPR5", 1300},
	{ "SZSPR4", 1301},
	{ "SZSPR3", 1302},
	{ "SZSPR2", 1303},
	{ "SZSPR1",1304 },

	{ "SZ133",1305 },
	{ "SZ132",1306 },
	{ "SZ129", 1307},
	{ "SZ131",1308 },
	{ "SZ130", 1309},
	
};

#endif

#if 1

class findNodeByNameVisitor_sz :public osg::NodeVisitor
{
public:

	findNodeByNameVisitor_sz(std::vector<std::string> names) :osg::NodeVisitor(TRAVERSE_ALL_CHILDREN)
	{
		_names = names;
	}

	virtual void apply(osg::Node& node)
	{
		for (int i = 0; i < _names.size(); i++)
		{
			if (node.getName() == _names[i])
			{
				int id = nameAdapter_sz[_names[i]];
				_IDNodes[id] = &node;
				_IDMatrixs[id] = node.asTransform()->asMatrixTransform()->getMatrix();
			}
		}

		traverse(node);
	}

	std::map<int, osg::Node*> & getIDNodes() { return _IDNodes; }
	std::map<int, osg::Matrix> & getIDMatrixs() { return _IDMatrixs; }

private:
	std::vector<std::string> _names;
	std::map<int, osg::Node*>  _IDNodes;
	std::map<int, osg::Matrix> _IDMatrixs;
};

#endif


class CZ2FCallbackVisitor_sz :public osg::NodeVisitor
{
public:
	//pivotPoint 参照点
	CZ2FCallbackVisitor_sz(const RocketAnimationPath::ControlPoint& cp, bool useInverseMatrix, const osg::Matrix& m, const osg::Vec3d& pivotPoint = osg::Vec3d(0.0, 0.0, 0.0))
	{
		_cp = cp;
		_pivotPoint = pivotPoint;
		_useInverseMatrix = useInverseMatrix;
		_m = m;
	}

	virtual void apply(osg::MatrixTransform& mt)
	{
		osg::Matrix matrix;
		if (_useInverseMatrix)
			_cp.getInverse(matrix);
		else
			_cp.getMatrix(matrix);

		mt.setMatrix(osg::Matrix::translate(-_pivotPoint)*matrix*_m);
	}

private:
	RocketAnimationPath::ControlPoint _cp;
	osg::Vec3d _pivotPoint;
	bool _useInverseMatrix;
	osg::Matrix _m;
};

CZ2FCallback_sz::CZ2FCallback_sz(osg::Node* root) :
_timeOffset(0.0),
_timeMultiplier(1.0),
_firstTime(DBL_MAX),
_latestTime(0.0),
_pause(false),
_pauseTime(0.0),
_timeID5(DBL_MAX),
_timeID10(DBL_MAX),
_timeID6(DBL_MAX),
_timeID11(DBL_MAX),
_timeID4(DBL_MAX),
_timeID12(DBL_MAX),
_timeID7(DBL_MAX),
_timeID8(DBL_MAX),
_timeID13(DBL_MAX),
_timeID14(DBL_MAX),
_timeID73(DBL_MAX),
_timeID9(DBL_MAX)
{
	_model = root;
	initIDNodes();
	initPaths();
}



void CZ2FCallback_sz::initIDNodes()
{

	findNodeByNameVisitor_sz finder(names_sz);
	if (_model)
	{
		_model->accept(finder);
	}
	_IDNodes = finder.getIDNodes();
	_IDMatrix = finder.getIDMatrixs();

	osg::Matrix m41 = _IDMatrix[41];
	_IDNodes[41]->asTransform()->asMatrixTransform()->setMatrix(osg::Matrix::scale(0.0, 0.0, 0.0)*m41);

}

void CZ2FCallback_sz::initPaths()
{

#if 1

	_SPath = new RocketAnimationPath;
	_SPath->setLoopMode(RocketAnimationPath::NO_LOOPING);
	_STPath = new RocketAnimationPath;
	_STPath->setLoopMode(RocketAnimationPath::NO_LOOPING);

	_SPath_up = new RocketAnimationPath;
	_SPath_up->setLoopMode(RocketAnimationPath::NO_LOOPING);

	//TG
	_STRPath_L1 = new RocketAnimationPath;
	_STRPath_L1->setLoopMode(RocketAnimationPath::NO_LOOPING);
	_STRPath_L2 = new RocketAnimationPath;
	_STRPath_L2->setLoopMode(RocketAnimationPath::NO_LOOPING);
	_STRPath_L3 = new RocketAnimationPath;
	_STRPath_L3->setLoopMode(RocketAnimationPath::NO_LOOPING);
	_STRPath_L4 = new RocketAnimationPath;
	_STRPath_L4->setLoopMode(RocketAnimationPath::NO_LOOPING);
	_STRPath_L5 = new RocketAnimationPath;
	_STRPath_L5->setLoopMode(RocketAnimationPath::NO_LOOPING);

	_STRPath_R1 = new RocketAnimationPath;
	_STRPath_R1->setLoopMode(RocketAnimationPath::NO_LOOPING);
	_STRPath_R2 = new RocketAnimationPath;
	_STRPath_R2->setLoopMode(RocketAnimationPath::NO_LOOPING);
	_STRPath_R3 = new RocketAnimationPath;
	_STRPath_R3->setLoopMode(RocketAnimationPath::NO_LOOPING);
	_STRPath_R4 = new RocketAnimationPath;
	_STRPath_R4->setLoopMode(RocketAnimationPath::NO_LOOPING);
	_STRPath_R5 = new RocketAnimationPath;
	_STRPath_R5->setLoopMode(RocketAnimationPath::NO_LOOPING);
	//end TG

#endif

	int numPoints = 100;
	float loopTime = 5.0f;
	float scale = 1.0f;
	float scale_delta = -scale / ((float)(numPoints - 1.0f));

	float scale_up = 0.0f;
	float scale_up_delta = scale / ((float)(numPoints - 1.0f));

	float time = 0.0f;
	float time_delta = loopTime / ((float)numPoints);

	//
	float translateZ = 0.0f;;
	float translateDistance = -10.0f;
	float translateZ_delta = translateDistance / ((float)(numPoints - 1.0f));

	//TG

	//帆板高度
	float _height = 9 * 0.82;
	float length = 0.0f;
	float length_delta = _height / ((float)(numPoints - 1.0f));
	float time_TG = 0.0f;
	float time_TG_delta = loopTime / ((float)numPoints);



	float angle = 0.0f;
	//dianshushao jingdudi
	while (angle <= osg::PI_2&&angle >= 0&& length<=_height)
	{
		angle = asin(length / (_height));

		if (angle > osg::PI_2) break;

		osg::Quat rotationR1(-angle, osg::Vec3d(0, 1, 0));
		_STRPath_R1->insert(time_TG, RocketAnimationPath::ControlPoint(rotationR1));

		osg::Quat rotationR2(angle*2.0, osg::Vec3d(0, 1, 0));
		_STRPath_R2->insert(time_TG, RocketAnimationPath::ControlPoint(/*translateR2, */rotationR2));

		osg::Quat rotationR3(2.0*angle, osg::Vec3d(0, 1, 0));
		_STRPath_R3->insert(time_TG, RocketAnimationPath::ControlPoint(rotationR3));

		osg::Quat rotationR4(2.0*angle, osg::Vec3d(0, 1, 0));
		_STRPath_R4->insert(time_TG, RocketAnimationPath::ControlPoint(rotationR4));

		osg::Quat rotationR5(2.0*angle, osg::Vec3d(0, 1, 0));
		_STRPath_R5->insert(time_TG, RocketAnimationPath::ControlPoint(rotationR5));

		osg::Quat rotationL1(-angle, osg::Vec3d(0, 1, 0));
		_STRPath_L1->insert(time_TG, RocketAnimationPath::ControlPoint(rotationL1));

		osg::Quat rotationL2(angle*2.0, osg::Vec3d(0, 1, 0));
		_STRPath_L2->insert(time_TG, RocketAnimationPath::ControlPoint(rotationL2));

		osg::Quat rotationL3(angle*2.0, osg::Vec3d(0, 1, 0));
		_STRPath_L3->insert(time_TG, RocketAnimationPath::ControlPoint( rotationL3));

		osg::Quat rotationL4(angle*2.0, osg::Vec3d(0, 1, 0));
		_STRPath_L4->insert(time_TG, RocketAnimationPath::ControlPoint(rotationL4));

		osg::Quat rotationL5(angle*2.0, osg::Vec3d(0, 1, 0));
		_STRPath_L5->insert(time_TG, RocketAnimationPath::ControlPoint(rotationL5));

		time_TG += time_TG_delta;
		length += length_delta;

	}
	//END TG

#if 1

	//整流罩
	_STRPath_F1 = new RocketAnimationPath;
	_STRPath_F1->setLoopMode(RocketAnimationPath::NO_LOOPING);
	_STRPath_F2 = new RocketAnimationPath;
	_STRPath_F2->setLoopMode(RocketAnimationPath::NO_LOOPING);

	float x_fair = 0.0f;
	float x = 5.0f;
	float x_fair_delta = x / ((float)numPoints);

	float angle_all = osg::PI_2;
	float angle_delta = angle_all / ((float)numPoints);
	float angle_fair = 0.0;


	for (int i = 0; i < numPoints; i++)
	{
		osg::Vec3d scaleV(scale, scale, scale);
		_SPath->insert(time, RocketAnimationPath::ControlPoint(scaleV));

		osg::Vec3d translate(0.0, 0.0, translateZ);
		_STPath->insert(time, RocketAnimationPath::ControlPoint(translate, scaleV));

		osg::Vec3d scaleUp(scale_up, scale_up, scale_up);
		_SPath_up->insert(time, RocketAnimationPath::ControlPoint(scaleUp));

		//右
		osg::Vec3d translate_fair1(x_fair, 0, translateZ);
		osg::Quat rotation_fair1(angle_fair, osg::Vec3d(0, 1, 0));
		_STRPath_F1->insert(time, RocketAnimationPath::ControlPoint(translate_fair1, rotation_fair1, scaleV));

		//左
		osg::Vec3d translate_fair2(-x_fair, 0, translateZ);
		osg::Quat rotation_fair2(-angle_fair, osg::Vec3d(0, 1, 0));
		_STRPath_F2->insert(time, RocketAnimationPath::ControlPoint(translate_fair2, rotation_fair2, scaleV));


		scale += scale_delta;
		translateZ += translateZ_delta;
		scale_up += scale_up_delta;
		time += time_delta;

		x_fair += x_fair_delta;
		angle_fair += angle_delta;
	}

#endif
}


void CZ2FCallback_sz::setStep(int step)
{
	_step = step;
}

void CZ2FCallback_sz::reset()
{
#if 1
	_firstTime = DBL_MAX;
	_pauseTime = DBL_MAX;
#else
	_firstTime = _latestTime;
	_pauseTime = _latestTime;
#endif
}

void CZ2FCallback_sz::setPause(bool pause)
{
	if (_pause == pause)
	{
		return;
	}

	_pause = pause;

	if (_firstTime == DBL_MAX) return;

	if (_pause)
	{
		_pauseTime = _latestTime;
	}
	else
	{
		_firstTime += (_latestTime - _pauseTime);
	}
}

double CZ2FCallback_sz::getAnimationTime() const
{
	return ((_latestTime - _firstTime) - _timeOffset)*_timeMultiplier;
}


double CZ2FCallback_sz::getAnimationTime(double referenceTime) const
{
	return ((_latestTime - _firstTime) - _timeOffset)*_timeMultiplier;
}


void CZ2FCallback_sz::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	if (/*_SPath.valid() &&*/
		nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR &&
		nv->getFrameStamp())
	{
		double time = nv->getFrameStamp()->getSimulationTime();
		_latestTime = time;

		if (!_pause)
		{
			// Only update _firstTime the first time, when its value is still DBL_MAX
			if (_firstTime == DBL_MAX) _firstTime = time;
			update();
		}
	}

	// must call any nested node callbacks and continue subgraph traversal.
	NodeCallback::traverse(node, nv);
}

void CZ2FCallback_sz::update()
{
	switch (_step)
	{

#if 1
	case 9:
	{
		if (_SPath.valid())
		{
			if (_timeID9 == DBL_MAX) _timeID9 = getAnimationTime();
			RocketAnimationPath::ControlPoint cp;
			if (_SPath->getInterpolatedControlPoint(getAnimationTime() - _timeID9, cp))
				{
					CZ2FCallbackVisitor_sz czcv(cp, false, _IDMatrix[91]);
					_IDNodes[91]->accept(czcv);
				}
		}
	break;
	}
	case 5:
	{
		if (_SPath.valid())
		{
			if (_timeID5 == DBL_MAX) _timeID5 = getAnimationTime();
			RocketAnimationPath::ControlPoint cp;
			if (_SPath->getInterpolatedControlPoint(getAnimationTime() - _timeID5, cp))
			{
				CZ2FCallbackVisitor_sz czcv(cp, false, _IDMatrix[51]);
				_IDNodes[51]->accept(czcv);
				_IDNodes[52]->accept(czcv);
				_IDNodes[53]->accept(czcv);
				_IDNodes[54]->accept(czcv);
			}
		}
		break;
	}
	case 10:
	{
		if (_STPath.valid())
		{
			if (_timeID10 == DBL_MAX) _timeID10 = getAnimationTime();
			RocketAnimationPath::ControlPoint cp;
			if (_STPath->getInterpolatedControlPoint(getAnimationTime() - _timeID10, cp))
			{
				CZ2FCallbackVisitor_sz czcv1(cp, false, _IDMatrix[101]);
				CZ2FCallbackVisitor_sz czcv2(cp, false, _IDMatrix[102]);
				CZ2FCallbackVisitor_sz czcv3(cp, false, _IDMatrix[103]);
				CZ2FCallbackVisitor_sz czcv4(cp, false, _IDMatrix[104]);
				_IDNodes[101]->accept(czcv1);
				_IDNodes[102]->accept(czcv2);
				_IDNodes[103]->accept(czcv3);
				_IDNodes[104]->accept(czcv4);
			}
		}
		break;
	}
	case 6:
	{
		if (_SPath.valid())
		{
			if (_timeID6 == DBL_MAX) _timeID6 = getAnimationTime();
			RocketAnimationPath::ControlPoint cp;
			if (_SPath->getInterpolatedControlPoint(getAnimationTime() - _timeID6, cp))
			{
				CZ2FCallbackVisitor_sz czcv(cp, false, _IDMatrix[61]);
				_IDNodes[61]->accept(czcv);
			}
		}
		break;
	}
	case 11:
	{
		if (_STPath.valid())
		{
			if (_timeID11 == DBL_MAX) _timeID11 = getAnimationTime();
			RocketAnimationPath::ControlPoint cp;
			if (_SPath->getInterpolatedControlPoint(getAnimationTime() - _timeID11, cp))
			{
				CZ2FCallbackVisitor_sz czcv(cp, false, _IDMatrix[111]);
				_IDNodes[111]->accept(czcv);
				_IDNodes[112]->accept(czcv);
			}
		}
		break;
	}
	case 4:
	{
		if (_SPath_up.valid())
		{
			if (_timeID4 == DBL_MAX) _timeID4 = getAnimationTime();
			RocketAnimationPath::ControlPoint cp;
			if (_SPath_up->getInterpolatedControlPoint(getAnimationTime() - _timeID4, cp))
			{
				CZ2FCallbackVisitor_sz czcv(cp, false, _IDMatrix[41]);
				_IDNodes[41]->accept(czcv);
			}
		}
		break;
	}
		/////整流罩
	case 12:
	{
		if (_STRPath_F1.valid() && _STRPath_F2.valid())
		{
			if (_timeID12 == DBL_MAX) _timeID12 = getAnimationTime();
			RocketAnimationPath::ControlPoint cp1;
			RocketAnimationPath::ControlPoint cp2;
			if (_STRPath_F1->getInterpolatedControlPoint(getAnimationTime() - _timeID12, cp1)
				&& _STRPath_F2->getInterpolatedControlPoint(getAnimationTime() - _timeID12, cp2)
				)
			{
				CZ2FCallbackVisitor_sz czcv1(cp1, false, _IDMatrix[121]);
				CZ2FCallbackVisitor_sz czcv2(cp1, false, _IDMatrix[122]);
				CZ2FCallbackVisitor_sz czcv3(cp1, false, _IDMatrix[123]);

				CZ2FCallbackVisitor_sz czcv4(cp2, false, _IDMatrix[124]);
				CZ2FCallbackVisitor_sz czcv5(cp2, false, _IDMatrix[125]);
				CZ2FCallbackVisitor_sz czcv6(cp2, false, _IDMatrix[126]);

				_IDNodes[121]->accept(czcv1);
				_IDNodes[122]->accept(czcv2);
				_IDNodes[123]->accept(czcv3);
				_IDNodes[124]->accept(czcv4);
				_IDNodes[125]->accept(czcv5);
				_IDNodes[126]->accept(czcv6);

			}

		}
		break;
	}
	case 7:
	{
		if (_SPath.valid())
		{
			if (_timeID7 == DBL_MAX) _timeID7 = getAnimationTime();
			RocketAnimationPath::ControlPoint cp;
			if (_SPath->getInterpolatedControlPoint(getAnimationTime() - _timeID7, cp))
			{
				CZ2FCallbackVisitor_sz czcv(cp, false, _IDMatrix[41]);
				_IDNodes[41]->accept(czcv);
			}
		}
		break;
	}
	case 8:
	{
		if (_SPath.valid())
		{
			if (_timeID8 == DBL_MAX) _timeID8 = getAnimationTime();
			RocketAnimationPath::ControlPoint cp;
			if (_SPath->getInterpolatedControlPoint(getAnimationTime() - _timeID8, cp))
			{
				CZ2FCallbackVisitor_sz czcv1(cp, false, _IDMatrix[81]);
				CZ2FCallbackVisitor_sz czcv2(cp, false, _IDMatrix[82]);
				CZ2FCallbackVisitor_sz czcv3(cp, false, _IDMatrix[83]);
				CZ2FCallbackVisitor_sz czcv4(cp, false, _IDMatrix[84]);
				_IDNodes[81]->accept(czcv1);
				_IDNodes[82]->accept(czcv2);
				_IDNodes[83]->accept(czcv3);
				_IDNodes[84]->accept(czcv4);
			}
		}
		break;
	}
	case 13:
	{
		if (_STPath.valid())
		{
			if (_timeID13 == DBL_MAX) _timeID13 = getAnimationTime();
			RocketAnimationPath::ControlPoint cp;
			if (_STPath->getInterpolatedControlPoint(getAnimationTime() - _timeID13, cp))
			{
				CZ2FCallbackVisitor_sz czcv(cp, false, _IDMatrix[131]);
				_IDNodes[131]->accept(czcv);
			}
		}
		break;
	}

#endif
		//左翻板

		///////youwenti
    case 44:
	{
		if (_STRPath_R1.valid() && _STRPath_R2.valid())
		{
			if (_timeID14 == DBL_MAX)
			{
				_timeID14 = getAnimationTime();

			}
			RocketAnimationPath::ControlPoint cpR1;
			RocketAnimationPath::ControlPoint cpR2;
			RocketAnimationPath::ControlPoint cpR3;
			RocketAnimationPath::ControlPoint cpR4;
			RocketAnimationPath::ControlPoint cpR5;

			if (_STRPath_R1->getInterpolatedControlPoint(getAnimationTime() - _timeID14, cpR1)
				&& _STRPath_R2->getInterpolatedControlPoint(getAnimationTime() - _timeID14, cpR2)
				&& _STRPath_R3->getInterpolatedControlPoint(getAnimationTime() - _timeID14, cpR3)
				&& _STRPath_R4->getInterpolatedControlPoint(getAnimationTime() - _timeID14, cpR4)
				&& _STRPath_R5->getInterpolatedControlPoint(getAnimationTime() - _timeID14, cpR5))
			{
				CZ2FCallbackVisitor_sz czcvR1(cpR1, false, _IDMatrix[1300]);
				_IDNodes[1300]->accept(czcvR1);
				CZ2FCallbackVisitor_sz czcvR2(cpR2, false, _IDMatrix[1301]);
				_IDNodes[1301]->accept(czcvR2);
				CZ2FCallbackVisitor_sz czcvR3(cpR3, false, _IDMatrix[1302]);
				_IDNodes[1302]->accept(czcvR3);
				CZ2FCallbackVisitor_sz czcvR4(cpR4, false, _IDMatrix[1303]);
				_IDNodes[1303]->accept(czcvR4);
				CZ2FCallbackVisitor_sz czcvR5(cpR5, false, _IDMatrix[1304]);
				_IDNodes[1304]->accept(czcvR5);

			}
		}
//        break;
	}
		//右翻板
    case 45:
	{
		if (_STRPath_L1.valid() && _STRPath_L2.valid())
		{
			if (_timeID73 == DBL_MAX) _timeID73 = getAnimationTime();
			{
				RocketAnimationPath::ControlPoint cpL1;
				RocketAnimationPath::ControlPoint cpL2;
				RocketAnimationPath::ControlPoint cpL3;
				RocketAnimationPath::ControlPoint cpL4;
				RocketAnimationPath::ControlPoint cpL5;
				if (_STRPath_L1->getInterpolatedControlPoint(getAnimationTime() - _timeID73, cpL1)
					&& _STRPath_L2->getInterpolatedControlPoint(getAnimationTime() - _timeID73, cpL2)
					&& _STRPath_L3->getInterpolatedControlPoint(getAnimationTime() - _timeID73, cpL3)
					&& _STRPath_L4->getInterpolatedControlPoint(getAnimationTime() - _timeID73, cpL4)
					&& _STRPath_L5->getInterpolatedControlPoint(getAnimationTime() - _timeID73, cpL5)
					)
				{
					CZ2FCallbackVisitor_sz czcvL1(cpL1, false, _IDMatrix[1305]);
					_IDNodes[1305]->accept(czcvL1);
					CZ2FCallbackVisitor_sz czcvL2(cpL2, false, _IDMatrix[1306]);
					_IDNodes[1306]->accept(czcvL2);
					CZ2FCallbackVisitor_sz czcvL3(cpL3, false, _IDMatrix[1307]);
					_IDNodes[1307]->accept(czcvL3);
					CZ2FCallbackVisitor_sz czcvL4(cpL4, false, _IDMatrix[1308]);
					_IDNodes[1308]->accept(czcvL4);
					CZ2FCallbackVisitor_sz czcvL5(cpL5, false, _IDMatrix[1309]);
					_IDNodes[1309]->accept(czcvL5);
				}

			}
		}
	}
		break;


	default:
		reset();
		break;
	}

}
