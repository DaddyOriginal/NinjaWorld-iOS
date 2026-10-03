--description:所有程序中使用到的字符串均应统一放置到该模块
--company:xckoo
-----------------------------------------------------------------
module("localizable", package.seeall )

--首充活动（首充礼包）
ui_firstmoneyaward_yuanbao_title = "详细说明"
ui_firstpurchase_label_award1 = "双倍元宝"
ui_firstmoneyaward_dlg_title = "活动说明"
ui_firstmoneyaward_dlg_tips = "新服7天后，活动下线，请玩家们及时领取首冲奖励哦~（充值月卡不算首冲）"
ui_firstmoneyaward_have_got = "已经领取过首充礼包"
ui_firstmoneyaward_cannot_got = "目前尚无资格，充值任一金额即可领取"
ui_firstmoneyaward_tip1 = "1、首次充值任何金额，即可获得等量元宝反馈，冲多少，送多少！（限时送哦~）"
ui_firstmoneyaward_tip2 = "2、更有超值首冲礼包：五星忍者，强力武器等你拿！"
ui_buy_success_tips = "恭喜您，购买成功！"
ui_buy_failed_tips = "不好意思，购买失败！"

--等级排行
ui_level_rank_rule_desc = "1、按照排名截止的排名发放奖励，请玩家在排行截止时间后的24小时内，点击奖励中的图片领取奖励；\n2、如果玩家等级一样，按照玩家最高攻击力大小排序；"
ui_rank_rule_title = "排名规则说明"
ui_level_rank_gift_tips = "还没到领取等级排行榜礼包时间！"
ui_level_fight_error = "不好意思，暂时不能进行挑战！"
ui_rank_gift_title = "奖品说明"
ui_rank_label = "第%s名奖励"
ui_fight_with_self_error = "不好意思，不能与自己进行挑战！"

--战斗力排行
ui_label_fight_rank_title = "战斗力排行榜"
ui_btn_fight_title = "战力榜规则说明"
ui_label_lv_or_power = "战斗力"
ui_fight_rank_rule_desc = "1：战斗力=首页界面中的最高攻击力+最高防守*0.5；\n2：奖励领取规则：按照排名截止的排名发放奖励，请玩家在排行截止时间后的24小时内，点击奖励中的图片领取奖励~"
ui_lable_my_fight = "我的战力："
ui_power_rank_gift_tips = "还没到领取战力排行榜礼包时间"

ui_buy_order_tips = "不好意思，订单已经处理！"

--刮刮乐
ui_scratch_not_enough_gold = "您的元宝不足，请到商城购买元宝！"
ui_scratch_go_on_tips = "别灰心，继续努力就会有收获！"
ui_not_enough_level = "该活动需要等级达到10级，加油吧！"

--累计充值
ui_status_1 = "领取"
ui_status_2	= "已领取"

--限时限量团购
ui_limit_buy_status_1 = "购买"
ui_limit_buy_status_2 = "已售完"

--大转盘
ui_rouletteLayer_title = "温馨提示"
ui_rouletteLayer_rank_title = "转盘礼包"
ui_rouletteLayer_get_gift = "转盘礼包"

--女神献花
ui_nvshen_title = "详细说明"
ui_nvshen_description = "1：活动期间每充值一元获得一朵玫瑰，好感度满后即可获得女神的回馈；\n2:纲手的回馈：紫卡三代雷影+黄金钥匙*3+转生丹*35\n春野樱的回馈：黄金钥匙*2+转生丹*15+超忍秘药*70\n照美冥的回馈：紫卡我爱罗+白银钥匙*3+高级仙石*20\n小南的回馈：紫卡赤砂之蝎+白银钥匙*2+高级仙石*10\n雏田的回馈：蓝卡迪达拉+青铜钥匙*2+高级仙石*5"
ui_nvshen_description1 = "活动期间每充值一元获得一朵玫瑰，好感度满后即可获得女神的回馈"

--跨服战斗
ui_person_count = "每期竞技场前%d名进入"
ui_inspire_title = "提示信息"
ui_inspire_description = "您是否要花费%d元宝激励一次？\n(激励提升%d%%的攻防属性，持续%d秒)\n您当前激励次数：%d/%d"
ui_multi_description = "1、每个服务器竞技场前20名进入跨服战（竞技场排名截止每周五晚20点）。\n2、未入选玩家可以花费500元宝购买门票进入跨服战。\n3、每周五20点参赛名单产生，每周六20点-21点为正式比赛时间。\n4、参赛玩家抢夺天梯位置，获得积分，天梯位置越高，获得积分越多，最终积分最多者获得当期跨服战冠军。\n5、每位参与者战斗时只出战前12张忍者卡(后面的卡牌的羁绊可以应用于前12张上阵卡牌)。"
ui_bingo_description = "押宝活动现在还没有开启"

ui_multi_batting_tips = "跨服战正在进行中，不能领取排名奖励"
ui_multi_batte_before_tips = "跨服战报名状态中，不能领取上期的奖励"
ui_multi_get_rank_gift_error_tips = "没有资格领取该用户的奖励"

ui_common_exchage_text = "兑换"

--炼魂
ui_train_soul_tips = "请选择需要炼魂的忍者"
ui_train_add_ninja = "目前没有符合炼魂条件的忍者"
ui_train_soul_description = "1、点击添加忍者，系统自动选择5个3星或3星以上的忍者进行炼魂，炼魂后忍	者卡会消失。\n2、手动点击“+”可进入忍者列表，每次只能选择一个忍者进行炼魂。"
ui_train_ninjalist_tips = "三星及以上忍者卡在商城中抽取，或者在历练中获得"
ui_train_update_times_limit_tips = "已达上限"
ui_train_tips_title = "温馨提示"
ui_train_tips_info = "您选择的忍者卡中有高级忍者卡，请问是否继续炼魂？"

