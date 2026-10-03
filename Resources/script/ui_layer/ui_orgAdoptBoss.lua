----------------------------------------------------------------------
--  Copyright (c) 2014-2016, XCKOO. All Rights Reserved.
--  Author :lyk
--  Time   :2015-11-04
--  Remark :领养尾兽界面
----------------------------------------------------------------------
module("ui_orgAdoptBoss", package.seeall)
baseClass(layer_base_t, ui_orgAdoptBoss)

pixelUnit = 5	-- 单位,当移动距离超过5以后，才进行放大缩小处理

function init(self, boss_list, archite)
    self.playerMgr_ = CPlayerDataMgr:instance()
    self.playerData_ = self.playerMgr_:GetPlayerInfoData()

    self.contentSize_ = GetMainMenu():GetSubContentNode():getContentSize()
    local ccbiAttrTable = { name = "sub_ui/OrgAdoptBoss.ccbi", size = self.contentSize_ }
    layer_base_t.init(self, true, ccbiAttrTable)

    -- pre page
    self.back_page = E_DEFAULTMENU

    -- data
    self.boss_list = boss_list
    self.boss_list_table = { }
    self.archite = archite
    self.archite_level = tonumber(self.archite:find("archite_level")[1])
    --
    self.m_bIsEndAnimatedScroll_ = true
    self.touchHandler_ = { }
    self.sprite_config_data = { }

    self.adopt_award = 0
    self.expend = 0
    self.kill_award = 0

    -- init
    self:init_ui()
    self:init_binding_event()

    self:init_data()
end


function init_ui(self)
    if self.proxy_ ~= nil then
        initHeader(self.proxy_)
        -- node	
        -- btn
        self.btn_back = tolua.cast(self.proxy_:getNode("btn_back"), "CCControlButton")
        self.btn_adopt = tolua.cast(self.proxy_:getNode("button_adopt"), "CCControlButton")
        self.btn_desc = tolua.cast(self.proxy_:getNode("button_desc"), "CCControlButton")

        self.node_sprite_content = tolua.cast(self.proxy_:getNode("node_content"), "CCNode")
        -- self.contentNode_ = self.node_sprite_content:getContentSize()
        self.sprite_nodes = { }
        self.sprite_points = { }
        self.sprite_scales = { [1] = 0.31, [2] = 0.65, [3] = 1, [4] = 0.65, [5] = 0.31 }
        self.sprite_opactiy = { [1] = 105, [2] = 180, [3] = 255, [4] = 180, [5] = 105 }
        -- node
        self.sprite_icons = { }
        self.node_nodes = { }
        for i = 1, 5 do
            self["sprite_avtor_" .. tostring(i)] = tolua.cast(self.proxy_:getNode("node_cell" .. tostring(i)), "CCNode")
            self.node_sprite_content:reorderChild(self["sprite_avtor_" .. tostring(i)], 1)
            table.insert(self.sprite_nodes, self["sprite_avtor_" .. tostring(i)])
            local x, y = self["sprite_avtor_" .. tostring(i)]:getPosition()
            local tmpPoint = { x = x, y = y }
            table.insert(self.sprite_points, tmpPoint)
            self["sprite_bg_0" .. tostring(i)] = tolua.cast(self.proxy_:getNode("sprite_bg_0" .. tostring(i)), "CCSprite")
            self["label_train_0" .. tostring(i)] = tolua.cast(self.proxy_:getNode("label_train_0" .. tostring(i)), "CCLabelBMFont")
            self["sprite_icon_0" .. tostring(i)] = tolua.cast(self.proxy_:getNode("sprite_icon_0" .. tostring(i)), "CCSprite")
            self["sprite_open_lv_0" .. tostring(i)] = tolua.cast(self.proxy_:getNode("sprite_open_lv_0" .. tostring(i)), "CCSprite")
            self["label_open_lv_0" .. tostring(i)] = tolua.cast(self.proxy_:getNode("label_open_lv_0" .. tostring(i)), "CCLabelTTF")

            table.insert(self.node_nodes, { icon = self["sprite_icon_0" .. tostring(i)], train = self["label_train_0" .. tostring(i)], sprite_open_lv = self["sprite_open_lv_0" .. tostring(i)], label_open_lv = self["label_open_lv_0" .. tostring(i)] })
        end
        -- bottom
        self.label_adopt_award = tolua.cast(self.proxy_:getNode("label_adopt_award"), "CCLabelTTF")
        self.label_expend = tolua.cast(self.proxy_:getNode("label_expend"), "CCLabelTTF")
        self.label_kill_award = tolua.cast(self.proxy_:getNode("label_kill_award"), "CCLabelTTF")

        for i = 1, 4 do
            self["btn_item_" .. tostring(i)] = tolua.cast(self.proxy_:getNode("btn_item_" .. tostring(i)), "CCControlButton")
            self["sprite_item" .. tostring(i)] = tolua.cast(self.proxy_:getNode("sprite_item" .. tostring(i)), "CCSprite")
        end

        local node_size = self.node_sprite_content:getContentSize()
        self.midX = node_size.width * 0.5
        self.left_right_width = node_size.width * 2
        self.sprite_distance = node_size.width * 0.4
        self.scalePara = 0.4 / self.sprite_distance
        -- 每移动单个像素的缩放比例
        self.scaleOpacity = 75 / self.sprite_distance
        self.scaleParameter = self.scalePara * 5
        -- 每移动5个像素的缩放比例
        self.allMoveDistance = 0
        self.circleNumber = -1
        self.move_speed = self.sprite_distance / 0.5

    end
