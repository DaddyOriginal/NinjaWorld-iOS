
module("ui_orgTechnologyLayer", package.seeall)
baseClass(layer_base_t, ui_orgTechnologyLayer)

require("ui_layer/ui_orgTechnologyCell")
require("global")

function init(self, datas)

	self.playerMgr_ = CPlayerDataMgr:instance()
	self.playerData_ = self.playerMgr_:GetPlayerInfoData()

    self.contentSize_ = GetMainMenu():GetSubContentNode():getContentSize()
	local ccbiAttrTable = { name = "sub_ui/OrgTechnologyView.ccbi", size = self.contentSize_ }
	layer_base_t.init(self, true, ccbiAttrTable)

    initHeader(self.proxy_)

    self.datas = datas

    global.is_click_outSide = false

	self.cellNodes = {}

	self:init_ui()
	self:init_binding_event()
end

function init_ui(self)
	if self.proxy_ ~= nil then
        local proxy = self.proxy_
        self.node_table_content = getNodeFromCCB(proxy, "node_table_content")
		self.node_cell = getNodeFromCCB(proxy, "node_cell_content")

        self.btn_back = getButtonFromCCB(proxy, "btn_back")
        self.layer_touch = getLayerFromCCB(proxy, "layer_touch")
        self.text_effect_title = getLabelTTFFromCCB(proxy, "text_effect_title")
        self.text_buildingLv_desc = getLabelTTFFromCCB(proxy, "text_buildingLv_desc")
        self.text_orgContribution_desc = getLabelTTFFromCCB(proxy, "text_orgContribution_desc")
        self.text_attac_add = getLabelTTFFromCCB(proxy, "text_attac_add")
        self.text_attac_percent = getLabelTTFFromCCB(proxy, "text_attac_percent")
        self.text_def_add = getLabelTTFFromCCB(proxy, "text_def_add")
        self.text_def_percent = getLabelTTFFromCCB(proxy, "text_def_percent")
        self.text_chakra_add = getLabelTTFFromCCB(proxy, "text_chakra_add")
        self.text_chakra_percent = getLabelTTFFromCCB(proxy, "text_chakra_percent")

		self:refreshData()
	end
end

