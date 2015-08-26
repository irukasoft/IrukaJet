#ifndef __IrukaJet__TitleScene__
#define __IrukaJet__TitleScene__

#include "cocos2d.h"
//#include <random>

class TitleScene :public cocos2d::Layer
{
protected:
	TitleScene();
	virtual ~TitleScene();
	bool init() override;

public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(TitleScene);

};

#endif /* defined(__IrukaJet__TitleScene__) */
