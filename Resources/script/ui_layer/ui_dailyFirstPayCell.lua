----------------------------------------------------------------------
--  Copyright (c) 2014-2016, XCKOO. All Rights Reserved.
--  Author :Tango
--  Time   :2015/5/25 22:05:02
--  Remark :每日首充
----------------------------------------------------------------------
module("ui_dailyFirstPayCell", package.seeall)
baseClass(layer_base_t, ui_dailyFirstPayCell)

function init(self, cellSize, data)
	local ccbiAttrTable = { name = "activity/DailyFirstPayCell.ccbi", size = cellSize }
	self.playerMgr_ = CPlayerDataMgr:instance()
	self.playerData_ = self.playerMgr_:GetPlayerInfoData()
	layer_base_t.init(self, true, ccbiAttrTable)

	self.cellData = data
	self:init_ui()
	self:init_binding_event()
end

function init_ui(self)
	if self.proxy_ ~= nil then
		-- label
		self.label_item_title = tolua.cast(self.proxy_:getNode("label_item_title"), "CCLabelTTF")

		for i = 1, 4 do
			self["label_item_name" .. i] = tolua.cast(self.proxy_:getNode("label_item_name" .. i), "CCLabelTTF")
			self["btn_item_" .. i] = tolua.cast(self.proxy_:getNode("btn_item_" .. i), "CCControlButton")
			self["sprite_item" .. i] = tolua.cast(self.proxy_:getNode("sprite_item" .. i), "CCSprite")
		end

		self.btnGet = tolua.cast(self.proxy_:getNode("btn_get"), "CCControlButton")
		self.spr_got = tolua.cast(self.proxy_:getNode("sprite_hasgot"), "CCSprite")

		-- show info
		self:showInfo()
	end
end

--
function showInfo(self)
	self.label_item_title:setString(string.format(localizable.ui_dailyFirstPay_text1, self.cellData.amount))
	--0 无奖励
	--1 可领取
	--2 已领取
	local state = tonumber(self.cellData.status)
	if 0 == state then
		self.spr_got:setVisible(false)
		self.btnGet:setVisible(true)
		setBtnTitle(self.btnGet,localizable.ui_dailyFirstPay_text3)
	elseif 1 == state then
		self.spr_got:setVisible(false)
		self.btnGet:setVisible(true)
		setBtnTitle(self.btnGet,localizable.ui_dailyFirstPay_text4)
	else
		self.spr_got:setVisible(true)
		self.btnGet:setVisible(false)
	end

	--- [[
	for i = 1, 4 do
		local _icon = self["sprite_item" .. tostring(i)]:getChildByTag(99)
		if _icon then
			_icon:removeFromParentAndCleanup(true)
		end

		if self.cellData['drop' .. i] ~= '0' then
			-- icon/frame
			local _maintype = 0
			local _subtype = 0
			local _id = -1
			local _num = -1
			local _drop_type = 0
			local _obj_info = { }
			_maintype, _subtype, _id, _num, _drop_type = setObjTypeInfo(tonumber(self.cellData['drop' .. i]))
			_obj_info.pIcon, _obj_info.pFrame, _obj_info.quality, _obj_info.objname = rl_get_iconsprite(_maintype, _subtype, E_FRAMETYPE_SMALL, _id)
			if nil ~= _obj_info.pFrame then
				self["sprite_item" .. tostring(i)]:setDisplayFrame(_obj_info.pFrame)
			end
			-- 合集icon用发过来的,防止集合出现问题
			if _drop_type == 1 then
				local pathName = "props/" .. self.cellData['icon' .. i] .. ".plist"
				CCSpriteFrameCache:sharedSpriteFrameCache():addSpriteFramesWithFile(pathName)
				local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(self.cellData['icon' .. i])
				if frame ~= nil then
					_obj_info.pIcon = CCSprite:createWithSpriteFrame(frame)
				end
			end

			if nil ~= _obj_info.pIcon then
				self["sprite_item" .. tostring(i)]:addChild(_obj_info.pIcon)
				local size = self["sprite_item" .. tostring(i)]:getContentSize()
				_obj_info.pIcon:setPosition(ccp(size.width * 0.5, size.height * 0.5))
				_obj_info.pIcon:setAnchorPoint(ccp(0.5, 0.5))
				_obj_info.pIcon:setTag(99)
				if _drop_type ~= 1 then
					_obj_info.pIcon:setScale(0.9)
				end
			end

			self["label_item_name" .. tostring(i)]:setVisible(true)
			self["label_item_name" .. tostring(i)]:setString(tostring(self.cellData['name' .. i]))
		else
			local pFrameNoGiftBK = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("small_no_obj_frame")
			self["sprite_item" .. tostring(i)]:setDisplayFrame(pFrameNoGiftBK)
			self["label_item_name" .. tostring(i)]:setVisible(false)
		end
	end
	-- ]]
end

function init_binding_event(self)
	-- 奖励详情
	local function onCLickedIcon(i)
		if self.cellData['drop' .. i] == nil or self.cellData['drop' .. i] == '0' then
			return nil
		end

		local _id = tonumber(self.cellData['drop' .. i])
		if nil ~= _id then
			CGameObjElement:ShowDropByID(_id)
		end
	end

	 for i = 1, 4 do
	 	self.proxy_:handleButtonEvent(self["btn_item_" .. i], function(button, event)
	 		onCLickedIcon(i)
	 		return nil
	 	end , CCControlEventTouchUpInside)
	 end

end

function onNodeCleanup(self)
	--- [[
	if self.proxy_ then
		self.proxy_:release()
		self.proxy_ = nil
	end
	-- ]]
	layer_base_t.onNodeCleanup(self)
end