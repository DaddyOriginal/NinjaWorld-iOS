-- descriptioin:闯关扫荡
-- company: xckoo
-- litao
-- 2014.5.6
---------------------------------------------
module("ui_towerSweepLayer", package.seeall)
baseClass(layer_base_t, ui_towerSweepLayer)

require("ui_layer/ui_sweepAwardLayer")

function init(self, node, cur_index)
	self.contentSize_ = GetMainMenu():GetModelLayer():getContentSize()

	local ccbiAttrTable = { name = "sub_ui/TowerSweepView.ccbi", size = self.contentSize_ }
	layer_base_t.init(self, true, ccbiAttrTable)

	-- 用户info
	self.playerMgr_ = CPlayerDataMgr:instance()
	self.playerData_ = self.playerMgr_:GetPlayerInfoData()

	-- pre info
	self.preNode = node

	-- tableView cell container
	self.cellNodes = { }
	-- data
	self.m_levelDatas = { }
	-- sweep_selected
	self.m_selectedDatas = { }
	--
	self.m_awardDatas = { }
	-- touch
	self.m_touchPoint = nil
	-- is all selected
	self.isAllSelected = false

	-- create data
	-- self:createData()

	self.deltatime = 0
	self.goalRound = 0
	self.totalTime = 0
	self.costCashPerStep = 0
	self.remainTime = 0

	-- init
	self:init_ui()
	self:init_binding_event()
end

function init_ui(self)
	if self.proxy_ ~= nil then
		-- label
		self.labelTitle = tolua.cast(self.proxy_:getNode("label_title"), "CCLabelTTF");
		self.labelTime = tolua.cast(self.proxy_:getNode("label_time"), "CCLabelTTF");
		self.labelVipTips = tolua.cast(self.proxy_:getNode("label_needvipLv"), "CCLabelTTF");
		

		-- btn
		self.btnLeft = tolua.cast(self.proxy_:getNode("leftButton"), "CCControlButton")
		self.btnRight = tolua.cast(self.proxy_:getNode("rightButton"), "CCControlButton")
		self.btn_close_dlg = tolua.cast(self.proxy_:getNode("closeButton"), "CCControlButton")

		-- edit
		self.spr_input_lv = tolua.cast(self.proxy_:getNode("spr_input_level"), "CCScale9Sprite")

		self.sprVipLv = tolua.cast(self.proxy_:getNode("spr_vip_lv"),"CCSprite")

		self:initEditBox()

		-- get info
		self:requestLayerInfo()
	end
end

function onInputDone(self)
	local lv = tonumber(self.editLv:getText())
	if lv == nil or lv < 0 then
		GetMainMenu():ShowTextTip(localizable.ui_orgDonate_input, -1)
		return nil
	end

	if lv > self.maxRound then
		GetMainMenu():ShowTextTip(string.format(localizable.ui_towerSweepLvTooBig, self.maxRound), -1)
		self.editLv:setText(self.maxRound)
		return nil
	end

	self.goalRound = lv
	self.totalTime = self.costTime *(self.goalRound - self.curRound + 1)

	self.labelTime:setString(tools.convertTimeElectronicWatchChinese(self.totalTime, 3))
end

