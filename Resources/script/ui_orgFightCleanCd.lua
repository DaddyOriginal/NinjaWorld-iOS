require "LuaSubView.lua"
require "RLRequest"
require "LuaXml.lua"
require "util/localizable"

--组织BOSS消除战斗CD
fightCleanCd=class(
		"fightCleanCd",
    function()
        return CCLayer:create() 
    end
)

fightCleanCd.m_touchPriority = kCCMenuHandlerPriority-1;

function fightCleanCd_onTouch(event, x, y)
     if event == "began" then   
        return true
    end
end

function fightCleanCd:create()
	local view = fightCleanCd.new();
	return view;
end

function fightCleanCd:loadCCBI()
	local view = LuaSubView:create();	
	local win = CCDirector:sharedDirector():getWinSize();
	view:LoadCCBI("activity/FightBossClearCd.ccbi",CCSize(768,win.height));
	self:addChild(view)
	view:setPosition(CCPoint(win.width / 2, win.height / 2));
	self.m_dlgview = view;
end

function fightCleanCd:initUI()		
	self:loadCCBI()
	self:BindControl()
	
	local function updateLeftTimeLabel(fDeltaTime)
		self.deltatime = self.deltatime + fDeltaTime
		if self.deltatime >= 1 then
			if self.m_resttime > 0 then
				self.m_resttime = self.m_resttime - 1
			end
			self:settimelabel(self.m_resttime)

			local intPart, floatPart = math.modf(self.deltatime)
			self.deltatime = floatPart
		end
	end
	
	self.deltatime = 0
	self.m_timelabel = tolua.cast(self.m_dlgview:getNode("label_time"), "CCLabelTTF")
	self.m_timelabel:scheduleUpdateWithPriorityLua(updateLeftTimeLabel, 0)
	
	self:settimelabel(self.m_resttime)
	local coststr = string.format(localizable.fightBoss_cost_clearCD_tip_desc, tostring(self.m_cost))
	tolua.cast(self.m_dlgview:getNode("label_desc"), "CCLabelTTF"):setString(coststr)
end

function fightCleanCd:settimelabel(_sec)
	local time = tools.convertTimeToTable(_sec)
	
	if _sec <= 0 then
		tolua.cast(self.m_dlgview:getNode("label_free_container"), "CCNode"):setVisible(true)
		tolua.cast(self.m_dlgview:getNode("label_cost_container"), "CCNode"):setVisible(false)
		return
	end

	local timestr
	timestr = string.format(localizable.mora_time_arg_2, time.m, time.s)
	tolua.cast(self.m_dlgview:getNode("label_free_container"), "CCNode"):setVisible(false)
	tolua.cast(self.m_dlgview:getNode("label_cost_container"), "CCNode"):setVisible(true)
	tolua.cast(self.m_dlgview:getNode("label_time"), "CCLabelTTF"):setString(timestr)
	return timestr
end

function fightCleanCd:setData(resttime, cost, bossView)
	self.m_resttime = resttime
	self.m_bossView = bossView
	self.m_cost = cost
	return nil
end

function fightCleanCd:BindControl()
	fightCleanCd.m_selfview:setTouchEnabled(true)
	fightCleanCd.m_selfview:registerScriptTouchHandler(fightCleanCd_onTouch,false,kCCMenuHandlerPriority-1,true)
	fightCleanCd.m_selfview:setTouchMode(0) 
	
	self.m_confirmBtn = tolua.cast(self.m_dlgview:getNode("leftButton"), "CCControlButton")
	self.m_cancelBtn = tolua.cast(self.m_dlgview:getNode("rightButton"), "CCControlButton")
	self.m_closeBtn = tolua.cast(self.m_dlgview:getNode("closeButton"), "CCControlButton")

	-- 初始化按钮
	self.m_confirmBtn:setTouchPriority(fightCleanCd.m_touchPriority);
	self.m_dlgview:handleButtonEvent(self.m_confirmBtn, function(button, event)
		self:ClickPayClearCd();
		return nil
	end, CCControlEventTouchUpInside)
	
	self.m_cancelBtn:setTouchPriority(fightCleanCd.m_touchPriority);
	self.m_dlgview:handleButtonEvent(self.m_cancelBtn, function(button, event)
		self:CloseView();
		return nil
	end, CCControlEventTouchUpInside)
	
	self.m_closeBtn:setTouchPriority(fightCleanCd.m_touchPriority);
	self.m_dlgview:handleButtonEvent(self.m_closeBtn, function(button, event)
		self:CloseView();
		return nil
	end, CCControlEventTouchUpInside)
end

function fightCleanCd:CloseView()    
	fightCleanCd.m_selfview:removeFromParentAndCleanup(true);
end

function fightCleanCd:ClickPayClearCd()	
	if self.m_resttime <= 0 then
		--self.m_bossView:setFightResttime(0)
		self:CloseView()
		return
	end
	
    
	local playerMgr = CPlayerDataMgr:instance()
	local playerData = playerMgr:GetPlayerInfoData()
	
	if playerData.m_gold < self.m_cost then
		GetMainMenu():ShowTextTip(localizable.fightBoss_god_not_enough_desc,-1)
		--通用付费引导
		local prePayLayer = createObj(ui_commonPrePay)
		GetMainMenu():GetModelLayer():AddDialog(prePayLayer.node_, 3)
		return
	end
	
	local uid = playerData.m_uid
	local urlpath = GetUrlNormalHeader(uid,9,"rl_x_group_boss")
	GetMainMenu():ShowLoadingDlg();
	CCHttpRequest:openWithUserData(urlpath, kHttpPost, p, "query=param1&other=params"):sendWithHandler(
	function(res, hnd)
		local p = res:getHttpRequest():getUserData()
		local resData = res:getResponseData();			
		local code = res:getResponseCode()
		local xfile = xml.parse(resData)
		local item = xfile:find("RENLONG")
		if item == nil then
			GetMainMenu():CloseLoadding();
			return nil
		end
		local retcode = item.code
		if retcode == "0" then			
		        local cost_cash = tonumber(item:find("cost_cash")[1])						
				CPlayerDataMgr:instance():AddGold(-self.m_cost)
				if self.m_bossView ~= nil then
					self.m_bossView:setFightResttime(0,cost_cash)
				end
				
				self:CloseView()
			GetMainMenu():CloseLoadding();				
		else
			GetMainMenu():ShowErrorTip(tonumber(retcode),-1)
			GetMainMenu():CloseLoadding();
		end
		
	end)
end



