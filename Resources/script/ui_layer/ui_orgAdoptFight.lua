----------------------------------------------------------------------
--  Copyright (c) 2014-2016, XCKOO. All Rights Reserved.
--  Author :lyk
--  Time   :2015-11-11
--  Remark :培养完成 进入狩猎界面
----------------------------------------------------------------------
module("ui_orgAdoptFight", package.seeall)
baseClass(layer_base_t, ui_orgAdoptFight)

require "PlayOnceAnimLayer.lua"
require "ui_orgFightInPasire.lua"
require "ui_orgFightCleanCd.lua"


function init(self, fightinfo)
    self.playerMgr_ = CPlayerDataMgr:instance()
    self.playerData_ = self.playerMgr_:GetPlayerInfoData()

    self.contentSize_ = GetMainMenu():GetSubContentNode():getContentSize()
    local ccbiAttrTable = { name = "sub_ui/OrgAdoptFight.ccbi", size = self.contentSize_ }
    layer_base_t.init(self, true, ccbiAttrTable)

    -- pre page
    self.back_page = E_DEFAULTMENU
    -- init
    self.deltatime_cd = 0
    self.boss_deltatime = 0
    self.show_deltatime = 0
    self.fightBossMsgImportantId = 0
    self.fightBossMsgNormalId = 0
    self.m_showIndex = 0
    self.m_showList = { }
    self.m_showInter = 1
    -- self.isdead = 0
    -- self.m_resttime = 0
    -- self.m_fight_resttime = 0
    -- data
    self.fightinfo = fightinfo
    -- init
    self:init_ui()
    self:init_binding_event()
end

function init_ui(self)
    if self.proxy_ ~= nil then
        initHeader(self.proxy_)
        -- label	
        self.label_hp = tolua.cast(self.proxy_:getNode("label_hp"), "CCLabelTTF")
        self.label_fight_add = tolua.cast(self.proxy_:getNode("label_fight_add"), "CCLabelTTF")
        self.label_cd = tolua.cast(self.proxy_:getNode("label_cd"), "CCLabelTTF")
        self.lable_left_time = tolua.cast(self.proxy_:getNode("label_left_time"), "CCLabelBMFont")
        self.label_left_time_desc = tolua.cast(self.proxy_:getNode("label_left_time_desc"), "CCLabelTTF")
        -- btn
        self.btn_back = tolua.cast(self.proxy_:getNode("btn_back"), "CCControlButton")
        self.button_fight = tolua.cast(self.proxy_:getNode("button_fight"), "CCControlButton")
        self.btn_desc = tolua.cast(self.proxy_:getNode("button_desc"), "CCControlButton")
        self.button_inspire = tolua.cast(self.proxy_:getNode("button_inspire"), "CCControlButton")

        -- sprite
        self.sprite_icon = tolua.cast(self.proxy_:getNode("sprite_icon"), "CCSprite")
        self.sprite_progress = tolua.cast(self.proxy_:getNode("sprite_progress"), "CCSprite")
        self:init_ui_data()
    end
end

