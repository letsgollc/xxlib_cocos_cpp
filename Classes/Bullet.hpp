﻿inline int Bullet::InitCascade(void* const& o) noexcept {
	scene = (Scene*)o;
	assert(player);
	assert(cannon);
	assert(cfg);

	// todo: calc pos, angle

	// 计算炮口坐标
	pos = cannon->pos + xx::Rotate(xx::Pos{ cfg->muzzleLen ,0 }, cannon->angle * (float(M_PI) / 180.0f));

	// 角度沿用炮台的( 在发射前炮台已经调整了角度 )
	angle = cannon->angle;

	// 计算出每帧移动增量
	moveInc = xx::Rotate(xx::Pos{ cfg->distance ,0 }, angle * (float(M_PI) / 180.0f));

	// 存储发射时炮台的倍率
	coin = cannon->coin;

	if (int r = this->BaseType::InitCascade(o)) return r;
	DrawInit();
	return 0;
}

inline int Bullet::Update(int const& frameNumber) noexcept {
	pos += moveInc;
	auto&& w = ::ScreenCenter.x + cfg->maxRadius;
	auto&& h = ::ScreenCenter.y + cfg->maxRadius;
	if (pos.x > w || pos.x < -w || pos.y > h || pos.y < -h) return -1;
	// todo: hit check

	DrawUpdate();
	return 0;
};

inline Bullet::~Bullet() {
	DrawDispose();
}

#pragma region

inline void Bullet::DrawInit() noexcept {
#ifdef CC_TARGET_PLATFORM
	assert(!body);
	body = cocos2d::Sprite::create();
	body->retain();
	body->setGlobalZOrder(cfg->zOrder);
	auto&& sf = xx::As<SpriteFrame>(cfg->frames->At(1))->spriteFrame;
	body->setSpriteFrame(sf);
	body->setPosition(pos);
	body->setScale(cfg->scale);
	body->setRotation(-angle);
	cc_scene->addChild(body);
#endif
}

inline void Bullet::DrawUpdate() noexcept {
#ifdef CC_TARGET_PLATFORM
	assert(body);
	body->setRotation(-angle);
	body->setPosition(pos);
#endif
}

inline void Bullet::DrawDispose() noexcept {
#ifdef CC_TARGET_PLATFORM
	if (!body) return;

	if (body->getParent()) {
		body->removeFromParent();
	}
	body->release();
	body = nullptr;
#endif
}

#pragma endregion
