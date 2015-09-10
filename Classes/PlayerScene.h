#ifndef __IrukaJet__PlayerScene__
#define __IrukaJet__PlayerScene__

#include "cocos2d.h"

class PlayerScene :public cocos2d::Layer
{
protected:
	PlayerScene();
	virtual ~PlayerScene();
	bool init() override;

public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(PlayerScene);
    CC_SYNTHESIZE(int, _level, Level);
private:
    std::string getInfoMessage(int level);
	cocos2d::Vec2 getSelectPlayerVec();
	void saveTouchSelect(cocos2d::Vec2 point);
};

#endif /* defined(__IrukaJet__PlayerScene__) */
