#ifndef __IrukaJet__Player__
#define __IrukaJet__Player__

#include "cocos2d.h"

class Player :public cocos2d::Sprite
{
protected:
	bool init() override;
public:
	void update(float dt) override;

	//ジェットの推進力
	CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Vec2, _acceleration, Acceleration);
	//キャラ選択
	CC_SYNTHESIZE_PASS_BY_REF(int, _level, Level);

	CREATE_FUNC(Player);
private:
	cocos2d::Vec2 getAccelerationLimitMax();
	cocos2d::Vec2 getAccelerationLimitMin();
};

#endif /* defined(__IrukaJet__Player__) */
