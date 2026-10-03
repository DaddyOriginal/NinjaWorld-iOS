----------------------------------------------------------------------
--  Copyright (c) 2014-2016, XCKOO. All Rights Reserved.
--  Author :lyk
--  Time   :2015-11-04
--  Remark :兽栏（有尾兽处于培养状态）
----------------------------------------------------------------------
module("ui_orgAdoptYes", package.seeall)
baseClass(layer_base_t, ui_orgAdoptYes)

function init(self, boss, trainlist)
    self.playerMgr_ = CPlayerDataMgr:instance()
    self.playerData_ = self.playerMgr_:GetPlayerInfoData()

    self.contentSize_ = GetMainMenu():GetSubContentNode():getContentSize()
    local ccbiAttrTable = { name = "sub_ui/OrgAdoptYes.ccbi", size = self.contentSize_ }
    layer_base_t.init(self, true, ccbiAttrTable)

    -- pre page
    self.back_page = E_DEFAULTMENU
    -- data
    self.add_progress = 0
    self.contribute = 0
    self.train_left = 0
    self.train_total = 0
    -- 时间增量
    self.deltatime = 0
    self.m_resttime = 0

    self.boss = boss
    self.trainlist = trainlist
    self.trainlist_list = { }
    self.current_select = 1

    -- init
    self:init_boss_info()
    self:init_ui()
    self:init_binding_event()
end

function init_boss_info(self)
    if self.boss then
        -- 领养的bossid
        self.boss_id = tonumber(self.boss:find("boss_id")[1])
        self.boss_name =(self.boss:find("boss_name")[1])
        self.boss_pic =(self.boss:find("boss_pic")[1])
        self.boss_total_train = tonumber(self.boss:find("boss_total_train")[1])
        self.boss_cur_train = tonumber(self.boss:find("boss_cur_train")[1])
        self.boss_attack = tonumber(self.boss:find("boss_attack")[1])
        self.boss_data = tonumber(self.boss:find("boss_data")[1])

        if self.trainlist then
            for i = 1, #self.trainlist do
                local train_item = { }
                -- 培养类型  1为元宝，2位银子
                train_item.train_type = tonumber(self.trainlist[i]:find("train_type")[1])
                -- 剩余培养次数
                train_item.train_left = tonumber(self.trainlist[i]:find("train_left")[1])
                -- 总的培养次数
                train_item.train_total = tonumber(self.trainlist[i]:find("train_total")[1])
                -- 元宝
                -- 培养消耗值
                train_item.train_cost = tonumber(self.trainlist[i]:find("train_cost")[1])
                train_item.train_cost_name =(self.trainlist[i]:find("train_cost_name")[1])
                -- 培养获得的贡献
                train_item.train_award = tonumber(self.trainlist[i]:find("train_award")[1])
                -- 培养获得的boss进度增加
                train_item.train_each_add = tonumber(self.trainlist[i]:find("train_each_add")[1])
                -- 培养剩余cd
                train_item.train_left_time = tonumber(self.trainlist[i]:find("train_left_time")[1])

                table.insert(self.trainlist_list, train_item)
            end
        end

    end

end

function init_ui(self)
    if self.proxy_ ~= nil then
        initHeader(self.proxy_)
        -- label
        self.label_progress = tolua.cast(self.proxy_:getNode("label_progress"), "CCLabelBMFont")
        self.label_add_progress = tolua.cast(self.proxy_:getNode("label_add_progress"), "CCLabelTTF")
        self.label_contribute = tolua.cast(self.proxy_:getNode("label_contribute"), "CCLabelTTF")
        self.label_use_sliver = tolua.cast(self.proxy_:getNode("label_use_sliver"), "CCLabelTTF")
        self.label_use_gold = tolua.cast(self.proxy_:getNode("label_use_gold"), "CCLabelTTF")
        self.label_use_cd = tolua.cast(self.proxy_:getNode("label_use_cd"), "CCLabelTTF")
        -- btn
        self.btn_back = tolua.cast(self.proxy_:getNode("btn_back"), "CCControlButton")
        self.btn_adopt = tolua.cast(self.proxy_:getNode("button_adopt"), "CCControlButton")
        self.btn_desc = tolua.cast(self.proxy_:getNode("button_desc"), "CCControlButton")
        self.btn_select_gold = tolua.cast(self.proxy_:getNode("button_select_gold"), "CCControlButton")
        self.btn_select_sliver = tolua.cast(self.proxy_:getNode("button_select_sliver"), "CCControlButton")
        -- sprite
        self.sprite_sliver_gou = tolua.cast(self.proxy_:getNode("sprite_sliver_gou"), "CCSprite")
        self.sprite_gold_gou = tolua.cast(self.proxy_:getNode("sprite_gold_gou"), "CCSprite")
        self.sprite_progress = tolua.cast(self.proxy_:getNode("sprite_progress"), "CCSprite")
        self.sprite_icon = tolua.cast(self.proxy_:getNode("sprite_icon"), "CCSprite")

        self:init_select_btn()
        self:init_ui_ext()
        self:init_ui_timer()
    end
