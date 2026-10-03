
require("ui_layer/ui_superNinjaCard")

module("ui_superNinjaChallengeLayer", package.seeall)
baseClass(layer_base_t, ui_superNinjaChallengeLayer)

local scales = {1,1.5,1}
local opactiy = {128,255,128}

function init(self, node)
	self.contentNode_ = GetActivityView():GetNodeContent()
	self.contentSize_ = self.contentNode_:getContentSize()

	local ccbiAttrTable = {name="activity/SuperNinjaChallenge.ccbi", size=self.contentSize_}
	layer_base_t.init(self, true, ccbiAttrTable)

	--用户info
	self.playerMgr_ = CPlayerDataMgr:instance()
	self.playerData_ = self.playerMgr_:GetPlayerInfoData()

	--pre node
	self.preNode = node
	--时间增量
	self.deltatime = 0
	--活动状态
	self.m_state = 0

	--touch
	self.m_touchPoint = nil

	self.cards = {}
	self.posList = {}
	self.items = {}

	self.difficulty = 0
	self.sellectedItem = nil

	self.isAnimating = false

	self:init_ui()
	self:init_binding_event()
end

function init_ui(self)
	if self.proxy_ ~= nil then
		--label
		self.labelTimeTips = tolua.cast(self.proxy_:getNode("label_time2"), "CCLabelBMFont")
		self.label_act_time = tolua.cast(self.proxy_:getNode("label_time"), "CCLabelTTF")
		self.labelFight = tolua.cast(self.proxy_:getNode("label_fight"), "CCLabelTTF")
		self.labelFragment = tolua.cast(self.proxy_:getNode("label_fragment"), "CCLabelTTF")
		self.labelLeftChallenge = tolua.cast(self.proxy_:getNode("label_challengeTimes"), "CCLabelTTF")
		self.labelCost = tolua.cast(self.proxy_:getNode("label_cost"), "CCLabelTTF")

		--btn
		self.btnPre = tolua.cast(self.proxy_:getNode("btn_pre"), "CCControlButton")
		self.btnNext = tolua.cast(self.proxy_:getNode("btn_next"), "CCControlButton")
		for i=1,3 do
			self["btn_tab" .. i] = tolua.cast(self.proxy_:getNode("btn_tab" .. i), "CCControlButton")
			self["btn_tab" .. i]:setTag(i)
		end

		self.btnChallenge = tolua.cast(self.proxy_:getNode("btn_challenge"),"CCControlButton")


		for i=1,3 do
			local card = tolua.cast(self.proxy_:getNode("node_card"..i), "CCNode")
			local x,y = card:getPosition()
			table.insert(self.posList,ccp(x,y))
		end

		--node
		self.cardContainer = tolua.cast(self.proxy_:getNode("node_card_container"), "CCNode")

		--init
		self:pre_base_info()
	end
end

function turnPage( self, isForward )
	if self.isAnimating then
		return
	end
	if isForward then --  向右翻
		local card = table.remove(self.cards)
		table.insert(self.cards,1, card)
		self.cardContainer:reorderChild(card.node_,1)-- 滚动前方的第一个卡片放到最底层
	else
		local card = table.remove(self.cards, 1)
		table.insert(self.cards, card)
		self.cardContainer:reorderChild(card.node_,1)-- 滚动前方的第一个卡片放到最底层
	end

	local animationNumer = 0
	local function actionFinished()
		animationNumer = animationNumer + 1
		if animationNumer == 3 then
			self:reorderCards()

			self:updateUI()
			self.isAnimating = false
		end
	end

	for i=1,3 do
		local moveTo = CCMoveTo:create(0.5,self.posList[i])
		local scaleTo = CCScaleTo:create(0.5, scales[i])

		local ccArraySpawn = CCArray:create()
    	ccArraySpawn:addObject(moveTo)
    	ccArraySpawn:addObject(scaleTo)
    	local ccSpawn = CCSpawn:create(ccArraySpawn)

    	local moveFinishCall = CCCallFuncN:create(actionFinished)
    	local moveSeq = CCSequence:createWithTwoActions(ccSpawn, moveFinishCall)
    	self.cards[i].node_:runAction(moveSeq)
	end

	self.isAnimating = true
end

function reorderCards( self )
	self.cardContainer:reorderChild(self.cards[2].node_,3) -- 中间的卡片放到顶层
	self.cardContainer:reorderChild(self.cards[1].node_,2)
	self.cardContainer:reorderChild(self.cards[3].node_,2)
end

