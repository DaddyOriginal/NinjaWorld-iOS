module("ui_commonOrgMainDescLayer", package.seeall)
baseClass(layer_base_t, ui_commonOrgMainDescLayer)

require("ui_layer/ui_commonOrgMainDescCell")

function init(self)
	self.contentSize = GetMainMenu():GetModelLayer():getContentSize()
    local ccbiAttrTable = { name = "dlg_ui/CommonOrgMainDesc.ccbi", size = self.contentSize }
    layer_base_t.init(self, true, ccbiAttrTable)

    self.playerMgr = CPlayerDataMgr:instance()
    self.playerData = self.playerMgr:GetPlayerInfoData()

    self.preLayer = preLayer

    self.cellNodes = {}

    self.datas = {
        localizable.ui_orgMainDesc_text1,
        localizable.ui_orgMainDesc_text2,
        localizable.ui_orgMainDesc_text3,
        localizable.ui_orgMainDesc_text4,
        localizable.ui_orgMainDesc_text5
    }

    self:init_ui()
    self:init_binding_event()
end

function init_ui(self)
    if self.proxy_ ~= nil then

        local proxy = self.proxy_
        self.btn_close = getButtonFromCCB(proxy, "btn_close")
        self.btn_sure = getButtonFromCCB(proxy, "btn_sure")
        self.node_content = getNodeFromCCB(proxy, "node_content")
		self.node_cell = getNodeFromCCB(proxy, "node_cell")
        self.text_title = getLabelTTFFromCCB(proxy, "text_title")
        self:refreshData()
    end
end

function initPicTableHandle(self)
	self._tableViewHandler = LuaEventHandler:create(function(fn, table, a1, a2, x, y)
		local r
		if fn == "cellSize" then
			r = self.cell_size;
		elseif fn == "cellAtIndex" then
    		local nodeLayer = createObj(ui_commonOrgMainDescCell, self.datas[a1 + 1], self.cell_size)
			--tableView cell container
			self.cellNodes[a1+1] = nodeLayer
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
			r = #self.datas;
		    -- Cell events:
		elseif fn == "cellTouched" then			-- A cell was touched, a1 is cell that be touched. This is not necessary.
			local cell_index = a1:getIdx() + 1
			local _layer = self.cellNodes[cell_index]

			self.m_touchPoint = nil
		elseif fn == "cellTouchBegan" then		-- A cell is touching, a1 is cell, a2 is CCTouch
			self.m_touchPoint = a2:getLocation()
			self.m_touchPoint = a1:convertToNodeSpace(self.m_touchPoint)

			local cell_index = a1:getIdx() + 1

			r = true
		elseif fn == "cellTouchEnded" then		-- A cell was touched, a1 is cell, a2 is CCTouch
			r = true
		elseif fn == "cellHighlight" then		-- A cell is highlighting, coco2d-x 2.1.3 or above
		elseif fn == "cellUnhighlight" then		-- A cell had been unhighlighted, coco2d-x 2.1.3 or above
		elseif fn == "cellWillRecycle" then		-- A cell will be recycled, coco2d-x 2.1.3 or above
		end
		return r
	end)
end

function createTabelView(self)

	-- body
	if self._tableView == nil then
		local cellContentSize = self.node_cell:getContentSize()
		self.cell_size = CCSizeMake(cellContentSize.width,cellContentSize.height)

		self.content_size = self.node_content:getContentSize()
		self:initPicTableHandle()
		self._tableView = LuaTableView:createWithHandler(self._tableViewHandler, CCSizeMake(self.content_size.width, self.content_size.height))
		self._tableView:setDirection(kCCScrollViewDirectionVertical)
		self._tableView:setVerticalFillOrder(kCCTableViewFillTopDown)
		self._tableView:setTouchPriority(kCCMenuHandlerPriority - 1)
		
		self.node_content:addChild(self._tableView)
	else
		self._tableView:reloadData()
	end
end

function refreshData(self)

    self.text_title:setString(localizable.ui_orgMainDesc_title)
    self:createTabelView()
end

function init_binding_event(self)
    local function CCLayerTouch(event, x, y)
        local rect = self.node_:boundingBox()
        rect.origin = ccp(0, 0)
        local p = self.node_:convertToNodeSpace(ccp(x, y))
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

    local function closeDialog(button, event)
        self.node_:removeFromParentAndCleanup(true)
    end

    self:init_btn_binding_event(self.btn_close, 
        closeDialog,
        ""
    )

    self:init_btn_binding_event(self.btn_sure, 
        closeDialog,
        localizable.ui_label_info_ok_text
    )
    
end

---------------------common------------------------

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

--endregion
