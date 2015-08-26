#ifndef __IrukaJet__InfoScene__
#define __IrukaJet__InfoScene__

#include "cocos2d.h"

class InfoScene :public cocos2d::Layer
{
protected:
	InfoScene();
	virtual ~InfoScene();
	bool init() override;

public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(InfoScene);

};

#endif /* defined(__IrukaJet__InfoScene__) */
