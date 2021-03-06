#include "AppDelegate.h"
#include "TitleScene.h"

USING_NS_CC;
//初回ステージ番号
const int INITIAL_LEVEL = 1;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    //ちらつき防止
    director->getInstance()->setProjection(Director::Projection::_2D);
    director->getInstance()->setDepthTest(false);

    //全てを540 * 960 で
    glview->setDesignResolutionSize(540, 960, ResolutionPolicy::SHOW_ALL);
    FileUtils::getInstance()->addSearchPath("images");

    // create a scene. it's an autorelease object
    //auto scene = TitleScene::createScene();
    //auto scene = GameScene::createWithLevel(INITIAL_LEVEL);
    auto scene = TitleScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
