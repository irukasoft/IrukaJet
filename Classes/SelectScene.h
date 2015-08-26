#ifndef __IrukaJet__SelectScene__
#define __IrukaJet__SelectScene__

#include "cocos2d.h"

class SelectScene :public cocos2d::Layer
{
protected:
	SelectScene();
	virtual ~SelectScene();
	bool init() override;

public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(SelectScene);

};

#endif /* defined(__IrukaJet__SelectScene__) */
