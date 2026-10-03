----------------------------------------------------------------------
--  Copyright (c) 2014-2016, XCKOO. All Rights Reserved.
--  Author :Tango
--  Time   :2015/1/8 17:33:14
--  Remark :扫荡奖励
----------------------------------------------------------------------

module("ui_sweepAwardCell", package.seeall)
baseClass(layer_base_t, ui_sweepAwardCell)

function init(self, cellSize, data)
	local ccbiAttrTable = {name="sub_ui/SweepAwardCell.ccbi", size=cellSize}
	layer_base_t.init(self, true, ccbiAttrTable)

	self.m_itemlist = data

	self:init_ui()
end

function init_ui(self)
	if self.proxy_ ~= nil then
		for i=1,4 do
			self["btn_icon" .. tostring(i)] = tolua.cast(self.proxy_:getNode("btn_icon" .. tostring(i)), "CCControlButton")	
			self["spr_icon_"..tostring(i)] = tolua.cast(self.proxy_:getNode("spr_icon_" .. tostring(i)), "CCSprite")
			self["label_item_num_"..tostring(i)] = tolua.cast(self.proxy_:getNode("label_item_num_"..tostring(i)), "CCLabelBMFont")
			self["label_item"..tostring(i)] = tolua.cast(self.proxy_:getNode("label_item"..tostring(i)), "CCLabelTTF")
		end

		self:init_ext_ui()
	end
end

function clearData(self )
	for i=1,4 do
		self["btn_icon" .. tostring(i)]:setVisible(false)
		self["spr_icon_"..tostring(i)]:setVisible(false)
		self["label_item_num_"..tostring(i)]:setVisible(false)
		self["label_item"..tostring(i)]:setVisible(false)
	end
end

function init_ext_ui(self)
	self:clearData()
	--init
	CCSpriteFrameCache:sharedSpriteFrameCache():addSpriteFramesWithFile("ccbResources/player_rank.plist")
	if self.m_itemlist then
		for i=1,#self.m_itemlist do
			item = self.m_itemlist[i]

			local _t_card = {}
			_t_card.pIcon, _t_card.pFrame, _t_card.quality, _t_card.objname = rl_get_iconsprite(item.maintype, item.subtype, E_FRAMETYPE_SMALL, item.id)

			self["spr_icon_"..tostring(i)]:setVisible(true)
			if nil ~= _t_card.pFrame then
				self["spr_icon_"..tostring(i)]:setDisplayFrame(_t_card.pFrame)
			end

			if nil ~= _t_card.pIcon then
				---[[
				self["spr_icon_"..tostring(i)]:addChild(_t_card.pIcon)
				--cclog("%s _icon = %s", _t_card.objname, _t_card.pIcon)

				local size = self["spr_icon_"..tostring(i)]:getContentSize()
				_t_card.pIcon:setPosition(ccp(size.width * 0.5, size.height * 0.5))
				_t_card.pIcon:setAnchorPoint(ccp(0.5, 0.5))
				--]]
			end

			if item.num > 1 then
				self["label_item_num_"..tostring(i)]:removeFromParentAndCleanup(false)
				self["spr_icon_"..tostring(i)]:addChild(self["label_item_num_"..tostring(i)])
				self["label_item_num_"..tostring(i)]:setVisible(true)
				self["label_item_num_"..tostring(i)]:setString(item.num)
			else
				self["label_item_num_"..tostring(i)]:setVisible(false)
			end

			self["label_item"..tostring(i)]:setString(_t_card.objname)
			self["label_item"..tostring(i)]:setVisible(true)

		end
	end
end

function onNodeCleanup(self)
    layer_base_t.onNodeCleanup(self)
end