end


function init_ui_ext(self)
    -- body
    local ratio = self.boss_cur_train / self.boss_total_train
    self.sprite_progress:setScaleX(ratio)
    self.label_progress:setString(self.boss_cur_train .. "/" .. self.boss_total_train)

    local pIconFrame = CGameObjElement:GetNinjaIcon(E_FRAMETYPE_MIDDLE, self.boss_pic)
    if pIconFrame then
        self.sprite_icon:setDisplayFrame(pIconFrame)
    end

    for i = 1, #self.trainlist_list do
        if self.trainlist_list[i].train_type == 1 then
            -- 元宝
            local str = string.format(localizable.ui_orgAdoptYes_text2, tonumber(self.trainlist_list[i].train_cost), tonumber(self.trainlist_list[i].train_left), tonumber(self.trainlist_list[i].train_total))
            self.label_use_gold:setString(str)
        elseif self.trainlist_list[i].train_type == 2 then
            local str = string.format(localizable.ui_orgAdoptYes_text1, tonumber(self.trainlist_list[i].train_cost), tonumber(self.trainlist_list[i].train_left), tonumber(self.trainlist_list[i].train_total))
            self.label_use_sliver:setString(str)
        end
    end

end


function init_ui_timer(self)
    for i = 1, #self.trainlist_list do
        if self.trainlist_list[i].train_type == 2 then
            self.m_resttime = tonumber(self.trainlist_list[i].train_left_time)
            self.label_use_cd:setString(self.m_resttime .. "s")          
        end
    end
end

