----------------------------------------------------------------------
--  Copyright (c) 2014-2016, XCKOO. All Rights Reserved.
--  Author :Tango
--  Time   :2015/5/16 11:41:07
--  Remark :通灵
----------------------------------------------------------------------

module("ui_summonLayer", package.seeall)
baseClass(layer_base_t, ui_summonLayer)

require('ui_layer/ui_summonAni')

function init(self)
	self.playerMgr_ = CPlayerDataMgr:instance()
	self.playerData_ = self.playerMgr_:GetPlayerInfoData()

	self.contentSize_ = GetMainMenu():GetSubContentNode():getContentSize()

	local ccbiAttrTable = { name = "activity/SummonView.ccbi", size = self.contentSize_ }
	layer_base_t.init(self, true, ccbiAttrTable)

	-- 活动剩余时间
	self.m_resttime = 0

	-- 活动状态(0开启、1结束)
	self.m_state = 0
	-- 时间增量
	self.deltatime = 0
	-- my
	self.cost = { once = 0, top = 0 }
	self.score = { cur = '0', history = '0' }
	self.curStep = 0

	self.awards = { }

	self.lastRequestTime = 0

	self:init_ui()
	self:init_binding_event()
end


function init_ui(self)
	-- 初始化界面信息
	if self.proxy_ ~= nil then
		self.label_gold = tolua.cast(self.proxy_:getNode("label_gold"), "CCLabelBMFont")
		self.spr_leftTime = tolua.cast(self.proxy_:getNode("spr_lefttime"), "CCSprite")
		self.label_lefttime = tolua.cast(self.proxy_:getNode("label_leftTime"), "CCLabelBMFont")
		self.label_end_desc = tolua.cast(self.proxy_:getNode("label_end_desc"), "CCLabelTTF")
		self.labelCostOnce = tolua.cast(self.proxy_:getNode("label_costOnce"), "CCLabelBMFont")
		self.labelCostTen = tolua.cast(self.proxy_:getNode("label_costTen"), "CCLabelBMFont")
		self.labelCostTop = tolua.cast(self.proxy_:getNode("label_costTop"), "CCLabelBMFont")
		self.labelScore = tolua.cast(self.proxy_:getNode("label_curScore"), "CCLabelBMFont")

		for i = 1, 5 do
			self['spr_ball_' .. i] = tolua.cast(self.proxy_:getNode('spr_ball_' .. i), "CCSprite")
		end


		self.btn_rank = tolua.cast(self.proxy_:getNode("btn_rank"), "CCControlButton")

		self.btn_back = tolua.cast(self.proxy_:getNode("btn_back"), "CCControlButton")
		self.btn_dice_icon = tolua.cast(self.proxy_:getNode("btn_dice_icon"), "CCSprite")
		self.btn_light_icon = tolua.cast(self.proxy_:getNode("btn_light_icon"), "CCSprite")
		-- btn
		self.btnOnce = tolua.cast(self.proxy_:getNode("btn_once"), "CCControlButton")
		self.btnTen = tolua.cast(self.proxy_:getNode("btn_ten"), "CCControlButton")
		self.btnTop = tolua.cast(self.proxy_:getNode("btn_top"), "CCControlButton")
		self.btnTips = tolua.cast(self.proxy_:getNode("btn_detail"),"CCControlButton")
		self.btnPreview = tolua.cast(self.proxy_:getNode("btn_preview"),"CCControlButton")

		self.label_gold:setString(tostring(self.playerData_.m_gold))

		for i = 1, 5 do
			self['spr_item2_' .. i] = tolua.cast(self.proxy_:getNode('spr_item2_' .. i), "CCSprite")
			self["btn_item2_" .. i] = tolua.cast(self.proxy_:getNode("btn_item2_" .. i), "CCControlButton")
			self["label_item_name2_" .. i] = tolua.cast(self.proxy_:getNode("label_item_name2_" .. i), "CCLabelTTF")
		end

		self.container = tolua.cast(self.proxy_:getNode("node_container"),"CCNode")

		self:requestBaseInfo()
	end
end