function init_ui_data(self)
    if self.fightinfo then
        self.boss_pic = self.fightinfo:find("boss_pic")[1]
        -- 是否在战斗时间中
        self.is_in_fight = tonumber(self.fightinfo:find("is_in_fight")[1])
        self.begin_time = tonumber(self.fightinfo:find("begin_time")[1])
        -- 剩余时间
        self.m_resttime = tonumber(self.fightinfo:find("rest_time")[1])
        -- 元宝鼓舞加成百分比
        self.cash_add = tonumber(self.fightinfo:find("cash_add")[1])
        -- 银子鼓舞加成百分比
        self.coin_add = tonumber(self.fightinfo:find("coin_add")[1])
        self.coin_cash_total = self.cash_add + self.coin_add
        -- 剩余血量
        self.rest_life = tonumber(self.fightinfo:find("rest_life")[1])
        -- 总的血量
        self.total_life = tonumber(self.fightinfo:find("total_life")[1])
        -- 战斗CD
        self.fight_left_time = tonumber(self.fightinfo:find("fig_left_time")[1])
        -- 鼓舞每次消耗元宝
        self.cost_cash = tonumber(self.fightinfo:find("cost_cash")[1])
        -- 鼓舞每次消耗银子
        self.cost_coin = tonumber(self.fightinfo:find("cost_coin")[1])
        -- 元宝鼓舞每次增加百分比
        self.cash_add_each = tonumber(self.fightinfo:find("cash_add_each")[1])
        -- 元宝鼓舞增加百分比最大值
        self.cash_add_max = tonumber(self.fightinfo:find("cash_add_max")[1])
        -- 银子鼓舞每次增加百分比
        self.coin_add_each = tonumber(self.fightinfo:find("coin_add_each")[1])
        -- 银子鼓舞增加百分比最大值
        self.coin_add_max = tonumber(self.fightinfo:find("coin_add_max")[1])
        -- BOSS清除CD
        self.clear_cd_cost = tonumber(self.fightinfo:find("clear_cd_cost")[1])

        self:init_ui_ext()
    end
end

function init_ui_ext(self)
    if self.fightinfo then
        self.deltatime = 0
        math.randomseed(os.time())
        local function updateLeftTimeLabel(fDeltaTime)
            self.deltatime = self.deltatime + fDeltaTime
            if self.deltatime >= 1 then
                local intPart, floatPart = math.modf(self.deltatime)
                self.m_resttime = self.m_resttime - intPart
                if self.m_resttime > 0 then
                    local timeStr = tools.convertTimeElectronicWatch(self.m_resttime, 3)
                    self.lable_left_time:setString(timeStr)
                    self.deltatime = floatPart
                else
                    self.m_state = 1
                    self.lable_left_time:setString("00")
                    self.lable_left_time:unscheduleUpdate()
                    self.node_:removeFromParentAndCleanup(true)
                    ShowAdoptView()
                end
            end
        end

        local function updateBossInfo(fDeltaTime)
            self.boss_deltatime = self.boss_deltatime + fDeltaTime
            if self.boss_deltatime >= 5 then
                -- if self:isFightOK() == 1 then
                if self.rest_life > 0 then
                    self:LoadBossInfo()
                else
                    self.button_fight:unscheduleUpdate()
                end
                -- end			

                local intPart, floatPart = math.modf(self.boss_deltatime)
                self.boss_deltatime = floatPart
            end
        end

        local function updateShowList(fDeltaTime)
            self.show_deltatime = self.show_deltatime + fDeltaTime
            if self.show_deltatime >= self.m_showInter then
                if self.rest_life > 0 then
                    self:ShowFightAttackList()
                else
                    self.node_:unscheduleUpdate()
                  --  if tonumber(self.rest_life) == 0 then       
                        self.node_:removeFromParentAndCleanup(true)
                        ShowAdoptView()
                  --  end
                end

                self.show_deltatime = 0
            end
        end

        -- 倒计时等操作
        if self.is_in_fight == 1 then
            -- 战斗时间内
            self.m_state = 0
            self.lable_left_time:scheduleUpdateWithPriorityLua(updateLeftTimeLabel, 0)
            self.lable_left_time:setString(tools.convertTimeElectronicWatch(self.m_resttime, 3))
            -- 请求BOSS信息
            self.button_fight:scheduleUpdateWithPriorityLua(updateBossInfo, 0)
            -- 显示BOSS战斗信息
            self.node_:scheduleUpdateWithPriorityLua(updateShowList, 0)
        else
            self.m_state = 1
            self.label_left_time_desc:setString(localizable.ui_orgAdoptFight_text3)
            self.lable_left_time:setVisible(false)
        end

        function updateLeftTimeLabelCD(fDeltaTime)
            self.deltatime_cd = self.deltatime_cd + fDeltaTime
            if self.deltatime_cd >= 1 then
                local intPart, floatPart = math.modf(self.deltatime_cd)
                self.fight_left_time = self.fight_left_time - intPart
                if self.fight_left_time > 0 then
                    local timeStr = tools.convertTimeElectronicWatch(self.fight_left_time, 3)
                    self.label_cd:setString(timeStr)
                    self.deltatime_cd = floatPart
                else
                    -- self.m_state = 1
                    -- self:init_ui_ext()
                    self.label_cd:setString("00")
                    self.label_cd:unscheduleUpdate()
                end
            end
        end


        if self.fight_left_time > 0 then
            self.label_cd:scheduleUpdateWithPriorityLua(updateLeftTimeLabelCD, 0)
            self.label_cd:setString(tools.convertTimeElectronicWatch(self.fight_left_time, 3))
        else
            self.label_cd:setString("00")
        end

        --local add = self.coin_add + self.cash_add
        self.label_fight_add:setString("100%+" .. self.coin_cash_total .. "%")
        local pIconFrame = CGameObjElement:GetNinjaIcon(E_FRAMETYPE_MIDDLE, self.boss_pic)
        if pIconFrame then
            self.sprite_icon:setDisplayFrame(pIconFrame)
        end

        self:init_boss_hp()
    end
