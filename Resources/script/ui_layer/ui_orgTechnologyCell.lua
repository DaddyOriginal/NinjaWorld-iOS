
module("ui_orgTechnologyCell", package.seeall)
baseClass(layer_base_t, ui_orgTechnologyCell)

function init(self, data, cellSize, parent)
	local ccbiAttrTable = { name = "sub_ui/OrgTechnologyCell.ccbi", size = cellSize }
	self.playerMgr_ = CPlayerDataMgr:instance()
	self.playerData_ = self.playerMgr_:GetPlayerInfoData()
	layer_base_t.init(self, true, ccbiAttrTable)

    self.parent = parent

    self.data = data

	self:init_ui()
    self:init_binding_event()
end

function init_ui(self)
	if self.proxy_ ~= nil then
        local proxy = self.proxy_

        self.text_skill_level = getLabelBMFontFromCCB(proxy, "text_skill_level")
        self.text_skill_desc = getLabelTTFFromCCB(proxy, "text_skill_desc")
        self.text_consume_desc = getLabelTTFFromCCB(proxy, "text_consume_desc")
        self.text_skill_name = getLabelTTFFromCCB(proxy, "text_skill_name")
        self.sprite_skill_icon = getSpriteFromCCB(proxy, "sprite_skill_icon")
        self.sprite_skill_frame = getSpriteFromCCB(proxy, "sprite_skill_frame")
        self.btn_levelUp = getButtonFromCCB(proxy, "btn_levelUp")

        self:refreshData()
	end
end

--
function refreshData(self)
    
    local skill_desc = self.data.skill_name .. "+"
    
    local isPercent = false
    local add_value = 0

    if self.data.add_attack_percent ~= 0 
        or self.data.add_defense_percent ~= 0
        or self.data.add_chakra_percent ~= 0
        then
        isPercent = true
    end

    if isPercent then
        if self.data.add_attack_percent ~= 0 then add_value = self.data.add_attack_percent
        elseif self.data.add_defense_percent ~= 0 then add_value = self.data.add_defense_percent
        elseif self.data.add_chakra_percent ~= 0 then add_value = self.data.add_chakra_percent
        end

    else 
        if self.data.add_attack_num ~= 0 then add_value = self.data.add_attack_num
        elseif self.data.add_defense_num ~= 0 then add_value = self.data.add_defense_num
        elseif self.data.add_chakra_num ~= 0 then add_value = self.data.add_chakra_num
        end
    end

    skill_desc = skill_desc .. add_value

    if isPercent then
        skill_desc = skill_desc .. "%"
    end

    self.text_skill_desc:setString(tostring(skill_desc))

    self.text_consume_desc:setString(localizable.ui_orgTechnology_consume .. self.data.need_score .. localizable.ui_orgTechnology_contributionValue)
    
    self.text_skill_name:setString(self.data.skill_name)

    self.text_skill_level:setString(self.data.skill_level .. "/" .. self.data.level_limit)

    local icon = CGameObjElement:GetConsumeIcon(E_FRAMETYPE_SMALL, self.data.skill_icon)
    if icon == nil then
        --cclog("get icon error")
        return
    end
    self.sprite_skill_icon:setDisplayFrame(icon)

    self.sprite_skill_frame:setDisplayFrame(rl_get_frameicon(E_FRAMETYPE_SMALL, self.data.skill_quality))
end


function init_binding_event(self)

     self:init_btn_binding_event(self.btn_levelUp, 
        function(button, event)
            if global.is_click_outSide == true then
                global.is_click_outSide = false
                return
            end
            self:levelUp()
        end,
        localizable.ui_ninjaTest_exchange
    )
end

function levelUp(self)

    -- 请求基本信息  
  	local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 2, "rl_x_group_tech")
    urlpath = AddData(urlpath,"SkillID", self.data.skill_id)
    --cclog("rl_x_group_tech & cmd = 2---%s", urlpath)

    function levelUpCallback(data)

        --cclog("callback data = %s", data)
        
        local temp = {}
        temp.cost_score = self.data.need_score
        local total = data:find("total")

        temp.attack_add = getNumber(total, "last_attack")
        temp.defense_add = getNumber(total, "last_defense")
        temp.chakra_add = getNumber(total, "last_chakra")
        temp.attack_percent = getNumber(total, "last_attack_percent")
        temp.defense_percent = getNumber(total, "last_defense_percent")
        temp.chakra_percent = getNumber(total, "last_chakra_percent")
        
        self.parent:updateUI(temp)

        self.data.skill_level = getNumber(data, "skill_level")
        self.data.need_score = getNumber(data, "next_score")
        self.data.add_attack_num = getNumber(data, "next_attack_num")
        self.data.add_defense_num = getNumber(data, "next_defense_num")
        self.data.add_chakra_num = getNumber(data, "next_chakra_num")
        self.data.add_attack_percent = getNumber(data, "next_attack_percent")
        self.data.add_defense_percent = getNumber(data, "next_defense_percent")
        self.data.add_chakra_percent = getNumber(data, "next_chakra_percent")
        self.data.add_gold = getNumber(data, "next_money")
        self.data.add_exp = getNumber(data, "next_exp")
        self:refreshData()

        GetMainMenu():ShowTextTip(localizable.ui_orgUpgrade_suc, -1)
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
    btn_node:setTouchPriority(kCCMenuHandlerPriority - 0)
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
