#include "TitleScene.h"

USING_NS_CC;
//const float IMAGE_SCALE = 0.5f;
//const float IMAGE_SCALE_LOGO = 0.8f;

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

    return true;
}
