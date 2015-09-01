#include "InfoScene.h"
#include "TitleScene.h"

USING_NS_CC;

Scene* InfoScene::createScene()
{
    auto scene = Scene::create();
    auto layer = InfoScene::create();
    scene->addChild(layer);
    return scene;
}
InfoScene::InfoScene()
{
}
InfoScene::~InfoScene()
{
}
bool InfoScene::init()
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
    auto titleLogo = Sprite::create("info_logo.png");
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

	//説明文
	std::string infoMessage ="●ゲームの説明●\n";
	infoMessage +="\n";
	infoMessage +="このゲームは画面をタップしてドルフィンを誘導して、\n";
	infoMessage +="画面の上部へ進めばゴールとなります。\n";
	infoMessage +="\n";
	infoMessage +="タップしていないときはドルフィンは上へ上へと\n";
	infoMessage +="勝手に進むので、うまく誘導してください！\n";
	infoMessage +="\n";
	infoMessage +="また、渦に当たるとドルフィンが吸い込まれて\n";
	infoMessage +="ゲームオーバーです。\n";
	infoMessage +="\n";
	infoMessage +="リタイヤしたいときは画面左上の爆弾ボタンを\n";
	infoMessage +="タップしてください。\n";
	infoMessage +="\n";
	infoMessage +="ステージクリア後に、 \n";
	infoMessage +="ハッシュタグ #TopTheDolphin で、\n";
	infoMessage +="ステージとタイムをツイートできたりします。\n";
	infoMessage +="\n";
	infoMessage +="古い端末だと処理速度的に最速タイムは狙えないのは\n";
	infoMessage +="ご了承ください^^;\n";
    auto messageLabel = Label::createWithSystemFont(
    		infoMessage,
    		"Marker Felt",
    		16);
    messageLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    messageLabel->setPosition(Vec2(
    		winSize.width / 2.0,
    		winSize.height / 2.0));
	this->addChild(messageLabel);

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
