﻿#pragma once
#include "xx_pos.h"
#include "PKG_class.h"
#include "chipmunk.h"

/**************************************************************************************************/
// ::
/**************************************************************************************************/

#define DRAW_PHYSICS_POLYGON 0

static constexpr int ScreenWidth = 1280;
static constexpr int ScreenHeight = 720;
static constexpr float ScreenWidthRatio = float(ScreenWidth) / float(ScreenWidth + ScreenHeight);
static constexpr xx::Pos ScreenCenter = xx::Pos{ ScreenWidth / 2, ScreenHeight / 2 };

#ifdef CC_TARGET_PLATFORM
inline cocos2d::Scene* cc_scene = nullptr;
inline xx::List<cocos2d::Touch*> cc_touchs;
inline cocos2d::EventListenerTouchAllAtOnce* cc_listener = nullptr;
#endif


/**************************************************************************************************/
// IDraw
/**************************************************************************************************/

// 有绘制行为的类需要实现该接口
struct IDraw {
	virtual void DrawInit() noexcept = 0;		// 初始化( 通常是显现性质 )
	virtual void DrawUpdate() noexcept = 0;		// 更新( 通常是移动 / 动画性质 )
	virtual void DrawDispose() noexcept = 0;	// 销毁( 通常是从图形系统中完全注销对象 )
};


/**************************************************************************************************/
// SpriteFrame
/**************************************************************************************************/

struct SpriteFrame : PKG::CatchFish::Configs::SpriteFrame {
	using BaseType = PKG::CatchFish::Configs::SpriteFrame;
	using BaseType::BaseType;

#ifdef CC_TARGET_PLATFORM
	cocos2d::SpriteFrame* spriteFrame = nullptr;
#endif

	virtual int InitCascade(void* const& o) noexcept override;
	~SpriteFrame();
};
using SpriteFrame_s = std::shared_ptr<SpriteFrame>;
using SpriteFrame_w = std::weak_ptr<SpriteFrame>;


/**************************************************************************************************/
// Physics
/**************************************************************************************************/

struct Physics : PKG::CatchFish::Configs::Physics {
	using BaseType = PKG::CatchFish::Configs::Physics;
	using BaseType::BaseType;

	cpSpace* space = nullptr;

	virtual int InitCascade(void* const& o) noexcept override;
	~Physics();
};
using Physics_s = std::shared_ptr<Physics>;
using Physics_w = std::weak_ptr<Physics>;


/**************************************************************************************************/
// Scene
/**************************************************************************************************/

struct Scene : PKG::CatchFish::Scene {
	using BaseType = PKG::CatchFish::Scene;
	using BaseType::BaseType;

	// 引用到配置. 由 Scene 创建者于调用 InitCascade 前填充. ( 需确保 cfg 比 Scene 死的晚 )
	PKG::CatchFish::Configs::Config* cfg = nullptr;

	// 自减id ( 从 -1 开始, 用于服务器下发鱼生成 )
	int autoDecId = 0;

	// server 下发鱼生成专用
	xx::Random rnd2;

	// 将 Scene 指针刷到所有子
	virtual int InitCascade(void* const& o = nullptr) noexcept override;

	// 帧逻辑更新
	virtual int Update(int const& frameNumber = 0) noexcept override;
};
using Scene_s = std::shared_ptr<Scene>;
using Scene_w = std::weak_ptr<Scene>;


/**************************************************************************************************/
// Fish
/**************************************************************************************************/

struct Fish : PKG::CatchFish::Fish, IDraw {
	using BaseType = PKG::CatchFish::Fish;
	using BaseType::BaseType;

	// 所在场景
	Scene* scene = nullptr;

	// 指向具体配置
	PKG::CatchFish::Configs::Fish* cfg = nullptr;

	virtual int InitCascade(void* const& o) noexcept override;
	virtual int Update(int const& frameNumber) noexcept override;
	~Fish();

#ifdef CC_TARGET_PLATFORM
	cocos2d::Sprite* body = nullptr;
	cocos2d::Sprite* shadow = nullptr;
#if DRAW_PHYSICS_POLYGON
	cocos2d::DrawNode* debugNode = nullptr;
#endif
#endif
	virtual void DrawInit() noexcept override;
	virtual void DrawUpdate() noexcept override;
	virtual void DrawDispose() noexcept override;
};
using Fish_s = std::shared_ptr<Fish>;
using Fish_w = std::weak_ptr<Fish>;


/**************************************************************************************************/
// Player
/**************************************************************************************************/

struct Player : PKG::CatchFish::Player/*, IDraw 绘制金币倍率面板啥的?  */ {
	using BaseType = PKG::CatchFish::Player;
	using BaseType::BaseType;

