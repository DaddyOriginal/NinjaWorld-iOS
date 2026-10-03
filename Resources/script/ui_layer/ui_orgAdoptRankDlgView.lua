----------------------------------------------------------------------
--  Copyright (c) 2014-2016, XCKOO. All Rights Reserved.
--  Author :lyk
--  Time   :2015-11-14
--  Remark :排行界面 dlg
----------------------------------------------------------------------
module("ui_orgAdoptRankDlgView", package.seeall)
baseClass(layer_base_t, ui_orgAdoptRankDlgView)

require("ui_layer/ui_orgAdoptRankDlgCell")

function init(self)
    self.playerMgr_ = CPlayerDataMgr:instance()
    self.playerData_ = self.playerMgr_:GetPlayerInfoData()

    self.contentSize_ = GetMainMenu():GetSubContentNode():getContentSize()
    local ccbiAttrTable = { name = "sub_ui/OrgAdoptRankDlgView.ccbi", size = self.contentSize_ }
    layer_base_t.init(self, true, ccbiAttrTable)

    -- pre page
    self.back_page = E_DEFAULTMENU

    -- data
    self.cellNodes ={}
    -- init
    self:init_ui()
    self:init_binding_event()
end

function init_ui(self)
    if self.proxy_ ~= nil then      
        self.btn_back = tolua.cast(self.proxy_:getNode("closeButton"), "CCControlButton")
        -- node
        self.node_cardcontent = tolua.cast(self.proxy_:getNode("node_cell"), "CCNode")
        self.node_tablecontent = tolua.cast(self.proxy_:getNode("node_content"), "CCNode")
        
        self:init_data_info()
    end
end


function init_data_info(self)
    --获取基本信息
	local urlpath = GetUrlNormalHeader(self.playerData_.m_uid, 10, "rl_x_group_boss")
	--cclog("rl_r_group_comm & cmd = 4---%s", urlpath)
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
			--cclog("rl_r_group_comm ret = %s", resData)
			local retcode = item.code
			if retcode == "0" then
				self.tableData = item:find("currrank")
													
				--ext init ui
				self:init_ext_ui()
			else
				GetMainMenu():ShowErrorTip(tonumber(retcode),-1)
			end
		end)
        
end


function init_ext_ui(self)
    self:createRankTableView()
end

function init_binding_event(self)
	if self.proxy_ ~= nil then
		local function onBtnBack(btn, event)
			-- GetMainMenu():ChangeToSub(self.back_page)
			self.node_:removeFromParentAndCleanup(true)
			
		end
        
		self.btn_back:setTouchPriority(kCCMenuHandlerPriority - 1)
		self.btn_back:setTouchEnabled(true)
		self.proxy_:handleButtonEvent(self.btn_back, function(button, event)
			onBtnBack(button)
			return nil
		end , CCControlEventTouchDown)
		

	end
end


function createRankTableView(self)
    -- body
    if self.rankTableView == nil then
        self.rank_cellsize = self.node_cardcontent:getContentSize()
        self.rank_tableContentSize = self.node_tablecontent:getContentSize()
        self:initRankTableHandle()
        self.rankTableView = LuaTableView:createWithHandler(self.rankTableViewHandler, CCSizeMake(self.rank_tableContentSize.width, self.rank_tableContentSize.height))
        self.rankTableView:setDirection(kCCScrollViewDirectionVertical)
        self.rankTableView:setVerticalFillOrder(kCCTableViewFillTopDown)
        self.rankTableView:setTouchPriority(kCCMenuHandlerPriority - 1)
        self.node_tablecontent:addChild(self.rankTableView)
    else
        self.rankTableView:reloadData()
    end
end

function initRankTableHandle(self)
    self.rankTableViewHandler = LuaEventHandler:create( function(fn, table, a1, a2, x, y)
        local r
        if fn == "cellSize" then
            r = self.rank_cellsize;
        elseif fn == "cellAtIndex" then
        
            local nodeLayer = createObj(ui_orgAdoptRankDlgCell, self._cell_size, self.tableData[a1 + 1])
           
            self.cellNodes[a1 + 1] = nodeLayer
            if not a2 then
                a2 = CCTableViewCell:create()
                a2:addChild(nodeLayer.node_)
            else
                a2:removeAllChildrenWithCleanup(true)
                a2:addChild(nodeLayer.node_)
            end
            nodeLayer.node_:setTag(100);
           
            r = a2
        elseif fn == "numberOfCells" then
            r = #self.tableData
        elseif fn == "cellTouched" then
           
            self.m_touchPoint = nil;

        elseif fn == "cellTouchBegan" then
            -- A cell is touching, a1 is cell, a2 is CCTouch
            self.m_touchPoint = a2:getLocation()
            local cell = self.cellNodes[a1:getIdx() + 1]
            self.m_touchPoint = cell.node_:convertToNodeSpace(self.m_touchPoint)
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