--限时神将
ui_activity_description = "1、超忍限时抽期间在活动页面招募和商城万里挑一界面招募都将获得积分。\n2、活动结束之后，根据积分进行排名，并发放超忍奖励。\n3、活动页面的免费抽取和商城的免费抽取，冷却时间不重叠。\n4、请在活动结束24小时内领取超忍奖励！"


--攻防队伍转换
ui_active_team_title = "攻防队列说明"
ui_active_team_desc = "1、第二队列在50级时开启。\n2、第二队列可以使用所有的忍者、忍术和装备。（可与第一队重合）\n3、在队伍页面点击激活队列按钮后，该队列才使用生效，同时只能有一支队伍生效。\n4、当前应用的队列页签上有对勾标示。\n5、建议第一队列设为攻击队列，第二队列设为防御队列。"

--首冲礼包配置
ui_firstpurchase_gift1 = "四代雷影"
ui_firstpurchase_gift2 = "最强之矛"
ui_firstpurchase_gift3 = "潜影蛇手"

--跨服战补充
ui_multi_not_start = "还未开始"
ui_multi_ended = "已经结束"
ui_multi_entered = "已进入"
ui_multi_not_enter = "未进入"
ui_multi_has_qualification = "您已经拥有参赛资格"
ui_multi_buy_ticket = "购买门票"
ui_multi_buy_ticket_tips = "您确定花费%d元宝购买门票吗？"
ui_multi_position = "所占位置:"
ui_multi_last_score = "上期积分:"
ui_multi_last_rank = "上期排名:"
ui_multi_region = "[%s区]"
ui_multi_not_open_team_info = "暂时没有开放查看阵容功能"
ui_multi_platform = "平台"
ui_multi_name = "名称"
ui_multi_not_rank_data = "暂时没有排名数据"
ui_multi_not_enough_get_gift_tips = "您的剩余积分不够领取该奖励"
ui_multi_exchange_gift = "兑换奖励"
ui_multi_acc_score = "积分/"
ui_multi_time_sec = "秒"
ui_multi_not_qualification_enter = "您没有资格参与这次跨服战活动"
ui_multi_region1 = "区"
ui_multi_get_gift_title = "领取奖励"



--神秘商店补充
ui_sceret_shop_update_tips = "每天只能刷新%d次，请明天再来吧！"
ui_secret_shop_suipian = "(碎片) x"
ui_secret_shop_soul = "忍魂:"
ui_secret_shop_gold = "元宝："
ui_secret_shop_not_ninja = "没有可以炼化的忍者"

--定时充值
ui_accu_big_gift = "元大礼包"
ui_accu_has_got_gift = "已经领取过该奖励"
ui_accu_has_not_reach = "充值金额未达到，请充值后领取"
ui_accu_activity_out_of_date = "活动已经过期"
ui_accu_has_not_start = "活动现在还没有开始"


--属性丹补充
ui_attribute_no = "无"
ui_attribute_water = "水"
ui_attribute_fire = "火"
ui_attribute_wind = "风"
ui_attribute_thunder = "雷"
ui_attribute_earth = "土"
ui_attribute_exchange_attr_error = "转换目标属性错误!"
ui_attribute_silver_not_enough = "银币不足!"
ui_attribute_wuxingdan_not_enough = "五行转换丹不足!"


--边境
ui_border_end = "活动结束"
ui_border_temp_not = "暂无"
ui_border_num_after = "以后"
ui_border_has_ended = "已经结束"
ui_border_has_got_tips = "您可能已经领取奖励，或者没有资格领取奖励！"
ui_border_gold = "元宝"
ui_border_provoke = "挑衅"
ui_border_silver = "银两"
ui_border_wander = "游荡"
ui_border_tips1 = "你已在%s边境%s.\n可获得如下奖励:"
ui_border_tips2 = "确定要[保留]奖励并%s[%s]吗?"
ui_border_tips3 = "你已在%s边境%s.\n时间不足无法获得奖励:"
ui_border_tips4 = "确定要%s吗?"
ui_border_title = "边界碑"
ui_border_blood_info = "边界碑血量情况"
ui_border_tips5 = "在边境%s,造成%s点伤害!"
ui_border_country_wind = "风之国"
ui_border_country_thunder = "雷之国"
ui_border_country_water = "水之国"
ui_border_country_fire = "火之国"
ui_border_country_earth = "土之国"
ui_border_my_country = "我国"
ui_border_wander_info = "边境游荡情况:"
ui_border_provoke_info = "边境挑衅情况:"
ui_border_reach_uplimit = "(已达上限)"
ui_border_tips6 = "您当前处于空闲状态!"
ui_border_not_start = "边界碑未开启!"
ui_border_tips7 = "在%s被%s击败!"
ui_border_cannot_enter_tip = "您当前无法进入边界碑!"
ui_border_tips8 = "每个国家都有自己的边界碑,拥有血量%s,每天11点半到14点半,可被敌国玩家攻击。\n"
ui_border_tips9 = "边界碑开启后，玩家可在其他国家边界挑衅和游荡，挑衅花费%s元宝，游荡花费%s银两。\n挑衅、游荡成功均会减少对方边界碑血量，并获得适量经验和武勋奖励。\n成功击破敌国边界碑的玩家将获得丰厚奖励，有助攻行为的本国玩家也将获得助攻奖励。\n"
ui_border_tips10 = "玩家可以通过清理边界上正在挑衅和游荡的玩家来保卫边界，清理成功会获取经验和武勋奖励，成为护国功臣！\n若昨日边界碑未被攻破，昨日的护国功臣们就可以根据排名领取奖励。\n"
ui_border_tips11 = "每天会根据昨日玩家成功进攻他国边界碑或清理入侵本国边界碑玩家的行为进行积分排名，根据昨日的积分排名玩家可以领取相应的排名奖励。\n"
ui_border_provoke_info1 = "边境挑衅情况"
ui_border_tips12 = "清理次数不足,升级VIP等级可增加清理次数!"
ui_border_battle_win = "战斗胜利"
ui_border_battle_gift = "击退对方,获得奖励:"
ui_border_battle_gift1 = "击退正在攻击他国的玩家无奖励!\n每日领奖次数达到上限也不会获得奖励!"
ui_border_get_gift_success = "获取奖励成功!"
ui_border_tips13 = "正在%s边境挑衅!威武!"
ui_border_tips14 = "正在%s边境游荡!威武!"
ui_border_tips15 = "在边境清理了%s的[%s]"
ui_border_tips16 = "正在%s边境挑衅,可恶!"
ui_border_tips17 = "正在%s边境挑衅!"
ui_border_tips18 = "正在%s边境游荡,可恶!"
ui_border_tips19 = "正在%s边境游荡"
ui_border_tips20 = "在边境清理了%s的%s"
ui_border_times_unit = "次"
ui_border_tips21 = "挑衅%s次,可获得奖励:"
ui_border_tips22 = "游荡%s次,可获得奖励:"
ui_border_tips23 = "挑衅被清理%s次,可获得奖励:"
ui_border_tips24 = "游荡被清理%s次,可获得奖励:"
ui_border_tips25 = "您今天领取的奖励已达上限或没有奖励信息!"
ui_border_tips26 = "已领取!"
ui_border_tips27 = "清理其他玩家会脱离当前对边界碑的攻击状态，并无法获得攻击边界碑的奖励。"
ui_border_wander_info1 = "边境游荡情况"

