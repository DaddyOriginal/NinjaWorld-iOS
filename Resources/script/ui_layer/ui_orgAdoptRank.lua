----------------------------------------------------------------------
--  Copyright (c) 2014-2016, XCKOO. All Rights Reserved.
--  Author :lyk
--  Time   :2015-11-11
--  Remark :BOSS死亡 排行界面
----------------------------------------------------------------------
module("ui_orgAdoptRank", package.seeall)
baseClass(layer_base_t, ui_orgAdoptRank)

require("ui_layer/ui_orgAdoptRankDlgView")

function init(self, huntinfo)
    self.playerMgr_ = CPlayerDataMgr:instance()
    self.playerData_ = self.playerMgr_:GetPlayerInfoData()

    self.contentSize_ = GetMainMenu():GetSubContentNode():getContentSize()
    local ccbiAttrTable = { name = "sub_ui/OrgAdoptRank.ccbi", size = self.contentSize_ }
    layer_base_t.init(self, true, ccbiAttrTable)

    -- pre page
    self.back_page = E_DEFAULTMENU

    -- data
    self.huntinfo = huntinfo
    self.deltatime = 0
    self.m_resttime = 0
    -- init
    self:init_ui()
    self:init_binding_event()
end

function init_ui(self)
    if self.proxy_ ~= nil then
        initHeader(self.proxy_)
        -- label	
        -- self.label_hp = tolua.cast(self.proxy_:getNode("label_hp"), "CCLabelTTF")
        self.label_my_rank = tolua.cast(self.proxy_:getNode("label_my_rank"), "CCLabelTTF")
        self.label_my_hurt = tolua.cast(self.proxy_:getNode("label_my_hurt"), "CCLabelTTF")
        self.label_last_man = tolua.cast(self.proxy_:getNode("label_last_man"), "CCLabelTTF")
        self.label_last_man_desc = tolua.cast(self.proxy_:getNode("label_last_man_desc"), "CCLabelTTF")

        self.lable_left_time = tolua.cast(self.proxy_:getNode("label_left_time"), "CCLabelBMFont")
        -- btn
        self.btn_back = tolua.cast(self.proxy_:getNode("btn_back"), "CCControlButton")
        self.btn_adopt = tolua.cast(self.proxy_:getNode("button_adopt"), "CCControlButton")
        self.btn_desc = tolua.cast(self.proxy_:getNode("button_desc"), "CCControlButton")
        -- sprite
        self.sprite_icon = tolua.cast(self.proxy_:getNode("sprite_icon"), "CCSprite")

        self:init_ui_ext()
    end
end


function init_ui_ext(self)
    if self.huntinfo then
        -- 排行剩余时间
        self.rest_time = tonumber(self.huntinfo:find("rest_time")[1])

        self.total_life = tonumber(self.huntinfo:find("total_life")[1])
        self.rest_life = tonumber(self.huntinfo:find("rest_life")[1])
        --  self.boss_life = tonumber(self.huntinfo:find("boss_life")[1])
        self.last_man =(self.huntinfo:find("last_man")[1])
        self.my_rank = tonumber(self.huntinfo:find("my_rank")[1])
        self.my_hurt = tonumber(self.huntinfo:find("my_hurt")[1])

        if not self.last_man then
            self.label_last_man:setString(localizable.ui_orgAdoptRank_text1)
            self.label_last_man_desc:setVisible(false)
        else
            self.label_last_man:setString(self.last_man)
        end

        self.label_my_hurt:setString(self.my_hurt)
        self.label_my_rank:setString(self.my_rank)
        -- self.label_hp:setString(self.rest_life .."/" .. self.total_life)

        local frame_name
        if self.rest_life == 0 then
            -- CCSpriteFrameCache:sharedSpriteFrameCache():addSpriteFramesWithFile()
            frame_name = "org_add_25"
        else
            frame_name = "org_add_26"
        end

        local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(frame_name)
        if frame then
            self.sprite_icon:setDisplayFrame(frame)
        end

        self.m_resttime = self.rest_time

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
                    -- self:init_ui_ext()
                    self.lable_left_time:setString("00")
                    self.lable_left_time:unscheduleUpdate()
                end
            end
        end


        self.lable_left_time:scheduleUpdateWithPriorityLua(updateLeftTimeLabel, 0)
        self.lable_left_time:setString(tools.convertTimeElectronicWatch(self.m_resttime, 3))


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
            --
            local monthLayer = createObj(ui_orgAdoptRankDlgView)
            local size1 = GetMainMenu():GetModelLayer():getContentSize()
            monthLayer.node_:setAnchorPoint(ccp(0.5, 0.5))
            monthLayer.node_:setPosition(size1.width / 2, size1.height / 2)
            GetMainMenu():GetModelLayer():addChild(monthLayer.node_)

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