function init_binding_event(self)
    if self.proxy_ ~= nil then
        local function updateLeftTimeLabel(fDeltaTime)
            self.deltatime = self.deltatime + fDeltaTime
            if self.deltatime >= 1 then
                local intPart, floatPart = math.modf(self.deltatime)
                self.m_resttime = self.m_resttime - intPart
                if self.m_resttime > 0 then
                    -- local timeStr = tools.convertTimeElectronicWatch(self.m_resttime, 3)
                    self.label_use_cd:setString(self.m_resttime .. "s")
                    self.deltatime = floatPart
                else
                    -- self.m_state = 1
                    -- self:init_ui_ext()
                    self.m_resttime = 0
                    self.label_use_cd:setString(self.m_resttime .. "s")
                    self.label_use_cd:unscheduleUpdate()
                end
            end
        end


        local function onBtnBack(btn, event)
            -- GetMainMenu():ChangeToSub(self.back_page)
            self.node_:removeFromParentAndCleanup(true)
            ShowOrgMapLayer()
        end

        local function onBtnAdopt(btn, event)
            if self.current_select == 0 then
                GetMainMenu():ShowTextTip(localizable.ui_orgAdoptYes_text3, -1)
                return
            end

            local playerinfo = CPlayerDataMgr:instance():GetPlayerInfoData()
            local urlpath = GetUrlNormalHeader(playerinfo.m_uid, 3, "rl_x_group_boss")
            urlpath = AddData(urlpath, "TrainType", self.current_select)
            -- cclog("rl_x_group_boss & cmd = 1---%s", urlpath)
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
                cclog("rl_x_group_boss ret = %s", resData)
                local retcode = item.code
                if retcode == "0" then
                    local preview = xfile:find("preview")
                    local status = tonumber(preview.status)
                    if status ~= 2 then
                        -- 状态改变 直接重新请求新的数据
                        self.node_:removeFromParentAndCleanup(true)
                        ShowAdoptView()
                        return
                    end
                    local train = xfile:find("train")
                    local train_type = tonumber(train:find("train_type")[1])
                    for i = 1, #self.trainlist_list do
                        if train_type == self.trainlist_list[i].train_type then
                            self.trainlist_list[i].train_cost = tonumber(train:find("train_cost")[1])
                            self.trainlist_list[i].train_award = tonumber(train:find("train_award")[1])
                            self.trainlist_list[i].train_left_time = tonumber(train:find("train_left_time")[1])
                            self.trainlist_list[i].train_left = tonumber(train:find("train_left")[1])
                        end
                    end
                    if train_type == 1 then
                        self.playerMgr_:AddGold(- tonumber(train:find("train_cost")[1]))
                        initHeader(self.proxy_)
                    elseif train_type == 2 then
                        self.playerMgr_:AddSilver(- tonumber(train:find("train_cost")[1]))
                        initHeader(self.proxy_)
                        self.m_resttime = tonumber(train:find("train_left_time")[1])
                        self.label_use_cd:setString(self.m_resttime .. "s")
                        self.label_use_cd:scheduleUpdateWithPriorityLua(updateLeftTimeLabel, 0)
                    end

                    self.boss_cur_train = tonumber(train:find("train_cur_process")[1])
                    self:init_ui_ext()
                else
                    GetMainMenu():ShowErrorTip(tonumber(retcode), -1)

                    --状态为培养满
                    if tonumber(retcode) == 640005 then
                        self.node_:removeFromParentAndCleanup(true)
                        ShowAdoptView()
                    end

                end
            end )
        end

        local function onBtnDesc(btn, event)
            local monthLayer = createObj(ui_orgAdoptDesc)
            local size1 = GetMainMenu():GetModelLayer():getContentSize()
            monthLayer.node_:setAnchorPoint(ccp(0.5, 0.5))

            monthLayer.node_:setPosition(size1.width / 2, size1.height / 2)
            GetMainMenu():GetModelLayer():addChild(monthLayer.node_)
        end

        local function onBtnGold(btn, event)
            cclog("onBtnGold")
            if self.current_select ~= 1 then
                self.current_select = 1
            else
                self.current_select = 0
            end
            self:init_select_btn()
        end

        local function onBtnSliver(btn, event)
            cclog("onBtnGold")
            if self.current_select ~= 2 then
                self.current_select = 2
            else
                self.current_select = 0
            end
            self:init_select_btn()
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

        self.btn_select_sliver:setTouchPriority(kCCMenuHandlerPriority - 1)
        self.btn_select_sliver:setTouchEnabled(true)
        self.proxy_:handleButtonEvent(self.btn_select_sliver, function(button, event)
            onBtnSliver(button)
            return nil
        end , CCControlEventTouchDown)

        self.btn_select_gold:setTouchPriority(kCCMenuHandlerPriority - 1)
        self.btn_select_gold:setTouchEnabled(true)
        self.proxy_:handleButtonEvent(self.btn_select_gold, function(button, event)
            onBtnGold(button)
            return nil
        end , CCControlEventTouchDown)

        if self.m_resttime > 0 then      
            self.label_use_cd:scheduleUpdateWithPriorityLua(updateLeftTimeLabel, 0)
        end
    end
end

function init_progress_contribute_ui(self)
    self.label_add_progress:setString("+" .. self.add_progress)
    self.label_contribute:setString("+" .. self.contribute)
end

function init_select_btn(self)
    if self.current_select == 0 then
        self.sprite_sliver_gou:setVisible(false)
        self.sprite_gold_gou:setVisible(false)
        self.add_progress = 0
        self.contribute = 0
    elseif self.current_select == 2 then
        -- 银子
        self.sprite_sliver_gou:setVisible(true)
        self.sprite_gold_gou:setVisible(false)
        -- todo 剩余次数		
    elseif self.current_select == 1 then
        -- 元宝
        self.sprite_sliver_gou:setVisible(false)
        self.sprite_gold_gou:setVisible(true)
    end
    self:init_trainlist_info()
    self:init_progress_contribute_ui()
end

function init_trainlist_info(self)
    for i = 1, #self.trainlist_list do
        if self.current_select == self.trainlist_list[i].train_type then
            self.add_progress = self.trainlist_list[i].train_each_add
            self.contribute = self.trainlist_list[i].train_award
            -- self.train_left = self.trainlist_list[i].train_left
            -- self.train_total = self.trainlist_list[i].train_total				
        end
    end
end



function onNodeCleanup(self)
    if self.proxy_ then
        self.proxy_:release()
    end

    layer_base_t.onNodeCleanup(self)
end
