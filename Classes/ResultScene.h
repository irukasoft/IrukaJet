#ifndef __IrukaJet__ResultScene__
#define __IrukaJet__ResultScene__

#include "cocos2d.h"

class ResultScene :public cocos2d::Layer
{
protected:
	ResultScene();
	virtual ~ResultScene();
	bool init() override;

public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(ResultScene);

};

#endif /* defined(__IrukaJet__ResultScene__) */
