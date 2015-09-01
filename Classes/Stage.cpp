#include "Stage.h"

USING_NS_CC;

//ステージファイル名のフォーマット
const char* STAGE_FILE_FORMAT="map/Stage%02d.tmx";

Stage::Stage()
: _tiledMap(nullptr)
, _player(nullptr)
, _level(0)
{
}
Stage::~Stage()
{
	CC_SAFE_RELEASE_NULL(_tiledMap);
	CC_SAFE_RELEASE_NULL(_player);
}
bool Stage::initWithLevel(int level)
{
    if ( !Layer::init() )
    {
        return false;
    }
    //ステージ番号を格納
    _level = level;

    //マップファイルからノードを作成する
    auto stageFile = StringUtils::format(STAGE_FILE_FORMAT, level);
    auto map = TMXTiledMap::create(stageFile);
    this->addChild(map);
    this->setTiledMap(map);

    //地形レイヤーを取得
    auto terrainLayer = map->getLayer("Terrain");
    //オブジェクトレイアー取得
    auto objectLayer = map->getLayer("Object");

    //マップのサイズ
    auto mapSize = map->getMapSize();
    for (int x = 0; x < mapSize.width; ++x){
    	for (int y = 0; y < mapSize.height; ++y) {
    		auto coordinate = Vec2(x, y);

    		this->addPhysicsBody(terrainLayer, coordinate);
    		if (objectLayer != nullptr) {
        		this->addPhysicsBody(objectLayer, coordinate);
    		}
    	}
    }

    //プレイヤーの作成
    auto player = Player::create();
    //player->setPosition(Vec2(40,160));
    player->setPosition(Vec2(100,100));
    this->addChild(player);
    this->setPlayer(player);

    //画面追従
    auto winSize = Director::getInstance()->getWinSize();
    this->runAction(Follow::create(player,
    		Rect(0,0,_tiledMap->getContentSize().width,_tiledMap->getContentSize().height)));

    this->scheduleUpdate();

    return true;

}

void Stage::update(float dt){

}

Sprite* Stage::addPhysicsBody(cocos2d::TMXLayer *layer, cocos2d::Vec2& coordinate){
	//タイルのスプライトを取り出し
	auto sprite = layer->getTileAt(coordinate);
	if (sprite) {
		//剛体用のマテリアルを作成
		auto material = PhysicsMaterial();
		//引っかからないように摩擦を０へ
		material.friction = 0;
		//剛体を配置する
		auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(), material);
		//剛体を固定する
		physicsBody->setDynamic(false);
		//剛体を付けるSpriteのアンカーポイントを中心にする
		sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

		//タイルのIDを取り出す
		auto gid = layer->getTileGIDAt(coordinate);
		//タイルのプロパティーをmapで取り出し
		auto properties = _tiledMap->getPropertiesForGID(gid).asValueMap();
		//カテゴリプロフィールが存在しているかチェック
		if (properties.count("category") > 0) {
			auto category = properties.at("category").asInt();
			//剛体にカテゴリーをセット
			physicsBody->setCategoryBitmask(category);
			//剛体と接触判定をとるカテゴリを指定する
			physicsBody->setContactTestBitmask(static_cast<int>(TileType::PLAYER));
			//剛体と衝突をとるカテゴリを指定する
			physicsBody->setCollisionBitmask(static_cast<int>(TileType::PLAYER));
		}

		//剛体をSpriteにつける
		sprite->setPhysicsBody(physicsBody);
		auto thisPos = sprite->getPosition();
		//スプライトのズレを修正
		sprite->setPosition(Vec2(
				thisPos.x + sprite->getContentSize().width/2.0,
				thisPos.y + sprite->getContentSize().height/2.0));

		return sprite;
	}
	return nullptr;
}

Stage* Stage::createWithLevel(int level){
	Stage *ret = new Stage();
	if (ret->initWithLevel(level)){
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}
