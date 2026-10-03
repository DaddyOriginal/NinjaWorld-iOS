--descriptioin:循环滑动选角色组件
--company: xckoo
--author: chenchun
--date: 2013-2-13

---------------------------------------------
module("ui_selectActor", package.seeall)
baseClass(layer_base_t, ui_selectActor)

pixelUnit = 5	--单位,当移动距离超过5以后，才进行放大缩小处理
label_money_node_instance = nil

--ccbiName:ccbi 的名称
--ccbiSpriteCount:ccbi中的sprite 的数量，分为两种情况：3（比如选择角色时候，一次只能显示一个） 和 5（比如女神献花）
--spriteDistanceScale: sprite 之间的距离的参数，该参数为相当于node_size 的比例
--leftRightWidthScale：最左边的sprite 移动到最右边，需要的参数，该参数为相当于node_size 的比例
--configData：配置数据{[1]={id=1， icon=""}, [2]=={id=2， icon=""}}， 配置数据中的数据要大于3，才能循环滑动
--scaleData: 放大缩小的比例数据
--opactiyData：透明度的设置数据，没有的话设置为nil
--parentSize:此ui的父节点的大小
--isCycle：是否循环滑动, 暂时定为该循环参数必须为true，（当考虑为不循环时，一是加重逻辑的复杂性，二是可以用tableview和scorllview解决）
--moveCallBack：移动的回调函数
--clickCallBack：点击的回调函数
--self.spriteScales = {[1] = 0.31, [2] = 0.65, [3] = 1, [4] = 0.65, [5] = 0.31}
--self.spriteOpactiy = {[1] = 105, [2] = 180, [3] = 255, [4] = 180, [5] = 105}

function init(self, ccbiName, ccbiSpriteCount, spriteDistanceScale, leftRightWidthScale, spriteOpactiyScale, configData, scaleData, opactiyData, parentSize, isCycle, moveCallBack, clickCallBack)
	--local ccbiAttrTable = {name="activity/NvShen.ccbi", size=self.parentSize}
	local ccbiAttrTable = {name=ccbiName, size=parentSize}
	layer_base_t.init(self, true, ccbiAttrTable)

	self.m_bIsEndAnimatedScroll_ = true
	self.touchHandler_ = {}
	self.ccbiSpriteCount = ccbiSpriteCount
	self.spriteDistanceScale = spriteDistanceScale
	self.leftRightWidthScale = leftRightWidthScale
	self.spriteOpactiyScale = spriteOpactiyScale
	self.configData = configData
	self.spriteScales = scaleData
	self.spriteOpactiy = opactiyData
	self.isCycle = isCycle or true
	self.clickCallBack = clickCallBack
	self.moveCallBack = moveCallBack
	self.reallyDataCount = #configData
	if #configData < 3 then
		self.isCycle = false
	end

	self:init_ui()
	self:init_binding_event()
end


function init_ui(self)
	if self.proxy_ ~= nil then
		local intPart = math.modf(self.ccbiSpriteCount / 2)
		for i = 1, intPart do
			local tmpValue = table.remove(self.configData)
			table.insert(self.configData, 1, tmpValue)
		end
		self.midSpriteIndex = intPart + 1

		self.node_sprite_content = tolua.cast(self.proxy_:getNode("node_sprite_content"), "CCNode")
		self.sprite_nodes = {}
		self.sprite_points = {}

		for i = 1, self.ccbiSpriteCount do
			self["sprite_avtor_" .. tostring(i)] =  tolua.cast(self.proxy_:getNode("sprite_avtor_" .. tostring(i)), "CCSprite")
			self.node_sprite_content:reorderChild(self["sprite_avtor_" .. tostring(i)] , 1)
			table.insert(self.sprite_nodes, self["sprite_avtor_" .. tostring(i)])
			local x, y = self["sprite_avtor_" .. tostring(i)]:getPosition()
			local tmpPoint = {x = x, y = y}
			table.insert(self.sprite_points, tmpPoint)
		end


		local node_size = self.node_sprite_content:getContentSize()
		self.midX = node_size.width * 0.5
		self.left_right_width = node_size.width * self.leftRightWidthScale
		self.sprite_distance = node_size.width * self.spriteDistanceScale
		self.scalePara = self.spriteDistanceScale / self.sprite_distance --每移动单个像素的缩放比例
		self.scaleOpacity = self.spriteOpactiyScale / self.sprite_distance
		--self.scaleParameter = self.scalePara * 5			--每移动5个像素的缩放比例
		self.allMoveDistance = 0
		self.circleNumber = -1
		self.move_speed = self.sprite_distance / 0.5
	end
