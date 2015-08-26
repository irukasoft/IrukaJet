#include "GameScene.h"

USING_NS_CC;

//重力を表すベクトル
const Vec2 GRAVITY_ACCELERATION = Vec2(0, 0);
//TODO 浮力を表すベクトル
const Vec2  IMPULSE_ACCELERATION = Vec2(0, -100);


Scene* GameScene::createWithLevel(int level)
{
	//物理エンジンを有効にしたシーン作成する
    auto scene = Scene::createWithPhysics();
	//物理空間
	auto world = scene->getPhysicsWorld();
	//重力を設定
	world->setGravity(GRAVITY_ACCELERATION);

	//物理空間のスピードを設定する
	world->setSpeed(5.0);

	//レイヤーを作成する
    //auto layer = MainScene::create();
	auto layer = new GameScene();
	if (layer && layer->initWithLevel(level)) {
		layer->autorelease();
	} else {
		CC_SAFE_DELETE(layer);
	}
	scene->addChild(layer);

	return scene;
}
GameScene::GameScene()
:_stage(nullptr)
, _isPress(false)
, _parallaxNode(nullptr)
{
}
GameScene::~GameScene()
{
	CC_SAFE_RELEASE_NULL(_stage);
	CC_SAFE_RELEASE_NULL(_parallaxNode);

}

bool GameScene::initWithLevel(int level)
{
    if ( !Layer::init() )
    {
        return false;
    }
    //Dierctorを取り出し
    auto director = Director::getInstance();
    //画像サイズ取り出し
    auto winSize = director->getWinSize();

    //ステージ作成
    auto stage = Stage::createWithLevel(level);
    this->setStage(stage);

    //背景画像の作成
    auto background = Sprite::create("background.jpg");
    //扱いやすいように背景の基準点を左下へ設定
    background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    //parallaxNodeの作成
    auto parallaxNode = ParallaxNode::create();
    this->setParallaxNode(parallaxNode);
    this->addChild(parallaxNode);

    //マップのサイズ取得
    auto mapSize = stage->getTiledMap()->getContentSize();
    //背景画像のサイズ取得
    auto backgroundSize = background->getContentSize();
    //スクロールの割合計算
    auto scrollRatioW = (backgroundSize.width - winSize.width) / mapSize.width;
    auto scrollRatioH = (backgroundSize.height - winSize.height) / mapSize.height;
    parallaxNode->addChild(background, 0, Vec2(scrollRatioW,scrollRatioH), Vec2::ZERO);

    this->addChild(stage);

    //タッチしたときにタッチされている処理
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch *touch, Event *event) {
    	_touchPoint = touch->getLocation();
    	this->setIsPress(true);
    	return true;
    };
    listener->onTouchMoved = [this](Touch *touch, Event *event) {
    	_touchPoint = touch->getLocation();
    	this->setIsPress(true);
    };
    listener->onTouchEnded = [this](Touch *touch, Event *event) {
    	this->setIsPress(false);
    };
    listener->onTouchCancelled = [this](Touch *touch, Event *event) {
    	this->setIsPress(false);
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


    //物体が接触したことを検知するEventListener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin= [this](PhysicsContact& contact){
    	//log("hit!");
    	//二つの剛体のうち、プレイヤーではない方を取り出す
    	auto otherShape = contact.getShapeA()->getBody() == _stage->getPlayer()->getPhysicsBody() ? contact.getShapeB() : contact.getShapeA();
    	auto body = otherShape->getBody();
    	//カテゴリーを取り出す
    	auto category = body->getCategoryBitmask();

    	if (category & static_cast<int>(Stage::TileType::ENEMY)){
    		//ヒットしたのが敵キャラだった場合
    		this->onGameOver();
//    	} else if(category & (int)Stage::TileType::COIN) {
//    		//ヒットしたのがコインの場合
//    		body->getNode()->removeFromParent();
//    		_coin += 1;
    	}
    	return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    this->scheduleUpdate();

    return true;
}
void GameScene::update(float dt){
	//画面がタップされている間
	//現在のプレイヤーの位置を取得
	log("update");
	auto playerPoint = _stage->getPlayer()->getPosition();
    auto winSize = Director::getInstance()->getWinSize();
	if (this->getIsPress()){
	    auto stageSize = _stage->getTiledMap()->getContentSize();
	    //タップしたスクリーンの位置をステージの位置に変換!!すげー便利!!
	    auto stageTouch = _stage->convertToNodeSpace(_touchPoint);
		//TODO 計算するよ
		float xPoint=0.0;
		float yPoint=0.0;
		//X軸計算 if（タップ位置　＜　プレイヤー位置）X＝ -１００} else {タップ位置　＜　プレイヤー位置）X＝ １００}
		stageTouch.x < playerPoint.x ? xPoint = -180 : xPoint = 180;
		//４）Y軸計算 if（タップ位置　＜　プレイヤー位置）Y＝ -１００} else {タップ位置　＜　プレイヤー位置）Y＝ １００}
		stageTouch.y < playerPoint.y ? yPoint = -360 : yPoint = 180;
		//プレイヤーに推進力を与える
		_stage->getPlayer()->getPhysicsBody()->applyImpulse(Vec2(xPoint,yPoint));
//		log("xPoint=%f,yPoint=%f",xPoint,yPoint);


	}else {
		//TODO いらないかも
		_stage->getPlayer()->getPhysicsBody()->applyImpulse(Vec2(0,0));
	}

	//回転
	auto velocity = _stage->getPlayer()->getPhysicsBody()->getVelocity();
	float bulletAngle = velocity.getAngle(Vec2(1,0));
	float angle = CC_RADIANS_TO_DEGREES(bulletAngle);
//	float angle = acos(velocity.x/sqrt(velocity.x*velocity.x+velocity.y*velocity.y));
//	angle = angle*180.0/3.141592;
//	if (velocity.y < 0 ) angle=360.0-angle;
//	log("angle = %f", velocity.getAngle());
	_stage->getPlayer()->setRotation(angle + 90);
	const auto margin = 50;
	//プレイヤーの位置が画面外にでたらゲームオーバーを呼び出す
	if (playerPoint.y < -margin || playerPoint.x >= winSize.height + margin) {
		//何度も呼ばれることを防ぐために、プレイヤーがステージ内にいること
		if (_stage->getPlayer()->getParent() != nullptr) {
//			this->onGameOver();
		}
	}
//	this->getCoinLavel()->setString(StringUtils::toString(_coin));
	_parallaxNode->setPosition(_stage->getPlayer()->getPosition() * -1);
}

void GameScene::onGameOver(){
	//プレイヤーをステージから削除
	_stage->getPlayer()->removeFromParent();
	//画面サイズを取り出す
	auto winSize = Director::getInstance()->getWinSize();

	//GAME OVERの表示
	auto gameover = Sprite::create("gameover.png");
	gameover->setPosition(Vec2(winSize.width / 2.0, winSize.height /1.5));
	this->addChild(gameover);

	//もう一度遊ぶメニュー
	auto menuItem = MenuItemImage::create("replay.png","replay_pressed.png",
			[this](Ref *sender){
				auto scene = GameScene::createWithLevel(_stage->getLevel());
				auto transition = TransitionFade::create(1.0, scene);
				Director::getInstance()->replaceScene(transition);
	});
	auto menu = Menu::create(menuItem, nullptr);
	this->addChild(menu);

	menu->setPosition(Vec2(winSize.width / 2.0, winSize.height /3));

	//パーティクル表示
	auto exlosion = ParticleExplosion::create();
	exlosion->setPosition(_stage->getPlayer()->getPosition());
	_stage->addChild(exlosion);
}
