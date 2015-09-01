#include "TitleScene.h"
#include "SelectScene.h"
#include "InfoScene.h"

USING_NS_CC;

Scene* TitleScene::createScene()
{
    auto scene = Scene::create();
    auto layer = TitleScene::create();
    scene->addChild(layer);
    return scene;
}
TitleScene::TitleScene()
{
}
TitleScene::~TitleScene()
{
}
bool TitleScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    //Dierctorを取り出し
    auto director = Director::getInstance();
    //画像サイズ取り出し
    auto winSize = director->getWinSize();

    //背景
    auto background = Sprite::create("title_background.png");
    background->setPosition(Vec2(winSize.width / 2.0, winSize.height / 2.0));
    this->addChild(background);

    //背景その２
    auto backgroundSub = Sprite::create("title_background_sub.png");
    backgroundSub->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    backgroundSub->setPosition(Vec2(winSize.width, 0));
    this->addChild(backgroundSub);

    //ロゴ表示
    auto titleLogo = Sprite::create("title_logo.png");
    titleLogo->setScale(1.3f);
    titleLogo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    titleLogo->setPosition(Vec2(winSize.width /2.0, winSize.height - 10));
	this->addChild(titleLogo);

    //selectボタン
	auto selectItem = MenuItemImage::create("common_btn_select.png","common_btn_select_pressed.png",
			[this](Ref *sender){
				auto scene = SelectScene::createScene();
				auto transition = TransitionFade::create(0.5, scene);
				Director::getInstance()->replaceScene(transition);

	});
	selectItem->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    //infoボタン
	auto infoItem = MenuItemImage::create("common_btn_info.png","common_btn_info_pressed.png",
			[this](Ref *sender){
				auto scene = InfoScene::createScene();
				auto transition = TransitionFade::create(0.5, scene);
				Director::getInstance()->replaceScene(transition);

	});
	infoItem->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);

	auto menu = Menu::create(selectItem, infoItem, nullptr);
	menu->alignItemsVerticallyWithPadding(15);
	menu->setPosition(Vec2(25, winSize.height - 250));

	this->addChild(menu);


	return true;
}
