module("ui_orgMsgCell", package.seeall)
baseClass(layer_base_t, ui_orgMsgCell)


function init(self, data, cellSize)
	local ccbiAttrTable = { name = "sub_ui/OrgMsgCell.ccbi", size = cellSize }
	self.playerMgr_ = CPlayerDataMgr:instance()
	self.playerData_ = self.playerMgr_:GetPlayerInfoData()
	layer_base_t.init(self, true, ccbiAttrTable)

    self.data = data 
	self:init_ui()
    self:init_binding_event()
end

function init_ui(self)
    if self.proxy_ ~= nil then
        local proxy = self.proxy_
        self.text_msg = getLabelTTFFromCCB(proxy, "text_msg")
        self:refreshData()
    end
end

function refreshData(self)
    self.text_msg:setString(self.data)
end

function init_binding_event(self)

end


---------------------common------------------------

function onNodeCleanup(self)
    if self.proxy_ then
        self.proxy_:release()
        self.proxy_ = nil
    end
    layer_base_t.onNodeCleanup(self)
end

function getNumber(data, name)
    return tonumber(data:find(name)[1])
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