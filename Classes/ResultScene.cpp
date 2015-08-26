#include "ResultScene.h"

USING_NS_CC;

Scene* ResultScene::createScene()
{
    auto scene = Scene::create();
    auto layer = ResultScene::create();
    scene->addChild(layer);
    return scene;
}
ResultScene::ResultScene()
{
}
ResultScene::~ResultScene()
{
}
bool ResultScene::init()
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