end



function init_binding_event(self)
    if self.proxy_ ~= nil then
        local function onTouched(_eventType, ...)
            -- cclog("_eventType = %s", _eventType)
            local result = self.touchHandler_[_eventType](self, ...)
            if self.touchBegan ~= nil and _eventType == "began" then
                assert(result ~= nil, "touchBegan must return a result!")

                local ret

                if result == true then
                    ret = 1
                else
                    ret = 0
                end
                return ret
            end
        end

        if self.touchBegan ~= nil then
            -- 单点触控
            self.node_:setTouchEnabled(true)
            self.node_:registerScriptTouchHandler(onTouched, false, 2, true)

            self.touchHandler_["began"] = self.touchBegan
            self.touchHandler_["moved"] = self.touchMoved
            self.touchHandler_["ended"] = self.touchEnded
            self.touchHandler_["cancelled"] = self.touchCancelled
        end



        local function onBtnBack(btn, event)
            -- GetMainMenu():ChangeToSub(self.back_page)
            self.node_:removeFromParentAndCleanup(true)
            ShowOrgAdoptNo(self.boss_list, self.archite)
        end

        local function onBtnAdopt(btn, event)
            cclog("onBtnAdopt")

            local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 2, "rl_x_group_boss")
            urlpath = AddData(urlpath, "BossId", self.sprite_config_data[3].boss_id)
            -- cclog("%s", urlpath)
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
                -- cclog("rl_r_group_comm ret = %s", resData)
                local retcode = item.code
                if retcode == "0" then
                    self.node_:removeFromParentAndCleanup(true)
                    ShowAdoptView()
                else

                    GetMainMenu():ShowErrorTip(tonumber(retcode), -1)
                    -- GetMainMenu():ShowTextTip(text.text_config[tonumber(retcode)].description, -1)
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

        -- 奖励详情
        local function onBtnClickAwardIcon(btn)
            local btnIndex = btn:getTag()

            if btnIndex > #self.award_datas then
                return nil
            end

            if btnIndex > 0 and btnIndex <= #self.award_datas then
                local _id = tonumber(self.award_datas[btnIndex].award_id)
                if nil ~= _id then
                    CGameObjElement:ShowDropByID(_id)
                end
            end
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


        for i = 1, 4 do
            self["btn_item_" .. i]:setTouchPriority(kCCMenuHandlerPriority - 1)
            self["btn_item_" .. i]:setTouchEnabled(true)
            self.proxy_:handleButtonEvent(self["btn_item_" .. i], function(button, event)
                onBtnClickAwardIcon(button)
                return nil
            end , CCControlEventTouchUpInside)
        end


    end
end


