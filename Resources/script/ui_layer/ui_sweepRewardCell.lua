--闯关扫荡_Cell
--litao
--2014.5.6
---------------------------------------------
module("ui_towerSweepCell", package.seeall)
baseClass(layer_base_t, ui_towerSweepCell)

function init(self, cellSize, data)
	local ccbiAttrTable = {name="sub_ui/TowerSweepCell.ccbi", size=cellSize}
	layer_base_t.init(self, true, ccbiAttrTable)

	self.cellData = data
	--init	
	self._data = {} 
	self:init_ui()
end

function init_ui(self)
	if self.proxy_ ~= nil then
		--spr/node/lebel
		for i=1,4 do
			self["spr_level_"..i] = tolua.cast(self.proxy_:getNode("spr_level_"..i), "CCSprite")
			self["spr_select_"..i] = tolua.cast(self.proxy_:getNode("spr_select_"..i), "CCSprite")
			self["spr_select_frame_"..i] = tolua.cast(self.proxy_:getNode("spr_select_frame_"..i), "CCSprite")
			self["label_level_name_"..i] = tolua.cast(self.proxy_:getNode("label_level_name_"..i), "CCLabelBMFont")
			self["node_content_"..i] = tolua.cast(self.proxy_:getNode("node_content_"..i), "CCNode")
		end

		--init info
		self:init_ui_ext()
	end
end

--
function init_ui_ext(self)
	CCSpriteFrameCache:sharedSpriteFrameCache():addSpriteFramesWithFile("ccbResources/tower_ui.plist")
	--init icon
	for i=1, 4 do
		if i > #self.cellData then
			self["node_content_"..i]:setVisible(false)
		else
			self["label_level_name_"..i]:setString(self.cellData[i].name)
			self["spr_select_"..i]:setVisible(self.cellData[i].bSelected)

			--level frame		
			local pFrame = nil
			if self.cellData[i].select_status == 0 then
				pFrame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("lock_tower")			
			elseif self.cellData[i].select_status == 1 then
				pFrame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("pass_tower")
			end
			if nil ~= pFrame then
				self["spr_level_"..i]:setDisplayFrame(pFrame)
			end
		end
	end
end

function init_binding_event(self)
	--
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