require("RLRequest")
require("util/protocol")
require("util/tools")
require("util/common")
require("util/localizable")
require("global/cfg")
require("ui_common/node_base_t")
require("ui_common/layer_base_t")
require("LuaXml")
require("CommonDialogView")

require("ui_layer/ui_stageGiftLayer")
require("ui_layer/ui_stageGiftCell")

function initStageGiftLayer()
	local view = createObj(ui_stageGiftLayer)
	local currentlayer = GetActivityView():GetNodeContent()
	currentlayer:addChild(view.node_)
end

xpcall(initStageGiftLayer, __G__TRACKBACK__)