function touchBegan(self, _touchX, _touchY)
    if self.m_bIsEndAnimatedScroll_ then
        -- self.node_sprite_content
        -- self.node_
        local tmpPoint = self.node_:convertToNodeSpace(ccp(_touchX, _touchY))
        if self.node_sprite_content:boundingBox():containsPoint(tmpPoint) then
            self.isTouchMoved_ = false
            self.moveDirection_ = true
            -- right direction
            self.beganX_ = _touchX
            self._preTouchX = _touchX
            self._preTouchY = _touchY
        end

        return true
    else
        return false
    end
end

function touchMoved(self, _touchX, _touchY)
    if self.beganX_ ~= nil and self.m_bIsEndAnimatedScroll_ then
        self.isTouchMoved_ = true
        local moveDistance = 0
        -- cclog("1111---%s", tostring(_preTouchX))
        if self._preTouchX > _touchX then
            -- 向左
            self.moveDirection_ = false
            moveDistance = self.beganX_ - _touchX

            local disX = _touchX - self._preTouchX
            self.allMoveDistance = self.allMoveDistance + disX
            self:moveSprite(false, moveDistance, disX)
            -- self:reorderChild()
            local num = tools.getIntPart((moveDistance / self.sprite_distance))

            if num ~= self.circleNumber then
                self.circleNumber = num
                local x, y = self.sprite_nodes[1]:getPosition()
                local tmpValue = table.remove(self.sprite_nodes, 1)
                tmpValue:setPosition(x + self.left_right_width, y)
                local tmpScale = math.abs(self.midX - x - self.left_right_width) * self.scalePara
                local tmpOpacity = math.abs(self.midX - x - self.left_right_width) * self.scaleOpacity
                tmpValue:setScale(1 - tmpScale)
                -- 			tmpValue:updateDisplayedOpacity(255 - tmpOpacity)
                table.insert(self.sprite_nodes, tmpValue)
                self:resetConfigData(false)
            end
        elseif self._preTouchX < _touchX then
            -- 向右
            self.moveDirection_ = true
            moveDistance = _touchX - self.beganX_
            local disX = _touchX - self._preTouchX
            self.allMoveDistance = self.allMoveDistance + disX
            self:moveSprite(true, moveDistance, disX)

            -- self:reorderChild()
            local num = tools.getIntPart((moveDistance / self.sprite_distance))

            if num ~= self.circleNumber then
                self.circleNumber = num
                local x, y = self.sprite_nodes[5]:getPosition()
                local tmpValue = table.remove(self.sprite_nodes)
                tmpValue:setPosition(x - self.left_right_width, y)
                local tmpScale = math.abs(self.midX - x + self.left_right_width) * self.scalePara
                tmpValue:setScale(1 - tmpScale)
                local tmpOpacity = math.abs(self.midX - x + self.left_right_width) * self.scaleOpacity
                -- tmpValue:updateDisplayedOpacity(255 - tmpOpacity)
                table.insert(self.sprite_nodes, 1, tmpValue)
                -- 重新移动配置数据
                self:resetConfigData(true)
            end
        end
        self._preTouchX = _touchX
        self._preTouchY = _touchY
    end
end

function touchEnded(self, _touchX, _touchY)
    if self.isTouchMoved_ then
        self.isTouchMoved_ = false
        self:startAnimatedScroll()
        self.allMoveDistance = 0
        self.circleNumber = -1
        self.beganX_ = nil
        -- self.btn_give_flower:setTitleForState(self.sprite_config_data[3].desc, CCControlStateNormal)
        -- self.btn_give_flower:setTitleForState(self.sprite_config_data[3].desc, CCControlStateHighlighted)
        -- self.btn_give_flower:setTitleForState(self.sprite_config_data[3].desc, CCControlStateDisabled)
        -- self.label_good_feel:setString(tostring(self.sprite_config_data[3].like) .. "/" .. tostring(self.sprite_config_data[3].alllikenum))
        -- self:init_reset_data()
        self:init_bottom_ui(3)
    end
end

function touchCancelled(self)
    for i = 1, 5 do
        self.sprite_nodes[i]:setPosition(self.sprite_points[i].x, self.sprite_points[i].y)
        self.sprite_nodes[i]:setScale(self.sprite_scales[i])
    end
    self:reorderChild()
    -- self.btn_give_flower:setTitleForState(self.sprite_config_data[3].desc, CCControlStateNormal)
    -- self.btn_give_flower:setTitleForState(self.sprite_config_data[3].desc, CCControlStateHighlighted)
    -- self.btn_give_flower:setTitleForState(self.sprite_config_data[3].desc, CCControlStateDisabled)
    -- self.label_good_feel:setString(tostring(self.sprite_config_data[3].like) .. "/" .. tostring(self.sprite_config_data[3].alllikenum))
    -- self:init_reset_data()
    self:init_bottom_ui(3)
