

	typedef bool (*sd_update_action_func)( compress_sd_update_para*parag );
	typedef struct sd_update_action_
	{
	  sd_update_action_func action;
	  char *action_name;
	}sd_update_action;

	static sd_update_action compress_action[]=
	{
    {get_factory_mode,"get current mode is factory or not"},
    {exe_pre_app_pkg_func,"exe update.app's pre func"},
    {check_battery,"check device has enough volume"},
    {process_app_from_zip,"process app file in zip pkg when in compress sd update"},
    {write_version,"write version to oeminfo"},
    {formate_userdata,"check there is resize data or not"},
    {clear_lock_status,"clear all lock status"}
	};

	 for( action_func_index = 0 ; action_func_index<sizeof(compress_action)/sizeof(compress_action[0]);action_func_index++)
    {
        if( false == compress_action[action_func_index].action(&cur_sd_compress_update_para))
        {
            USB_UPDATE_DBG("error in:%s\n",compress_action[action_func_index].action_name);
            return ErrorAbort(state, "error execute %s failure,in:%s", name,compress_action[action_func_index].action_name);
        }
    }