function initEditBox(self)
	-- inti edit box
	local function editLvEventHandler(eventType, sender)
		local strFmt
		if eventType == "began" then
			-- triggered when an edit box gains focus after keyboard is shown
			strFmt = string.format("editBox began !")
			cclog("editboxEventHandler began = %s", strFmt)
		elseif eventType == "ended" then
			-- triggered when an edit box loses focus after keyboard is hidden.
			strFmt = string.format("editBox DidEnd !")
			cclog("editboxEventHandler ended = %s", strFmt)

			self:onInputDone()
		elseif eventType == "changed" then
			-- triggered when the edit box text was changed.
			strFmt = string.format("editBox changed !")
			cclog("editboxEventHandler changed = %s", strFmt)
		elseif eventType == "return" then
			-- triggered when the return button was pressed or the outside area of keyboard was touched.
			strFmt = string.format("editBox return !")
			cclog("editboxEventHandler return = %s", strFmt)
		end
	end

	-- lv
	local ptx, pty = self.spr_input_lv:getPosition()
	local size = self.spr_input_lv:getContentSize()

	local parentNode = self.spr_input_lv:getParent()
	self.spr_input_lv:removeFromParentAndCleanup(true)

	self.editLv = CCEditBox:create(size, self.spr_input_lv)
	self.editLv:registerScriptEditBoxHandler(editLvEventHandler)
	parentNode:addChild(self.editLv, 1)

	self.editLv:setPosition(ccp(ptx + size.width / 2, pty))
	self.editLv:setAnchorPoint(ccp(0.5, 0.5))
	self.editLv:setMaxLength(20)
	self.editLv:setInputMode(kEditBoxInputModeSingleLine)
	-- 任何文本_不包括换行
	self.editLv:setReturnType(kKeyboardReturnTypeDone)
	self.editLv:setFontColor(ccc3(0, 0, 0))
	-- ccBLACK)

	self.editLv:setTouchPriority(kCCMenuHandlerPriority - 1)
	self.editLv:setTouchEnabled(true)
end

function requestLayerInfo(self)
	local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 7005, "rl_r_dup")
	-- cclog("rl_r_dup----%s", urlpath)

	GetMainMenu():ShowLoadingDlg()
	CCHttpRequest:open(urlpath, kHttpPost, "query=param1&other=params"):sendWithHandler(
	function(res, hnd)
		GetMainMenu():CloseLoadding();
		local resData = res:getResponseData()
		local code = res:getResponseCode()
		local xfile = xml.parse(resData)
		local item = xfile:find("RENLONG")
		if item == nil then
			return nil
		end
		local retcode = item.code
		if retcode == "0" then
			local root = item:find("dup_root")

			local dup_count = root:find("dup_count")
			if dup_count then
				self.maxRound = tonumber(dup_count:find("max_round")[1])
				self.curRound = tonumber(dup_count:find("current_round")[1])
				self.costTime = tonumber(dup_count:find("cost_time")[1])
				self.minVip = tonumber(dup_count:find("min_vip_level")[1])
				self.costCashPerStep = tonumber(dup_count:find("cost_cash")[1])
			end

			local status = root:find("dup_status")
			if status then
				self.status = tonumber(status:find("status")[1])
				self.remainTime = tonumber(status:find("remain_time")[1])
				self.goalRound = tonumber(status:find("end_round")[1])
			end

			self:init_ext_ui()
		else
			GetMainMenu():ShowErrorTip(tonumber(retcode), -1)
		end
	end )
end

function init_ext_ui(self)
	local function updateTime(fDeltaTime)
		self.deltatime = self.deltatime + fDeltaTime
		if self.deltatime >= 1 then
			if 0 == self.remainTime then
				self.labelTime:setString("0")
			end

			local intPart, floatPart = math.modf(self.deltatime)
			self.remainTime = self.remainTime - intPart
			if self.remainTime > 0 then
				local timeStr = tools.convertTimeElectronicWatchChinese(self.remainTime, 3)
				self.labelTime:setString(timeStr)
				self.deltatime = floatPart
			else
				self.labelTime:setString("0")
				self.labelTime:unscheduleUpdate()

				GetMainMenu():ShowTextTip(localizable.ui_towerSweepCompleted, -1)
				
				local function requestAward(  )
					self:closeDlg()
				end

				-- 延迟一秒关闭界面同时申请奖励信息
				local ccArray = CCArray:create()
				ccArray:addObject(CCDelayTime:create(1))
				ccArray:addObject(CCCallFuncN:create(requestAward))
				local sequen = CCSequence:create(ccArray)
				self.node_:runAction(sequen)
			end
		end
	end

	if self.status == 0 then
		self:setVipIcon(self.minVip)

		setBtnTitle(self.btnLeft, localizable.ui_towerSweepbtnOk)
		setBtnTitle(self.btnRight, localizable.ui_towerSweepBtnThink)

		self.goalRound = self.maxRound

		self.labelTime:unscheduleUpdate();
		self.totalTime = self.costTime *(self.goalRound - self.curRound + 1)
		self.labelTime:setString(tools.convertTimeElectronicWatchChinese(self.totalTime, 3))

		self.labelTitle:setString(localizable.ui_towerSweepTitle0);
	else
		self:setVipIcon(nil)
		setBtnTitle(self.btnLeft, localizable.ui_towerSweepBtnCompleteNow)
		setBtnTitle(self.btnRight, localizable.ui_towerSweepBtnCancel)

		self.labelTime:scheduleUpdateWithPriorityLua(updateTime, 0)
		self.labelTitle:setString(localizable.ui_towerSweepTitle1);
	end

	if self.editLv ~= nil then
		self.editLv:setText(self.goalRound)
		self.editLv:setTouchEnabled(self.status == 0)-- 只有等于0时才可输入
	end

