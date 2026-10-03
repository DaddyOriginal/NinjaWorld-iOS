
module("ui_orgUpgradeCell", package.seeall)
baseClass(layer_base_t, ui_orgUpgradeCell)

function init(self, data, index, cellSize, parent)
	local ccbiAttrTable = { name = "sub_ui/OrgUpgradeCell.ccbi", size = cellSize }
	self.playerMgr_ = CPlayerDataMgr:instance()
	self.playerData_ = self.playerMgr_:GetPlayerInfoData()
	layer_base_t.init(self, true, ccbiAttrTable)

    self.index = index

    self.data = data

    self.parent = parent

	self:init_ui()
    self:init_binding_event()
end

function init_ui(self)
	if self.proxy_ ~= nil then
        local proxy = self.proxy_

        self.sprite_building_icon = getSpriteFromCCB(proxy, "sprite_building_icon")
        self.btn_upgrade = getButtonFromCCB(proxy, "btn_upgrade")
        self.text_condition = getLabelTTFFromCCB(proxy, "text_condition")
        self.text_consume = getLabelTTFFromCCB(proxy, "text_consume")
        self.text_effect = getLabelTTFFromCCB(proxy, "text_effect")
        self.text_building_level = getLabelTTFFromCCB(proxy, "text_building_level")
        self.text_building_name = getLabelTTFFromCCB(proxy, "text_building_name")

        self.text_condition_desc = getLabelTTFFromCCB(proxy, "text_condition_desc")
        self.text_consume_desc = getLabelTTFFromCCB(proxy, "text_consume_desc")
        self.text_effect_desc = getLabelTTFFromCCB(proxy, "text_effect_desc")

        self:refreshData()
	end
end

--
function refreshData(self)
    
    self.text_condition_desc:setString(localizable.ui_orgUpgrade_condition)
    self.text_consume_desc:setString(localizable.ui_orgUpgrade_consume)
    self.text_effect_desc:setString(localizable.ui_orgUpgrade_effect)

    self.sprite_building_icon:setDisplayFrame(CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(self.data.icon))
    self.text_building_name:setString(self.data.name)

    if self.data.max_level_flag == 1 then
        self.text_condition:setString(localizable.ui_orgUpgrade_condition_0)
        self.text_consume:setString(localizable.ui_orgUpgrade_condition_0)
        self.text_effect:setString(localizable.ui_orgUpgrade_top)
    else
        self.text_consume:setString(self.data.consume_score .. localizable.ui_orgUpgrade_expend)
        self.text_effect:setString(self.data.effect_desc)
		local needConstruct = tonumber(self.data.require_construct_id)
		local strCondition = localizable.ui_orgUpgrade_condition_0
		if needConstruct > 0 and needConstruct <= #localizable.OrgBuilding then
			strCondition = localizable.OrgBuilding[needConstruct] .. self.data.require_construct_level .. localizable.ui_orgUpgrade_condition_1
		end
		self.text_condition:setString(strCondition)
    end

    self.text_building_level:setString(localizable.ui_orgBuildLv .. self.data.level)
end


function init_binding_event(self)

     self:init_btn_binding_event(self.btn_upgrade, 
        function(button, event)
            self:levelUp()
        end,
        localizable.ui_label_upgrade_text
    )
end

function levelUp(self)

    -- 请求基本信息  
  	local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 9, "rl_w_group_admin")
	urlpath = AddData(urlpath, "GroupId", global.myOrgId)
	urlpath = AddData(urlpath, "ConstructID", self.data.construct_id)

    --cclog("rl_w_group_admin & cmd = 2---%s", urlpath)

    function levelUpCallback(data)

        --cclog("data = %s", data)

        self.score = getNumber(data:find("group_info"), "score")

        data = data:find("construct_list")
 
        local item = data[self.index]:find("item")
        self.data.level = tonumber(item.level)
        self.data.require_construct_level = tonumber(item.require_construct_level)
        self.data.consume_score = tonumber(item.consume_score)
        self.data.construct_level = tonumber(item.construct_level)
        self.data.max_level_flag = tonumber(item.max_level_flag)
        self.data.require_construct_id = tonumber(item.require_construct_id)
        self.data.effect_desc = tostring(item.effect_desc)

        self.parent:refreshData(self.score)

        GetMainMenu():ShowTextTip(localizable.ui_orgUpgrade_suc, -1)
        self:refreshData()
    end

    sendRequest(urlpath, levelUpCallback)
end

function onNodeCleanup(self)
	--- [[
	if self.proxy_ then
		self.proxy_:release()
		self.proxy_ = nil
	end
	-- ]]
	layer_base_t.onNodeCleanup(self)
end

function init_btn_binding_event(self, btn_node, callback, btn_title_text)
    btn_node:setTouchEnabled(true)
    btn_node:setTouchPriority(kCCMenuHandlerPriority - 1)
    self.proxy_:handleButtonEvent(btn_node, callback , CCControlEventTouchUpInside)
    if btn_title_text ~= nil then
        --btn_node:setTitleForState(btn_title_text, CCControlStateNormal)
        --btn_node:setTitleForState(btn_title_text, CCControlStateHighlighted)
        --btn_node:setTitleForState(btn_title_text, CCControlStateDisabled)    
    end
end

function getNumber(data, name)
    return tonumber(data:find(name)[1])
end