ui_border_boxTips2 = "（战功>%d可领取）"
ui_border_boxErr = "昨日战功未达到领取条件"

--套装信息
ui_suit_color_card = "忍者颜色套卡"


--每日任务
ui_daily_score_gift = "积分奖励"
ui_daily_get_gift_success = "领取奖励成功!"
ui_daily_score = "积分"


--战斗力排行榜
ui_fight_tips1 = "您可能已经领取奖励，或者没有资格领取奖励！"

--
ui_first_money_left = "还剩"
ui_soul_name = "忍魂"
ui_gift_tips = "尊贵礼包"

--成长基金
ui_growth_get_gold_tip = "领取%s元宝成功"
ui_growth_tips1 = "只需要投入"
ui_growth_tips2 = "元宝,就可以获"
ui_growth_tips3 = "得"
ui_growth_tips4 = "倍返还,共计"
ui_growth_tips5 = "级成长基金"
ui_growth_tips6 = "到达%s级可以领取%s元宝"
ui_growth_tips7 = "已经购买过了"
ui_growth_tips8 = "购买成功"


--名人堂
ui_hall_battle_honour = "武勋"
ui_hall_defense = "防御力"
ui_hall_attack = "攻击力"
ui_hall_net_error = "网络不稳定,请稍后再试!"
ui_hall_my_attack = "我的攻击:"
ui_hall_my_defense = "我的防御:"
ui_hall_ninja_level = "忍阶:"
ui_hall_level = "阶"
ui_hall_attack_value_not_enougt = "决斗值不足!"
ui_hall_tips1 = "等级差距过大,禁止挑战!"
ui_hall_tips2 = "骚年,你今天欺负过人家5次了,明天再来吧!"

--淬炼套装
ui_level_suit_card = "忍者淬炼套卡"

--限制购买
ui_limit_title = "物品介绍"


--彩票
ui_lottry_tips1 = "下注截止,敬请期待开奖!"
ui_lottry_bet_gold = "元宝下注"
ui_lottry_tips2 = "上期彩票:获得%s等奖!"
ui_lottry_tips3 = "你没有中奖,加油哦!"
ui_lottry_month = "月"
ui_lottry_day = "日"
ui_lottry_dian = "点"
ui_lottry_min = "分"
ui_lottry_gold_not_enough = "您的元宝数量不足"
ui_lottry_tips4 = "下注成功!敬请期待开奖!"
ui_lottry_tips5 = "彩票投注"
ui_lottry_tips6 = "您确定花费%s元宝随机下注号码:%s吗?"

ui_lottry_tips7 = "1.彩票流程"
ui_lottry_tips8 = "忍村彩票每天一期！每天8点半开奖！"

ui_lottry_tips9 = "好奖天天开！天天等你拿！"
ui_lottry_tips10 = "玩家当天晚上8点半到次日晚上8点期间可"

ui_lottry_tips11 = "以下注~每天20点到20点半为开奖时间哦!"
ui_lottry_tips12 = "比如玩家在1日0点登陆游戏,这期间到晚上"

ui_lottry_tips13 = "8点玩家可以买三注彩票(vip玩家可以购买"
ui_lottry_tips14 = "更多),1号晚上8点半系统会开出中奖号码."

ui_lottry_tips15 = "玩家在当日8点半到次日8点可以购买新一期"
ui_lottry_tips16 = "的彩票并领取奖励,获奖玩家请次日8点之前"

ui_lottry_tips17 = "领取奖励，否则失效!~一注彩票对应"
ui_lottry_tips18 = "一次奖励,玩家单期可获奖次数不做限制."

ui_lottry_tips19 = "2.彩票规则"
ui_lottry_tips20 = "每期开奖会开出4位数."

ui_lottry_tips21 = "2.1 下注号码中3位数字和中奖号码一致且"
ui_lottry_tips22 = "位子一样即可获得一等奖."

