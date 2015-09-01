#ifndef __IrukaJet__GameScene__
#define __IrukaJet__GameScene__

#include "Stage.h"
#include "cocos2d.h"
//#include "ui/CocosGUI.h"

class GameScene :public cocos2d::Layer
{
protected:
	GameScene();
	virtual ~GameScene();
	//bool init() override;
	bool initWithLevel(int level);

public:
    ///////////////////////////////////
    /// ゲームの状態を表します
    enum class GameState {
    	///ゲーム中
    	READY,
    	///ゲーム中
    	PLAYING,
    	///スコア表示
    	RESULT
    };
    /**
	 * ステージ番号からステージを作成する
	 */
	static cocos2d::Scene * createWithLevel(int level);
    //static cocos2d::Scene* createScene();
	void update(float dt) override;
	//画面がタップされているか
	CC_SYNTHESIZE(bool, _isPress,IsPress);
    //ゲームステータス
    CC_SYNTHESIZE(GameState, _state, State);
    //開始時間
    CC_SYNTHESIZE(double, _startTime, StartTime);
    //経過時間
    CC_SYNTHESIZE(double, _second, Second);
    //経過時間
    CC_SYNTHESIZE(int, _thisLevel, ThisLevel);
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
	//ゲームスタート用アイコン
	CC_SYNTHESIZE_RETAIN(cocos2d::Sprite *, _ready, Ready);
    //経過時間ラベル
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _secondLabel, SecondLabel);

    //経過時間ラベル
    CC_SYNTHESIZE_RETAIN(cocos2d::LayerColor *, _menuground, Menuground);

    //CREATE_FUNC(MainScene);
private:
	void onGameOver();
	void onGameClear();
    //計測
    double getSec();


};

#endif /* defined(__IrukaJet__GameScene__) */