end

function init_boss_hp(self)
    self.label_hp:setString(self.rest_life .. "/" .. self.total_life)
    local ratio = self.rest_life / self.total_life
    self.sprite_progress:setScaleX(ratio)   
end


function LoadBossInfo(self)
    local playerMgr = CPlayerDataMgr:instance()
    local playerData = playerMgr:GetPlayerInfoData()
    local uid = playerData.m_uid
    local urlpath = GetUrlNormalHeader(uid, 8, "rl_x_group_boss")
    GetMainMenu():ShowUnvisibleLoadingDlg();
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
            self.rest_life = tonumber(item:find("restlife")[1])
            self:init_boss_hp()
            GetMainMenu():CloseLoadding();
            self:LoadFightMsg()
        else
            GetMainMenu():ShowErrorTip(tonumber(retcode), -1)
            GetMainMenu():CloseLoadding();
        end

    end )
end

function LoadFightMsg(self)
    local playerMgr = CPlayerDataMgr:instance()
    local playerData = playerMgr:GetPlayerInfoData()
    local uid = playerData.m_uid
    local urlpath = GetUrlNormalHeader(uid, 7, "rl_x_group_boss")
    urlpath = AddData(urlpath, "MsgImportID", self.fightBossMsgImportantId)
    urlpath = AddData(urlpath, "MsgNormalID", self.fightBossMsgNormalId)
    GetMainMenu():ShowUnvisibleLoadingDlg();
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
            self:parseFightData(item)
            -- self:updateUI()
            GetMainMenu():CloseLoadding();
        else
            GetMainMenu():ShowErrorTip(tonumber(retcode), -1)
            GetMainMenu():CloseLoadding();
        end

    end )
end

