----------------------------------------------------------------------
--  Copyright (c) 2014-2016, XCKOO. All Rights Reserved.
--  Author :Tango
--  Time   :2015/5/21 16:30:08
--  Remark :通灵动画
----------------------------------------------------------------------
module("ui_summonAni", package.seeall)
baseClass(layer_base_t, ui_summonAni)

function init(self, parentSize, index)
	local ccbiAttrTable = {name="animations/summonAni.ccbi", size=parentSize}
	layer_base_t.init(self, true, ccbiAttrTable)
end

function onNodeCleanup(self)
	--cclog("1111---001:")
	if self.proxy_ then
    	self.proxy_:release()
    end
    layer_base_t.onNodeCleanup(self)
end