function refreshData(self)

    initHeader(self.proxy_)
    
    local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 1, "rl_x_group_tech")
    --cclog("urlpath = %s", urlpath)

    function getTechnologyDatasCallback(data) 
        
        --cclog("callback data = %s", data)

        data = data:find("tech")

        self.tech_level = data.tech_level

        -- 贡献值
        self.score = getNumber(data, "score")

        self.datas = {}

        local effect = data:find("total")
        attack_add = getNumber(effect, "last_attack") 
        defense_add = getNumber(effect, "last_defense")        
        chakra_add = getNumber(effect, "last_chakra")        
        attack_percent = getNumber(effect, "last_attack_percent")         
        defense_percent = getNumber(effect, "last_defense_percent") 
        chakra_percent = getNumber(effect, "last_chakra_percent") 

        for i = 2, #data - 1 do
            local skill = data[i]:find("skill")
            local temp = {}
            -- 兑换id
            temp.skill_id = getNumber(skill, "skill_id")
            -- 名称
            temp.skill_name = getString(skill, "skill_name")
            -- 最大等级
            temp.level_limit = getNumber(skill, "level_limit")
            -- 当前等级
            temp.skill_level = getNumber(skill, "skill_level")
            -- 兑换消耗
            temp.need_score = getNumber(skill, "need_score")
            -- 攻击力增加
            temp.add_attack_num = getNumber(skill, "add_attack_num")
            -- 防御值增加
            temp.add_defense_num = getNumber(skill, "add_defense_num")
            -- 攻击力百分比增加
            temp.add_attack_percent = getNumber(skill, "add_attack_percent")
            -- 防御值百分比增加
            temp.add_defense_percent = getNumber(skill, "add_defense_percent")
            -- 查克拉百分比增加
            temp.add_chakra_percent = getNumber(skill, "add_chakra_percent")
            -- 查克拉增加
            temp.add_chakra_num = getNumber(skill, "add_chakra_num")
            -- 元宝产出提升
            temp.add_gold = getNumber(skill, "add_money")
            -- 经验产出提升
            temp.add_exp = getNumber(skill, "add_exp")
            ---[[
            -- 技能图标
            temp.skill_icon = getString(skill, "skill_icon")
            -- 技能品质
            temp.skill_quality = getNumber(skill, "skill_quality")
            --]]
            self.datas[#self.datas + 1] = temp
        end   

        self.text_effect_title:setString(localizable.ui_orgTechnology_effect)

        self.text_buildingLv_desc:setString(string.format(localizable.ui_orgTechnology_curBuildingLv .. "%d", self.tech_level))
        self.text_orgContribution_desc:setString(string.format(localizable.ui_orgTechnology_orgContribution .. "%d", self.score))

        self.text_attac_add:setString(tostring(localizable.ui_orgTechnology_attackAdd .. attack_add))
        self.text_attac_percent:setString(tostring(localizable.ui_orgTechnology_attackPercent .. attack_percent .. "%"))

        self.text_def_add:setString(tostring(localizable.ui_orgTechnology_defenseAdd .. defense_add))
        self.text_def_percent:setString(tostring(localizable.ui_orgTechnology_defensePercent .. defense_percent .. "%"))

        self.text_chakra_add:setString(tostring(localizable.ui_orgTechnology_chakraAdd .. chakra_add))
        self.text_chakra_percent:setString(tostring(localizable.ui_orgTechnology_chakraPercent .. chakra_percent .. "%"))
    -- ]]

        self:createTableView()
    end

    sendRequest(urlpath, getTechnologyDatasCallback)

    --- [[
end

function init_binding_event(self)

    local function CCLayerTouch(event,x,y)
        local rect = self.layer_touch:boundingBox()
        rect.origin = ccp(0,0)
        local p = self.layer_touch:convertToNodeSpace(ccp(x,y))
        if event == "began" then
            if rect:containsPoint(p) == true then
                global.is_click_outSide = true
                return true
            else
                global.is_click_outSide = false
                return false
            end
        end
    end
    self.layer_touch:setTouchEnabled(true)
    self.layer_touch:registerScriptTouchHandler(CCLayerTouch, false, kCCMenuHandlerPriority - 1, false)

    self:init_btn_binding_event(self.btn_back, 
        function(button, event)
            self.node_:removeFromParentAndCleanup(true)
			ShowOrgMapLayer()
        end,
        localizable.ui_label_back_text
    )
end

function createTableView(self)
	-- body
	if self._tableView == nil then
		local cellContentSize = self.node_cell:getContentSize()
		self.cell_size = CCSizeMake(cellContentSize.width, cellContentSize.height)

		self.content_size = self.node_table_content:getContentSize()
		self:initPicTableHandle()
		self._tableView = LuaTableView:createWithHandler(self._tableViewHandler, CCSizeMake(self.content_size.width, self.content_size.height))
		self._tableView:setDirection(kCCScrollViewDirectionVertical)
		self._tableView:setVerticalFillOrder(kCCTableViewFillTopDown)
		self._tableView:setTouchPriority(kCCMenuHandlerPriority - 1)

		self.node_table_content:addChild(self._tableView)
	else
		self._tableView:reloadData()
	end
end

function updateUI(self, data)

        self.text_attac_add:setString(tostring(localizable.ui_orgTechnology_attackAdd .. data.attack_add))
        self.text_attac_percent:setString(tostring(localizable.ui_orgTechnology_attackPercent .. data.attack_percent .. "%"))

        self.text_def_add:setString(tostring(localizable.ui_orgTechnology_defenseAdd .. data.defense_add))
        self.text_def_percent:setString(tostring(localizable.ui_orgTechnology_defensePercent .. data.defense_percent .. "%"))

        self.text_chakra_add:setString(tostring(localizable.ui_orgTechnology_chakraAdd .. data.chakra_add))
        self.text_chakra_percent:setString(tostring(localizable.ui_orgTechnology_chakraPercent .. data.chakra_percent .. "%"))   

        self.score = self.score - data.cost_score
        if self.score <= 0 then self.score = 0 end
        self.text_orgContribution_desc:setString(string.format(localizable.ui_orgTechnology_orgContribution .. "%d", self.score))
end

function initPicTableHandle(self)
	self._tableViewHandler = LuaEventHandler:create( function(fn, table, a1, a2, x, y)
		local r
		if fn == "cellSize" then
			r = self.cell_size
		elseif fn == "cellAtIndex" then
			local nodeLayer = createObj(ui_orgTechnologyCell, self.datas[a1 + 1], self.cell_size, self)
			-- tableView cell container
			self.cellNodes[a1 + 1] = nodeLayer
			if not a2 then
				a2 = CCTableViewCell:create()
				a2:addChild(nodeLayer.node_)
			else
				a2:removeAllChildrenWithCleanup(true)
				a2:addChild(nodeLayer.node_)
			end

			nodeLayer.node_:setTag(100)
			r = a2
		elseif fn == "numberOfCells" then
            r = #self.datas
			-- Cell events:
		elseif fn == "cellTouched" then
			-- A cell was touched, a1 is cell that be touched. This is not necessary.
			--- [[
			local cell_index = a1:getIdx() + 1
			local _layer = self.cellNodes[cell_index]

--			if _layer.btnGet:boundingBox():containsPoint(self.m_touchPoint) then
--				self:onClickedGet(cell_index)
--			end

			self.m_touchPoint = nil
			-- ]]
		elseif fn == "cellTouchBegan" then
			-- A cell is touching, a1 is cell, a2 is CCTouch
			self.m_touchPoint = a2:getLocation()
			self.m_touchPoint = a1:convertToNodeSpace(self.m_touchPoint)

			local cell_index = a1:getIdx() + 1

			r = true
		elseif fn == "cellTouchEnded" then
			-- A cell was touched, a1 is cell, a2 is CCTouch
			r = true
		elseif fn == "cellHighlight" then
			-- A cell is highlighting, coco2d-x 2.1.3 or above
		elseif fn == "cellUnhighlight" then
			-- A cell had been unhighlighted, coco2d-x 2.1.3 or above
		elseif fn == "cellWillRecycle" then
			-- A cell will be recycled, coco2d-x 2.1.3 or above
		end
		return r
	end )
end

function onNodeCleanup(self)
	-- cclog("1111---001")
	if self.proxy_ then
		self.proxy_:release()
	end
	if self.tableData ~= nil then
		table.remove(self.tableData)
	end
	layer_base_t.onNodeCleanup(self)
end

function getNumber(data, name)
    return tonumber(data:find(name)[1])
end

function getString(data, name)
    return tostring(data:find(name)[1])
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