ui_lottry_tips23 = "2.2 下注号码中千百(前二位)中的任意一"
ui_lottry_tips24 = "个数字和中奖号码的前二位相同(位子也"

ui_lottry_tips25 = "相同)且十位(第三位)也一样即可获得二"
ui_lottry_tips26 = "等奖."

ui_lottry_tips27 = "2.3 下注号码中有一个数字和中奖号码一样且位子一样"
ui_lottry_tips28 = "且位子一样即可获得三等奖."

ui_lottry_tips29 = "2.4 比如中奖号码为1890，1790获得一等奖,"
ui_lottry_tips30 ="1692获得二等奖，1789获得三等奖."

ui_lottry_tips31 = "2.5 玩家只可获得当前最高的一份奖励!~获"
ui_lottry_tips32 = "得一等奖的玩家不可领取二等奖和三等奖."


--大富翁
ui_monopoly_end = "活动已结束!"
ui_monopoly_not_start = "活动未开启!"
ui_monopoly_gold_not_enough = "您的元宝数量不足!"
ui_monopoly_touzhi_failed = "投掷骰子不成功!"
ui_monopoly_title = "大富翁"
ui_monopoly_tips1 = "您确定花费%s元宝投掷一次骰子吗?"
ui_monopoly_tips2 = "10连转未结束!"
ui_monopoly_tips3 = "没领取到任何排名奖励!"
ui_monopoly_tips4 = "获取奖励成功!"
ui_monopoly_tips5 = "没领取到任何排名奖励!"
ui_monopoly_tips6 = "活动尚未结束!"
ui_monopoly_tips7 = "您已经领取过排名奖励!"
ui_monopoly_tips8 = "您没有资格领取排名奖励!"
ui_monopoly_tips9 = "活动已经结束!"
ui_monopoly_tips10 = "奖励列表为空!"
ui_monopoly_tips11 = "没领取到任何历程奖励!"
ui_monopoly_tips12 = "您已经领取过历程奖励!"
ui_monopoly_tips13 = "您没有资格领取此历程奖励!"
ui_monopoly_tips14 = "大富翁行程奖励"
ui_monopoly_tips15 = "您确定领取[%s]的奖励[%s]吗?"


--女神补充
ui_nvshen_chutian_gift = "雏田的回馈"
ui_nvshen_xiaonan_gift = "小南的回馈"
ui_nvshen_zhaomeimi_gift = "照美名的回馈"
ui_nvshen_chunyeyin_gift = "春野樱的回馈"
ui_nvshen_gangshou_gift = "纲手的回馈"
ui_nvshen_not_start = "活动还没开始"
ui_nvshen_give_flower_tips1 = "献花%s朵"


--充值界面
ui_purchase_buy_again = "再充%s元"

--大转盘补充
ui_roulette_not_qualification = "暂时没有资格领取"

--七天活动
ui_seven_day_tips1 = "还有%s天结束"
ui_seven_day_tips2 = "登录%s天解锁"
ui_seven_day_tips3 = "登录天数不够"
ui_seven_day_tips4 ="领取成功"
ui_seven_day_1day_left ="奖励明天过期"
ui_seven_day_2day_left ="奖励后天过期"
ui_seven_day_award_lost ="奖励已过期"

--闯关扫荡
ui_tower_sweep_tips1 = "你已经选择0关"
ui_tower_sweep_tips2 = "今日可扫荡关数:"
ui_tower_sweep_tips3 = "下级VIP:+%s次"
ui_tower_sweep_tips4 = "VIP等级已达最高"
ui_tower_sweep_tips5 = "当前关卡今日已扫荡!请明日再来!"
ui_tower_sweep_tips6 = "当前关卡不可选,通过此关后才能扫荡!"
ui_tower_sweep_tips7 = "你已经选择%s关"
ui_tower_sweep_tips8 = "主人,已为你取消全部选择!"
ui_tower_sweep_tips9 = "主人,已为你选择%s关!"
ui_tower_sweep_tips10 = "请选择可扫荡的关卡"
ui_tower_sweep_tips11 = "剩余扫荡关卡数不足!"

--litao_2014.6.3_lua统一文字调用格式化

--vip列表
vip_list_vipTips = "(成为VIP可以购买相应超值VIP大礼包一次)"
vip_list_need_gold =  "再充%s元"
vip_list_vipLv_notEnough = "VIP等级不够!"
vip_list_cell_need_viplv = "(VIP等级达到%s可以购买)"
vip_first_item_desc = "1.可够买VIP%s礼包"
vip_giftpack_desc = "尊享礼包"
vip_giftpack101_desc = "周年庆实惠礼包"
vip_giftpack102_desc = "周年庆豪华礼包"

--升级礼包
upgradeGift_cell_title = "%s级大礼包"
upgradeGift_cell_subtitle = "%s级就送新手大礼包"

--转换国家
switchCountry_select_country_first = "请先选择国家。"
switchCountry_card_notEnough = "转国卡数量不足。"

--拆卡
splitCard_tiptext = "拆卡提示"
splitCard_info1 = "确定把%s拆成碎片吗?"
splitCard_split_success = "拆卡成功"

--RLRequest
RLRequest_got_god = "获得元宝%s"
RLRequest_got_silver = "获得银子%s"

--reNewLevel
reNewLevel_tiptext = "重置提示"
reNewLevel_info1 = "确定重置%s吗?"
reNewLevel_reNew_success = "重置成功!"

--充值礼包
payhistorydrop_info1 = "充值%s元礼包"
payhistorydrop_info2 = "充值%s元可领取大礼包"

