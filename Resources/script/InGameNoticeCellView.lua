require "LuaSubView.lua"
require "util/localizable"

InGameNoticeCellView = class(
"InGameNoticeCellView",
function()
	return LuaSubView:create()
end
)
function InGameNoticeCellView:loadCellCCBI()
	self:LoadCCBI("sub_ui/NoticeCell.ccbi", self.m_size);
end
function InGameNoticeCellView:create()
	local view = InGameNoticeCellView.new();
	return view;
end

function InGameNoticeCellView:setData(data)
	self.m_data = data;
end

function InGameNoticeCellView:getData()
	return self.m_data;
end

function InGameNoticeCellView:setSize(size)
	self.m_size = size;
end
function InGameNoticeCellView:getBtnRect()
	return self.m_sprite:boundingBox();
end

function InGameNoticeCellView:getSprite()
	return self.m_sprite;
end

function InGameNoticeCellView:setIndex(index)
	self.m_index = index;
end
function InGameNoticeCellView:getIndex()
	return self.m_index;
end

function InGameNoticeCellView:clearAward()
	for i = 1, 4 do
		self:getNode("sprite_item" .. i):setVisible(false)
	end
end

function InGameNoticeCellView:initUI()
	self.m_sprite = self:getNode("sprite_btngoto");

	local title = self.m_data:find("title")[1];
	tolua.cast(self:getNode("label_title"), "CCLabelTTF"):setString(title);
	local desc = self.m_data:find("inside")[1];
	tolua.cast(self:getNode("label_content"), "CCLabelTTF"):setString(desc);
	tolua.cast(self:getNode("label_title"), "CCLabelTTF"):setVisible(false);
	tolua.cast(self:getNode("label_toptitle"), "CCLabelTTF"):setVisible(true);
	tolua.cast(self:getNode("label_toptitle"), "CCLabelTTF"):setString(title);
	local ideval = self:getIndex();
	if ideval == 1 then
		local sprite = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("inner_notice_02");
		tolua.cast(self:getNode("sprite_toptip"), "CCSprite"):setDisplayFrame(sprite);
	elseif ideval == 2 then
		local sprite = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("inner_notice_03");
		tolua.cast(self:getNode("sprite_toptip"), "CCSprite"):setDisplayFrame(sprite);
	elseif ideval == 3 then
		local sprite = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("inner_notice_04");
		tolua.cast(self:getNode("sprite_toptip"), "CCSprite"):setDisplayFrame(sprite);
	else
		tolua.cast(self:getNode("label_title"), "CCLabelTTF"):setVisible(true);
		tolua.cast(self:getNode("label_toptitle"), "CCLabelTTF"):setVisible(false);
		tolua.cast(self:getNode("sprite_toptip"), "CCSprite"):setVisible(false);
	end

	self:clearAward()

	for i = 1, 4 do
		local drop = self.m_data:find('drop' .. i)
		if drop == nil then
			break
		end

		local dropid = tonumber(drop[1]) or 0
		if dropid ~= 0 then
			local sprItem = tolua.cast(self:getNode("sprite_item" .. i), "CCSprite")
			local _icon = sprItem:getChildByTag(99)
			if _icon then
				_icon:removeFromParentAndCleanup(true)
			end
			sprItem:setVisible(true)
			-- icon/frame
			local _maintype = 0
			local _subtype = 0
			local _id = -1
			local _num = -1
			local _drop_type = 0
			local _obj_info = { }
			_maintype, _subtype, _id, _num, _drop_type = setObjTypeInfo(dropid)
			_obj_info.pIcon, _obj_info.pFrame, _obj_info.quality, _obj_info.objname = rl_get_iconsprite(_maintype, _subtype, E_FRAMETYPE_SMALL, _id)
			if nil ~= _obj_info.pFrame then
				sprItem:setDisplayFrame(_obj_info.pFrame)
			end

			local icon = self.m_data:find('icon' .. i)[1]

			local pathName = "props/" .. icon .. ".plist"
			CCSpriteFrameCache:sharedSpriteFrameCache():addSpriteFramesWithFile(pathName)
			local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(icon)
			if frame ~= nil then
				_obj_info.pIcon = CCSprite:createWithSpriteFrame(frame)
			end

			if nil ~= _obj_info.pIcon then
				sprItem:addChild(_obj_info.pIcon)
				local size = sprItem:getContentSize()
				_obj_info.pIcon:setPosition(ccp(size.width * 0.5, size.height * 0.5))
				_obj_info.pIcon:setAnchorPoint(ccp(0.5, 0.5))
				_obj_info.pIcon:setTag(99)
				if _drop_type ~= 1 then
					_obj_info.pIcon:setScale(0.9)
				end
			end

		else
			local pFrameNoGiftBK = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("small_no_obj_frame")
			sprItem:setDisplayFrame(pFrameNoGiftBK)
		end

	end

end

function InGameNoticeCellView:checkTouchAward(pt)
	for i = 1, 4 do
		local btn = self:getNode("sprite_item" .. i)

		if btn:boundingBox():containsPoint(pt) then
			self:onTouchedIcon(i)
			break;
		end
	end
end

function InGameNoticeCellView:onTouchedIcon(i)
	local drop = self.m_data:find('drop' .. i)
	if drop == nil or drop[1] == '0' then
		return 
	end

	local id = tonumber(drop[1])
	if nil ~= id then
		CGameObjElement:ShowDropByID(id)
	end
end 