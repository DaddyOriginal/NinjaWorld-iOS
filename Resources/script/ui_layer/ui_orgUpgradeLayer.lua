----------------------------------------------------------------------
--  Copyright (c) 2014-2016, XCKOO. All Rights Reserved.
--  Author :Tango
--  Time   :2014/11/21 21:00:04
--  Remark :建筑升级
----------------------------------------------------------------------
module("ui_orgUpgradeLayer", package.seeall)
baseClass(layer_base_t, ui_orgUpgradeLayer)

require("ui_layer/ui_orgUpgradeCell")


function init(self, node)
	self.contentSize_ = GetMainMenu():GetModelLayer():getContentSize()
	local ccbiAttrTable = {name="sub_ui/OrgUpgradeView.ccbi", size=self.contentSize_}
	layer_base_t.init(self, true, ccbiAttrTable)

	--用户info
	self.playerMgr_ = CPlayerDataMgr:instance()
	self.playerData_ = self.playerMgr_:GetPlayerInfoData()

	--pre info
	self.preNode = node

    self.buildingInfo = {
        {icon = "iconOrg", name = localizable.OrgBuilding[1]},
        {icon = "iconShop", name = localizable.OrgBuilding[2]},
        {icon = "iconDonate", name = localizable.OrgBuilding[3]},
        {icon = "iconHall", name = localizable.OrgBuilding[4]},
        {icon = "iconHall", name = localizable.OrgBuilding[5]},
        {icon = "org_add_23", name = localizable.OrgBuilding[6]},
        {icon = "org_add_24", name = localizable.OrgBuilding[7]},
    }

	self.build = 0
	self.constructs = {}

	self.cellNodes = {}
	--init
	self:init_ui()		
	self:init_binding_event()

end

function init_ui(self)
	if self.proxy_ ~= nil then
        local proxy = self.proxy_
        self.node_table_content = getNodeFromCCB(proxy, "node_table_content")
		self.node_cell = getNodeFromCCB(proxy, "node_cell")
		self.btn_close = getButtonFromCCB(proxy, "btn_close")
        self.text_upgrade_title = getLabelTTFFromCCB(proxy, "text_upgrade_title")
        self.text_constructionValue_desc = getLabelTTFFromCCB(proxy, "text_constructionValue_desc")
        self.text_construction_value = getLabelTTFFromCCB(proxy, "text_construction_value")

		self:requestBaseInfo()
	end
end

function refreshData(self, cur_score)
    
    self.text_constructionValue_desc:setString(localizable.ui_orgUpgrade_expend)
    self.score = cur_score
    self.text_construction_value:setString(self.score)
end


function requestBaseInfo(self)
	local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 8, "rl_w_group_admin")
	urlpath = AddData(urlpath, "GroupId", global.myOrgId)

    function requestCallback(data)
        
        --cclog("data = %s", data)

        self.score = getNumber(data:find("group_info"), "score")

        data = data:find("construct_list")
        self.datas = {}
        for i = 1, #data do
            local temp = {}
            local item = data[i]:find("item")
            --cclog("item = %s", item)
            temp.level = tonumber(item.level)
            temp.require_construct_level = tonumber(item.require_construct_level)
            temp.construct_id = tonumber(item.construct_id)
            temp.consume_score = tonumber(item.consume_score)
            temp.construct_level = tonumber(item.construct_level)
            temp.max_level_flag = tonumber(item.max_level_flag)
            temp.require_construct_id = tonumber(item.require_construct_id)
            temp.effect_desc = tostring(item.effect_desc)
            temp.icon = self.buildingInfo[temp.construct_id].icon
            temp.name = self.buildingInfo[temp.construct_id].name

            self.datas[i] = temp
        end

        self:refreshData(self.score)
        self:createTableView()
    end

    sendRequest(urlpath, requestCallback)
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

function init_binding_event(self)
	if self.proxy_ ~= nil then
        local function CCLayerTouch(event,x,y)
            local rect = self.node_:boundingBox()
            rect.origin = ccp(0,0)
            local p = self.node_:convertToNodeSpace(ccp(x,y))
            if event == "began" then
                if rect:containsPoint(p) == true then
                    return true
                else
                    return false
                end
            end
        end
        self.node_:setTouchEnabled(true)
        self.node_:registerScriptTouchHandler(CCLayerTouch, false, kCCMenuHandlerPriority - 1, true)

        self:init_btn_binding_event(self.btn_close, 
            function(button, event)
                self.preNode:refresh()
	            self.node_:removeFromParentAndCleanup(true)
            end,
            ""
        )
	end
end

function initPicTableHandle(self)
	self._tableViewHandler = LuaEventHandler:create( function(fn, table, a1, a2, x, y)
		local r
		if fn == "cellSize" then
			r = self.cell_size
		elseif fn == "cellAtIndex" then
			local nodeLayer = createObj(ui_orgUpgradeCell, self.datas[a1 + 1], a1 + 1, self.cell_size, self)
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
	if self.proxy_ then
    	self.proxy_:release()
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