----------------------------------------------------------------------
--  Copyright (c) 2014-2016, XCKOO. All Rights Reserved.
--  Author :Tango
--  Time   :2015/1/15 17:34:52
--  Remark :超忍卡片层
----------------------------------------------------------------------

module("ui_superNinjaCard", package.seeall)
baseClass(layer_base_t, ui_superNinjaCard)


function init(self, _size, data)
	local ccbininja_typeTable = {name="activity/SuperNinjaCard.ccbi", size = _size}
	layer_base_t.init(self, true, ccbininja_typeTable)

	self.data = data
	self:init_ui()
	self:init_binding_event()
end


function init_ui(self)
	if self.proxy_ ~= nil then

		self.ninja_id = self.data.ninja_id
		--获取忍者信息
		self.ninjaIcon = nil
		self.ninjaFrame = nil
		self.quality = nil
		self.ninjaIcon, self.ninjaFrame, self.quality = rl_get_iconsprite("1", "1", E_FRAMETYPE_MIDDLE, self.data.ninja_id)

		if self.ninjaIcon ~= nil then
			self.ninja_ninja_type        = tolua.cast(self.proxy_:getNode("ninja_attr"), "CCSprite")
			self.node_ninja        = tolua.cast(self.proxy_:getNode("node_ninja"), "CCSprite")

			for i = 1, 5 do
				self["sprite_star" .. tostring(i)] = tolua.cast(self.proxy_:getNode("sprite_star" .. tostring(i)), "CCSprite")
				self["sprite_star" .. tostring(i)]:setVisible(false)
			end

			self.ninjaFrame = rl_get_ninjaFrame(E_FRAMETYPE_MIDDLE, self.quality)
			if self.ninjaFrame ~= nil then
				self.node_ninja:setDisplayFrame(self.ninjaFrame)
			end

			local size = self.node_ninja:getContentSize()
			self.ninjaIcon:setPosition(size.width/2, size.height/2)
			self.ninjaIcon:setScale(0.6)
			self.node_ninja:removeAllChildrenWithCleanup(true)
			self.node_ninja:addChild(self.ninjaIcon)

			local ninjaType = tonumber(self.data.ninja_type)
			if ninjaType == 1 then--攻
				self.ninja_typeStr = "com_text_attack_small_icon"
			elseif ninjaType == 0 then--防
				self.ninja_typeStr = "com_text_defense_small_icon"
			elseif ninjaType == 3 then--神
				self.ninja_typeStr = "com_text_god_small_icon"
			end
			local pFrameNinjaninja_type = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(tostring(self.ninja_typeStr))
			self.ninja_ninja_type:setDisplayFrame(pFrameNinjaninja_type)

			if self.quality > 5 then
				self.quality = 5
			end
			for i=1,self.quality do
				self["sprite_star" .. tostring(i)]:setVisible(true)
			end
		end
	end
end

function init_binding_event(self)

end

function onNodeCleanup(self)
    if self.proxy_ then
    	self.proxy_:release()
    end
    layer_base_t.onNodeCleanup(self)
end
