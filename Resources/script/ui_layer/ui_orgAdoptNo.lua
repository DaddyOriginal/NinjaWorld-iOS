----------------------------------------------------------------------
--  Copyright (c) 2014-2016, XCKOO. All Rights Reserved.
--  Author :lyk
--  Time   :2015-11-04
--  Remark :兽栏（没有尾兽状态）
----------------------------------------------------------------------
module("ui_orgAdoptNo", package.seeall)
baseClass(layer_base_t, ui_orgAdoptNo)

function init(self,boss_list,archite)
    cclog("ui_orgAdoptNo__init_start")
	self.playerMgr_ = CPlayerDataMgr:instance()
	self.playerData_ = self.playerMgr_:GetPlayerInfoData()

	self.contentSize_ = GetMainMenu():GetSubContentNode():getContentSize()
	local ccbiAttrTable = { name = "sub_ui/OrgAdoptNo.ccbi", size = self.contentSize_ }
	layer_base_t.init(self, true, ccbiAttrTable)

    cclog("ui_orgAdoptNo__init_end")
	-- pre page
	self.back_page = E_DEFAULTMENU

	-- data
	self.boss_list = boss_list
    self.archite = archite
	-- init
	self:init_ui()
	self:init_binding_event()
end

function init_ui(self)
	if self.proxy_ ~= nil then
		initHeader(self.proxy_)
		-- node	
		-- btn
		self.btn_back = tolua.cast(self.proxy_:getNode("btn_back"), "CCControlButton")
		self.btn_adopt = tolua.cast(self.proxy_:getNode("button_adopt"), "CCControlButton")
		self.btn_desc = tolua.cast(self.proxy_:getNode("button_desc"), "CCControlButton")		

	end
end



function init_binding_event(self)
	if self.proxy_ ~= nil then
		local function onBtnBack(btn, event)
			-- GetMainMenu():ChangeToSub(self.back_page)
			self.node_:removeFromParentAndCleanup(true)
			ShowOrgMapLayer()
		end

		local function onBtnAdopt(btn, event)
			cclog("onBtnAdopt")
            self.node_:removeFromParentAndCleanup(true)
            ShowOrgAdoptBoss(self.boss_list,self.archite)
		end

		local function onBtnDesc(btn, event)
			local monthLayer = createObj(ui_orgAdoptDesc)
		    local size1 = GetMainMenu():GetModelLayer():getContentSize()
		    monthLayer.node_:setAnchorPoint(ccp(0.5, 0.5))

		    monthLayer.node_:setPosition(size1.width / 2, size1.height / 2)
		    GetMainMenu():GetModelLayer():addChild(monthLayer.node_)
		end	

		self.btn_back:setTouchPriority(kCCMenuHandlerPriority - 1)
		self.btn_back:setTouchEnabled(true)
		self.proxy_:handleButtonEvent(self.btn_back, function(button, event)
			onBtnBack(button)
			return nil
		end , CCControlEventTouchDown)

		self.btn_adopt:setTouchPriority(kCCMenuHandlerPriority - 1)
		self.btn_adopt:setTouchEnabled(true)
		self.proxy_:handleButtonEvent(self.btn_adopt, function(button, event)
			onBtnAdopt(button)
			return nil
		end , CCControlEventTouchDown)

		self.btn_desc:setTouchPriority(kCCMenuHandlerPriority - 1)
		self.btn_desc:setTouchEnabled(true)
		self.proxy_:handleButtonEvent(self.btn_desc, function(button, event)
			onBtnDesc(button)
			return nil
		end , CCControlEventTouchDown)


	end
end

function onNodeCleanup(self)
	if self.proxy_ then
		self.proxy_:release()
	end

	layer_base_t.onNodeCleanup(self)
end