--猜拳
mora_not_open = "本期猜拳活动已经结束"
mora_time_arg_4 = "%d天%d小时%d分%d秒"
mora_time_arg_3 = "%d小时%d分%d秒"
mora_time_arg_2 = "%d分%d秒"
mora_time_arg_1 = "%d秒"
mora_desc_end = "%s后结束"
mora_desc_getaward_end = "%s后领奖结束"
mora_normal_text_tip = "普通模式，消耗1点生命值，获得1倍幸运币。"
mora_gamele_text_tip = "孤注一掷模式，消耗5点生命值，获得5倍幸运币。"
mora_detail_text_info = "1、 猜拳胜利不消耗生命，每次失败扣除1点生命。\n2、 若生命为0则不能继续挑战。\n3、 勾选\"孤注一掷\"每次扣除5点生命，获胜则获得5倍奖励。\n"..
	"4、 每天0点补足3点生命，若超出3点生命则不补充。\n5、可以消耗点券补充生命，最高可补至20点。\n6、将对手生命值清零还可以获得额外的战争手册奖励。\n7、每天0点清空挑战数据，从头开始挑战。"
mora_dlg_rank_title = "至尊奖励"

--月卡
monthCard_card_name_25 = "25元月卡"
monthCard_card_name_50 = "50元月卡"	
monthCard_card_name_100 = "100元月卡"	
monthCard_failed_to_buy = "此平台无法购买该商品。"
monthCard_get_success = "领取成功"
monthCard_not_enough_time = "还没到领取时间"

--登录礼包
login_item_title_desc = "登录第%s天"
login_item_subtitle_desc = "登录第%s天可领取大礼包"

--连续登录
keepLogin_gotaward_today_desc = "您今天已经领取过奖励,请明天再来!"
keepLogin_failed_getaward_desc = "不满足领取条件!"
keepLogin_cell_title_desc = "第%s天"
keepLogin_cell_title_desc_2 = "第7天及以后"

--世界Boss
fightBoss_not_open = "世界boss活动已经结束"
fightBoss_status_1 = "每天12:30分战斗开始"
fightBoss_status_2 = "boss已经被击杀"
fightBoss_status_3 = "晓组织正在入侵"
fightBoss_status_4 = "活动尚未开始"
fightBoss_award_title_desc = "奖励描述"
fightBoss_cost_tip_desc = "确认花费%s元宝投掷筛子获得伤害加成？"
fightBoss_hurt_desc = "伤害x"
fightBoss_hurt_desc_1 = "伤害第"
fightBoss_hurt_desc_2 = "伤害输出可获取"
fightBoss_prestige_desc = "声望。"
fightBoss_cost_clearCD_tip_desc = "确定花费%s元宝来清除CD？"
fightBoss_god_not_enough_desc = "元宝不足"
fightBoss_fight_reputation = "恭喜你获得%s声望!"
fightBoss_new_detail_desc_1 = "每次战斗中,每造成%s伤害即可获取1银子"
fightBoss_new_detail_desc_2 = "每次战斗中,每造成%s伤害即可获取1声望"

--通用购买框
commonBuy_use_desc = "使用"
commonBuy_buy_desc = "购买"
commonBuy_max_vipLv_desc = "已达最高"
commonBuy_vipLv_add_desc = "次/日"
commonBuy_use_success_desc = "使用成功"

--搜集兑换
collect_cell_title_desc = "可收集次数："
collect_cell_can_collect_desc = "可收集"
collect_cell_can_exchange_desc = "可兑换"
collect_cell_onetime_desc = "次"
collect_cell_can_not_get_pack_desc = "条件没满足"
collect_cell_exchange_confirm_tip_desc = "兑换确认"
collect_cell_exchange_confirm_desc = "确认要用材料卡兑换相应的奖励？"

--购买拉面对话框
buyRemenDlg_bigRemenName = "大拉面"
buyRemenDlg_smallRemenName = "小拉面"

--开箱子
boxOpen_open_times_not_enough = "单日开启金箱子次数用尽!请明日再来!"
boxOpen_cost_confirm_desc = "确认花费%s元宝开启宝箱？"
boxOpen_double_award_desc = "恭喜你获得双倍奖励，下次开启宝箱奖励翻倍。"

ui_common_silver = "银子"
ui_common_gold = "元宝"
ui_common_soul = "忍魂"
vip_gift_text="尊享礼包"

ui_evaluate_application_tips1 = "可能已经领取过评价奖励"
ui_evaluate_tips2 = "前往应用商店给应用五星评价可以领取超值礼包"
ui_evaluate_tips3 = "先进行五星评价，才能领取"

--litao_2014.6.4_统一字符_跑马灯
ui_borderShow_type_1 = "【抽卡】"
ui_borderShow_type_2 = "【淬炼】"
ui_borderShow_type_3 = "【转生】"
ui_borderShow_type_4 = "【夺宝】"
ui_borderShow_type_5 = "【活动】"
ui_borderShow_type_6 = "【世界BOSS】"
ui_borderShow_type_desc_player = "玩家"
ui_borderShow_type_1_desc = "运气爆棚,在万里挑一中获得五星紫卡[%s]！"
ui_borderShow_type_2_desc = "得到了上天眷顾，达成[%s]阶淬炼！"
ui_borderShow_type_3_desc_player = "恭喜土豪"
ui_borderShow_type_3_desc = "的[%s]卡牌完成[%s]转！"
ui_borderShow_type_4_desc_player = "天降十尾！玩家"
ui_borderShow_type_4_desc = "成功合成十尾印记！"
ui_borderShow_type_5_desc = "获得至尊忍者卡，得到忍者[%s]！"
ui_borderShow_type_6_desc = "在[%s]活动中获得积分第一名！"
ui_borderShow_type_7_desc = "获得飞来横财！百宝箱中开出1000元宝！"
ui_borderShow_type_8_desc = "天降神力！最终击杀了今日的世界BOSS！"
ui_borderShow_type_9_desc = "神勇无双！对今日世界BOSS输出了最高的伤害！"