function pre_base_info(self)
	local function updateLeftTimeLabel(fDeltaTime)
		self.deltatime = self.deltatime + fDeltaTime
		if self.deltatime >= 1 then
			local intPart, floatPart = math.modf(self.deltatime)
			self.restTime = self.restTime - intPart
			if self.restTime > 0 then
				local timeStr = tools.convertTimeElectronicWatchHaveDay(self.restTime, 3)
				self.label_act_time:setString(timeStr)
				self.deltatime = floatPart
				self.labelTimeTips:setVisible(true)
			else
				self.m_state = 1
				self.label_act_time:setString(localizable.ui_monopoly_end)
				self.label_act_time:unscheduleUpdate()
				self.labelTimeTips:setVisible(false)
			end
		end
	end
	--请求基本信息
	local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 1, "rl_x_ninja_challenge")
	--cclog("rl_x_ninja_challenge & cmd = 1---%s", urlpath)
	GetMainMenu():ShowLoadingDlg()
	CCHttpRequest:open(urlpath, kHttpPost, "query=param1&other=params"):sendWithHandler(
		function(res, hnd)
			GetMainMenu():CloseLoadding()
			local resData = res:getResponseData()
			local code = res:getResponseCode()
			local xfile = xml.parse(resData)
			local item = xfile:find("RENLONG")
			if item == nil then
				return nil
			end
			cclog("%s", resData)
			local retcode = item.code
			if retcode == "0" then
				--basic
				self.restTime = tonumber(item:find("expire")[1])
				if self.restTime > 0 then
					self.label_act_time:scheduleUpdateWithPriorityLua(updateLeftTimeLabel, 0)
					self.label_act_time:setString(tools.convertTimeElectronicWatchHaveDay(self.restTime, 3))
					self.labelTimeTips:setVisible(false)
				else
					self.m_state = 1
					self.label_act_time:setString(localizable.ui_monopoly_end)
					self.labelTimeTips:setVisible(false)
				end

				self.items = item:find("items")

				self:init_ui_ext()
			else
				GetMainMenu():ShowErrorTip(tonumber(retcode),-1)
			end
		end)
end

function init_ui_ext(self)
	-- 从9个item里提取三张不同的忍者卡，生成卡片
	local lastId = 0
	local count = 0
	for i = 1, #self.items do
		if self.items[i].ninja_id ~= lastId then
			count  = count + 1
			local card = createObj(ui_superNinjaCard,self.cardContainer:getContentSize(), self.items[i])
			card.node_:setPosition(self.posList[count].x,self.posList[count].y)
			card.node_:setAnchorPoint(ccp(0.5, 0.5))
			self.cardContainer:addChild(card.node_)
			table.insert(self.cards,card)

			lastId = self.items[i].ninja_id
		end
	end

	self:reorderCards()
	local midCard = self.cards[2]
	midCard.node_:setScale(scales[2])
	
	self:setDifficulty(1)
end

function updateUI( self )
	local selectedNinja = self.cards[2].ninja_id

	local item = self:findItem(selectedNinja, self.difficulty)
	if nil == item then
		return
	end

	local info = DataMgr.GetDataByID("Struct_Consumeinfo", tonumber(item.propid))
	if info then
		local costnum = tonumber(item.propnum)
		local caststr = info.m_namestr .. " * " ..costnum
		self.labelCost:setString(caststr)
	end

	self.labelFight:setString(item.ninja_pk)
	self.labelFragment:setString(item.desc)
	self.labelLeftChallenge:setString(item.times)

	self.sellectedItem = item
end

function findItem( self, ninjaid, difficulty )
	for i=1,#self.items do
		if self.items[i].ninja_id == ninjaid and tonumber(self.items[i].type) == difficulty then
			return self.items[i]
		end
	end

	return nil
end

function setDifficulty(self, index )
	if index < 1 or index > 3 then
		return
	end

	for i=1,3 do
		self["btn_tab"..i]:setEnabled(i ~= index)
	end

	self.difficulty = index

	self:updateUI()
end

function requestInfo(self)
	local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 1, "rl_x_ninja_challenge")
	--cclog("rl_x_ninja_challenge & cmd = 1---%s", urlpath)
	GetMainMenu():ShowLoadingDlg()
	CCHttpRequest:open(urlpath, kHttpPost, "query=param1&other=params"):sendWithHandler(
		function(res, hnd)
			GetMainMenu():CloseLoadding()
			local resData = res:getResponseData()
			local code = res:getResponseCode()
			local xfile = xml.parse(resData)
			local item = xfile:find("RENLONG")
			if item == nil then
				return nil
			end
			cclog("%s", resData)
			local retcode = item.code
			if retcode == "0" then
				--basic
				self.restTime = tonumber(item:find("expire")[1])
				self.items = item:find("items")

				self:updateUI()
			else
				GetMainMenu():ShowErrorTip(tonumber(retcode),-1)
			end
		end)