end

function setVipIcon(self,vipLv)
	if vipLv == nil then
		self.labelVipTips:setVisible(false)
		self.sprVipLv:setVisible(false)
	else
		self.labelVipTips:setVisible(true)
		self.sprVipLv:setVisible(true)
		local vipFrames = {"vip_015","vip_003","vip_004","vip_005","vip_006",
			"vip_007","vip_008","vip_009","vip_010","vip_011","vip_012","vip_013",
			"vip_014","vip_s_13","vip_s_14","vip_s_15","vip_s_16","vip_s_17","vip_s_18"}
	
		local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(vipFrames[vipLv+1])
		self.sprVipLv:setDisplayFrame(frame)
	end
end

function requestSweep(self, toLv)
	local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 3104, "rl_w_dup")
	urlpath = AddData(urlpath, "EndRound", toLv)
	-- cclog("rl_w_dup---%s", urlpath)
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
		-- cclog("%s", resData)
		local retcode = item.code
		if retcode == "0" then
			self:requestLayerInfo()
		else
			GetMainMenu():ShowErrorTip(tonumber(retcode), -1)
		end
	end )
end

function requestCompleteNow(self, gold)
	local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 3106, "rl_w_dup")
	-- cclog("rl_w_dup---%s", urlpath)
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
		-- cclog("%s", resData)
		local retcode = item.code
		if retcode == "0" then
			require("activitySubmenHelper.lua")
			ShowSweepAward(self,resData)
			GetMainMenu():ShowTextTip(localizable.ui_towerSweepCompleted, -1)

			self.playerMgr_:AddGold(-gold)

			self:closeDlg()
		else
			GetMainMenu():ShowErrorTip(tonumber(retcode), -1)
		end
	end )
end

function requestCalcelSweep(self)
	local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 3107, "rl_w_dup")
	-- cclog("rl_w_dup---%s", urlpath)
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
		-- cclog("%s", resData)
		local retcode = item.code
		if retcode == "0" then
			self:requestLayerInfo()
		else
			GetMainMenu():ShowErrorTip(tonumber(retcode), -1)
		end
	end )
end

function closeDlg(self)
	--GetMainMenu():ChangeToSub(E_DEFAULTMENU)
	GetMainMenu():refreshTowerSweepLayer()
	self.node_:removeFromParentAndCleanup(true)
end