--litao_2014.6.4_统一字符_下限活动
ui_monopoly_info_1 = "活动累计历程达到"
ui_limitSuperNinja_info_1 = "活动累计积分达到"
ui_limitSuperNinja_info_2 = "及以上,才能领取第一名奖励"

--litao_2014.6.9_神卡合成
ui_godCard_info1 = "1.同一忍者攻防两张卡牌并结合进化丹可以合成这"
ui_godCard_info2 = "  个忍者的神卡."
ui_godCard_info3 = "2.合成后的新卡会保留原来卡片的转生和淬炼数据."
ui_godCard_info4 = "  (以最高值为准)"
ui_godCard_info5 = "3.转生之后会退还两张卡牌消耗的超忍迷药.(培养"
ui_godCard_info6 = "  消耗元宝不退还)"
ui_godCard_info7 = "4.上阵卡牌不能够进化,不会出现在显示列表中."
ui_godCard_error1 = "请选择同一忍者的攻卡、防卡再进行合成！"
ui_godCard_confirm_title = "神卡合成"
ui_godCard_confirm = "确定合成神卡[%s]吗？合成成功将退还已使用的超忍秘药!"
ui_godCard_dan_not_enough = "进化丹不足,请购买进化丹!"
ui_godCard_dan_back = "进化成功!退还[%s]个超忍秘药!"
ui_godCard_synthetic_succeed = "进化成功！"

--litao_2014.6.13_火影秘宝
ui_ninjaTreasure_error = "探索失败，请重新探索!"
ui_ninjaTreasure_refresh_tip = "刷新成功！"
ui_ninjaTreasure_got_all_tip = "确定花费[%s]元宝获取当前所有秘宝吗？"
ui_ninjaTreasure_got_all_title = "全部获取"
ui_ninjaTreasure_got_all_succeed = "全部获取成功!"
ui_ninjaTreasure_refresh_error = "探寻未结束,请稍后刷新!"

--litao_2014.6.20_拉面换经验
ui_saveTime_chapter_desc = "%s章，第%s回合"
ui_saveTime_add_desc = "+%s个/天"
ui_saveTime_tip = "(VIP%s且等级大于%s级才能使用该功能)"
ui_saveTime_ramen_not_enough = "拉面数量不足，请购买！"

--litao_2014.6.23_传承
ui_inherit_choose_card_desc = "请选择想要传承和继承的忍者卡!"
ui_inherit_choose_card_desc = "请选择想要传承和继承的忍者卡!"
ui_inherit_readme_info1 = "1.传承卡等级、转生、淬炼数值将会完整的"
ui_inherit_readme_info2 = "  传递给继承卡."
ui_inherit_readme_info3 = "2.传承只能在同品质卡牌之间进行."
ui_inherit_readme_info4 = "3.传承完成之后,传承卡将变成0级基础卡牌."
ui_inherit_confirm_tip = "您确定完成这次传承吗？"
ui_inherit_title = "传承确认"
ui_inherit_quality_confirm = "请选择相同品质的传承卡和继承卡!"
ui_inherit_same_card = '请选择不同的卡'
--litao_2014.7.18_八门遁甲
ui_limitTrain_att = "攻击"
ui_limitTrain_def = "防御"
ui_limitTrain_chakra = "查克拉"
ui_limitTrain_ninjasu = "忍术触发"
ui_limitTrainSoul_free_times = "可用次数:"
ui_limitTrainSoul_gold_train = "本次消耗元宝:"
ui_limitTrainSoul_muti_ok = "翻倍成功!"
ui_limitTrainSoul_muti_failed = "翻倍失败!"
ui_limitTrainSoul_isDoing = "正在进行!请稍后再试!"
ui_limitTrainSoul_notimes = "今天已经不能训练了。"
ui_limitTrainSoul_vipadd = "+%d次/日"
ui_limitTrainSoul_freeMutiTimes = "今日免费次数：%d/%d"

--litao_2014.8.13_连续充值
ui_keepPay_time = "本次活动将在%s天后结束"
ui_keepPay_time_today = "本次活动将在明天结束"
ui_keepPay_cell_title = "连续充值%s天"

--litao_每日消耗
ui_dailyPay_desc_1 = "活动期间,消耗指定元宝数,即可领取相应奖励!每日重新计算元宝消耗数!"
ui_dailyPay_title = "%s元宝礼包"

--litao_充值翻倍
ui_randPay_tips1 = "您确定花费%s元宝转动一次轮盘吗?"
ui_randPay_tips2 = "1.本活动不与首充3倍叠加\n2.每次转动仅获1次充值返利"
ui_randPay_tips3 = "请在本次转盘结束后再转动!"
ui_randPay_tips4 = "需%s元宝"

-- 每日限购礼包
ui_dailyRestriction_remain = "今日还可购买%s次"
ui_dailyRestriction_timeleft = "后活动结束"
ui_dailyRestriction_timesout = "礼包购买次数已达上限"

--勤奋礼包
ui_diligenceGift_checkError = "领取条件不足"
ui_diligenceGift_noneLeft = "今天的奖励已经领完了"

-- 组织系统

ui_org_pls_select = "请先选择角色"

ui_orgHall_tasktype = "执行了"
ui_orgHall_taskDone = "执行成功"

ui_orgApplied_refuse = "拒绝"
ui_orgApplied_check = "审核中"

ui_orgDetail_applied = "已申请"
ui_orgDetail_apply = "申请加入"
ui_orgHall_hasSignIn = "今天已经执行过任务了"