function requestBaseInfo(self)
	--- [[
	local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 14, "rl_r_comm")
	GetMainMenu():ShowLoadingDlg()
	CCHttpRequest:open(urlpath, kHttpPost, "query=param1&other=params"):sendWithHandler(
	function(res, hnd)
		GetMainMenu():CloseLoadding();
		local resData = res:getResponseData()
		local code = res:getResponseCode()
		local xfile = xml.parse(resData)
		local item = xfile:find("RENLONG")
		if item == nil then
			-- cclog("CGI : rl_r_comm is down!")
			return nil
		end
		local retcode = item.code
		if retcode == "0" then
			local itemInfo = item:find("basic")
			-- 剩余时间
			self.m_resttime = tonumber(item:find("remain")[1])

			self.cost.once = tonumber(item:find("cost")[1])
			self.cost.top = tonumber(item:find("total_cost")[1])

			-- 当前宝珠
			self.curStep = tonumber(item:find("cur_id")[1])

			-- 积分
			self.score.cur = item:find("cur_score")[1]
			self.score.history = item:find("his_score")[1]

			self.awards = item:find("awards")

			-- 显示活动时间信息
			self:updateUI()
		else
			self.label_lefttime:setString(localizable.ui_monopoly_end)
			GetMainMenu():ShowTextTip(localizable.ui_monopoly_not_start, -1)
		end
	end )
end

function updateUI(self)
	-- 实时更新活动时间
	local function updateLeftTimeLabel(fDeltaTime)
		self.deltatime = self.deltatime + fDeltaTime
		if self.deltatime >= 1 then
			local intPart, floatPart = math.modf(self.deltatime)
			self.m_resttime = self.m_resttime - intPart
			if self.m_resttime > 0 then
				local timeStr = tools.convertTimeElectronicWatch(self.m_resttime, 3)
				self.label_lefttime:setString(timeStr)
				self.deltatime = floatPart
			else
				self.m_state = 1
				self:updateUI()
				self.label_lefttime:unscheduleUpdate()
			end
		end
	end

	if self.m_resttime > 0 then
		self.m_state = 0
		self.label_lefttime:scheduleUpdateWithPriorityLua(updateLeftTimeLabel, 0)
		self.label_lefttime:setString(tools.convertTimeElectronicWatch(self.m_resttime, 3))
	else
		self.m_state = 1
		self.label_lefttime:setString(localizable.ui_monopoly_end)
	end

	self.playerData_ = self.playerMgr_:GetPlayerInfoData()
	self.label_gold:setString(tostring(self.playerData_.m_gold))

	self.labelCostOnce:setString(self.cost.once)
	self.labelCostTen:setString(self.cost.once * 10)
	self.labelCostTop:setString(self.cost.top)
	self.labelScore:setString(self.score.cur .. '/' .. self.score.history)

	-- 活动结束？
	if self.m_resttime > 0 then
		self.spr_leftTime:setVisible(true)
		self.label_lefttime:setVisible(true)
		self.label_end_desc:setVisible(false)
	else
		self.spr_leftTime:setVisible(false)
		self.label_lefttime:setVisible(false)
		self.label_end_desc:setVisible(true)
	end

	self:setAwards()

	self:updateBalls()

end


function setAwards(self)
	for i = 1, 5 do
		local item = self.awards[i]
		if item == nil then
			break
		end
		local sprIcon = getSpriteByProps(item.icon)
		if sprIcon then
			sprIcon:setPosition(46, 46)
			sprIcon:setAnchorPoint(ccp(0.5, 0.5))
			self['spr_item2_' .. i]:addChild(sprIcon)
		end

		-- self["btn_item1_" ..i] = tolua.cast(self.proxy_:getNode("btn_item1_" ..i), "CCControlButton")
		self["label_item_name2_" .. i]:setString(item.score .. localizable.ui_summon_text1)

	end
end

function requestOnce(self)
	self.lastRequestTime = os.time()

	local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 19, "rl_w_comm")
	GetMainMenu():ShowLoadingDlg()
	CCHttpRequest:open(urlpath, kHttpPost, "query=param1&other=params"):sendWithHandler(
	function(res, hnd)
		GetMainMenu():CloseLoadding();
		local resData = res:getResponseData()
		local code = res:getResponseCode()
		local xfile = xml.parse(resData)
		local item = xfile:find("RENLONG")
		if item == nil then
			-- cclog("CGI : rl_r_comm is down!")
			return nil
		end
		local retcode = item.code
		if retcode == "0" then
			local awardXML = item:find("award")
			-- 只加入背包/显示掉落动画	
			ShowAward(awardXML)

			self.curStep = tonumber(item:find('cur_id')[1])
			self:updateBalls()

			self.score.cur = tonumber(item:find('cur_score')[1])
			self.score.history = tonumber(item:find('his_score')[1])
			self.labelScore:setString(self.score.cur .. '/' .. self.score.history)

			self.playerMgr_:AddGold(- self.cost.once)
			self.label_gold:setString(tostring(self.playerData_.m_gold))
		else
			GetMainMenu():ShowErrorTip(tonumber(retcode), -1)
		end
	end )
end

