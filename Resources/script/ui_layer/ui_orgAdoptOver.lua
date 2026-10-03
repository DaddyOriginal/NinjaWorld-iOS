----------------------------------------------------------------------
--  Copyright (c) 2014-2016, XCKOO. All Rights Reserved.
--  Author :lyk
--  Time   :2015-11-11
--  Remark :培养完成 进入狩猎界面
----------------------------------------------------------------------
module("ui_orgAdoptOver", package.seeall)
baseClass(layer_base_t, ui_orgAdoptOver)

function init(self, huntinfo)
    self.playerMgr_ = CPlayerDataMgr:instance()
    self.playerData_ = self.playerMgr_:GetPlayerInfoData()

    self.contentSize_ = GetMainMenu():GetSubContentNode():getContentSize()
    local ccbiAttrTable = { name = "sub_ui/OrgAdoptOver.ccbi", size = self.contentSize_ }
    layer_base_t.init(self, true, ccbiAttrTable)

    -- pre page
    self.back_page = E_DEFAULTMENU

    -- data
    self.huntinfo = huntinfo
    -- init
    self:init_ui()
    self:init_binding_event()
end

function init_ui(self)
    if self.proxy_ ~= nil then
        initHeader(self.proxy_)
        -- label	
        self.label_hp = tolua.cast(self.proxy_:getNode("label_hp"), "CCLabelTTF")
        -- btn
        self.btn_back = tolua.cast(self.proxy_:getNode("btn_back"), "CCControlButton")
        self.btn_adopt = tolua.cast(self.proxy_:getNode("button_adopt"), "CCControlButton")
        self.btn_desc = tolua.cast(self.proxy_:getNode("button_desc"), "CCControlButton")
        --sprite
        self.sprite_icon = tolua.cast(self.proxy_:getNode("sprite_icon"), "CCSprite")
        self:init_ui_ext()
    end
end


function init_ui_ext(self)
    if self.huntinfo then
        self.begin_time = tonumber(self.huntinfo:find("begin_time")[1])
        self.boss_life = tonumber(self.huntinfo:find("boss_life")[1])
        self.label_hp:setString(self.boss_life .."/" .. self.boss_life)
        self.boss_pic = self.huntinfo:find("boss_pic")[1]
        local pIconFrame = CGameObjElement:GetNinjaIcon(E_FRAMETYPE_MIDDLE, self.boss_pic)
	    if pIconFrame then
	        self.sprite_icon:setDisplayFrame(pIconFrame)
	    end
    end
end


function init_binding_event(self)
	if self.proxy_ ~= nil then
		local function onBtnBack(btn, event)
			-- GetMainMenu():ChangeToSub(self.back_page)
			self.node_:removeFromParentAndCleanup(true)
			ShowOrgMapLayer()
		end
        
		local function onBtnAdopt(btn, event)
			--开启狩猎			
            local playerinfo = CPlayerDataMgr:instance():GetPlayerInfoData()
		    local urlpath = GetUrlNormalHeader(playerinfo.m_uid, 4, "rl_x_group_boss")
	        urlpath = AddData(urlpath, "TrainType", self.current_select)
		    --cclog("rl_x_group_boss & cmd = 1---%s", urlpath)
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
				    cclog("rl_x_group_boss ret = %s", resData)
				    local retcode = item.code
				    if retcode == "0" then
					    local preview = xfile:find("preview")                
	                    local status = tonumber(preview.status)
	                    if status~= 3 then --状态改变 直接重新请求新的数据
	                        self.node_:removeFromParentAndCleanup(true)
	                        ShowAdoptView()
                            return
	                    end              
	                	           
			        else
				        GetMainMenu():ShowErrorTip(tonumber(retcode),-1)
                        
                        if tonumber(retcode) == 640014 then
                             self.node_:removeFromParentAndCleanup(true)
	                         ShowAdoptView()
                        end
			        end        
		        end)                   
        end

		local function onBtnDesc(btn, event)
			local monthLayer = createObj(ui_orgAdoptDesc)
		    local size1 = GetMainMenu():GetModelLayer():getContentSize()
		    monthLayer.node_:setAnchorPoint(ccp(0.5, 0.5))

		    monthLayer.node_:setPosition(size1.width / 2, size1.height / 2)
		    GetMainMenu():GetModelLayer():addChild(monthLayer.node_)
		end	

		self.btn_back:setTouchPriority(kCCMenuHandlerPriority - 1)
		self.btn_back:setTouchEnabled(true)
		self.proxy_:handleButtonEvent(self.btn_back, function(button, event)
			onBtnBack(button)
			return nil
		end , CCControlEventTouchDown)

		self.btn_adopt:setTouchPriority(kCCMenuHandlerPriority - 1)
		self.btn_adopt:setTouchEnabled(true)
		self.proxy_:handleButtonEvent(self.btn_adopt, function(button, event)
			onBtnAdopt(button)
			return nil
		end , CCControlEventTouchDown)

		self.btn_desc:setTouchPriority(kCCMenuHandlerPriority - 1)
		self.btn_desc:setTouchEnabled(true)
		self.proxy_:handleButtonEvent(self.btn_desc, function(button, event)
			onBtnDesc(button)
			return nil
		end , CCControlEventTouchDown)


	end
end

function onNodeCleanup(self)
	if self.proxy_ then
		self.proxy_:release()
	end

	layer_base_t.onNodeCleanup(self)
end