OrgTitleName = {"普通", "队长","副首领","首领"}
OrgBuilding = {"组织","商城","供奉","大厅","排行", "科技", "兽栏"}

ui_orgUpgrade_expend = "建设值:"
ui_orgUpgrade_condition_0 = "无"
ui_orgUpgrade_condition_1 = "级"
ui_orgUpgrade_top = "等级已达到上限"
ui_orgUpgrade_suc = "升级成功"
ui_orgUpgrade_title = "建筑升级"
ui_orgUpgrade_condition = "条件:"
ui_orgUpgrade_consume = "消耗:"
ui_orgUpgrade_effect = "效果:"
ui_orgSetting_suc = "修改成功"
ui_orgApply_suc = "申请成功"

ui_orgAbdicate = "确定移交首领职位？"
ui_orgDisbandTip = "驱逐首领将解散组织"
ui_orgDisbanded = "组织已解散"

ui_orgShop_openLevel = "级商城开启"
ui_orgShop_btn_0 = "购  买"
ui_orgShop_btn_1 = "已购买"

ui_orgDonate_pls_select = "请先选择"
ui_orgDonate_suc = "捐献成功"
ui_orgDonate_input = "请输入有效数字"

ui_orgMinLevel = "级开启"
ui_orgBuildLv = "Lv."
ui_operationSuc = "操作成功"

ui_orgMemQuit = "是否退出该组织?"
ui_orgLeaderQuit = "由于您是首领，无法退出，\n请先将首领职位转移给组织内其它成员。"
ui_orgQuitSuc = "您已退出组织"
ui_orgMyScore = "您当前的贡献度是："

ui_orgCreateTips = '请正确填写组织名称'

ui_orgMsgTitle = "情报"

--todo
ui_orgBuild_number = "%d建设值"
ui_orgContribute_number = "%d贡献"
ui_orgTechnology_title = "组织科技"
ui_orgTechnology_effect = "个人总效果"
ui_orgTechnology_attackAdd = "攻击+"
ui_orgTechnology_defenseAdd = "防御+"
ui_orgTechnology_chakraAdd = "查克拉+"
ui_orgTechnology_attackPercent = "超强攻击+"
ui_orgTechnology_defensePercent = "超强防御+"
ui_orgTechnology_chakraPercent = "超强查克拉+"

ui_orgTechnology_curBuildingLv = "当前建筑等级:"
ui_orgTechnology_orgContribution = "组织贡献值:"
ui_orgTechnology_consume = "消耗:"
ui_orgTechnology_contributionValue = "贡献值"

ui_diligenceGiftView_activity_time = "活动倒计时"

ui_towerSweepMinVipLv = "VIP%d以上玩家可进行扫荡"
ui_towerSweepbtnOk = "确定"
ui_towerSweepBtnThink = "再想想"
ui_towerSweepBtnCompleteNow = "立即完成"
ui_towerSweepBtnCancel = "取消扫荡"
ui_towerSweepLvTooBig = "你最高只通过到%d关"

ui_towerSweepTipsCancelSweep = "取消扫荡将从扫荡前的关卡开始挑战"
ui_towerSweepTipsCompleteNow = "立即完成需要花费%d元宝"
ui_towerSweepCompleted = "扫荡完成"
ui_towerSweepTitle0 = "选择关卡"
ui_towerSweepTitle1 = "扫荡中"

ui_superNinjaChallenge1 = "今日剩余次数已用完"

ui_awardCenter_date = "%m月%d日 %H:%M"
ui_awardCenter_silver = "银子*"
ui_awardCenter_prestige = "竞技声望*"
ui_awardCenter_name1 = "万里挑一"
ui_awardCenter_wanlitiaoyi = "较大概率获得五星紫卡，保底获得四星蓝卡。"
ui_awardCenter_suc = "领取成功"
ui_awardCenter_contribution = "组织贡献"

ui_crossLimitSuperNinja_noAward = "没有奖励"

ui_trainsoul_maxsize = "您的炼魂卡位已满"

ui_advanceEquip_noequip = "请先选择要进阶的装备"
ui_advanceEquip_diff = "请选择相同的装备"
ui_advanceEquip_sameone = "请不要选择同一件装备"
ui_advanceEquip_diffGrade = "请选择相同阶数的装备"

ui_consumption_tip1 = "消耗的元宝数不足"
ui_consumption_tip2 = "已领取"
ui_consumption_title = "累计消耗达到%s元宝"

ui_summon_text1 = '积分'
ui_summon_text2 = '确认要花费%s'
ui_summon_text3 = '通灵成功'
ui_summon_text4 = '通灵失败'
ui_summon_text5 = '积分不足'
ui_summon_text6 = '奖励已领取'

ui_dailyFirstPay_text1 = '首充%s元奖励'
ui_dailyFirstPay_text2 = '已领取'
ui_dailyFirstPay_text3 = '充值'
ui_dailyFirstPay_text4 = '领取'
ui_dailyFirstPay_error1 = '该奖励已领取'

--宠物
--milo 2015年8月24日 19:44:47
ui_petTraining_normal_desc = '每次获得%d点训练值\n小概率暴击'
ui_petTraining_special_desc = '每次获得%d点训练值\n大概率暴击'
ui_petTraining_maxStar_tip = '已经达到最高星级'
ui_petAdvance_maxRank_tip = '已经达到最高阶级'
ui_petAdvance_maxLevel_tip = '已经达到最高等级'
ui_petAdvance_chance_text = '成功几率:%d'

