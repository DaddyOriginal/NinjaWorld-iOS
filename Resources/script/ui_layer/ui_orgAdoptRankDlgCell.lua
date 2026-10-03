----------------------------------------------------------------------
--  Copyright (c) 2014-2016, XCKOO. All Rights Reserved.
--  Author :lyk
--  Time   :2015-11-14
--  Remark :排行榜cell
----------------------------------------------------------------------
module("ui_orgAdoptRankDlgCell", package.seeall)
baseClass(layer_base_t, ui_orgAdoptRankDlgCell)

function init(self, cellSize, data)
	local ccbiAttrTable = {name="sub_ui/OrgAdoptRankDlgCell.ccbi", size=cellSize}
	layer_base_t.init(self, true, ccbiAttrTable)
	
	self.cellData = data
	--init
	self:init_ui()
end

function init_ui(self)
	if self.proxy_ ~= nil then
		self.label_cell_rank = tolua.cast(self.proxy_:getNode("label_cell_rank"),"CCLabelTTF")
		self.label_cell_name = tolua.cast(self.proxy_:getNode("label_cell_name"),"CCLabelTTF")
		self.label_cell_hurt = tolua.cast(self.proxy_:getNode("label_cell_hurt"),"CCLabelTTF")
		self.label_cell_score = tolua.cast(self.proxy_:getNode("label_cell_score"),"CCLabelTTF")
		
		--init info
		self:init_ui_ext()
	end
end

function init_ui_ext(self)

    local rank = self.cellData:find("rank")[1]
    local nick = self.cellData:find("nick")[1]
    local hurt = self.cellData:find("score")[1]
    local score = self.cellData:find("real_score")[1]
	self.label_cell_rank:setString(rank)
	self.label_cell_name:setString(nick)
	self.label_cell_hurt:setString(hurt)
    self.label_cell_score:setString(score)
	
end


function onNodeCleanup(self)
	---[[
	if self.proxy_ then
		self.proxy_:release()
		self.proxy_ = nil
	end
	--]]
	layer_base_t.onNodeCleanup(self)
end