function init_binding_event(self)
	if self.proxy_ ~= nil then
		--屏蔽掉后层触摸事件
		local function CCLayerTouch(event, x, y)
			if event == "began" then
				return true
			end
		end

		self.node_:setTouchEnabled(true)
		self.node_:registerScriptTouchHandler(CCLayerTouch, false, kCCMenuHandlerPriority-1, true)

		local function onBtnLeft(btn, event)
			CSoundMgr:instance():PlayEffect(SOUND_BUTTON)

			if self.status == 0 then --扫荡
				-- win32平台不能触发输入结束事件，主动模拟
				if GetPlatformStr() == "Win" then
					self:onInputDone()
				end

				if self.minVip > self.playerMgr_:GetPlayerInfoData().m_viplevel then
					GetMainMenu():ShowTextTip(localizable.vip_list_vipLv_notEnough, -1)
					return nil
				end
				self:requestSweep(self.goalRound)
			elseif self.status == 1 then -- 立即完成
				-- 根据还需要扫荡的凑数计算需要花费的元宝
				local costCash = (self.goalRound - self.curRound + 1) * self.costCashPerStep

				local function doNow()
					if self.playerData_.m_gold < costCash then
						--提示购买元宝
						GetMainMenu():ShowTextTip(localizable.ui_monopoly_gold_not_enough,-1)
						--通用付费引导
						local prePayLayer = createObj(ui_commonPrePay)
						GetMainMenu():GetModelLayer():AddDialog(prePayLayer.node_, 3)
						return nil
					end
					self:requestCompleteNow(costCash)
				end

				local dlg = CommonDialogView.create()
				CommonDialogView.m_selfview = dlg
				dlg:SetTitle(localizable.ui_rouletteLayer_title)
				local tips = string.format(localizable.ui_towerSweepTipsCompleteNow,costCash) -- 计算花费
				dlg:SetDescription(tips)
				dlg:loadCCBI()
				dlg:initUI()
				dlg:SetConfirmHandler(doNow)
				GetMainMenu():GetModelLayer():AddDialog(dlg, 3)
			end	
		end
		
		function onBtnRight( btn, event )
			CSoundMgr:instance():PlayEffect(SOUND_BUTTON)
			if self.status == 0 then -- 再想想
				self:closeDlg()
			elseif self.status == 1 then -- 取消扫荡
				local function doCancel()
					self:requestCalcelSweep()
				end
		
				local dlg = CommonDialogView.create()
				CommonDialogView.m_selfview = dlg
				dlg:SetTitle(localizable.ui_rouletteLayer_title)
				dlg:SetDescription(localizable.ui_towerSweepTipsCancelSweep)
				dlg:loadCCBI()
				dlg:initUI()
				dlg:SetConfirmHandler(doCancel)
				GetMainMenu():GetModelLayer():AddDialog(dlg, 3)
			end
		end
		
		self.btn_close_dlg:setTouchPriority(kCCMenuHandlerPriority-1)
		self.btn_close_dlg:setTouchEnabled(true)
		self.proxy_:handleButtonEvent(self.btn_close_dlg, function(button, event)
			self:closeDlg()
			return nil
		end, CCControlEventTouchDown)

		self.btnRight:setTouchPriority(kCCMenuHandlerPriority-1)
		self.btnRight:setTouchEnabled(true)
		self.proxy_:handleButtonEvent(self.btnRight, function(button, event)
			onBtnRight(button)
			return nil
		end, CCControlEventTouchDown)

		self.btnLeft:setTouchPriority(kCCMenuHandlerPriority - 1)
		self.btnLeft:setTouchEnabled(true)
		self.proxy_:handleButtonEvent(self.btnLeft, function(button, event)
			onBtnLeft(button)
			return nil
		end, CCControlEventTouchDown)
	end
end

function onNodeCleanup(self)
	if self.proxy_ then
    	self.proxy_:release()
    end

    layer_base_t.onNodeCleanup(self)
end

--创建测试数据
function createData(self)
	---[[
	local t_level_datas = {}
	for i=1, 25 do
		local t_item = {}
		t_item.id = i
		t_item.bSelected = false
		t_item.name = tostring("木叶大劫")
		t_item.select_status = 2
		table.insert(t_level_datas, t_item)
	end	

	--重新排列数据结构/每4个作一个子table
	self.m_levelDatas = {}
	local index = 0
	for i = 1, #t_level_datas do
		local inpart, decimal = tools.getIntAndDecimal(i / 4)
		if decimal == 0 then
			index = inpart
		else
			index = inpart + 1
		end
		self.m_levelDatas[index] = self.m_levelDatas[index] or {}
		table.insert(self.m_levelDatas[index], t_level_datas[i])
	end
	--]]
end