ui_label_back_text = '返回'
ui_label_train_text = '训练'
ui_label_special_text = '特训'
ui_label_advance_text = '进阶'
ui_label_upgrade_text = '升级'
ui_label_list_text = '列表'
ui_label_select_text = '选择'
ui_label_property_buff_text = '被动属性加成'
ui_label_consume_text = '消耗:'
ui_label_inuse_text = '已上阵'
ui_label_psychic_text = '通灵阵'
ui_label_petlist_text = '通灵兽'
ui_label_ninjatest_text = '中忍考试'
ui_label_gotomystore_text = '前往神秘商店'
ui_label_info_title_text = '说明'
ui_label_info_ok_text = '确定'

ui_pet_text_1 = '尚未达到开放等级'
ui_pet_text_2 = '级'
ui_pet_text_3 = '1、通灵阵中心的通灵兽为主通灵兽，在战斗中显示并施放技能，同时提供属性加成\n2、其他上阵的通灵兽为辅通灵兽，只提供属性加成\n3、通灵兽提供的总属性加成平均分配给每一个上阵的忍者\n4、普通训练每次消耗的元宝数逐渐增加，每日0点重置\n5、可上阵多个相同的通灵兽'
ui_pet_text_4 = '通灵兽可以从神秘商店获取'
ui_pet_text_5 = '开放'
ui_pet_text_6 = '通灵兽提供的总属性加成'

ui_myBase_text1 = '级开放'
ui_myBase_text2 = '建设中，敬请期待'
ui_myBase_text3 = '建设中'
ui_myBase_text4 = '中忍考试'


-- 中忍考试
ui_ninjaTest_exchange = "兑换"
ui_ninjaTest_exchange_desc = "兑换消耗:"
ui_ninjaTest_award_title = "奖励兑换"
ui_ninjaTest_desc_gold = "元宝"
ui_ninjaTest_desc_silver = "银子"
ui_ninjaTest_desc_soul = "忍魂"
ui_ninjaTest_desc_piece = "碎片"

ui_ninjaTestMain_text1 = "当前宝箱奖励已领取"
ui_ninjaTestMain_text2 = "挑战"
ui_ninjaTestMain_text3 = "下一关"
ui_ninjaTestMain_text4 = "尚有奖励未领取，是否放弃奖励"
ui_ninjaTestMain_text5 = "请先设置出战忍者"
ui_ninjaTestMain_text6 = "出战队伍未满5人，是否继续挑战"
ui_ninjaTestMain_text7 = "队伍中有阵亡卡牌，是否继续挑战"
ui_ninjaTestMain_text8 = "该关卡挑战次数不足，是否花费%d元宝购买一次挑战次数"
ui_ninjaTestMain_text9 = "考场"
ui_ninjaTestMain_text10 = "死亡森林"
ui_ninjaTestMain_text11 = "比赛场"
ui_ninjaTestMain_text12 = "是否花费%d元宝开启该宝箱"

ui_ninjaTestMain_text13 = "获得%d积分（积分可以用于兑换奖励）"



ui_ninjaTestDesc_text1 = "1、中忍考试一共十五关，通关当前关卡后才能挑战下一关"
ui_ninjaTestDesc_text2 = "2、玩家拥有的四星和五星忍者都可上阵战斗"
ui_ninjaTestDesc_text3 = "3、上阵的忍者不计算装备和羁绊的属性加成"
ui_ninjaTestDesc_text4 = "4、战斗中阵亡的忍者不复活"

ui_ninjaTestSelect_text1 = "当前卡牌已阵亡，不能上阵"
ui_ninjaTestSelect_text2 = "当前卡牌已上阵，不能在上阵"
ui_ninjaTestSelect_text3 = "出战忍者已满"

ui_ninjaTest_number = "第%d关"

--add orgboss
ui_orgAdoptYes_text1 = "消耗%d银子(今日次数%d/%d)"
ui_orgAdoptYes_text2 = "消耗%d元宝(今日次数%d/%d)"
ui_orgAdoptYes_text3 = "请选择消耗类型"

ui_orgAdoptRank_text1 = "BOSS未死亡"


ui_orgAdoptFight_text1 = "挑战将于20:00之后开启"
ui_orgAdoptFight_text2 = "剩余挑战时间:"
ui_orgAdoptFight_text3 = "挑战将于开启后当晚20:00开启"
ui_orgAdoptFight_text4 = "鼓舞将于20:00之后开启"
ui_orgAdoptFight_text5 = "注：激励鼓舞数值叠加，作用于本次组织boss战斗"

ui_orgMainDesc_text1 = "1.组织首领不登录游戏10天即会开启自动更换"
ui_orgMainDesc_text2 = "2.系统统计最近7天内所有活动成员的贡献值"
ui_orgMainDesc_text3 = "3.系统将按照贡献值以及职位，自动更换首领"
ui_orgMainDesc_text4 = "4.更换前后将会有组织公告发布"
ui_orgMainDesc_text5 = "5.所有更换首领的行为，将会有相应的倒计时，倒计时完成之后才可以再次更换"
ui_orgMainDesc_title = "组织首领更换说明"

ui_festival_firstItemGetFrom = "从历炼，闯关，中忍考试中获得"
ui_festival_secondItemGetFrom = "从切磋中获得"
ui_festival_buyTitle = "道具购买"
ui_festival_beginTimeDesc = "掉落时间:"
ui_festival_endTimeDesc = "结束时间:"
ui_festival_buyDesc = "购买"
ui_festival_exchangeTotalDesc = "兑换次数"
ui_festival_exchangeDesc = "兑换"
ui_festival_buyTimeDesc = "购买时间:"
ui_festival_buyTimesDesc = "购买次数"
ui_timeleft = '剩余时间；'
ui_hotTreasurePreviewTitle = '可能获得'
ui_showAwards_title = '恭喜获得'
