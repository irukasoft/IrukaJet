#include "SelectScene.h"

USING_NS_CC;

Scene* SelectScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SelectScene::create();
    scene->addChild(layer);
    return scene;
}
SelectScene::SelectScene()
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

	return true;

}
