#include "PlayerScene.h"
#include "TitleScene.h"

USING_NS_CC;

//プレイヤーファイル名のフォーマット
const char* PLAYER_FILE_FORMAT_S="player%02d.png";
// ハイスコア格納用のキー
const char* PLAYER_SELECT_KEY_S = "player_key";
//プレイヤーの最大数
const int MAX_PLAYER = 3;
//アニメーションが何フレームあるか
const int FRAME_COUNT_S = 4;

Scene* PlayerScene::createScene()
{
    auto scene = Scene::create();
    auto layer = PlayerScene::create();
    scene->addChild(layer);
    return scene;
}
PlayerScene::PlayerScene()
: _level(0)
{
}
PlayerScene::~PlayerScene()
{
}
bool PlayerScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    //Dierctorを取り出し
    auto director = Director::getInstance();
    //画像サイズ取り出し
    auto winSize = director->getWinSize();
    //背景色
    auto background = Sprite::create("select_background.png");
    background->setPosition(Vec2(winSize.width / 2.0, winSize.height / 2.0));
    this->addChild(background);

    //ロゴ表示
    auto titleLogo = Sprite::create("player_logo.png");
    titleLogo->setScale(1.3f);
    titleLogo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    titleLogo->setPosition(Vec2(winSize.width /2.0, winSize.height - 10));
	this->addChild(titleLogo);

	//説明の半透明枠を表示
	auto menuground = LayerColor::create(Color4B(51,75,112,200),
			winSize.width - 50,
			winSize.height - 320);
	this->addChild(menuground);
	menuground->ignoreAnchorPointForPosition(false);
	//menuground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menuground->setPosition(Vec2(winSize.width /2.0, winSize.height/2.0));

	//説明文追加
	std::string infoMessage ="ゲームのプレイヤーを選べます。\n最高スピードがそれぞれ違います。";
    auto playerInfo = Label::createWithSystemFont(
    		infoMessage.c_str(),
    		"Marker Felt",
    		24);
    playerInfo->setColor(Color3B::WHITE);
    playerInfo->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    playerInfo->setPosition(Vec2(winSize.width /2.0, winSize.height - 200));
	this->addChild(playerInfo);

	//プレイヤー表示
    int i = 1;
    for (; i <= MAX_PLAYER; i++){
    	//キャラクター画像表示
    	auto playerFormat = StringUtils::format(PLAYER_FILE_FORMAT_S, i);
    	auto playerImg = Sprite::create(playerFormat);
    	auto frameSize = Size(playerImg->getContentSize().width / FRAME_COUNT_S,
    			playerImg->getContentSize().height);

    	playerImg->setTextureRect(Rect(0,0,frameSize.width, frameSize.height));
    	playerImg->setTag(i);
//    	playerImg->setScale(0.5f);
    	//ハイスコアレベル
        auto playerInfo = Label::createWithSystemFont(
        		getInfoMessage(i).c_str(),
        		"Marker Felt",
        		18);
        playerInfo->setColor(Color3B::YELLOW);
        playerInfo->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);

    	//扱いやすいように背景の基準点変更
    	playerImg->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    	playerInfo->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    	this->addChild(playerImg);
    	this->addChild(playerInfo);

    	playerImg->setPosition(Vec2(winSize.width /2.0 - 70, winSize.height - 300 - ((i-1)* 100)));
    	playerInfo->setPosition(Vec2(winSize.width /2.0 -35, winSize.height - 300 - ((i-1)* 100)));
    }

	//menuボタン
	auto selectItem = MenuItemImage::create("common_btn_menu.png","common_btn_menu_pressed.png",
			[this](Ref *sender){
				auto scene = TitleScene::createScene();
				auto transition = TransitionFade::create(0.5, scene);
				Director::getInstance()->replaceScene(transition);

	});
	selectItem->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	auto menu = Menu::create(selectItem, nullptr);
	menu->setPosition(Vec2(25, 25));

	this->addChild(menu);

	//選択
    auto playerSelect = Sprite::create("player_select.png");
    playerSelect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    playerSelect->setPosition(getSelectPlayerVec());
	this->addChild(playerSelect);



    //=======================
    //画面をタッチしたときにタッチされている処理
    //-----------------------
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch *touch, Event *event) {
    	//タッチした場所を検知
    	auto point = Vec2(touch->getLocation().x, touch->getLocation().y);
        //target : ターゲットのスプライト
        auto target = (Sprite*)event->getCurrentTarget();
    	//タッチした場所をセーブ
    	saveTouchSelect(point);
    	//タッチした場所へ選択枠変更
    	target->setPosition(getSelectPlayerVec());

    	return true;
    };
    listener->onTouchMoved = [this](Touch *touch, Event *event) {
    };
    listener->onTouchEnded = [this](Touch *touch, Event *event) {
    };
    listener->onTouchCancelled = [this](Touch *touch, Event *event) {
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, playerSelect);

	return true;

}
std::string PlayerScene::getInfoMessage(int level){
	std::string infoMessage ="";
	switch(level){
	case 1:
		infoMessage = "浮き輪です。\n";
		infoMessage +="最高スピードは遅い";
		break;
	case 2:
		infoMessage = "ドルフィンです。\n";
		infoMessage +="最高スピードは普通";
		break;
	case 3:
		infoMessage = "ボートです。\n";
		infoMessage +="最高スピードは速い";
		break;
	}
	return infoMessage;
}

