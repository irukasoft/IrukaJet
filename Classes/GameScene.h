#ifndef __IrukaJet__GameScene__
#define __IrukaJet__GameScene__

#include "Stage.h"
#include "cocos2d.h"

class GameScene :public cocos2d::Layer
{
protected:
	GameScene();
	virtual ~GameScene();
	//bool init() override;
	bool initWithLevel(int level);

public:
	/**
	 * ステージ番号からステージを作成する
	 */
	static cocos2d::Scene * createWithLevel(int level);
    //static cocos2d::Scene* createScene();
	void update(float dt) override;
	//画面がタップされているか
	CC_SYNTHESIZE(bool, _isPress,IsPress);
	//タッチされてるポイント
	CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Vec2, _touchPoint,TouchPoint);
	//現在の取得コイン数
	//CC_SYNTHESIZE(int, _coin, Coin);
	//ステージ
	CC_SYNTHESIZE_RETAIN(Stage *, _stage, Stage);
	//コイン枚数表示用のラベル
	//CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _coinLavel, CoinLavel);
	//背景用のParallaxNode
	CC_SYNTHESIZE_RETAIN(cocos2d::ParallaxNode *, _parallaxNode, ParallaxNode);

	//CREATE_FUNC(MainScene);
private:
	void onGameOver();

};

#endif /* defined(__IrukaJet__GameScene__) */