end

function moveSprite(self, direction, moveDistance, disX)
    -- if not direction then  --局部往左做移动
    for i = 1, 5 do
        if moveDistance > 0 then
            -- 整体是往左移动
            local preX, preY = self.sprite_nodes[i]:getPosition()
            local x = preX + disX
            -- local tmpX = self.sprite_points[i].x - moveDistance
            local scaleCur = math.abs(self.midX - x) * self.scalePara
            local tmpOpacity = math.abs(self.midX - x) * self.scaleOpacity
            -- self.sprite_nodes[i]:updateDisplayedOpacity(255 - tmpOpacity)
            self.sprite_nodes[i]:setPosition(x, preY)
            self.sprite_nodes[i]:setScale(1 - scaleCur)
        else
            -- 整体往右移动
            local preX, preY = self.sprite_nodes[i]:getPosition()
            local x = preX + disX
            -- local tmpX = self.sprite_points[i].x + moveDistance
            local scaleCur = math.abs(self.midX - x) * self.scalePara
            local tmpOpacity = math.abs(self.midX - x) * self.scaleOpacity
            -- self.sprite_nodes[i]:updateDisplayedOpacity(255 - tmpOpacity)
            self.sprite_nodes[i]:setPosition(x, preY)
            self.sprite_nodes[i]:setScale(1 - scaleCur)
        end
    end

end

function startAnimatedScroll(self)
    self.m_bIsEndAnimatedScroll_ = false
    local animationNumer = 0
    local function actionFinished()
        animationNumer = animationNumer + 1
        if animationNumer == 5 then
            self:reorderChild()
            self.m_bIsEndAnimatedScroll_ = true
        end
    end
    local sprite_posX = self.sprite_nodes[3]:getPosition()
    local time1 = math.abs(self.sprite_points[3].x - sprite_posX) / self.move_speed
    for i = 1, 5 do
        local endPos = ccp(self.sprite_points[i].x, self.sprite_points[i].y)
        local ccMoveto = CCMoveTo:create(time1, endPos)
        local scaleTo = CCScaleTo:create(time1, self.sprite_scales[i])
        local ccFadeTo = CCFadeTo:create(time1, self.sprite_opactiy[i])
        local ccArraySpawn = CCArray:create()
        ccArraySpawn:addObject(ccMoveto)
        ccArraySpawn:addObject(scaleTo)
        ccArraySpawn:addObject(ccFadeTo)
        local ccSpawn = CCSpawn:create(ccArraySpawn)

        local moveFinishCall = CCCallFuncN:create(actionFinished)

        local moveSeq = CCSequence:createWithTwoActions(ccSpawn, moveFinishCall)
        self.sprite_nodes[i]:runAction(moveSeq)
    end

end


function reorderChild(self)
    local disMid = 2000
    local index = 1
    for i = 1, 5 do
        local x = self.sprite_nodes[i]:getPosition()
        local tmpDis = math.abs(x - self.midX)
        if tmpDis < disMid then
            disMid = tmpDis
            index = i
        end
    end
    for i = 1, 5 do
        if i == index then
            self.node_sprite_content:reorderChild(self.sprite_nodes[i], 5)
        else
            self.node_sprite_content:reorderChild(self.sprite_nodes[i], 1)
            -- self.sprite_nodes[i]:updateDisplayedOpacity(self.sprite_opactiy[i])
        end
    end
end

