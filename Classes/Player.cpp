#include "Player.h"
#include "Stage.h"

USING_NS_CC;

//アニメーションが何フレームあるか
const int FRAME_COUNT = 4;
//上方向の速度の最大値
const int ACCELERATION_LIMIT_UP = 45;
//下方向の速度の最大値
const int ACCELERATION_LIMIT_BTM = -45;
//右方向の速度の最大値
const int ACCELERATION_LIMIT_RIGHT = 45;
//左方向の速度の最大値
const int ACCELERATION_LIMIT_LEFT = -45;
//初期ジェット加速度
//const Vec2 INITIAL_ACCELERATION = Vec2(0, 200);

bool Player::init()
{
    if (!Sprite::initWithFile("player.png")) {
    	return false;
    }
    //１フレームの画像サイズを取得する(６フレームキャラクター）
    auto frameSize = Size(this->getContentSize().width / FRAME_COUNT,
    		this->getContentSize().height);

    //テスクチャの大きさを１フレーム分にする
    this->setTextureRect(Rect(0,0,frameSize.width, frameSize.height));

    Vector<SpriteFrame *> frames;
    for(int i=0; i < FRAME_COUNT; ++i){
    	//一コマずつアニメーションを作成
    	auto frame = SpriteFrame::create("player.png",
    			Rect(frameSize.width * i,0,frameSize.width, frameSize.height));
    	frames.pushBack(frame);
    }
    auto animation = Animation::createWithSpriteFrames(frames);
    animation->setDelayPerUnit(0.05);
    this->runAction(RepeatForever::create(Animate::create(animation)));

    auto body = PhysicsBody::createCircle(this->getContentSize().width /2.0);
    //剛体の回転を有効
    body->setDynamic(true);
    body->setRotationEnable(true);

    //カテゴリをセット
    body->setCategoryBitmask(static_cast<int>(Stage::TileType::PLAYER));
    //壁のみ衝突する
    body->setCollisionBitmask(static_cast<int>(Stage::TileType::WALL));
    //すべての剛体と接触判定を行う
    body->setContactTestBitmask(INT_MAX);

    // 初期加速度を設定する
 //   _acceleration = INITIAL_ACCELERATION;

    this->scheduleUpdate();

    this->setPhysicsBody(body);

    return true;

}

void Player::update(float dt){
	this->getPhysicsBody()->applyImpulse(_acceleration);
    // 加速度のLIMITの設定
    auto v = this->getPhysicsBody()->getVelocity();
    if (v.y > ACCELERATION_LIMIT_UP) {
        v.y = ACCELERATION_LIMIT_UP;
    }
    if (v.y < ACCELERATION_LIMIT_BTM) {
        v.y = ACCELERATION_LIMIT_BTM;
    }
    if (v.x > ACCELERATION_LIMIT_RIGHT) {
        v.x = ACCELERATION_LIMIT_RIGHT;
    }
    if (v.x < ACCELERATION_LIMIT_LEFT) {
        v.x = ACCELERATION_LIMIT_LEFT;
    }
    this->getPhysicsBody()->setVelocity(v);
}