end

function init_binding_event(self)

	local function onTouched(_eventType, ...)
        --cclog("_eventType = %s", _eventType)
        local result = self.touchHandler_[_eventType](self, ...)
        if self.touchBegan ~= nil and _eventType == "began" then
            assert(result ~= nil, "touchBegan must return a result!")

            local ret

            if result == true then
                ret = 1
            else
                ret = 0
            end
            return ret
        end
    end

    if self.touchBegan ~= nil then
        --单点触控
        self.node_:setTouchEnabled(true)
        self.node_:registerScriptTouchHandler(onTouched, false, 2, true)

        self.touchHandler_["began"] = self.touchBegan
        self.touchHandler_["moved"] = self.touchMoved
        self.touchHandler_["ended"] = self.touchEnded
        self.touchHandler_["cancelled"] = self.touchCancelled
 	end

	--self.node_:setTouchEnabled(true)
	--self.node_:registerScriptTouchHandler(CCLayerTouch, false, -1, true)

	if self.proxy_ ~= nil then

	end
end


function touchBegan(self, _touchX, _touchY)
	if self.m_bIsEndAnimatedScroll_ then
		--[[]]
		local tmpPoint = self.node_sprite_content:convertToNodeSpace(ccp(_touchX, _touchY))
		if self.node_sprite_content:boundingBox():containsPoint(tmpPoint) then
			self.isTouchMoved_ = false
			self.moveDirection_ = true	--right direction
			self.beganX_ = _touchX
			self._preTouchX = _touchX
			self._preTouchY = _touchY
		end


		--self.circleNumber = 0
		--[[
		if _touchX < self.maxRightX_ then
			self.beganX_ = _touchX
		else
			self.beganX_ = nil
		end
		]]
		return true
	else
		return false
	end
end

function touchMoved(self, _touchX, _touchY)
	if self.beganX_ ~= nil and self.m_bIsEndAnimatedScroll_ then
		self.isTouchMoved_ = true
		local moveDistance = 0
		--cclog("1111---%s", tostring(_preTouchX))
		if self._preTouchX > _touchX then --向左
			self.moveDirection_ = false
			moveDistance = self.beganX_ - _touchX

			local disX = _touchX - self._preTouchX
			self.allMoveDistance = self.allMoveDistance + disX
			self:moveSprite(false, moveDistance, disX)
			--self:reorderChild()
			local num = tools.getIntPart((moveDistance / self.sprite_distance))

			if num ~= self.circleNumber then
				self.circleNumber = num
				local x, y = self.sprite_nodes[1]:getPosition()
				local tmpValue = table.remove(self.sprite_nodes, 1)
				tmpValue:setPosition(x + self.left_right_width, y)
				local tmpScale = math.abs(self.midX - x - self.left_right_width) * self.scalePara
				local tmpOpacity = math.abs(self.midX - x - self.left_right_width) * self.scaleOpacity
				tmpValue:setScale(1 - tmpScale)
				tmpValue:updateDisplayedOpacity(255 - tmpOpacity)
				table.insert(self.sprite_nodes, tmpValue)
				self:resetConfigData(false)
			end
		elseif self._preTouchX < _touchX then --向右
			self.moveDirection_ = true
			moveDistance = _touchX - self.beganX_
			local disX = _touchX - self._preTouchX
			self.allMoveDistance = self.allMoveDistance + disX
			self:moveSprite(true, moveDistance, disX)

			--self:reorderChild()
			local num = tools.getIntPart((moveDistance / self.sprite_distance))

			if num ~= self.circleNumber then
				self.circleNumber = num
				local x, y = self.sprite_nodes[self.ccbiSpriteCount]:getPosition()
				local tmpValue = table.remove(self.sprite_nodes)
				tmpValue:setPosition(x - self.left_right_width, y)
				local tmpScale = math.abs(self.midX - x + self.left_right_width) * self.scalePara
				tmpValue:setScale(1 - tmpScale)
				local tmpOpacity = math.abs(self.midX - x + self.left_right_width) * self.scaleOpacity
				tmpValue:updateDisplayedOpacity(255 - tmpOpacity)
				table.insert(self.sprite_nodes, 1, tmpValue)
				--重新移动配置数据
				self:resetConfigData(true)
			end
		end
		self._preTouchX = _touchX
		self._preTouchY = _touchY
	end
end

function touchEnded(self, _touchX, _touchY)
	if self.isTouchMoved_ then
		self.isTouchMoved_ = false
		self:startAnimatedScroll()
		self.allMoveDistance = 0
		self.circleNumber = -1
		self.beganX_ = nil
	else
		self.clickCallBack(2)
	end
