require "LuaSubView.lua"
require "InGameNoticeCellView.lua"
require ("util/protocol")

InGameNoticeListView=class(
	"InGameNoticeListView",
    function()
        return CCLayer:create()
    end
)
InGameNoticeListView.m_touchPriority = kCCMenuHandlerPriority-1;

function InGameNoticeListView:create()
	local view = InGameNoticeListView.new();
	InGameNoticeListView.m_selfview = view;
	return view;
end
function onTouch(event, x, y)
     if event == "began" then
        return true
    end
end

function InGameNoticeListView:initUI()

	local view = LuaSubView:create();
	local win = CCDirector:sharedDirector():getWinSize();
	view:LoadCCBI("dlg_ui/ShowAdvertise.ccbi",CCSize(768,win.height));
	self:addChild(view)
	view:setPosition(CCPoint(win.width / 2, win.height / 2));
	self.m_dlgview = view;
   -- local btnclose = tolua.cast(self.m_dlgview:getNode("button_close"), "CCControlButton");
    self["sprite_bg"] = tolua.cast(self.m_dlgview:getNode("sprite_bg"), "CCSprite")
    self["label_date"] = tolua.cast(self.m_dlgview:getNode("label_date"), "CCLabelTTF")


    --总共需要显示多少次
    self.count = 1;
    --当前显示的次数
    self.currNumber =1;
    
    self.data_inited = 0;

	self:BindControl();
	self:LoadList();
    
   
	self:setTouchEnabled(true)
	self:setTouchMode(0)
	self:registerScriptTouchHandler(onTouch,false,InGameNoticeListView.m_touchPriority,true)
end

function InGameNoticeListView:InitData()
    if self.m_listdata~= nil then
        local filePath =  self.m_listdata[self.currNumber]:find("icon")[1]
       local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(filePath)
       if frame then 
        self["sprite_bg"]:setDisplayFrame(frame)
        end
        self["label_date"]:setString(self.m_listdata[self.currNumber]:find("time")[1]);	
    end
end

function InGameNoticeListView:BindControl()
	local btnclose = tolua.cast(self.m_dlgview:getNode("button_close"), "CCControlButton");
    btnclose:setTouchPriority(InGameNoticeListView.m_touchPriority);
	self.m_dlgview:handleButtonEvent(btnclose, function(button, event)
		self:CloseView();
		return nil
	end, CCControlEventTouchUpInside)

    --[[
    local btnGoto = tolua.cast(self.m_dlgview:getNode("button_goto"), "CCControlButton");
    btnGoto:setTouchPriority(InGameNoticeListView.m_touchPriority);
	self.m_dlgview:handleButtonEvent(btnGoto, function(button, event)
		self:GotoView();
		return nil
	end, CCControlEventTouchUpInside)
    ]]
end

function InGameNoticeListView:CloseView()
	cclog("****** 1 ****** currNumber:%d count:%d", self.currNumber, self.count);

	if self.data_inited == 0 then
		return nil
	end

	-- body
    if self.currNumber ~= self.count then
    	
        local blink = CCBlink:create(0.3,1)
        self.m_dlgview:runAction(blink)
        self.currNumber = self.currNumber+1;
        self:InitData()
    else
    	
	    InGameNoticeListView.m_selfview:removeFromParentAndCleanup(true);
	    
     end
end

function InGameNoticeListView:GotoView()
    local funcs =  self.m_listdata[self.currNumber]:find("gopage")[1]
    loadstring(funcs)();
    --关闭界面
    InGameNoticeListView.m_selfview:removeFromParentAndCleanup(true)
end

function InGameNoticeListView:LoadList()
		self.m_listdata = nil;
		local areaid = CNetUser:instance():GetAreaID();
		--calc view news id
		local playerMgr = CPlayerDataMgr:instance()
		local playerData = playerMgr:GetPlayerInfoData()
		local uid = playerData.m_uid
        local urlpath = GetInGameNewsNormalHeader(uid, 2, protocol.URL_R_NOTICE)    
		GetMainMenu():ShowLoadingDlg()
		CCHttpRequest:open(urlpath, kHttpPost,"query=param1&other=params"):sendWithHandler(
		function(res, hnd)
			
			self.data_inited = 1;
		
			local resData = res:getResponseData()
			local code = res:getResponseCode()
			local xfile = xml.parse(resData)
			local item = xfile:find("RENLONG")
			if item == nil then			
				GetMainMenu():CloseLoadding()
				return nil
			end
			local retcode = item.code
			if retcode == "0" then
				
                local oinnews_info_data = item:find("innnernews");
                local size = #oinnews_info_data;
				for k=1,size do
					local visual = oinnews_info_data[k]:find("way")[1];
					local list = Split(visual,'|');
					for i = 1, #list
					do
						if tonumber(list[i]) == areaid then
							if self.m_listdata == nil then
								self.m_listdata={};
								self.m_listdata[1] = oinnews_info_data[k];
							else
								self.m_listdata[#self.m_listdata+1] = oinnews_info_data[k];
							end
							break;
						end
					end
				end
               if self.m_listdata~= nil then 
                   self.count = #self.m_listdata;
                   self:InitData()
               end
			end			
			GetMainMenu():CloseLoadding()
		end)

end

function showInGameNoticeView()
	local view = InGameNoticeListView:create();
	view:initUI();
	GetMainMenu():GetModelLayer():addChild(view);
	view:setTag(100);
end