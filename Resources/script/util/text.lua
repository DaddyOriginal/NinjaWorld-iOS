-- This file is created by tools.

module( "text" )

text_config = {
[8000] = {id=8000,
description="参数错误"
},

[8001] = {id=8001,
description="参数错误"
},

[8002] = {id=8002,
description="参数错误"
},

[50000] = {id=50000,
description="无此数据"
},

[9998] = {id=9998,
description="不正确的提交方式"
},

[9999] = {id=9999,
description="非法命令字"
},

[10002] = {id=10002,
description="用户名已经被注册"
},

[10003] = {id=10003,
description="用户名不存在"
},

[10004] = {id=10004,
description="密码错误"
},

[10005] = {id=10005,
description="数据查询不到，无法提交"
},

[10006] = {id=10006,
description="您的元宝不足！"
},

[10007] = {id=10007,
description="您的体力不足！"
},

[10008] = {id=10008,
description="您的银子不足！ "
},

[10009] = {id=10009,
description="数据查询失败"
},

[10010] = {id=10010,
description="数据提交失败"
},

[10011] = {id=10011,
description="已经初始化过"
},

[10012] = {id=10012,
description="没有足够的id"
},

[10013] = {id=10013,
description="名字库已经用光，请自己起个名字吧！"
},

[10014] = {id=10014,
description="已经在此服务器注册账号，不能重复注册！"
},

[10014] = {id=10014,
description="平台号或分区号非法！"
},

[10015] = {id=10015,
description="时间未到,或者已经吃过拉面了。"
},

[10017] = {id=10017,
description="输入的词语中包含敏感字符"
},

[20000] = {id=20000,
description="背包中无数据!"
},

[20001] = {id=20001,
description="待升级的卡不在背包中"
},

[20002] = {id=20002,
description="一些被吞噬的卡不在背包中"
},

[20003] = {id=20003,
description="一些卡的类型和要吞噬的卡类型不一样"
},

[20004] = {id=20004,
description="背包已满"
},

[20005] = {id=20005,
description="吞噬的卡状态不对"
},

[20006] = {id=20006,
description="卡不在背包中"
},

[20007] = {id=20007,
description="卡片不够"
},

[30000] = {id=30000,
description="队伍序列号不对"
},

[30001] = {id=30001,
description="需要更换的卡片不在包裹中"
},

[30002] = {id=30002,
description="需要替换的卡的状态不对"
},

[30003] = {id=30003,
description="需要替换上的卡的状态不对"
},

[30004] = {id=30004,
description="没有更多的卡槽"
},

[30005] = {id=30005,
description="必须要是忍者卡才能开启新槽位"
},

[30006] = {id=30006,
description="忍者卡不能卸下"
},

[30007] = {id=30007,
description="不能新加卡片，因为原来有新卡"
},

[30008] = {id=30008,
description="新卡类型和摆放的位置类型不同"
},

[30009] = {id=30009,
description="非法命令"
},

[30010] = {id=30010,
description="需要卸载某位置的卡不存在"
},

[30011] = {id=30011,
description="队伍中无数据"
},

[30012] = {id=30012,
description="找不到队伍"
},

[30013] = {id=30013,
description="等级不足50级"
},

[30014] = {id=30014,
description="卡片已经在队列中"
},

[30015] = {id=30015,
description="所操作队列位置无数据"
},

[30016] = {id=30016,
description="卡不在队列中"
},

[40001] = {id=40001,
description="无法获取世界历练数据"
},

[40002] = {id=40002,
description="历练数据和后台不匹配"
},

[40003] = {id=40003,
description="无法取得这一回合的信息"
},

[40004] = {id=40004,
description="体力不够，不能进行历练"
},

[40005] = {id=40005,
description="获取NPC信息失败"
},

[40006] = {id=40006,
description="已经打到了世界的尽头"
},

[40007] = {id=40007,
description="你需要历练的章节暂未开启"
},

[40008] = {id=40008,
description="本日回退次数已经用光"
},

[40009] = {id=40009,
description="在章节末尾不能回退"
},

[40010] = {id=40010,
description="无此章节"
},

[40011] = {id=40011,
description="没有找到重置的章节"
},

[40012] = {id=40012,
description="没有达到通关"
},

[80001] = {id=80001,
description="没有取到掉落的忍者卡"
},

[80002] = {id=80002,
description="返回了多张忍者卡"
},

[80003] = {id=80003,
description="还没有到免费抽卡的时间"
},

[80004] = {id=80004,
description="抽卡放用户背包失败"
},

[80005] = {id=80005,
description="到达当天免费抽卡上限"
},

[90001] = {id=90001,
description="碎片不足，不能合成印记"
},

[90002] = {id=90002,
description="合成时印记ID非法"
},

[90003] = {id=90003,
description="已经在合成中了！"
},

[90004] = {id=90004,
description="尚未开始合成"
},

[90005] = {id=90005,
description="时间还没到"
},

[90006] = {id=90006,
description="合成时被抢，碎片不足，合成失败      "
},

[100001] = {id=100001,
description="对方没有你所要抢夺的碎片"
},

[100002] = {id=100002,
description="等级段不匹配"
},

[110001] = {id=110001,
description="下标非法"
},

[110002] = {id=110002,
description="下标指示的内容不存在"
},

[11003] = {id=11003,
description="设置为已读失败"
},

[120001] = {id=120001,
description="已经达到最大好友数目"
},

[120002] = {id=120002,
description="已经是你的好友了"
},

[120003] = {id=120003,
description="已经到达最大数目"
},

[120004] = {id=120004,
description="已经处理过了"
},

[120005] = {id=120005,
description="操作的不是好友申请消息"
},

[130001] = {id=130001,
description="物品数量不足"
},

[130003] = {id=130003,
description="今天使用此物品次数已满"
},

[140001] = {id=140001,
description="进度不匹配"
},

[140002] = {id=140002,
description="没有更多的关卡数"
},

[150001] = {id=150001,
description="不能发起国战，国战数量已经到达上限。"
},

[150002] = {id=150002,
description="抢夺职位失败，正在被其他人夺取此职位。"
},

[150003] = {id=150003,
description="正在宣战中，不能宣战。"
},

[150004] = {id=150004,
description="你所在的国家尚未开始国战"
},

[150005] = {id=150005,
description="暂时没有可以对战的用户"
},

[150006] = {id=150006,
description="国战未开放"
},

[150007] = {id=150007,
description="你已经无复活次数，请先购买."
},

[150008] = {id=150008,
description="无此兑换ID"
},

[150009] = {id=150009,
description="此礼包ID不是兑换id"
},

[150010] = {id=150010,
description="功勋值不足"
},

[150011] = {id=150011,
description="已经领取了该礼包"
},

[150012] = {id=150012,
description="上次国战未开放"
},

[150013] = {id=150013,
description="上次国战未胜利，不能领取."
},

[150014] = {id=150014,
description="您没权发动国战"
},

[150015] = {id=150015,
description="不能对自己国家宣战"
},

[150016] = {id=150016,
description="正在国战中，不能宣战"
},

[150017] = {id=150017,
description="正在国战或者在国战前一个小时，不能查看结果页面"
},

[150018] = {id=150018,
description="上期未参与国战，结果页面不展示"
},

[150019] = {id=150019,
description="没有资格领取官员礼包"
},

[150020] = {id=150020,
description="今天已经领取过了，不能重复领取"
},

[150021] = {id=150021,
description="不能跟自己抢夺官职"
},

[150022] = {id=150022,
description="在职必须满24小时才能领取礼包"
},

[150023] = {id=150023,
description="我方士气已经为0,无法再参与攻击!"
},

[160001] = {id=160001,
description="有人正在抢夺"
},

[160002] = {id=160002,
description="不能攻击，请查看CD或者攻击次数"
},

[160003] = {id=160003,
description="不能跟自己打架"
},

[160004] = {id=160004,
description="目标正在被攻击，请稍后重试"
},

[160005] = {id=160005,
description="不能挑战比自己等级低的玩家"
},

[160006] = {id=160006,
description="领取礼包时间未到"
},

[160007] = {id=160007,
description="未达到领取的条件"
},

[160008] = {id=160008,
description="挑战的等级超出范围"
},

[160009] = {id=160009,
description="可以攻击，不需要重置cd，请别浪费元宝"
},

[160010] = {id=160010,
description="今天可攻击次数用完，重置无效！"
},

[160011] = {id=160011,
description="总兑换次数到达上限"
},

[160012] = {id=160012,
description="今日兑换次数到达上限"
},

[160013] = {id=160013,
description="已经领取当日奖励"
},

[160014] = {id=160014,
description="奖励领取时间未到"
},

[160015] = {id=160015,
description="奖励领取时间已过."
},

[160016] = {id=160016,
description="您当前没有排名"
},

[160017] = {id=160017,
description="当前正处于领奖时间,不能挑战."
},

[160018] = {id=160018,
description="打斗时间不能领奖"
},

[170001] = {id=170001,
description="领取的活动已经过期。"
},

[170002] = {id=170002,
description="暂时没有资格领取"
},

[170003] = {id=170003,
description="原来的月卡还未领取完成，暂时无法购买同样类型的产品。"
},

[170004] = {id=170004,
description="已经领取完成，不能再次领取。"
},

[170005] = {id=170005,
description="今天已经领取，请明天再来领取。"
},

[170006] = {id=170006,
description="活动不存在"
},

[170007] = {id=170007,
description="已经领取，不能重复领取"
},

[170008] = {id=170008,
description="笨活动不存在"
},

[180001] = {id=180001,
description="需要cd时间"
},

[180002] = {id=180002,
description="今天没有剩余的次数了"
},

[180003] = {id=180003,
description="您已经是守卫了"
},

[180004] = {id=180004,
description="抢夺的国家不能是本国"
},

[180005] = {id=180005,
description="已经没有可以抢夺的碎片了"
},

[180006] = {id=180006,
description="可以攻击，不需要清除cd"
},

[180007] = {id=180007,
description="无剩余攻击次数，清除cd无效"
},

[180007] = {id=180007,
description="没有可以领取的奖励"
},

[190008] = {id=190008,
description="已经达到最高等级"
},

[190009] = {id=190009,
description="仙石数目错误"
},

[190010] = {id=190010,
description="此类型的卡牌暂不能强化"
},

[210000] = {id=210000,
description="没有此APPID"
},

[210001] = {id=210001,
description="秘钥错误"
},

[210002] = {id=210002,
description="SESSION错误"
},

[210004] = {id=210004,
description="未找到此平台的配置"
},

[210005] = {id=210005,
description="账号验证失败"
},

[210006] = {id=210006,
description="平台id错误"
},

[210007] = {id=210007,
description="区域错误"
},

[210008] = {id=210008,
description="未开放注册"
},

[210009] = {id=210009,
description="通用鉴权参数错误"
},

[210010] = {id=210010,
description="登陆过期"
},

[210011] = {id=210011,
description="机器码验证失败"
},

[210012] = {id=210012,
description="请先注册一个帐号保存当前游戏的数据,才可以登录其他帐号。"
},

[210014] = {id=210014,
description="账号被封，请联系管理员"
},

[220000] = {id=220000,
description="没有此平台的的信息"
},

[230001] = {id=230001,
description="平台号找不到"
},

[230002] = {id=230002,
description="分区号找不到"
},

[240000] = {id=240000,
description="骚年，今天已经欺负过人家5次了，明天再来吧。"
},

[250001] = {id=250001,
description="发货失败，错误的订单号。"
},

[250002] = {id=250002,
description="创建订单失败,请不要重复下单。"
},

[250003] = {id=250003,
description="已经购买过月卡了。"
},

[250004] = {id=250004,
description="非法商品ID"
},

[250005] = {id=250005,
description="还没有资格领取首充礼包"
},

[250006] = {id=250006,
description="还没发货完成"
},

[250007] = {id=250007,
description="找不到的订单号"
},

[250008] = {id=250008,
description="没权限领取首充礼包"
},

[250009] = {id=250009,
description="10分钟内不能重复生成月卡订单 "
},

[270000] = {id=270000,
description="大转盘已关闭"
},

[270001] = {id=270001,
description="大转盘已经过期"
},

[270002] = {id=270002,
description="大转盘id不一致,应该是非同一次"
},

[270003] = {id=270003,
description="您的元宝不足"
},

[270004] = {id=270004,
description="您的背包已满"
},

[270005] = {id=270005,
description="排行礼包请在活动结束3小时内自行领取"
},

[270006] = {id=270006,
description="大转盘礼包已经领取"
},

[270007] = {id=270007,
description="您的积分不足,兑换失败"
},

[280001] = {id=280001,
description="邀请码已经使用"
},

[280002] = {id=280002,
description="操作过于频繁"
},

[280003] = {id=280003,
description="今天超过输入总限制,明天再来吧"
},

[280004] = {id=280004,
description="邀请码不存在"
},

[280005] = {id=280005,
description="已经领取了此类型的礼包"
},

[20006] = {id=20006,
description="卡不存在"
},

[20005] = {id=20005,
description="卡已经上阵，不能拆解"
},

[290000] = {id=290000,
description="不支持的卡片类型"
},

[289999] = {id=289999,
description="低等级卡不能拆解"
},

[290001] = {id=290001,
description="忍者卡未到最大等级"
},

[290002] = {id=290002,
description="没有足够的同类型卡"
},

[290003] = {id=290003,
description="没有足够的转生丹"
},

[290004] = {id=290004,
description="被转生的卡类型错误"
},

[290005] = {id=290005,
description="转生等级错误"
},

[290006] = {id=290006,
description="请选择同样的卡"
},

[300001] = {id=300001,
description="群号错误"
},

[300002] = {id=300002,
description="平台错误"
},

[300003] = {id=300003,
description="分区号错误"
},

[300004] = {id=300004,
description="爆满，请选择其他服"
},

[300005] = {id=300005,
description="爆满，请选择其他服"
},

[300006] = {id=300006,
description="爆满，请选择其他服"
},

[290101] = {id=290101,
description="已经达到当日购买上限"
},

[290102] = {id=290102,
description="vip等级不足,不能购买该礼包"
},

[290103] = {id=290103,
description="元宝不足"
},

[290104] = {id=290104,
description="今天已经买过,这个礼包"
},

[290200] = {id=290200,
description="要转的目标国家不存在"
},

[290201] = {id=290201,
description="正在国战或者国战前1小时不能转国"
},

[290202] = {id=290202,
description="不能转到你当前自己的国家"
},

[290203] = {id=290203,
description="卡的类型错误"
},

[290300] = {id=290300,
description="刮刮乐元宝不够"
},

[310001] = {id=310001,
description="收集ID错误"
},

[310002] = {id=310002,
description="兑换次数达到上限"
},

[310003] = {id=310003,
description="条件不满足"
},

[240002] = {id=240002,
description="切磋值不足,不能切磋"
},

[310004] = {id=310004,
description="已经自动战斗，不需要购买，请勿浪费元宝"
},

[312000] = {id=312000,
description="此活动已经结束"
},

[312001] = {id=312001,
description="无资格领取"
},

[312002] = {id=312002,
description="未到领取时间"
},

[312003] = {id=312003,
description="已经过了领取时间"
},

[313000] = {id=313000,
description="此活动已经结束"
},

[313001] = {id=313001,
description="不是第一名，无法领取奖励"
},

[313002] = {id=313002,
description="未到领取奖励的时间"
},

[313003] = {id=313003,
description="已经过了领取奖励的时间"
},

[313004] = {id=313004,
description="没有血量继续挑战"
},

[313005] = {id=313005,
description="随机失败"
},

[313006] = {id=313006,
description="和当前NPC进度不匹配"
},

[313007] = {id=313007,
description="非奖励领取时间.不能领取奖励"
},

[313008] = {id=313008,
description="没有可以兑换的奖励"
},

[313009] = {id=313009,
description="对不起，幸运值不足"
},

[313010] = {id=313010,
description="奖励类型不对"
},

[313011] = {id=313011,
description="获取排行榜数据失败"
},

[313012] = {id=313012,
description="增加名次出错"
},

[313013] = {id=313013,
description="没有权限领取此奖励"
},

[313014] = {id=313014,
description="已经领取过奖励"
},

[313015] = {id=313015,
description="生命值已经达到上限，无须购买"
},

[315001] = {id=315001,
description="活动关闭"
},

[315002] = {id=315002,
description="BOSS已经被打败"
},

[315003] = {id=315003,
description="BOSS还未死亡或则还未结束"
},

[315004] = {id=315004,
description="已经领取过了声望奖励"
},

[315005] = {id=315005,
description="活动正在进行中，请结束后来领取"
},

[315006] = {id=315006,
description="已经领取过奖励"
},

[315007] = {id=315007,
description="未达到领取的条件"
},

[315008] = {id=315008,
description="声望不够"
},

[315009] = {id=315009,
description="上期活动已关闭,不能领取"
},

[316009] = {id=316009,
description="今天本章回退次数已满"
},

[317001] = {id=317001,
description="道具不是双倍增益"
},

[317002] = {id=317002,
description="效果道具不存在"
},

[318001] = {id=318001,
description="频率受限"
},

[319001] = {id=319001,
description="来晚了，已经卖光了"
},

[319002] = {id=319002,
description="未找到商品"
},

[319003] = {id=319003,
description="活动已经结束"
},

[320001] = {id=320001,
description="活动已经结束"
},

[320002] = {id=320002,
description="鲜花不够"
},

[321001] = {id=321001,
description="只有忍者卡才能培养"
},

[321002] = {id=321002,
description="超忍秘药不足"
},

[321003] = {id=321003,
description="区间配置错误"
},

[321004] = {id=321004,
description="需要更多的潜力值"
},

[323001] = {id=323001,
description="报名进行中"
},

[323002] = {id=323002,
description="战斗已经结束"
},

[323003] = {id=323003,
description="您没有没战斗资格"
},

[323004] = {id=323004,
description="未开放"
},

[323005] = {id=323005,
description="不在报名期间"
},

[323006] = {id=323006,
description="不在战斗期间"
},

[323007] = {id=323007,
description="激励次数已经达到上限"
},

[323008] = {id=323008,
description="积分不足"
},

[323009] = {id=323009,
description="已经参加，不用报名了"
},

[323010] = {id=323010,
description="挑战的位置id出错"
},

[323011] = {id=323011,
description="不能pk自己"
},

[323012] = {id=323012,
description="位置已经有人在打"
},

[323013] = {id=323013,
description="自己的位置已经变化"
},

[323014] = {id=323014,
description="战斗还未结束"
},

[323015] = {id=323015,
description="奖励已经领取"
},

[323016] = {id=323016,
description="不够资格受奖"
},

[323017] = {id=323017,
description="页码不在可接受范围内"
},

[323018] = {id=323018,
description="本期战斗还未开始"
},

[325000] = {id=325000,
description="不能刷新"
},

[325001] = {id=325001,
description="商品不存在"
},

[325002] = {id=325002,
description="商品已经被买过了"
},

[325003] = {id=325003,
description="错误的支付类型"
},

[325004] = {id=325004,
description="魂不够"
},

[325005] = {id=325005,
description="卡等级太低"
},

[325006] = {id=325006,
description="每天只能刷新150次，请明天再来吧！"
},

[324000] = {id=324000,
description="碎片数目不足"
},

[324001] = {id=324001,
description="碎片不存在"
},

[324002] = {id=324002,
description="碎片背包已满"
},

[325000] = {id=325000,
description="无招募令"
},

[330000] = {id=330000,
description="您扫荡次数已用完!"
},

[330001] = {id=330001,
description="您已经扫荡过了!"
},

[340000] = {id=340000,
description="任务列表为空"
},

[340001] = {id=340001,
description="奖励列表为空"
},

[340002] = {id=340002,
description="领取奖励的积分不足!"
},

[340003] = {id=340003,
description="奖励已经领取过了!"
},

[350000] = {id=350000,
description="每天晚上8点到8点半为开奖时间，请晚上8点半再下注吧！"
},

[350001] = {id=350001,
description="您当期投注次数达到限制，升级vip可以投注更多哦~"
},

[350002] = {id=350002,
description="您没有获得奖励，请继续加油！"
},

[350003] = {id=350003,
description="您已经领取过奖励"
},

[350005] = {id=350005,
description="此活动需要50级才能参加哦~请努力升级吧！"
},

[520001] = {id=520001,
description="需要开启此功能，请下载完整版"
},

[140003] = {id=140003,
description="成就不足。"
},

[140004] = {id=140004,
description="奖励已经领取，不能重复领取。"
},

[322008] = {id=322008,
description="体力已满，无需增加"
},

[326700] = {id=326700,
description="日期不在目标范围内         "
},

[326701] = {id=326701,
description="奖励领取过了                "
},

[326702] = {id=326702,
description="不够条件领取"
},

[290105] = {id=290105,
description="已经买过,这个礼包"
},

[325007] = {id=325007,
description="达到开箱限制"
},

[326000] = {id=326000,
description="活动结束"
},

[326001] = {id=326001,
description="超忍配置表读取失败"
},

[326002] = {id=326002,
description="超忍排行列表获取失败"
},

[326003] = {id=326003,
description="超忍玩家排行获取失败"
},

[326004] = {id=326004,
description="免费时间未到"
},

[326005] = {id=326005,
description="增加名次出错"
},

[326006] = {id=326006,
description="活动不在领奖期间"
},

[326007] = {id=326007,
description="无奖励可领"
},

[326008] = {id=326008,
description="已经领奖"
},

[326700] = {id=326700,
description="日期不在目标范围内  "
},

[326701] = {id=326701,
description="奖励领取过了"
},

[326702] = {id=326702,
description="不够条件领取"
},

[327000] = {id=327000,
description="此类型已经增加到最大"
},

[327001] = {id=327001,
description="时间正在cd中，不能攻击"
},

[327002] = {id=327002,
description="当前可以攻击，请不要浪费元宝清除cd"
},

[327003] = {id=327003,
description="上次未参加世界BOSS活动，不能领取奖励"
},

[327004] = {id=327004,
description="每天8点开始可以增加攻击力"
},

[327005] = {id=327005,
description="BOSS已死，增加无用,请不要浪费资源"
},

[328000] = {id=328000,
description="基金已购买"
},

[328001] = {id=328001,
description="配置表读取失败"
},

[328002] = {id=328002,
description="没有奖励领取"
},

[328003] = {id=328003,
description="已经领取奖励"
},

[328004] = {id=328004,
description="获取奖励信息失败"
},

[328005] = {id=328005,
description="数据更新失败"
},

[328006] = {id=328006,
description="成长基金未购买"
},

[328007] = {id=328007,
description="活动未开启"
},

[329000] = {id=329000,
description="获取某一国家的边境碑数据失败"
},

[329001] = {id=329001,
description="设置某一国家的边境碑数据失败"
},

[329002] = {id=329002,
description="获取边境碑数据失败"
},

[329003] = {id=329003,
description="设置边境碑数据失败"
},

[329004] = {id=329004,
description="获取配置项失败"
},

[329005] = {id=329005,
description="边境战未开始"
},

[329006] = {id=329006,
description="边境战已经结束"
},

[329007] = {id=329007,
description="您决斗值不足"
},

[329008] = {id=329008,
description="您已经领取过奖励"
},

[329009] = {id=329009,
description="您已经领取过边境丰碑时间奖励"
},

[329010] = {id=329010,
description="您已经领取过边境前三奖励"
},

[329011] = {id=329011,
description="您所在的排名没有奖励"
},

[329012] = {id=329012,
description="您正在挑衅此国"
},

[329013] = {id=329013,
description="您正在游荡此国"
},

[329014] = {id=329014,
description="获取奖励信息失败"
},

[329015] = {id=329015,
description="设置奖励信息失败"
},

[329016] = {id=329016,
description="边境碑暂时不能被攻击"
},

[329017] = {id=329017,
description="边境碑已经被摧毁"
},

[329018] = {id=329018,
description="元宝不足"
},

[329019] = {id=329019,
description="银子不足"
},

[329020] = {id=329020,
description="有奖励的情况下，切换目标攻击"
},

[329021] = {id=329021,
description="玩家正在被别人清理"
},

[329022] = {id=329022,
description="敌人不在挑衅或游荡状态，无法清理"
},

[630001] = {id=630001,
description="已签到"
},

[630002] = {id=630002,
description="本日不可签到"
},

[610003] = {id=610003,
description="至少要三星卡才可以上阵"
},

[610004] = {id=610004,
description="积分不够"
},
[610005] = {id=610005,
description="宝箱索引错误"
},
[610006] = {id=610006,
description="已经打完所有关卡"
},
[610007] = {id=610007,
description="挑战次数用完"
},
[610008] = {id=610008,
description="请先设置阵容"
},
[610009] = {id=610009,
description="已经领取奖励"
},
[610010] = {id=610010,
description="该英雄已经阵亡，不可以上阵"
},
[610011] = {id=610011,
description="该关卡未打过"
},
[610012] = {id=610012,
description="已经通过该关卡"
},

---

[1] = {id=1,
description="购买成功"
},

[2] = {id=2,
description="使用成功，获得%d两银两"
},

[3] = {id=3,
description="使用成功，为您恢复%d点体力值"
},

[4] = {id=4,
description="使用成功，为您恢复%d%%的决斗值"
},

[5] = {id=5,
description="您的体力值已满"
},

[6] = {id=6,
description="您的决斗值已满"
},

[7] = {id=7,
description="您的道具不足"
},

[8] = {id=8,
description="您的元宝不足，请充值"
},

[9] = {id=9,
description="您的银子不足，可以用元宝在商城购买"
},

[10] = {id=10,
description="风之国"
},

[11] = {id=11,
description="雷之国"
},

[12] = {id=12,
description="水之国"
},

[13] = {id=13,
description="火之国"
},

[14] = {id=14,
description="土之国"
},

[15] = {id=15,
description="%2d天%2d小时后免费"
},

[16] = {id=16,
description="%02d:%02d后免费"
},

[17] = {id=17,
description="今天可领取%d次"
},

[18] = {id=18,
description="请选择一个国家宣战"
},

[19] = {id=19,
description="已经是这个官职了"
},

[20] = {id=20,
description="好友申请"
},

[21] = {id=21,
description="好友消息"
},

[22] = {id=22,
description=[[玩家%s想要跟您成为好友。
附言: %s。]]
},

[23] = {id=23,
description="玩家%s想要跟您成为好友。附言: %s。"
},

[24] = {id=24,
description="玩家%s同意跟您成为好友。"
},

[25] = {id=25,
description="玩家%s拒绝跟您成为好友。"
},

[26] = {id=26,
description="切磋"
},

[27] = {id=27,
description="夺宝"
},

[28] = {id=28,
description="玩家%s在切磋中战胜了您，您损失了%d银子。"
},

[29] = {id=29,
description="玩家%s在切磋中输给了您，您获得了%d银子。"
},

[30] = {id=30,
description="玩家%s战胜了您，并从您手中抢夺到了%s。"
},

[31] = {id=31,
description="玩家%s想要夺取您手里的%s，战胜了您，可惜没抢去。"
},

[32] = {id=32,
description="玩家%s想要夺取您手里的%s，但是被您打败了。"
},

[33] = {id=33,
description="您获胜了"
},

[34] = {id=34,
description="您失败了"
},

[35] = {id=35,
description="系统消息"
},

[36] = {id=36,
description="已经对%s宣战，开战时间：%s"
},

[37] = {id=37,
description="%s克%s"
},

[38] = {id=38,
description="战争已经开始，请加入战争吧！！"
},

[39] = {id=39,
description="竞该职位需要花费元宝：%d"
},

[40] = {id=40,
description="忍者卡"
},

[41] = {id=41,
description="装备卡"
},

[42] = {id=42,
description="武器卡"
},

[43] = {id=43,
description="饰品卡"
},

[44] = {id=44,
description="忍术卡"
},

[45] = {id=45,
description="印记碎片"
},

[46] = {id=46,
description="印记卡"
},

[47] = {id=47,
description="功勋值不足，请稍后再试"
},

[48] = {id=48,
description="君子报仇十年不晚，请提升实力后再来报仇雪恨吧。"
},

[49] = {id=49,
description="恭喜你大发神威，抢夺尾兽印记：%s 成功！"
},

[50] = {id=50,
description="太背了，你虽然获胜了，但对方带着%s逃跑了！"
},

[51] = {id=51,
description="君子报仇十年不晚，请提升实力后再来报仇雪恨吧。"
},

[52] = {id=52,
description="你棋高一筹，将对方击败，成功抢夺到对方的%s：%s"
},

[53] = {id=53,
description="本次闯关获得奖励："
},

[54] = {id=54,
description="胜败乃兵家常事，年轻的忍者请增强实力后再来吧！"
},

[55] = {id=55,
description="（可以通过装备忍者、装备、忍术来提高能力，也可以通过升级、强化这些卡来提高能力。）"
},

[56] = {id=56,
description="恭喜你，打败了强大的对手，成功晋级！"
},

[57] = {id=57,
description="胜败乃兵家常事，年轻的忍者，请增强实力后再来挑战吧。"
},

[58] = {id=58,
description="你棋高一筹，击败火之国的敌人：%s，捍卫了国家荣耀。本次国战斩杀：%d人"
},

[59] = {id=59,
description="胜败乃兵家常事，你不敌火之国的玩家木叶丸，请再接再厉！本次国战剩余复活次数：%d"
},

[60] = {id=60,
description="国战功勋：%d（国战功勋可兑换宝箱，有机会获得六道仙人的卡牌哦。）"
},

[61] = {id=61,
description="你异常神勇，击败%s的宝藏守护者：%s，成功抢夺到一块国家宝藏。"
},

[62] = {id=62,
description="胜败乃兵家常事，你被%s的宝藏守护者：%s所击败，没有抢到国家宝藏，请继续努力。"
},

[63] = {id=63,
description="国家宝藏%d块（请返回您的国家宝藏界面，上交，可获得5000两银子！）"
},

[64] = {id=64,
description="忍术伤害会受查克拉加成，每500点查克拉，忍术伤害提高一倍。"
},

[65] = {id=65,
description="可免费招募"
},

[66] = {id=66,
description="六星橙卡："
},

[67] = {id=67,
description="五星紫卡："
},

[68] = {id=68,
description="四星蓝卡："
},

[69] = {id=69,
description="三星绿卡："
},

[70] = {id=70,
description="二星黄卡："
},

[71] = {id=71,
description="今天回退次数不足"
},

[72] = {id=72,
description="回退成功，已回退%d个回合"
},

[73] = {id=73,
description="现在不是免费招募时间，继续招募将花费您%d元宝"
},

[74] = {id=74,
description="体力值："
},

[75] = {id=75,
description="经验值："
},

[76] = {id=76,
description="第%d回合："
},

[77] = {id=77,
description="当你打不过当前怪物的时候,可以选择退一步,成长起来再继续挑战当前怪物。每天可以后退50次。"
},

[78] = {id=78,
description="技能伤害会受查克拉加成，每500点查克拉，技能伤害提高一倍。"
},

[79] = {id=79,
description="回退失败"
},

[80] = {id=80,
description="技能增加伤害：%d"
},

[81] = {id=81,
description="(查克拉增加：%d)"
},

[82] = {id=82,
description="职位空缺"
},

[83] = {id=83,
description="国战尚未发起（成为国家官员可以发起！）"
},

[84] = {id=84,
description="国战尚未开始，将于%s%s对%s发起！"
},

[85] = {id=85,
description="国战正在进行中，为了国家荣誉，参战吧！"
},

[86] = {id=86,
description="今天"
},

[87] = {id=87,
description="明天"
},

[88] = {id=88,
description="您已经在当前职位。"
},

[89] = {id=89,
description="本国已经发起了国战。"
},

[90] = {id=90,
description="此国家已经发起了国战。请选择别的国家。"
},

[91] = {id=91,
description="您目前不是官员职位，不能发起国战。"
},

[92] = {id=92,
description="%s忍者%s击败了%s忍者%s，获得了%d连胜！"
},

[93] = {id=93,
description="国战战胜， 你的国家在与%s的戮战中取得了胜利！（点击箱子领取奖励）"
},

[94] = {id=94,
description="国战战败，你的国家在与%s的戮战中失败了，请加油。"
},

[95] = {id=95,
description="正在戮战： 你的国家正在与%s戮战，为了国家荣誉，杀！"
},

[96] = {id=96,
description="您不在此职位，不能领取此奖励。"
},

[97] = {id=97,
description="没到领奖时间，不能领取奖励。"
},

[98] = {id=98,
description="国战尚未开始。"
},

[99] = {id=99,
description="请选择一个国家发起国战。"
},

[100] = {id=100,
description="成功领取奖励。"
},

[101] = {id=101,
description="风之国"
},

[102] = {id=102,
description="雷之国"
},

[103] = {id=103,
description="水之国"
},

[104] = {id=104,
description="火之国"
},

[105] = {id=105,
description="土之国"
},

[106] = {id=106,
description="是否花费%d元宝购买10次复活。"
},

[107] = {id=107,
description="是否要争夺国家守卫？"
},

[108] = {id=108,
description="争夺成功。"
},

[109] = {id=109,
description="没有宝物可以上交。"
},

[110] = {id=110,
description="今天的抢夺次数已经用完。"
},

[111] = {id=111,
description="是否花费%d元宝清除冷却时间？"
},

[112] = {id=112,
description="请选择要抢夺的国家。"
},

[113] = {id=113,
description="成功夺取宝藏，上交可获得%d银子。"
},

[114] = {id=114,
description="很幸运，%s没有守卫，成功获得一块宝藏。"
},

[115] = {id=115,
description="你神勇异常，击败了%s的守卫%s，成功夺取一块宝藏。"
},

[116] = {id=116,
description="你被%s的守卫%s击败了，没能夺取宝藏。"
},

[117] = {id=117,
description="抢夺宝藏失败，请继续努力。"
},

[118] = {id=118,
description="今天已经没有挑战机会了。"
},

[119] = {id=119,
description="成功领取奖励。"
},

[120] = {id=120,
description="购买失败。"
},

[121] = {id=121,
description="%s不够，要购买%s？"
},

[122] = {id=122,
description="体力"
},

[123] = {id=123,
description="决斗值"
},

[124] = {id=124,
description="银子"
},

[125] = {id=125,
description="输入的用户名字过长。"
},

[126] = {id=126,
description="已经没有投注机会了。"
},

[127] = {id=127,
description="开始合成"
},

[128] = {id=128,
description="卡牌已经满级，不能再强化。"
},

[129] = {id=129,
description="没有足够的银子进行强化。"
},

[130] = {id=130,
description="没有一星或两星的卡牌。"
},

[131] = {id=131,
description="一次不能选择超过30张卡牌进行强化。"
},

[132] = {id=132,
description="玩家%s想跟您成为好友。"
},

[133] = {id=133,
description="发送好友邀请成功。"
},

[134] = {id=134,
description="已经是好友了，不能重复添加。"
},

[135] = {id=135,
description="您已经是国家守卫了。"
},

[136] = {id=136,
description="输入要搜索的名字："
},

[137] = {id=137,
description="请选择卡牌强化。"
},

[138] = {id=138,
description="竞选成功"
},

[139] = {id=139,
description="花费%d元宝消除冷却？"
},

[140] = {id=140,
description="用户名只能是数字或字母"
},

[141] = {id=141,
description="输入的密码只能是数字和字母"
},

[142] = {id=142,
description="用户名长度必须在6到30之间"
},

[143] = {id=143,
description="密码长度必须在6到30之间"
},

[144] = {id=144,
description="请选择您的国家"
},

[145] = {id=145,
description="请选择一名队友"
},

[146] = {id=146,
description="名字不能包含特殊字符"
},

[147] = {id=147,
description="名字长度最少2个字，最多6个字"
},

[148] = {id=148,
description="您已通过该章节的全部难度，请选择其他章节进行历练"
},

[149] = {id=149,
description="没达到领奖条件。"
},

[150] = {id=150,
description="还没到领奖时间。"
},

[151] = {id=151,
description="清除冷却成功。"
},

[152] = {id=152,
description="玩家%s在天梯里击败了你，你的天梯名次降为%d名。"
},

[153] = {id=153,
description="%s玩家%s，前来抢夺我国国家宝藏，但被你击败！"
},

[154] = {id=154,
description="%s玩家%s，前来抢夺我国国家宝藏，击败你抢走了一块！"
},

[155] = {id=155,
description="我国玩家%s，成功击败你，抢夺了本来属于你的%s职位。"
},

[156] = {id=156,
description="我国玩家%s，前来抢夺你的%s职位，但是被你击败！"
},

[157] = {id=157,
description="火影"
},

[158] = {id=158,
description="土影"
},

[159] = {id=159,
description="风影"
},

[160] = {id=160,
description="水影"
},

[161] = {id=161,
description="雷影"
},

[162] = {id=162,
description="护法"
},

[163] = {id=163,
description="长老"
},

[164] = {id=164,
description="已经打过它了，请明天再来。"
},

[165] = {id=165,
description="请先打败前面的。"
},

[166] = {id=166,
description="暂未开放"
},

[167] = {id=167,
description="竞技场"
},

[168] = {id=168,
description="国家宝藏"
},

[169] = {id=169,
description="国战"
},

[170] = {id=170,
description="注册账号"
},

[171] = {id=171,
description="请输入正确的邮件地址"
},

[172] = {id=172,
description="密码不相同"
},

[173] = {id=173,
description="竞技场礼包还没能领取。"
},

[174] = {id=174,
description="还没到礼包领取时间。"
},

[175] = {id=175,
description="玩家%s邀请你成为好友。"
},

[176] = {id=176,
description="没有国战信息。"
},

[177] = {id=177,
description="元宝不足。"
},

[178] = {id=178,
description="已经没有下注机会了。"
},

[179] = {id=179,
description="成功发送好友申请。"
},

[180] = {id=180,
description="为了您的数据安全，30秒内不能重复购买月卡。"
},

[181] = {id=181,
description="购买月卡成功，请重新进入活动领取元宝"
},

[182] = {id=182,
description="购买元宝成功"
},

[183] = {id=183,
description="粹练即将开放"
},

[184] = {id=184,
description="体力、斗力全满，可以继续冒险了！"
},

[185] = {id=185,
description="目前尚无资格，充值任一金额即可领取。"
},

[186] = {id=186,
description="获得5张四星忍者卡碎片 "
},

[187] = {id=187,
description="获得5张五星忍者卡碎片"
},

[188] = {id=188,
description="已经领取过首充礼包"
},

[189] = {id=189,
description="我国玩家%s，成功击败你，抢夺了你的国家守卫职位。"
},

[190] = {id=190,
description="每天可以摇动人参果树%d次，最大获得%d倍的银子回报，保底%d倍"
},

[191] = {id=191,
description="快来尝试下手气吧，朋友！每天前%d次免费。"
},

[192] = {id=192,
description="还有%d次免费机会，最低获得%d两银子。"
},

[193] = {id=193,
description="摇树的次数不足，请明天再来！"
},

[194] = {id=194,
description="免费摇树的次数已经用尽，继续摇树将花费您%d元宝"
},

[195] = {id=195,
description="第一章"
},

[196] = {id=196,
description="第二章"
},

[197] = {id=197,
description="第三章"
},

[198] = {id=198,
description="第四章"
},

[199] = {id=199,
description="第五章"
},

[200] = {id=200,
description="第六章"
},

[201] = {id=201,
description="第七章"
},

[202] = {id=202,
description="第八章"
},

[203] = {id=203,
description="第九章"
},

[204] = {id=204,
description="第十章"
},

[205] = {id=205,
description="第十一章"
},

[206] = {id=206,
description="第十二章"
},

[207] = {id=207,
description="第十三章"
},

[208] = {id=208,
description="第十四章"
},

[209] = {id=209,
description="第十五章"
},

[210] = {id=210,
description="第十六章"
},

[211] = {id=211,
description="第十七章"
},

[212] = {id=212,
description="第十八章"
},

[213] = {id=213,
description="恭喜您成功通过历练%s-%s"
},

[214] = {id=214,
description="第二难度"
},

[215] = {id=215,
description="第三难度"
},

[216] = {id=216,
description="第四难度"
},

[217] = {id=217,
description="第五难度"
},

[218] = {id=218,
description="第六难度"
},

[219] = {id=219,
description="第七难度"
},

[220] = {id=220,
description="第八难度"
},

[221] = {id=221,
description="第九难度"
},

[222] = {id=222,
description="第十难度"
},

[223] = {id=223,
description="第十一难度"
},

[224] = {id=224,
description="第十二难度"
},

[225] = {id=225,
description="第十三难度"
},

[226] = {id=226,
description="第十四难度"
},

[227] = {id=227,
description="第十五难度"
},

[228] = {id=228,
description="（%s%s开启！）"
},

[229] = {id=229,
description="（%s所有难度已完成！）"
},

[230] = {id=230,
description="请先选择要淬炼的卡牌。"
},

[231] = {id=231,
description="已经达到最大淬炼等级。"
},

[232] = {id=232,
description="已经达到自动淬炼等级。"
},

[233] = {id=233,
description="仙石不足。"
},

[234] = {id=234,
description="请先取消自动淬炼。"
},

[235] = {id=235,
description="元宝"
},

[256] = {id=256,
description="银子"
},

[257] = {id=257,
description="攻击下限增加"
},

[258] = {id=258,
description="攻击上限增加"
},

[259] = {id=259,
description="防御下限增加"
},

[260] = {id=260,
description="防御上限增加"
},

[261] = {id=261,
description="查克拉下限增加"
},

[262] = {id=262,
description="查克拉上限增加"
},

[263] = {id=263,
description="打出最大防御概率"
},

[264] = {id=264,
description="打出最大攻击概率"
},

[265] = {id=265,
description="%d天%d小时%d分%d秒"
},

[266] = {id=266,
description="请您加QQ群:172109254"
},

[267] = {id=267,
description="确定花费%d元宝抽奖？"
},

[268] = {id=268,
description="转盘"
},

[269] = {id=269,
description="请输入邀请码："
},

[270] = {id=270,
description="请输入用户名"
},

[271] = {id=271,
description="材料"
},

[272] = {id=272,
description="%d级开启"
},

[273] = {id=273,
description="请先选择要转生的卡牌。"
},

[274] = {id=274,
description="已经达到转生最大等级。"
},

[275] = {id=275,
description="卡牌需要达到最大等级才可以转生。"
},

[276] = {id=276,
description="转生丹不足。"
},

[277] = {id=277,
description="材料卡不足。"
},

[278] = {id=278,
description="没有50级卡牌。"
},

[279] = {id=279,
description="%d天之前"
},

[280] = {id=280,
description="%d小时之前"
},

[281] = {id=281,
description="%d分钟之前"
},

[282] = {id=282,
description="暂时还没攻击录像。"
},

[283] = {id=283,
description="已经没有复活次数了，自动国战将会停止。"
},

[284] = {id=284,
description="确认花费%d元宝进行自动国战？（自动国战会在切出游戏后停止，重新进入游戏即可恢复。）"
},

[285] = {id=285,
description="获得%d银子"
},

[286] = {id=286,
description="获得%d金币"
},

[287] = {id=287,
description="明日再来，即可在登陆奖励里领取到280元宝。"
},

[288] = {id=288,
description="该活动需要等级达到10级，加油吧！"
},

[289] = {id=289,
description="请先选择一张卡牌"
},

[290] = {id=290,
description="潜能秘药不足"
},

[291] = {id=291,
description="卡牌已经没有可用的潜能值"
},

[292] = {id=292,
description="本日次数已耗光，是否花费%d元宝继续挑战？"
},

[293] = {id=293,
description="请先完成前面的章节!"
},

[294] = {id=294,
description="需要元宝："
},

[295] = {id=295,
description="需要银子："
},

[296] = {id=296,
description="当前拥有：%d"
},

[297] = {id=297,
description="您拥有的招募令不足"
},

[298] = {id=298,
description="消耗招募令X%d"
},

[299] = {id=299,
description="忍魂"
},

[300] = {id=300,
description="忍者攻击提升：%.2f%%"
},

[301] = {id=301,
description="忍者防御提升：%.2f%%"
},

[302] = {id=302,
description="忍者查克拉提升：%.2f%%"
},

[303] = {id=303,
description="忍者技能激发概率提升：%.2f%%"
},

[304] = {id=304,
description="首次充值或者30级可以跳过战斗"
},

[305] = {id=305,
description="碎片"
},

[306] = {id=306,
description="满级以后才可以转生"
},

[307] = {id=307,
description="确定花费%d元宝立即合成？"
},

[308] = {id=308,
description="请先获取印记。"
},

[309] = {id=309,
description="印记碎片没集齐。"
},

[310] = {id=310,
description="每五连粹一次 ，都将消耗%d元宝 。(自动淬炼也将以此种方式扣除元宝）"
},

[311] = {id=311,
description="元宝"
},

[312] = {id=312,
description="目前没有材料卡：%s"
},

[313] = {id=313,
description="目前没有材料卡：一转%s"
},

[314] = {id=314,
description="目前没有材料卡：二转%s"
},

[315] = {id=315,
description="请先升级至VIP1或以上才能购买成长基金"
},

[316] = {id=316,
description="使用成功"
},

[317] = {id=317,
description="%d级开启该功能"
},

[318] = {id=318,
description="成功激活"
},

[319] = {id=319,
description="激活失败"
},
[320] = {id = 320,
description = "不能加自己为好友"
},

}
