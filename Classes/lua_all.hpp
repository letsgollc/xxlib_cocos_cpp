﻿#pragma once

#ifndef var
#define var decltype(auto)
#endif

// todo: 优化函数名和使用, 考虑参考 cocos lua 框架代码提供 return self 以便连写

#include "lua_keys.hpp"
#include "lua_funcholder.hpp"
#include "lua_pushxxx.hpp"
#include "lua_newxxx.hpp"
#include "lua_toxxx.hpp"

#include "lua_cc_ref.hpp"
#include "lua_cc_node.hpp"
#include "lua_cc_scene.hpp"
#include "lua_cc_touch.hpp"
#include "lua_cc_event.hpp"
#include "lua_cc_eventlisteners.hpp"
//#include "lua_cc_spriteframe.hpp"
#include "lua_cc_sprite.hpp"
#include "lua_cc_texturecache.hpp"
#include "lua_cc_texture.hpp"
#include "lua_cc.hpp"

#include "lua_xx_mempool.hpp"
#include "lua_xx_object.hpp"
#include "lua_xx_bbuffer.hpp"
#include "lua_xx_uvloop.hpp"
#include "lua_xx_uvtcpclient.hpp"
#include "lua_xx.hpp"

#include "lua_init.hpp"
//...