function parseFightData(self, data)
    self.m_showIndex = 0
    self.m_showList = { }
    --[[
    self.m_showList[1] = {nick="春爷", val="250"}
	self.m_showList[2] = {nick="林老师", val="2500"}
	self.m_showList[3] = {nick="小朱", val="250000"}
	self.m_showList[4] = {nick="土豪", val="2500000"}
	self.m_showList[5] = {nick="小胖", val="25000000"}
	self.m_showList[6] = {nick="熊猫", val="350"}
	self.m_showList[7] = {nick="辉辉", val="3500"}
	self.m_showList[8] = {nick="左左", val="35000"}
	self.m_showList[9] = {nick="宋哥", val="350000"}
	self.m_showList[10] = {nick="日总", val="3500000"}
	self.m_showList[11] = {nick="半仙", val="450"}
	self.m_showList[12] = {nick="小雨", val="4500"}
    ]]

    local itemList = data:find("import_msg")
    if itemList ~= nil then
        for i = 1, #itemList do
            table.insert(self.m_showList, itemList[i])
            local itemId = tonumber(itemList[i].id)
            if itemId > self.fightBossMsgImportantId then
                self.fightBossMsgImportantId = itemId
            end
        end
    end

    itemList = data:find("normal_msg")
    if itemList ~= nil then
        for i = 1, #itemList do
            table.insert(self.m_showList, itemList[i])
            local itemId = tonumber(itemList[i].id)
            if itemId > self.fightBossMsgNormalId then
                self.fightBossMsgNormalId = itemId
            end
        end
    end

    self.m_showInter = self:calcInterval()


end

function calcInterval(self)
    if #self.m_showList > 0 then
        local inter = 5.0 / #self.m_showList
        if inter < 0.2 then
            return 0.2
        end
        return inter
    else
        return 1
    end
end


function ShowFightAttackList(self)
    self.m_showIndex = self.m_showIndex + 1
    if self.m_showIndex > #self.m_showList then
        return
    end

    local rect = tolua.cast(self.proxy_:getNode("node_rand_pos_sequare"), "CCNode"):boundingBox()
    local randPosX = math.random(rect.origin.x, rect.origin.x + rect.size.width)
    local randPosY = math.random(rect.origin.y, rect.origin.y + rect.size.height)

    local digitView = PlayOnceAnimLayer:create()
    if digitView == nil then
        return nil
    end
    digitView:initUI("activity/ShowFightBossValue.ccbi", CCSize(280, 280), 1)
    digitView:setPosition(ccp(randPosX - 140, randPosY - 80))
    tolua.cast(digitView:getNode("label_playername"), "CCLabelTTF"):setString(self.m_showList[self.m_showIndex].nick)
    tolua.cast(digitView:getNode("label_value"), "CCLabelBMFont"):setString(self.m_showList[self.m_showIndex].val)
    self.proxy_:getNode("node_rand_pos_sequare"):getParent():addChild(digitView, 10)

     
   

end


function init_binding_event(self)
    if self.proxy_ ~= nil then
        local function onBtnBack(btn, event)
            -- GetMainMenu():ChangeToSub(self.back_page)
            self.node_:removeFromParentAndCleanup(true)
            ShowOrgMapLayer()
        end

        local function onBtnFight(btn, event)
            -- 开始战斗
            if self.m_state == 1 then
                GetMainMenu():ShowTextTip(localizable.ui_orgAdoptFight_text1, -1)
                return
            end

            if self.fight_left_time > 0 then
                self:showCdView()
            else
                self:submit_fight_info()
            end


        end

        local function onBtnDesc(btn, event)
           local monthLayer = createObj(ui_orgAdoptDesc)
		    local size1 = GetMainMenu():GetModelLayer():getContentSize()
		    monthLayer.node_:setAnchorPoint(ccp(0.5, 0.5))

		    monthLayer.node_:setPosition(size1.width / 2, size1.height / 2)
		    GetMainMenu():GetModelLayer():addChild(monthLayer.node_)
          
        end

        local function onBtnInspire(btn, event)

            if self.m_state == 1 then
               GetMainMenu():ShowTextTip(localizable.ui_orgAdoptFight_text4, -1)
               return
            end

            local dlg = fightInpasire:create()
            fightInpasire.m_selfview = dlg
            -- (silverCost, goldCost, silverUp, silverMax, goldUp, goldMax, totalUp, bossView)
            dlg:setData(self.cost_coin, self.cost_cash, self.coin_add_each, self.coin_add_max, self.cash_add_each, self.cash_add_max, self.coin_cash_total, self)

            dlg:initUI()
            GetMainMenu():AddDialog(dlg, 3);
        end

        self.btn_back:setTouchPriority(kCCMenuHandlerPriority - 1)
        self.btn_back:setTouchEnabled(true)
        self.proxy_:handleButtonEvent(self.btn_back, function(button, event)
            onBtnBack(button)
            return nil
        end , CCControlEventTouchDown)

        self.button_fight:setTouchPriority(kCCMenuHandlerPriority - 1)
        self.button_fight:setTouchEnabled(true)
        self.proxy_:handleButtonEvent(self.button_fight, function(button, event)
            onBtnFight(button)
            return nil
        end , CCControlEventTouchDown)

        self.btn_desc:setTouchPriority(kCCMenuHandlerPriority - 1)
        self.btn_desc:setTouchEnabled(true)
        self.proxy_:handleButtonEvent(self.btn_desc, function(button, event)
            onBtnDesc(button)
            return nil
        end , CCControlEventTouchDown)

        self.button_inspire:setTouchPriority(kCCMenuHandlerPriority - 1)
        self.button_inspire:setTouchEnabled(true)
        self.proxy_:handleButtonEvent(self.button_inspire, function(button, event)
            onBtnInspire(button)
            return nil
        end , CCControlEventTouchDown)


    end
