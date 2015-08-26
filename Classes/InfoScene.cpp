#include "InfoScene.h"

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

	return true;

}