end

function onClickedChallenge( self )
	if self.m_state == 1 then
		GetMainMenu():ShowTextTip(localizable.ui_monopoly_end, -1)
		return
	end

	if self.sellectedItem == nil then
		return
	end

	if tonumber(self.sellectedItem.times) < 1 then
		GetMainMenu():ShowTextTip(localizable.ui_superNinjaChallenge1, -1)
		return
	end

	--请求基本信息
	local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 2, "rl_x_ninja_challenge")
	urlpath = AddData(urlpath, "ChallengeID", self.sellectedItem.id)
	--cclog("rl_x_ninja_challenge & cmd = 1---%s", urlpath)
	GetMainMenu():ShowLoadingDlg()
	CCHttpRequest:open(urlpath, kHttpPost, "query=param1&other=params"):sendWithHandler(
		function(res, hnd)
			GetMainMenu():CloseLoadding()
			local resData = res:getResponseData()
			local code = res:getResponseCode()
			local xfile = xml.parse(resData)
			local item = xfile:find("RENLONG")
			if item == nil then
				return nil
			end
			cclog("%s", resData)
			local retcode = item.code
			if retcode == "0" then
				--显示战斗过程动画
				--GetMainMenu():ShowArenaView(resData)
				local fightinfo = item:find("fight")
				local resultData = CFightResultData:instance()
				resultData:Clear()
				InitFightXML(fightinfo, resultData)
				resultData:SetFightType(11)
				
				local awardXML
				local hasAward = false
				for i = 1, #item do
					if item[i][0] == "award" then
						hasAward = true
						awardXML = item[i]
					end
				end
				if hasAward == true then
					InitAwardData(resultData:GetRewardData(),awardXML)
					CPlayerDataMgr:instance():AddDataFromReward(resultData:GetRewardData())
				end
				
				local fightview = CRoundFightView:new()
				fightview:init()
				GetMainMenu():addChild(fightview,5)
				fightview:Start()
				fightview:release()

				self:requestInfo()
			else
				GetMainMenu():ShowErrorTip(tonumber(retcode),-1)
			end
		end)

end

function init_binding_event(self)
	if self.proxy_ ~= nil then
		self.btnPre:setTouchPriority(kCCMenuHandlerPriority-1)
		self.btnPre:setTouchEnabled(true)
		self.proxy_:handleButtonEvent(self.btnPre, function(button, event)
			self:turnPage(false)
			return nil
		end, CCControlEventTouchUpInside)

		self.btnNext:setTouchPriority(kCCMenuHandlerPriority-1)
		self.btnNext:setTouchEnabled(true)
		self.proxy_:handleButtonEvent(self.btnNext, function(button, event)
			self:turnPage(true)
			return nil
		end, CCControlEventTouchUpInside)

		self.btnChallenge:setTouchPriority(kCCMenuHandlerPriority-1)
		self.proxy_:handleButtonEvent(self.btnChallenge, function ( button, event )
			self:onClickedChallenge()
		end, CCControlEventTouchDown)

		for i=1,3 do
			local btn = self["btn_tab"..i]
			btn:setTouchPriority(kCCMenuHandlerPriority-1)
			self.proxy_:handleButtonEvent(btn, function ( button, event )
				self:setDifficulty(i)
			end, CCControlEventTouchUpInside)
		end

		local function onClickedCard( i )
			if i <= 0 or i > 3 then return end
			CGameObjElement:ShowCommonItemDetail(1, 1, self.cards[i].ninja_id, "")
		end

		local function CCLayerTouch(event, x, y)
			if event == "began" then
				for i=1,3 do
					local card = tolua.cast(self.proxy_:getNode("node_card"..i), "CCNode")
					local rect = card:boundingBox()
					rect.origin = ccp(0,0)
					local p = card:convertToNodeSpace(ccp(x,y))
					if rect:containsPoint(p) == true then
						onClickedCard(i)
						break
					end
				end
				return false
			end
		end

		self.node_:setTouchEnabled(true)
		self.node_:registerScriptTouchHandler(CCLayerTouch, false, kCCMenuHandlerPriority-1, true)
		---
	end
end

function refreshData(self)
	self:pre_base_info()
end

function onNodeCleanup(self)
    --cclog("onNodeCleanup")
    if self.proxy_ then
    	self.proxy_:release()
    end
    layer_base_t.onNodeCleanup(self)
end

function createTestData(self)
	--testData
	return nil
end