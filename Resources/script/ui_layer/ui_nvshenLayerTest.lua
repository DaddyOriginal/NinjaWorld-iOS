--descriptioin:女神献花界面
--company: xckoo
--author: chenchun
--date: 2013-1-16

---------------------------------------------
module("ui_nvshenLayerTest", package.seeall)
baseClass(layer_base_t, ui_nvshenLayerTest)


function init(self)
	self.playerMgr_ = CPlayerDataMgr:instance()
	self.playerData_ = self.playerMgr_:GetPlayerInfoData()

	self.contentNode_ = GetActivityView():GetNodeContent()
	self.contentSize_ = self.contentNode_:getContentSize()

	--cclog("1111---%d, %d", self.contentSize_.width, self.contentSize_.height)
	local ccbiAttrTable = {name="activity/SuperNinjaChallenge.ccbi", size=self.contentSize_}
	layer_base_t.init(self, true, ccbiAttrTable)

	self.m_bIsEndAnimatedScroll_ = true
	self.touchHandler_ = {}
	self.sprite_config_data = {}
	self.leftTime = 0
	self.acc_money = 0
	self.deltatime = 0

	--self:create_test_data()
	--self.cellNodes = {}   --首充信息列表的单元格信息，元素为ui_purchaseTableCell 类型
	self:init_ui()
	self:init_binding_event()
end


function init_ui(self)
	if self.proxy_ ~= nil then
		self.node_sprite_content = tolua.cast(self.proxy_:getNode("node_sprite_content"), "CCNode")
		local node_size = self.node_sprite_content:getContentSize()
		local configData = {
			[1] = {id = 1, icon = "nvshen_meinv1", desc = "献花1朵", ccbi="nvshenxianhua_chutian"},
			[2] = {id = 2, icon = "nvshen_meinv2", desc = "献花10朵", ccbi="nvshenxianhua_xiaonan"},
			[3] = {id = 3, icon = "nvshen_meinv3", desc = "献花30朵", ccbi="nvshenxianhua_shuiying"},
			[4] = {id = 4, icon = "nvshen_meinv4", desc = "献花50朵", ccbi="nvshenxianhua_chunyeying"},
			[5] = {id = 5, icon = "nvshen_meinv5", desc = "献花100朵", ccbi="nvshenxianhua_gangshou"}
		}
		--local sprite_scales = {[1] = 0.31, [2] = 0.65, [3] = 1, [4] = 0.65, [5] = 0.31}
		local sprite_scales = {[1] = 0.65, [2] = 1, [3] = 0.65}
		local sprite_opactiy = {[1] = 192, [2] = 255, [3] = 192}

		local function moveCallback(id)
			cclog("1111----move:%d", id)
		end

		local function clickCallback(id)
			cclog("1111----click:%d", id)
		end

		--self.huadongCtrl = createObj(ui_selectActor, "activity/HuaDongCtrl.ccbi", 5, 0.4, 2, 75, configData, sprite_scales, sprite_opactiy, node_size, true, moveCallback,clickCallback)
		require("ui_common/ui_selectActor")
		self.huadongCtrl = createObj(ui_selectActor, "activity/HuaDongCtrl1.ccbi", 3, 0.75, 2.25, 155, configData, sprite_scales, sprite_opactiy, node_size, true, moveCallback,clickCallback)
		self.huadongCtrl.node_:setPosition(0, 0)
		self.huadongCtrl.node_:setAnchorPoint(ccp(0, 0))
		self.node_sprite_content:addChild(self.huadongCtrl.node_)

	end
end

function init_binding_event(self)


end


function create_test_data(self)
	self.test_data = {
		[1] = {id = 1, icon = "nvshen_meinv1", desc = "献花1朵", ccbi="nvshenxianhua_chutian"},
		[2] = {id = 2, icon = "nvshen_meinv2", desc = "献花10朵", ccbi="nvshenxianhua_xiaonan"},
		[3] = {id = 3, icon = "nvshen_meinv3", desc = "献花30朵", ccbi="nvshenxianhua_shuiying"},
		[4] = {id = 4, icon = "nvshen_meinv4", desc = "献花50朵", ccbi="nvshenxianhua_chunyeying"},
		[5] = {id = 5, icon = "nvshen_meinv5", desc = "献花100朵", ccbi="nvshenxianhua_gangshou"}
	}
end