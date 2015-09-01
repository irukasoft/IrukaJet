#include "SelectScene.h"
#include "GameScene.h"
#include "TitleScene.h"

USING_NS_CC;
//ステージファイル名のフォーマット
const char* STAGE_IMG_FORMAT="stage%02d.png";
const char* STAGE_IMG_PRESSED_FORMAT="stage%02d_pressed.png";
const int MAX_STAGE = 4;
// ハイスコア格納用のキー
const char* HIGHSCORE_KEY_STAGE = "highscore_key%02d";
//
const int MENU_MARGIN = 50;

Scene* SelectScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SelectScene::create();
    scene->addChild(layer);
    return scene;
}
SelectScene::SelectScene()
: _level(0)
{
}
SelectScene::~SelectScene()
{
}
bool SelectScene::init()
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
    auto titleLogo = Sprite::create("select_logo.png");
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

	//スコア表示
    int i = 1;
    for (; i <= MAX_STAGE; i++){
    	auto stageImg = StringUtils::format(STAGE_IMG_FORMAT, i);
    	auto stageImgPressed = StringUtils::format(STAGE_IMG_PRESSED_FORMAT, i);
    	//ステージ選択ボタン
    	auto gameItem = MenuItemImage::create(stageImg,stageImgPressed,
    			[this](Ref *sender){
    		auto tapItem = (MenuItemImage*)sender;
    		//ゲームスタート
    		onGameStart(tapItem->getTag());
    	});
    	gameItem->setTag(i);
    	gameItem->setScale(0.5f);
    	//ハイスコアレベル
        auto stageHighKey = StringUtils::format(HIGHSCORE_KEY_STAGE, i);
    	auto highScore = UserDefault::getInstance()->getDoubleForKey(stageHighKey.c_str());
        auto scoreLabel = Label::createWithSystemFont(
        		"HIGH SCORE : --.---  ",
        		"Marker Felt",
        		28);
    	if (highScore > 0.0 ) {
    		scoreLabel->setString(StringUtils::format("HIGH SCORE : %6.3lf", highScore));
    	}
    	scoreLabel->setColor(Color3B::YELLOW);
    	scoreLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    	scoreLabel->enableOutline(Color4B::BLACK, 1.5);

    	//扱いやすいように背景の基準点変更
    	gameItem->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    	scoreLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    	auto gameMenu = Menu::create(gameItem, NULL);
    	this->addChild(gameMenu);
    	this->addChild(scoreLabel);

    	gameMenu->setPosition(Vec2(winSize.width /2.0 - 70, winSize.height - 200 - ((i-1)* 85)));
    	scoreLabel->setPosition(Vec2(winSize.width /2.0 -35, winSize.height - 200 - ((i-1)* 85)));
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

	return true;

}
/**
 * ゲームスタートへ
 */
void SelectScene::onGameStart(int level){
	auto scene = GameScene::createWithLevel(level);
	auto transition = TransitionFade::create(1.0, scene);
	Director::getInstance()->replaceScene(transition);
}