end

function touchCancelled(self)
	for i = 1, self.ccbiSpriteCount do
		self.sprite_nodes[i]:setPosition(self.sprite_points[i].x, self.sprite_points[i].y)
		self.sprite_nodes[i]:setScale(self.spriteScales[i])
	end
	self:reorderChild()
end

function reorderChild(self)
	local disMid = 3000
	local index = 1
	for i = 1, self.ccbiSpriteCount do
		local x = self.sprite_nodes[i]:getPosition()
		local tmpDis = math.abs(x - self.midX)
		if tmpDis < disMid then
			disMid = tmpDis
			index = i
		end
	end
	for i = 1, self.ccbiSpriteCount do
		if i == index then
			self.node_sprite_content:reorderChild(self.sprite_nodes[i], 5)
		else
			self.node_sprite_content:reorderChild(self.sprite_nodes[i], 1)
			self.sprite_nodes[i]:updateDisplayedOpacity(self.spriteOpactiy[i])
		end
	end
end

function resetConfigData(self, direction)
	if not direction then
		local tmpValue = table.remove(self.configData, 1)
		table.insert(self.configData, tmpValue)
	else
		local tmpValue = table.remove(self.configData)
		table.insert(self.configData, 1, tmpValue)
	end
	for i = 1, self.ccbiSpriteCount do
		--cclog("1111---sprite:%s", self.configData[i].icon)
		local pFrame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(self.configData[i].icon)
		self.sprite_nodes[i]:setDisplayFrame(pFrame)
	end
end

function moveSprite(self, direction, moveDistance, disX)
	--if not direction then  --局部往左做移动
		for i = 1, self.ccbiSpriteCount do
			if moveDistance > 0 then --整体是往左移动
				local preX, preY = self.sprite_nodes[i]:getPosition()
				local x = preX + disX
				--local tmpX = self.sprite_points[i].x - moveDistance
				local scaleCur = math.abs(self.midX - x) * self.scalePara
				local tmpOpacity = math.abs(self.midX - x) * self.scaleOpacity
				self.sprite_nodes[i]:updateDisplayedOpacity(255 - tmpOpacity)
				self.sprite_nodes[i]:setPosition(x, preY)
				self.sprite_nodes[i]:setScale(1 - scaleCur)
			else                     --整体往右移动
				local preX, preY = self.sprite_nodes[i]:getPosition()
				local x = preX + disX
				--local tmpX = self.sprite_points[i].x + moveDistance
				local scaleCur = math.abs(self.midX - x) * self.scalePara
				local tmpOpacity = math.abs(self.midX - x) * self.scaleOpacity
				self.sprite_nodes[i]:updateDisplayedOpacity(255 - tmpOpacity)
				self.sprite_nodes[i]:setPosition(x, preY)
				self.sprite_nodes[i]:setScale(1 - scaleCur)
			end
		end

end

function startAnimatedScroll(self)
	self.m_bIsEndAnimatedScroll_ = false
	local animationNumer = 0
	local function actionFinished()
		animationNumer = animationNumer + 1
		if animationNumer == self.ccbiSpriteCount then
			self:reorderChild()
			self.m_bIsEndAnimatedScroll_ = true
		end
	end
	local sprite_posX = self.sprite_nodes[self.midSpriteIndex]:getPosition()
	local time1 = math.abs(self.sprite_points[self.midSpriteIndex].x - sprite_posX) / self.move_speed
	for i = 1, self.ccbiSpriteCount do
		local endPos = ccp(self.sprite_points[i].x, self.sprite_points[i].y)
    	local ccMoveto = CCMoveTo:create(time1, endPos)
    	local scaleTo = CCScaleTo:create(time1, self.spriteScales[i])
    	local ccFadeTo = CCFadeTo:create(time1, self.spriteOpactiy[i])
		local ccArraySpawn = CCArray:create()
    	ccArraySpawn:addObject(ccMoveto)
    	ccArraySpawn:addObject(scaleTo)
    	ccArraySpawn:addObject(ccFadeTo)
    	local ccSpawn = CCSpawn:create(ccArraySpawn)

    	local moveFinishCall = CCCallFuncN:create(actionFinished)

    	local moveSeq = CCSequence:createWithTwoActions(ccSpawn, moveFinishCall)
    	self.sprite_nodes[i]:runAction(moveSeq)
	end

end

function onNodeCleanup(self)
    --cclog("onNodeCleanup")
    if self.proxy_ then
    	self.proxy_:release()
    end
    layer_base_t.onNodeCleanup(self)
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