function resetConfigData(self, direction)
    if not direction then
        -- 左
        local tmpValue = table.remove(self.sprite_config_data, 1)
        table.insert(self.sprite_config_data, tmpValue)
        local tmpIconValue = table.remove(self.node_nodes, 1)
        table.insert(self.node_nodes, tmpIconValue)
    else
        local tmpValue = table.remove(self.sprite_config_data)
        table.insert(self.sprite_config_data, 1, tmpValue)

        local tmpIconValue = table.remove(self.node_nodes)
        table.insert(self.node_nodes, 1, tmpIconValue)
    end

    for i = 1, 5 do
        local pIconFrame = CGameObjElement:GetNinjaIcon(E_FRAMETYPE_MIDDLE, self.sprite_config_data[i].boss_pic)
        if pIconFrame then
            self.node_nodes[i].icon:setDisplayFrame(pIconFrame)
        end
        self.node_nodes[i].train:setString(self.sprite_config_data[i].boss_cur_train .. "/" .. self.sprite_config_data[i].boss_total_train)

        local building_level = self.sprite_config_data[i].boss_building
        if self.archite_level >= building_level then
            self.node_nodes[i].sprite_open_lv:setVisible(false)
        else
            self.node_nodes[i].sprite_open_lv:setVisible(true)
            self.node_nodes[i].label_open_lv:setString(building_level)
        end
    end

end

--[[
function init_reset_data(self)
    for i = 1, 5, 4 do
        local pIconFrame = CGameObjElement:GetNinjaIcon(E_FRAMETYPE_MIDDLE, self.boss_list_table[i].boss_pic)
        if pIconFrame then
            self["sprite_icon_0" .. tostring(i)]:setDisplayFrame(pIconFrame)
        end
        self["label_train_0" .. tostring(i)]:setString(self.boss_list_table[i].boss_cur_train .. "/" .. self.boss_list_table[i].boss_total_train)
    end
end
]]

function init_data(self)
    if self.boss_list then
        for i = 1, #self.boss_list do
            local item = self.boss_list[i]:find("boss")
            if item then
                local item_item = { }
                item_item.boss_id = tonumber(item:find("boss_id")[1])
                item_item.boss_name =(item:find("boss_name")[1])
                -- boss 图片位置			
                item_item.boss_pic =(item:find("boss_pic")[1])
                -- boss 可领养的建筑等级
                item_item.boss_building = tonumber(item:find("boss_building")[1])
                -- boss 领养消耗建设值
                item_item.boss_cost = tonumber(item:find("boss_cost")[1])
                -- boss 培养当前值
                item_item.boss_cur_train = tonumber(item:find("boss_cur_train")[1])
                -- boss 培养总值
                item_item.boss_total_train = tonumber(item:find("boss_total_train")[1])
                -- boss 攻防类型 1为攻，0为防
                item_item.boss_attack = tonumber(item:find("boss_attack")[1])
                -- boss 攻防数值(即血量)
                item_item.boss_data = tonumber(item:find("boss_data")[1])
                -- boss 击杀获得的建设值
                item_item.boss_award = tonumber(item:find("boss_award")[1])
                -- boss 击杀获得的贡献值
                item_item.boss_pay = tonumber(item:find("boss_pay")[1])
                -- boss 每次领养BOSS获取的建设值
                item_item.boss_score = tonumber(item:find("boss_score")[1])
                -- boss 掉落ID名称
                item_item.item_id_1 = tonumber(item:find("item_id_1")[1])
                item_item.item_name_1 =(item:find("item_name_1")[1])
                item_item.item_icon_1 =(item:find("item_name_1_icon")[1])

                item_item.item_id_2 = tonumber(item:find("item_id_2")[1])
                item_item.item_name_2 =(item:find("item_name_2")[1])
                item_item.item_icon_2 =(item:find("item_name_2_icon")[1])

                item_item.item_id_3 = tonumber(item:find("item_id_3")[1])
                item_item.item_name_3 =(item:find("item_name_3")[1])
                item_item.item_icon_3 =(item:find("item_name_3_icon")[1])

                item_item.item_id_4 = tonumber(item:find("item_id_4")[1])
                item_item.item_name_4 =(item:find("item_name_4")[1])
                item_item.item_icon_4 =(item:find("item_name_4_icon")[1])

                item_item.item_id_5 = tonumber(item:find("item_id_5")[1])
                item_item.item_name_5 =(item:find("item_name_5")[1])
                item_item.item_icon_5 =(item:find("item_name_5_icon")[1])
                -- 物品说明
                table.insert(self.boss_list_table, item_item)
            end
        end
        self.sprite_config_data = self.boss_list_table
        self:init_boss_ui()

    end
end