end
-- 消除CD
function showCdView(self)

    local dlg = fightCleanCd:create()
    fightCleanCd.m_selfview = dlg
    dlg:setData(self.fight_left_time, 5, self)
    dlg:initUI()
    GetMainMenu():AddDialog(dlg, 3)

end

function setFightResttime(self, resttime, cost_cash)
    self.fight_left_time = resttime
    local timeStr = tools.convertTimeElectronicWatch(self.fight_left_time, 3)
    self.label_cd:setString(timeStr)
    initHeader(self.proxy_)
   
end

function setTotalUp(self, add)
    self.coin_cash_total = add
    self.label_fight_add:setString("100%+" .. self.coin_cash_total .. "%")
    initHeader(self.proxy_)
end

function submit_fight_info(self)
    local playerMgr = CPlayerDataMgr:instance()
    local playerData = playerMgr:GetPlayerInfoData()
    local uid = playerData.m_uid
    local urlpath = GetUrlNormalHeader(uid, 6, "rl_x_group_boss")

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
            GetMainMenu():CloseLoadding();

            self.rest_life = tonumber(item:find("restlife")[1])
            self.fight_left_time = tonumber(item:find("fig_left_time")[1])
            self.clear_cd_cost = tonumber(item:find("clear_cd_cost")[1])
            local fightinfo = item:find("fight")
            local resultData = CFightResultData:instance()
            resultData:Clear()
            InitFightXML(fightinfo, resultData)
            resultData:SetFightType(FT_BOSS)

            local awardXML
            local hasAward = false
            for i = 1, #item do
                if item[i][0] == "award" then
                    hasAward = true
                    awardXML = item[i]
                end
            end
            if hasAward == true then
                InitAwardData(resultData:GetRewardData(), awardXML)
                CPlayerDataMgr:instance():AddDataFromReward(resultData:GetRewardData())
            end

            local fightview = CRoundFightView:new()
            fightview:init()
            GetMainMenu():addChild(fightview, 5)
            fightview:Start()
            fightview:release()

            local iswin = resultData:IsWin()

            if  iswin ~= true then
                self:init_ui_ext()
            else
                self.node_:removeFromParentAndCleanup(true)
                ShowAdoptView()
            end
          
        else
            GetMainMenu():ShowErrorTip(tonumber(retcode), -1)
            GetMainMenu():CloseLoadding();

            if tonumber(retcode) == 640013 then 
                self.node_:removeFromParentAndCleanup(true)
                ShowAdoptView()
            end
            
        end
    end )
end


function onNodeCleanup(self)
    if self.proxy_ then
        self.proxy_:release()
    end

    layer_base_t.onNodeCleanup(self)
end