function requestTen(self)
	local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 22, "rl_w_comm")
	GetMainMenu():ShowLoadingDlg()
	CCHttpRequest:open(urlpath, kHttpPost, "query=param1&other=params"):sendWithHandler(
	function(res, hnd)
		GetMainMenu():CloseLoadding();
		local resData = res:getResponseData()
		local code = res:getResponseCode()
		local xfile = xml.parse(resData)
		local item = xfile:find("RENLONG")
		if item == nil then
			-- cclog("CGI : rl_r_comm is down!")
			return nil
		end
		local retcode = item.code
		if retcode == "0" then
			self.curStep = tonumber(item:find('cur_id')[1])
			self:updateBalls()

			local awardList = item:find("awards")
			for i = 1, #awardList do
				ShowAward(awardList[i])
			end
			
--			local awardXML = item:find("award")
--			-- 只加入背包/显示掉落动画	
--			ShowAward(awardXML)

			self.score.cur = tonumber(item:find('cur_score')[1])
			self.score.history = tonumber(item:find('his_score')[1])
			self.labelScore:setString(self.score.cur .. '/' .. self.score.history)

			self.playerMgr_:AddGold(- self.cost.once * 10)
			self.label_gold:setString(tostring(self.playerData_.m_gold))

		else
			GetMainMenu():ShowErrorTip(tonumber(retcode), -1)
		end
	end )
end

function requestTop(self)
	local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 21, "rl_w_comm")
	GetMainMenu():ShowLoadingDlg()
	CCHttpRequest:open(urlpath, kHttpPost, "query=param1&other=params"):sendWithHandler(
	function(res, hnd)
		GetMainMenu():CloseLoadding();
		local resData = res:getResponseData()
		local code = res:getResponseCode()
		local xfile = xml.parse(resData)
		local item = xfile:find("RENLONG")
		if item == nil then
			-- cclog("CGI : rl_r_comm is down!")
			return nil
		end
		local retcode = item.code
		if retcode == "0" then
			self.curStep = 5
			self:updateBalls()

			local awardXML = item:find("award")
			-- 只加入背包/显示掉落动画	
			ShowAward(awardXML)

			self.score.cur = tonumber(item:find('cur_score')[1])
			self.score.history = tonumber(item:find('his_score')[1])
			self.labelScore:setString(self.score.cur .. '/' .. self.score.history)

			self.playerMgr_:AddGold(- self.cost.top)
			self.label_gold:setString(tostring(self.playerData_.m_gold))
		else
			GetMainMenu():ShowErrorTip(tonumber(retcode), -1)
		end
	end )
end

-- 控制请求的频率
function checkFrequency(self)
	local ret = true
	local now = os.time()
	if now - self.lastRequestTime < 2 then
		ret = false
	end

	return ret
end
function updateBalls(self)
	CCSpriteFrameCache:sharedSpriteFrameCache():addSpriteFramesWithFile('ccbResources/summon.plist')
	for i = 1, 5 do
		local frameName = ''
		if i <= self.curStep then
			frameName = tostring(i)
		else
			frameName = '0'
		end
		local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(frameName)
		self['spr_ball_' .. i]:setDisplayFrame(frame)
	end
end