	// 所在场景
	Scene* scene = nullptr;

	//// 令牌
	//std::string token;

	// 开炮等行为花掉的金币数汇总 ( 统计 )
	int64_t consumeCoin = 0;

	// todo: peer, recv, ....

	virtual int InitCascade(void* const& o) noexcept override;
	virtual int Update(int const& frameNumber) noexcept override;
	// ~Player
};
using Player_s = std::shared_ptr<Player>;
using Player_w = std::weak_ptr<Player>;


/**************************************************************************************************/
// Cannon
/**************************************************************************************************/

struct Cannon : PKG::CatchFish::Cannon, IDraw {
	using BaseType = PKG::CatchFish::Cannon;
	using BaseType::BaseType;

	// 所在场景
	Scene* scene = nullptr;

	// 所在玩家( 由 Player 预填充 )
	Player* player = nullptr;

	// 指向具体配置
	PKG::CatchFish::Configs::Cannon* cfg = nullptr;

	// 根据玩家位置从 cfg 读取到坐标记录在此以便于使用
	xx::Pos pos;

	// 存放下次可开火的 frameNumber
	int shootCD = 0;

	// 剩余子弹数量. 炮台初创时从 cfg 读取填充. ( -1 表示无限, 其余情况每次发射 -1, 到 0 时无法发射 )
	int quantity = -1;

	// 发射子弹. 成功返回 true
	bool Shoot(int const& frameNumber) noexcept;

	int InitCascade(void* const& o) noexcept override;
	virtual int Update(int const& frameNumber) noexcept override;
	~Cannon();

#ifdef CC_TARGET_PLATFORM
	cocos2d::Sprite* body = nullptr;
#endif
	virtual void DrawInit() noexcept override;
	virtual void DrawUpdate() noexcept override;
	virtual void DrawDispose() noexcept override;
};
using Cannon_s = std::shared_ptr<Cannon>;
using Cannon_w = std::weak_ptr<Cannon>;


/**************************************************************************************************/
// Bullet
/**************************************************************************************************/

struct Bullet : PKG::CatchFish::Bullet, IDraw {
	using BaseType = PKG::CatchFish::Bullet;
	using BaseType::BaseType;

	// 所在场景
	Scene* scene = nullptr;

	// 所在玩家( 由 Cannon 预填充 )
	Player* player = nullptr;

	// 所属炮台( 由 Cannon 预填充 )
	Cannon* cannon = nullptr;

	// 指向具体配置( 由 Cannon 预填充, Bullet 与 Cannon 共享配置 )
	PKG::CatchFish::Configs::Cannon* cfg = nullptr;

	int InitCascade(void* const& o) noexcept override;
	virtual int Update(int const& frameNumber) noexcept override;
	~Bullet();

#ifdef CC_TARGET_PLATFORM
	cocos2d::Sprite* body = nullptr;
#endif
	virtual void DrawInit() noexcept override;
	virtual void DrawUpdate() noexcept override;
	virtual void DrawDispose() noexcept override;
};
using Bullet_s = std::shared_ptr<Bullet>;
using Bullet_w = std::weak_ptr<Bullet>;

/**************************************************************************************************/
// todo: more
/**************************************************************************************************/

// todo: more 






/**************************************************************************************************/
// CatchFish
/**************************************************************************************************/

struct CatchFish {
	CatchFish();
	CatchFish(CatchFish&& o) = default;
	CatchFish(CatchFish const& o) = delete;
	CatchFish& operator=(CatchFish&& o) = default;
	CatchFish& operator=(CatchFish const& o) = delete;

	PKG::CatchFish::Configs::Config_s cfg;
	xx::List<Player_s> players;
	Scene_s scene;
	Player_w selfPlayer;				// 指向玩家自己的 player

	int Init(std::string cfgName);
	int Update();
	PKG::CatchFish::Way_s MakeBeeline(int const& itemRadius);
	Fish_s MakeRandomFish();

	bool disposed = false;
	void Dispose(int const& flag = 1) noexcept;
	~CatchFish();
};
using CatchFish_s = std::shared_ptr<CatchFish>;


/**************************************************************************************************/
// hpp includes
/**************************************************************************************************/

#include "CatchFish.hpp"
#include "CatchFish_MakeBeeline.hpp"
#include "CatchFish_Update.hpp"
#include "CatchFish_Init.hpp"
#include "CatchFish_MakeRandomFish.hpp"

#include "SpriteFrame.hpp"
#include "Physics.hpp"
#include "Scene.hpp"
#include "Player.hpp"
#include "Fish.hpp"
#include "Cannon.hpp"
#include "Bullet.hpp"