Vec2 PlayerScene::getSelectPlayerVec(){
    //Dierctorを取り出し
    auto director = Director::getInstance();
    //画像サイズ取り出し
    auto winSize = director->getWinSize();

    auto selectkey = UserDefault::getInstance()->getIntegerForKey(PLAYER_SELECT_KEY_S);
    if(selectkey == 0) {
    	selectkey= 2;
    }
	auto selectVec = Vec2(0,0);
	switch(selectkey) {
	case 1:
		selectVec = Vec2(winSize.width /2.0, winSize.height - 300 + 12);
		break;
	case 2:
		selectVec = Vec2(winSize.width /2.0, winSize.height - 400 + 12);
		break;
	case 3:
		selectVec = Vec2(winSize.width /2.0, winSize.height - 500 + 12);
		break;
	}
	return selectVec;
}

void PlayerScene::saveTouchSelect(Vec2 point){
    //Dierctorを取り出し
    auto director = Director::getInstance();
    //画像サイズ取り出し
    auto winSize = director->getWinSize();

    int selectItem = 0;

    auto playerSelect = Sprite::create("player_select.png");
    playerSelect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);

    //touchPoint : タッチされた場所
    //=================
    //sono1
    playerSelect->setPosition(Vec2(winSize.width /2.0, winSize.height - 300 + 12));
    //touchPointがtargetBoxの中に含まれているか判定
    if (playerSelect->getBoundingBox().containsPoint(point)){
    	UserDefault::getInstance()->setIntegerForKey(PLAYER_SELECT_KEY_S, 1);
    	return;
    }
    //=================
    //sono2
    playerSelect->setPosition(Vec2(winSize.width /2.0, winSize.height - 400 + 12));
    //touchPointがtargetBoxの中に含まれているか判定
    if (playerSelect->getBoundingBox().containsPoint(point)){
    	UserDefault::getInstance()->setIntegerForKey(PLAYER_SELECT_KEY_S, 2);
    	return;
    }
    //=================
    //sono3
    playerSelect->setPosition(Vec2(winSize.width /2.0, winSize.height - 500 + 12));
    //touchPointがtargetBoxの中に含まれているか判定
    if (playerSelect->getBoundingBox().containsPoint(point)){
    	UserDefault::getInstance()->setIntegerForKey(PLAYER_SELECT_KEY_S, 3);
    	return;
    }
}