function init_binding_event(self)
	if self.proxy_ ~= nil then
		local function onBtnOnce(btn)
			if not self:checkFrequency() then
				return
			end
			self.lastRequestTime = os.time()
			-- 判断活动状态
			if self.m_state == 1 then
				GetMainMenu():ShowTextTip(localizable.ui_monopoly_not_start, -1)
				return nil
			end

			if self.playerData_.m_gold < self.cost.once then
				-- 提示购买元宝
				GetMainMenu():ShowTextTip(localizable.ui_monopoly_gold_not_enough, -1)
				-- 通用付费引导
				local prePayLayer = createObj(ui_commonPrePay)
				GetMainMenu():GetModelLayer():AddDialog(prePayLayer.node_, 3)
				return nil
			end

			local onEnd = function (  )
				self.m_animLayer.node_:removeFromParentAndCleanup(true)
				self:requestOnce()
			end

			self:playerAnim(onEnd)
		end

		local function onBtnTen(btn)
			if not self:checkFrequency() then
				return
			end
			self.lastRequestTime = os.time()
			-- 判断活动状态
			if self.m_state == 1 then
				GetMainMenu():ShowTextTip(localizable.ui_monopoly_not_start, -1)
				return nil
			end

			if self.playerData_.m_gold < tonumber(self.cost.once * 10) then
				-- 提示购买元宝
				GetMainMenu():ShowTextTip(localizable.ui_monopoly_gold_not_enough, -1)
				-- 通用付费引导
				local prePayLayer = createObj(ui_commonPrePay)
				GetMainMenu():GetModelLayer():AddDialog(prePayLayer.node_, 3)
				return nil
			end

			local onEnd = function (  )
				self.m_animLayer.node_:removeFromParentAndCleanup(true)
				self:requestTen()
			end

			self:playerAnim(onEnd)
		end

		local function onBtnTop(btn)
			if not self:checkFrequency() then
				return
			end
			self.lastRequestTime = os.time()
			-- 判断活动状态
			if self.m_state == 1 then
				GetMainMenu():ShowTextTip(localizable.ui_monopoly_not_start, -1)
				return nil
			end

			if self.playerData_.m_gold < tonumber(self.cost.once * 10) then
				-- 提示购买元宝
				GetMainMenu():ShowTextTip(localizable.ui_monopoly_gold_not_enough, -1)
				-- 通用付费引导
				local prePayLayer = createObj(ui_commonPrePay)
				GetMainMenu():GetModelLayer():AddDialog(prePayLayer.node_, 3)
				return nil
			end

			local onEnd = function (  )
				self.m_animLayer.node_:removeFromParentAndCleanup(true)
				self:requestTop()
			end

			self:playerAnim(onEnd)
		end

		local function onClickedAward(btn, i)
			self:requestExchange(i)
		end

		-- 判断是否点击格子
		for i = 1, 5 do
			self.proxy_:handleButtonEvent(self["btn_item2_" .. i], function(button, event)
				onClickedAward(button, i)
				return nil
			end , CCControlEventTouchUpInside)
		end

		self.proxy_:handleButtonEvent(self.btnOnce, function(button, event)
			onBtnOnce(button)
			return nil
		end , CCControlEventTouchUpInside)

		self.proxy_:handleButtonEvent(self.btnTen, function(button, event)
			onBtnTen(button)
			return nil
		end , CCControlEventTouchUpInside)

		self.proxy_:handleButtonEvent(self.btn_back, function(button, event)
			GetMainMenu():ChangeToSub(E_DEFAULTMENU)
			return nil
		end , CCControlEventTouchDown)

		self.proxy_:handleButtonEvent(self.btnTop, function(button, event)
			onBtnTop(button)
			return nil
		end , CCControlEventTouchUpInside)

		self.proxy_:handleButtonEvent(self.btnTips, function(button, event)
			require("ui_layer/ui_summonTips")
			showModelLayer(ui_summonTips)
			return nil
		end , CCControlEventTouchUpInside)

		self.proxy_:handleButtonEvent(self.btnPreview, function(button, event)
			require("ui_layer/ui_summonAwardPreview")
			showModelLayer(ui_summonAwardPreview)
			return nil
		end , CCControlEventTouchUpInside)

	end
end

function requestExchange(self, index)
	local id = tonumber(self.awards[index].id)

	local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 20, "rl_w_comm")
	urlpath = AddData(urlpath, "ID", id)
	GetMainMenu():ShowLoadingDlg()
	CCHttpRequest:open(urlpath, kHttpPost, "query=param1&other=params"):sendWithHandler(
	function(res, hnd)
		GetMainMenu():CloseLoadding();
		local resData = res:getResponseData()
		local code = res:getResponseCode()
		local xfile = xml.parse(resData)
		local item = xfile:find("RENLONG")
		if item == nil then
			-- cclog("CGI : rl_r_comm is down!")
			return nil
		end
		local retcode = item.code
		if retcode == "0" then
			local awardXML = item:find("award")
			-- 只加入背包/显示掉落动画	
			ShowAward(awardXML)

			self.score.cur = tonumber(item:find('cur_score')[1])
			self.score.history = tonumber(item:find('his_score')[1])
			self.labelScore:setString(self.score.cur .. '/' .. self.score.history)
		elseif retcode == '590001' then
			GetMainMenu():ShowTextTip(localizable.ui_summon_text5, -1)
		elseif retcode == '590002' then
			GetMainMenu():ShowTextTip(localizable.ui_summon_text6, -1)
		else
			GetMainMenu():ShowErrorTip(tonumber(retcode), -1)
		end
	end )
end

function playerAnim(self,onEnd)
	--播放light动画
	---[[	
	local pre_animLayer = self.container:getChildByTag(100)
	if pre_animLayer then
		pre_animLayer:removeFromParentAndCleanup(true)
	end

	local animNodeSize = self.container:getContentSize()
	self.m_animLayer = createObj(ui_summonAni, animNodeSize)

	self.m_animLayer.node_:ignoreAnchorPointForPosition(false)
	self.container:addChild(self.m_animLayer.node_)

	self.m_animLayer.node_:setPosition(ccp(0,0))
	self.m_animLayer.node_:setAnchorPoint(ccp(0, 0))
	self.m_animLayer.node_:setTag(100)
	--]]

	local ccArray = CCArray:create()
    ccArray:addObject(CCDelayTime:create(0.5))
    ccArray:addObject(CCCallFuncN:create(onEnd))
	local sequen = CCSequence:create(ccArray)  

    --播放动画
	self.container:runAction(sequen)
end



function onNodeCleanup(self)
	if self.proxy_ then
		self.proxy_:release()
	end
	layer_base_t.onNodeCleanup(self)
end
