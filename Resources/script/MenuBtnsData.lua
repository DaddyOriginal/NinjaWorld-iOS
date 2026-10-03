MeunBtnsData = {
	{icon="def_btn_ninjabook",actionscript="GetMainMenu():ChangeToSub(E_GAMEBOOKLISTVIEW);"},
	{icon="def_msg_btn",actionscript="GetMainMenu():ChangeToSub(E_INFORMATIONVIEW);"},
	{icon="def_btn_chat",actionscript="ShowChatView();"},
	{icon="def_godCard",actionscript="GetMainMenu():ChangeToActivitySubMenu(\"ShowGodCardSyntheticView\");"},
	{icon="def_btn_limitTrain",actionscript="GetMainMenu():ChangeToActivitySubMenu(\"ShowLimitTrainView\");"},
	{icon="def_btn_trainsoul",actionscript="GetMainMenu():ChangeToActivitySubMenu(\"ShowTrainSoulView\");"},
	{icon="cuilian",actionscript="GetMainMenu():ChangeToSub(E_STRENGTHVIEW);"},
	{icon="def_btn_potential",actionscript="GetMainMenu():ChangeToSub(E_POTENTIALVIEW);"},
	{icon="def_btn_rein",actionscript="GetMainMenu():ChangeToSub(E_REINCARNATIONVIEW);"},
	{icon="def_btn_setting",actionscript="local gameSettingView =CGameSettingView:create();GetMainMenu():AddDialog(gameSettingView);"},
	{icon="def_btn_friends",actionscript="GetMainMenu():ChangeToSub(E_FRIENDLISTVIEW);"}
}