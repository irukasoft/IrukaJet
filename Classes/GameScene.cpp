#include "GameScene.h"
//#include "BrowserLauncher.h"
#include "SelectScene.h"

USING_NS_CC;

//重力を表すベクトル
const Vec2 GRAVITY_ACCELERATION = Vec2(0, 0);
//初期プレイヤー加速度
const Vec2 INITIAL_PLAYER_ACCELERATION = Vec2(0, 200);
//タイムオーバー
const int MAX_TIME = 60;
// ハイスコア格納用のキー
const char* HIGHSCORE_KEY = "highscore_key%02d";

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
, _state(GameState::READY)
, _ready(nullptr)
,_startTime(0)
,_second(0)
,_secondLabel(nullptr)
,_menuground(nullptr)
,_thisLevel(0)
{
}
GameScene::~GameScene()
{
	CC_SAFE_RELEASE_NULL(_stage);
	CC_SAFE_RELEASE_NULL(_parallaxNode);
	CC_SAFE_RELEASE_NULL(_ready);
	CC_SAFE_RELEASE_NULL(_secondLabel);
	CC_SAFE_RELEASE_NULL(_menuground);

}

bool GameScene::initWithLevel(int level)
{
    if ( !Layer::init() )
    {
        return false;
    }
    //画像サイズ取り出し
    auto winSize = Director::getInstance()->getWinSize();

    //ステージ作成
    auto stage = Stage::createWithLevel(level);
    this->setStage(stage);
    _thisLevel = level;

    //=======================
    //背景画像の作成
    //-----------------------
    auto background = Sprite::create("game_background.png");
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

    //=======================
    //画面をタッチしたときにタッチされている処理
    //-----------------------
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


    //=======================
    //物体が接触したことを検知するEventListener
    //-----------------------
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
	    	_state = GameState::RESULT;
    		this->onGameOver();
//    	} else if(category & (int)Stage::TileType::COIN) {
//    		//ヒットしたのがコインの場合
//    		body->getNode()->removeFromParent();
//    		_coin += 1;
    	}
    	return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    //=======================
    //タイマーラベルの追加
    //-----------------------
    auto secondLabel = Label::createWithSystemFont(
    		StringUtils::format("%6.3lf", _second),
    		"Marker Felt",
    		40);
    this->setSecondLabel(secondLabel);
    secondLabel->setColor(Color3B::YELLOW);
    secondLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    secondLabel->enableOutline(Color4B::BLACK, 1.5);
    //扱いやすいように背景の基準点を左上へ設定
    secondLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    secondLabel->setPosition(Vec2(50, winSize.height - 30));
    this->addChild(secondLabel);

    //=======================
    //ゲームオーバー爆弾の追加
    //-----------------------
    auto bakudan = Sprite::create("marubakudan.png");
	this->addChild(bakudan);
	bakudan->setScale(0.4f);
	bakudan->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	bakudan->setPosition(Vec2(10, winSize.height - 10));
    auto bakudanlistener = EventListenerTouchOneByOne::create();
    bakudanlistener->onTouchBegan = [this](Touch *touch, Event *event) {
        //target : ターゲットのスプライト
        auto target = (Sprite*)event->getCurrentTarget();
        //targetBox : タッチされたスプライトの領域
        Rect targetBox = target->getBoundingBox();
        //touchPoint : タッチされた場所
        Point touchPoint = Vec2(touch->getLocation().x, touch->getLocation().y);
        //touchPointがtargetBoxの中に含まれているか判定
        if (targetBox.containsPoint(touchPoint)){
        	if (_state == GameState::PLAYING){
            	_state = GameState::RESULT;
            	this->onGameOver();
        	}
        }
    	return true;
    };
    bakudanlistener->onTouchEnded = [](Touch *touch, Event *event) {
    };
    bakudanlistener->onTouchCancelled = [](Touch *touch, Event *event) {
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(bakudanlistener, bakudan);

    //=======================
    //ゲーム初期表示設定
    //-----------------------
	//メニューのバックは暗くする
	auto menuground = LayerColor::create(Color4B(51,75,112,120), winSize.width, winSize.height);
	this->setMenuground(menuground);
	this->addChild(menuground);

	//点灯するレディゴー
	auto ready = Sprite::create("ready.png");
    this->setReady(ready);
	this->addChild(ready);
	ready->setPosition(Vec2(winSize.width / 2.0, winSize.height /2.0));
	auto blink = Blink::create(10,20);
	ready->runAction(blink);

    //タップするとゲームスタート
    auto readylistener = EventListenerTouchOneByOne::create();
    readylistener->onTouchBegan = [this](Touch *touch, Event *event) {
    	_ready->removeFromParentAndCleanup(true);
    	_state = GameState::PLAYING;
    	_startTime = getSec();
    	_second = 0;
    	_stage->getPlayer()->setAcceleration(INITIAL_PLAYER_ACCELERATION);
    	//メニューバックカラー非表示
    	_menuground->setVisible(false);
    	return true;
    };
    readylistener->onTouchEnded = [this](Touch *touch, Event *event) {
    };
    readylistener->onTouchCancelled = [this](Touch *touch, Event *event) {
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(readylistener, ready);

    this->scheduleUpdate();

    return true;
}

/**
 *
 */
void GameScene::update(float dt){
	//スタートしたとき
	if (_state == GameState::PLAYING){
		//画面がタップされている間

		//ゲームがスタートしていたらタイム計測
		_second = getSec() - _startTime;
	    _secondLabel->setString(StringUtils::format("%6.3lf", _second));

		//現在のプレイヤーの位置を取得
		auto playerPoint = _stage->getPlayer()->getPosition();
	    auto winSize = Director::getInstance()->getWinSize();
	    auto stageSize = _stage->getTiledMap()->getContentSize();
		if (this->getIsPress()){
		    //タップしたスクリーンの位置をステージの位置に変換!!すげー便利!!
		    auto stageTouch = _stage->convertToNodeSpace(_touchPoint);
			//TODO 計算するよ
			float xPoint=0.0;
			float yPoint=0.0;
			//X軸計算
			stageTouch.x < playerPoint.x ? xPoint = -180 : xPoint = 180;
			//４）Y軸計算
			stageTouch.y < playerPoint.y ? yPoint = -360 : yPoint = 180;
			//プレイヤーに推進力を与える
			_stage->getPlayer()->getPhysicsBody()->applyImpulse(Vec2(xPoint,yPoint));
//			log("stageTouch.x=%f,stageTouch.y=%f",stageTouch.y,stageTouch.y);
//			log("playerPoint.x=%f,playerPoint.y=%f",playerPoint.y,playerPoint.y);

		}

		//回転
		auto velocity = _stage->getPlayer()->getPhysicsBody()->getVelocity();
		float bulletAngle = velocity.getAngle(Vec2(1,0));
		float angle = CC_RADIANS_TO_DEGREES(bulletAngle);

		_stage->getPlayer()->setRotation(angle + 90);
		const auto margin = 50;
		//プレイヤーの位置が画面下にでたらゲームオーバーを呼び出す
		if (playerPoint.y < -margin) {
			//何度も呼ばれることを防ぐために、プレイヤーがステージ内にいること
			if (_stage->getPlayer()->getParent() != nullptr) {
		    	//リザルト状態へ
		    	_state = GameState::RESULT;
		    	this->onGameOver();
			}
		} else if (playerPoint.y >= stageSize.height) {
	    	//リザルト状態へ
	    	_state = GameState::RESULT;
	    	this->onGameClear();
		}

	    //タイムアウト計測
	    if (_second > MAX_TIME) {
	    	_state = GameState::RESULT;
	    	this->onGameOver();
	    }
	}
//	this->getCoinLavel()->setString(StringUtils::toString(_coin));
	//TODO 重いので・・・・
	_parallaxNode->setPosition(_stage->getPlayer()->getPosition() * -1);
}

/**
 *
 */
void GameScene::onGameOver(){
	//プレイヤーをステージから削除
	_stage->getPlayer()->removeFromParent();
	//画面サイズを取り出す
	auto winSize = Director::getInstance()->getWinSize();

	//メニューバックカラー再表示
	_menuground->setVisible(true);

	//GAME OVERの表示
	auto gameover = Sprite::create("gameover.png");
	gameover->setPosition(Vec2(winSize.width / 2.0, winSize.height /1.5));
	this->addChild(gameover);
	auto blink = Blink::create(10,20);
	gameover->runAction(blink);

	//もう一度遊ぶメニュー
	auto menuItem = MenuItemImage::create("replay.png","replay_pressed.png",
			[this](Ref *sender){
				auto scene = GameScene::createWithLevel(_stage->getLevel());
				auto transition = TransitionFade::create(1.0, scene);
				Director::getInstance()->replaceScene(transition);
	});
	//ステージ選択へ戻る
	auto selectItem = MenuItemImage::create("select.png","select_pressed.png",
			[this](Ref *sender){
				auto scene = SelectScene::createScene();
				auto transition = TransitionFade::create(1.0, scene);
				Director::getInstance()->replaceScene(transition);
	});
	auto menu = Menu::create(menuItem, selectItem,nullptr);
	menu->alignItemsVerticallyWithPadding(15);
	this->addChild(menu);

	menu->setPosition(Vec2(winSize.width / 2.0, winSize.height /3));

	//パーティクル表示
	auto exlosion = ParticleGalaxy::create();
	exlosion->setPosition(_stage->getPlayer()->getPosition());
	_stage->addChild(exlosion);
}

/**
 *
 */
void GameScene::onGameClear(){
	//プレイヤーをステージから削除
	_stage->getPlayer()->removeFromParent();
	//画面サイズを取り出す
	auto winSize = Director::getInstance()->getWinSize();

	//メニューバックカラー再表示
	_menuground->setVisible(true);

	//GAME CLEARの表示
	auto gameclear = Sprite::create("gameclear.png");
	gameclear->setPosition(Vec2(winSize.width / 2.0, winSize.height /1.5));
	this->addChild(gameclear);

	//もう一度遊ぶメニュー
	auto menuItem = MenuItemImage::create("replay.png","replay_pressed.png",
			[this](Ref *sender){
				auto scene = GameScene::createWithLevel(_stage->getLevel());
				auto transition = TransitionFade::create(1.0, scene);
				Director::getInstance()->replaceScene(transition);
	});
	//ステージ選択へ戻る
	auto selectItem = MenuItemImage::create("select.png","select_pressed.png",
			[this](Ref *sender){
				auto scene = SelectScene::createScene();
				auto transition = TransitionFade::create(1.0, scene);
				Director::getInstance()->replaceScene(transition);
	});
	//ハイスコアをツイートする
	auto tweetItem = MenuItemImage::create("tweet.png","tweet_pressed.png",
			[this](Ref *sender){
//				Application::getInstance()->openURL("http://www.google.co.jp");
//				auto tweetString = StringUtils::format("ステージ%02dでスコア %6.3lf でクリアしました。 #TapTheDolphin", _thisLevel,_second);
				auto tweetString = StringUtils::format("テストー　ステージ%02d＝%6.3lf #TapTheDolphin", _thisLevel,_second);
				Application::getInstance()->openTweetDialog(tweetString.c_str());
	});
	auto menu = Menu::create(menuItem, selectItem, tweetItem,nullptr);
	menu->alignItemsVerticallyWithPadding(15);
	this->addChild(menu);
//	menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	menu->setPosition(Vec2(winSize.width / 2.0, winSize.height /3.5));

	//スコアチェック
	bool isHigh =false;
    auto stageHighKey = StringUtils::format(HIGHSCORE_KEY, _thisLevel);
	auto highScore = UserDefault::getInstance()->getDoubleForKey(stageHighKey.c_str());
	if (highScore <= 0.0) {
		//初回は必ず保存
    	UserDefault::getInstance()->setDoubleForKey(stageHighKey.c_str(), _second);
    	isHigh=true;
	}else if (_second > 0.0 && _second < highScore) {
    	//更新
    	UserDefault::getInstance()->setDoubleForKey(stageHighKey.c_str(), _second);
    	isHigh=true;
    }

	//タイムを移動して表示
    auto secondLabel = Label::createWithSystemFont(
    		StringUtils::format("Clear Time : %6.3lf", _second),
    		"Marker Felt",
    		40);
    secondLabel->setColor(Color3B::YELLOW);
    secondLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    secondLabel->enableOutline(Color4B::BLACK, 1.5);

    auto highLabel = Label::createWithSystemFont(
    		"!!! NEW GAME THANK YOU !!!",
    		"Marker Felt",
    		40);
	if (isHigh){
		highLabel->setString("!!! New Recode !!!");
	    highLabel->setColor(Color3B::RED);
	}else{
		 if (highScore > 0.0){
				highLabel->setString(StringUtils::format("HIGH SCORE : %6.3lf", highScore));
		 }
	    highLabel->setColor(Color3B::BLUE);
	}

    highLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    highLabel->enableOutline(Color4B::BLACK, 1.5);

    auto menuItemHigh = MenuItemLabel::create(highLabel,[](Ref *sender){});
	auto menuItemSecond = MenuItemLabel::create(secondLabel, [](Ref *sender){});
	auto menuTime = Menu::create(menuItemHigh, menuItemSecond, nullptr);
	menuTime->alignItemsVerticallyWithPadding(3);
	this->addChild(menuTime);
	menuTime->setPosition(Vec2(winSize.width / 2.0, winSize.height /2.0));

}


/**
 * 時間計測メソッド
 */
double GameScene::getSec()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}