function init_boss_ui(self)
    if self.boss_list_table then
        if #self.boss_list_table >= 5 then
            for i = 1, 5 do
                local pIconFrame = CGameObjElement:GetNinjaIcon(E_FRAMETYPE_MIDDLE, self.boss_list_table[i].boss_pic)
                if pIconFrame then
                    self["sprite_icon_0" .. tostring(i)]:setDisplayFrame(pIconFrame)
                end
                self["label_train_0" .. tostring(i)]:setString(self.boss_list_table[i].boss_cur_train .. "/" .. self.boss_list_table[i].boss_total_train)
                local building_level = self.boss_list_table[i].boss_building

                if self.archite_level >= building_level then
                    self["sprite_open_lv_0" .. tostring(i)]:setVisible(false)
                else
                    self["label_open_lv_0" .. tostring(i)]:setString(building_level)
                end

            end
        end
    end
    local boss_index = 3
    self:init_bottom_ui(boss_index)
end

-- 根据当前BOSS 初始化底部信息
function init_bottom_ui(self, boss_index)

    self.expend = self.boss_list_table[boss_index].boss_cost
    self.kill_award = self.boss_list_table[boss_index].boss_award
    self.adopt_award = self.boss_list_table[boss_index].boss_score

    local adopt_award = string.format(localizable.ui_orgContribute_number, self.adopt_award)
    local expend = string.format(localizable.ui_orgBuild_number, self.expend)
    local kill_award = string.format(localizable.ui_orgBuild_number, self.kill_award)

    self.label_adopt_award:setString(adopt_award)
    self.label_expend:setString(expend)
    self.label_kill_award:setString(kill_award)

    --
    self.award_datas = { }
    for i = 1, 5 do
        local id = tonumber(self.boss_list_table[boss_index]["item_id_" .. tostring(i)])
        local icon ;
        if id ~= 0 then
            icon = self.boss_list_table[boss_index]["item_icon_" .. tostring(i)]
            table.insert(self.award_datas, { award_id = id  ,award_icon = icon  } )
        end
    end

    for i = 1, 4 do
        if i <= #self.award_datas then
            -- icon/frame
            local _maintype = 0
            local _subtype = 0
            local _id = -1
            local _num = -1
            local _drop_type = 0
            local _obj_info = { }
            _maintype, _subtype, _id, _num, _drop_type = setObjTypeInfo(tonumber(self.award_datas[i].award_id))
            _obj_info.pIcon, _obj_info.pFrame, _obj_info.quality, _obj_info.objname = rl_get_iconsprite(_maintype, _subtype, E_FRAMETYPE_SMALL, _id)
            if nil ~= _obj_info.pFrame then
                self["sprite_item" .. tostring(i)]:setDisplayFrame(_obj_info.pFrame)
            end

            CCSpriteFrameCache:sharedSpriteFrameCache():addSpriteFramesWithFile("props/" .. self.award_datas[i].award_icon .. ".plist")
		    local sprite_icon = CCSprite:createWithSpriteFrameName(self.award_datas[i].award_icon)
            if sprite_icon then
                _obj_info.pIcon = sprite_icon
            end      

            if nil ~= _obj_info.pIcon then
                self["sprite_item" .. tostring(i)]:removeAllChildrenWithCleanup(true)
                self["sprite_item" .. tostring(i)]:addChild(_obj_info.pIcon)
                local size = self["sprite_item" .. tostring(i)]:getContentSize()
                _obj_info.pIcon:setPosition(ccp(size.width * 0.5, size.height * 0.5))
                _obj_info.pIcon:setAnchorPoint(ccp(0.5, 0.5))
                _obj_info.pIcon:setTag(99)
                if _drop_type ~= 1 then
                    _obj_info.pIcon:setScale(0.9)
                end
            end
            -- bGot						
        else
            self["sprite_item" .. tostring(i)]:removeAllChildrenWithCleanup(true)
            local pFrameNoGiftBK = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("small_no_obj_frame")
            self["sprite_item" .. tostring(i)]:setDisplayFrame(pFrameNoGiftBK)
        end
    end

end




function onNodeCleanup(self)
    if self.proxy_ then
        self.proxy_:release()
    end

    layer_base_t.onNodeCleanup(self)
end
