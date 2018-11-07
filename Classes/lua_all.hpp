﻿#pragma once

#include "lua.hpp"

#ifndef var
#define var decltype(auto)
#endif

// todo: 优化函数名和使用, 考虑参考 cocos lua 框架代码提供 return self 以便连写

// 用于临时装载从 lua 获取的参数
inline cocos2d::Vector<cocos2d::FiniteTimeAction*> gActions;
inline std::vector<std::string> gStrings;

#include "lua_keys.hpp"
#include "lua_func.hpp"
#include "lua_pushcall.hpp"
#include "lua_new_xxx.hpp"
#include "lua_to_xxx.hpp"

#include "lua_xx_mempool.hpp"
#include "lua_xx_object.hpp"
#include "lua_xx_bbuffer.hpp"
#include "lua_xx_uvloop.hpp"
#include "lua_xx_uvtcpclient.hpp"
#include "lua_xx.hpp"

#include "lua_cc_data.hpp"
#include "lua_cc_ref.hpp"
#include "lua_cc_actions.hpp"
#include "lua_cc_node.hpp"
#include "lua_cc_label.hpp"
#include "lua_cc_scene.hpp"
#include "lua_cc_touch.hpp"
#include "lua_cc_event.hpp"
#include "lua_cc_eventlisteners.hpp"
#include "lua_cc_layer.hpp"
#include "lua_cc_spriteframe.hpp"
#include "lua_cc_sprite.hpp"
#include "lua_cc_scrollview.hpp"
#include "lua_cc_texture.hpp"

#include "lua_cc_uiwidget.hpp"
#include "lua_cc_uibutton.hpp"
#include "lua_cc_uieditbox.hpp"
#include "lua_cc_uiimageview.hpp"
#include "lua_cc.hpp"

#include "lua_cca.hpp"

#include "lua_init.hpp"
//...
