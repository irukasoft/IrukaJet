#ifndef __IrukaJet__Stage__
#define __IrukaJet__Stage__

#include "cocos2d.h"
#include "Player.h"

//#include <random>

class Stage :public cocos2d::Layer
{
protected:
	Stage();
	virtual ~Stage();
	//bool init() override;
	bool initWithLevel(int level);

public:
	///タイルの種類
	enum class TileType {
		//壁
		WALL = 1 <<0, //1
		//プレイヤー
		PLAYER = 1 << 1,//2
		//敵キャラ
		ENEMY = 1 << 2, //4
		//コイン
		COIN = 1 <<3 //8
	};
	void update(float dt) override;

	CC_SYNTHESIZE_READONLY(int, _level, Level);
	//タイルマップ
	CC_SYNTHESIZE_RETAIN(cocos2d::TMXTiledMap *, _tiledMap, TiledMap);
	//static cocos2d::Scene* createScene();
	//プレイヤー
	CC_SYNTHESIZE_RETAIN(Player *, _player, Player);
	//CREATE_FUNC(Stage);
	/**
	 * ステージ番号からステージを作成する
	 */
	static Stage * createWithLevel(int level);

private:
	/**
	 * 指定のレイアーの特定一のタイルに剛体を配置
	 * @param layer
	 * @param coordinate
	 * @return タイルのスプライト、またはnullptr
	 */
	cocos2d::Sprite* addPhysicsBody(cocos2d::TMXLayer *layer, cocos2d::Vec2& coordinate);
};

#endif /* defined(__IrukaJet__Stage__) */
