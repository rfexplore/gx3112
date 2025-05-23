CMD:set_tp
 example:"set_tp fre=1150000 sym=27500000 HV=1 22K=0 PORT=1\r\n"
 note:HV
　　　　　	0水平
　　　　　　	1垂直 
　　　　　　22K
	0关1开．
　　　　　　port
        0不开．
　　　　　　　　1-4 :portA-->portD



CMD:get_tp_strength
 example:"get_sat_list\r\n"

CMD:get_tp_quality
 example:"get_tp_quality\r\n"

CMD:get_tp_info 循环获取信号强度和质量．
 example:"get_tp_info\r\n"

CMD:stop_get_tp_info 停止循环获取信号强度和质量．
 example:"stop_get_tp_info\r\n"


CMD:get_sat_list
 example:"get_sat_list\r\n"

CMD:set_sat_params 
 example:"set_sat_params SATID=84 LNB1=5150 LNB2=0 HV=1 22K=0 PORT=1\r\n"
 note:HV
　　　　　　	0关闭
　　　　　　	1自动
　　　　　	　2水平
　　　　　　	3垂直 
　　　　　　22k
	0关1开．
　　　　　　port
        0不开．
　　　　　　　　1-4 :portA-->portD

CMD:blind_search_start 
   exapmle:"blind_search_start SATID=84\r\n"


CMD:search_get_process
   exapmle:"search_get_process\r\n"

CMD:get_search_program_list
   exapmle:"get_search_program_list SATID=84\r\n"

CMD:get_all_program_list
　　　exapmle:"get_all_program_list\r\n"

CMD:play_program_id
   exapmle:"play_program_id PROGID=994\r\n"


CMD:get_program_epg
   example:"get_program_epg PROGID=994\r\n"

CMD:set_lnb_params
   example:"HV=1 22K=0 PORT=1\r\n"
   

CMD:set_agc_step  默认为４
   example:"set_agc_step STEP=4"

CMD:get_agc_value 
 example:"get_agc_value\r\n"
CMD :channel //通道切换
     example:"channel SATID=12\r\n"
CMD :sw i//切换视频通道
	 example："sw i01\r\n"
CMD :sw on//使能视频输出
	example："sw on\r\n"
CMD :sw off//关闭视频输出
	example："sw off\r\n"
附录:
nt parse_cmd(char * p[],int num)
{
	int i;
	char *base;
	char *params;
	int pccmdtotal=sizeof(pccmd)/sizeof(char *);
	U8 buf[100];
	U8 buf2[100];
	U8 buf3[100];
	int satid;
	for(i=0;i<pccmdtotal;i++)
	{
		//Printf("p");
		if(0==strcmp(p[0],pccmd[i]))
		{
			break;
		}
	}

	if(i==pccmdtotal)//get head failure,Error
		return -1;

	switch(i)
	{
		case CMD_SET_TYPE:

			//Printf("set type!!\n");
			base=p[1];
			params=(char *)strsep(&base,"=");
			if(params==NULL) return -1;
			if(0==strcmp(params,"type"))
			{
				params=strsep(&base,"=");
			    if(params==NULL) return -1;
				//Printf("2222\n",base);
					if(0==strcmp(params,"dvbs"))
					{
						const S8 *p="code:0 message:reply set_demod_type succussful\r\n";
						//Printf("3333");
						set_tp_dvbs();
						//Printf("send data is %s\n",p);
						GXCOM_UartSendData((U8*)p,strlen(p));
					}
					else
					{
						const S8 *p="code:1 message:reply set_demod_type failed:the demod type is wrong or not register!\r\n";
						GXCOM_UartSendData((U8*)p,strlen(p));
					}
			}
			else 
				return -1;
			break;

		case CMD_SET_TP:
			{
				int freq;//11500000
				int sym;//27500000
				int pol;
				int p22k;
				int disqc;
				base=p[1];	
				//Printf("set tp!!\n");
				
				params=strsep(&base,"=");
				if(params==NULL) return -1;
				//Printf("set tp1!!%s\n",params);
				
				if(0==strcmp(params,"fre"))
				{
						params=strsep(&base,"=");
						if(params==NULL) return -1;
						freq=atoi(params);
						//Printf("set tp2!!%s\n",params);
				
				}
				else
				{

					return -1;
				}
								

				
				base=p[2];
				params=strsep(&base,"=");
				//Printf("set tp3!!%s\n",params);
				if(params==NULL) return -1;
				if(0==strcmp(params,"sym"))
				{
						params=strsep(&base,"=");
					//	Printf("set tp4!!%s\n",params);
						if(params==NULL) return -1;
						sym=atoi(params);	
						
				}
				else
				{

					return -1;
				}


				base=p[3];
				params=strsep(&base,"=");
				//Printf("set tp5!!%s\n",params);
				if(params==NULL) return -1;
				if(0==strcmp(params,"HV"))
				{
						params=strsep(&base,"=");
						//Printf("set tp6!!%s\n",params);
						if(params==NULL) return -1;
						pol=atoi(params);		
				}
				else
				{

					return -1;
				}

				base=p[4];
				params=strsep(&base,"=");
				//Printf("set tp7!!%s\n",params);
				if(params==NULL) return -1;
				if(0==strcmp(params,"22K"))
				{
						params=strsep(&base,"=");
						//Printf("set tp8!!%s\n",params);
						if(params==NULL) return -1;
						p22k=atoi(params);		
				}
				else
				{

					return -1;
				}


				base=p[5];
				params=strsep(&base,"=");
				//Printf("set tp9!!%s\n",params);
				if(params==NULL) return -1;
				if(0==strcmp(params,"PORT"))
				{
						params=strsep(&base,"=");
						//Printf("set tp10!!%s\n",params);
						if(params==NULL) return -1;
						disqc=atoi(params);		
				}
				else
				{

					return -1;
				}	
				
				set_tp_params(freq/1000,sym/1000,pol,p22k,disqc);

				GxCore_ThreadDelay(100);

				if(get_lock_status())
				{
					const S8 *p="code:0 data(1) message:reply get_tp_status successfull:locked\r\n";
					GXCOM_UartSendData((U8*)p,strlen(p));
				}
				else
				{
					const S8 *p="code:0 data(0) message:reply get_tp_status successfull:unlocked\r\n";
					GXCOM_UartSendData((U8*)p,strlen(p));	
				}
			}
			break;

		case CMD_GET_TPSTATE:
			{
				int state_r;
				//Printf("get  tp state!!\n");
				if(1==check_param_lock(&state_r))
				{
					sprintf((char*)buf,"code:0 message:reply clear_tp_lock successfull: %x\r\n",state_r);
					GXCOM_UartSendData(buf,strlen((char*)buf));
				}
				else
				{
					sprintf((char*)buf,"code:1 message:reply clear_tp_lock failure\r\n");
					GXCOM_UartSendData(buf,strlen((char*)buf));
				}
				
			}
			
			break;
		case CMD_GET_STRENGTH:
			{
				int strength;
				//Printf("Get strength!!\n");
				strength=get_tp_strength();
				sprintf((char*)buf,"code:0 message:reply get successfull: %d\r\n",strength);
				GXCOM_UartSendData(buf,strlen((char*)buf));
			}
			break;

		case CMD_GET_QUALITY:
			{
				int quality;
				quality=get_tp_quality();
				//Printf("Get quality!!\n");
				sprintf((char*)buf,"code:0 message:reply get successfull: %d\r\n",quality);
				GXCOM_UartSendData(buf,strlen((char*)buf));
			}
			break;

		case CMD_GET_SNR:
			{
				int snr;
				
				//Printf("Get Snr!!\n");
				snr=get_tp_snr();
				sprintf((char*)buf,"code:0 message:reply get successfull: %1x%02x\r\n",snr/100,snr%100);
				GXCOM_UartSendData(buf,strlen((char*)buf));
			}

			break;

		case CMD_GET_INFO:
			//Printf("Get INFO!!\n");	
			start_send_tpinfo();
			break;

		case CMD_STOP:
			//Printf("Get Stop!!\n");
			stop_send_tpinfo();
			break;

		case CMD_GET_SAT_LIST:
			{
				int num;
				int i;
				
				num=dvbs_get_sat_params(satlist,256);
				sprintf((char*)buf,"code:0 message:reply get satlist (%d sats) successfull\r\n",num);
				GXCOM_UartSendData(buf,strlen((char*)buf));
				sprintf((char*)buf,"SATID\t    SAT NAME \t\t   LNB1\t LNB2\tPORT\tHV\t22K\t \r\n");
				GXCOM_UartSendData(buf,strlen((char*)buf));
				for(i=0;i<num;i++)
				{				
					sprintf((char*)buf,"%02d\t %-20s\t   %d\t %d\t  %d\t  %d\t  %d\t \r\n",satlist[i].id,satlist[i].sat_s.sat_name,satlist[i].sat_s.lnb1,
						satlist[i].sat_s.lnb2,satlist[i].sat_s.diseqc10,satlist[i].sat_s.lnb_power,satlist[i].sat_s.switch_22K);
					GXCOM_UartSendData(buf,strlen((char*)buf));
				}
			}
			break;

		case CMD_SET_SAT:

			base=p[1];	
			int id;	
			int k22;
			int hv;
			int port;
			int lnb1;
			int lnb2;
			params=strsep(&base,"=");
			if(params==NULL) return -1;
				//Printf("set tp1!!%s\n",params);
				
			if(0==strcmp(params,"SATID"))
			{
						params=strsep(&base,"=");
						if(params==NULL) return -1;
						id=atoi(params);
						//Printf("set tp2!!%s\n",params);
				
			}
			else
			{
				return -1;
			}
								

				
			base=p[2];
			params=strsep(&base,"=");
				//Printf("set tp3!!%s\n",params);
			if(params==NULL) return -1;
			if(0==strcmp(params,"LNB1"))
			{
				params=strsep(&base,"=");
					//	Printf("set tp4!!%s\n",params);
				if(params==NULL) return -1;
					lnb1=atoi(params);	
						
			}
			else
			{
				return -1;
			}


			base=p[3];
			params=strsep(&base,"=");
				//Printf("set tp5!!%s\n",params);
			if(params==NULL) return -1;
			if(0==strcmp(params,"LNB2"))
			{
				params=strsep(&base,"=");
						//Printf("set tp6!!%s\n",params);
						if(params==NULL) return -1;
				lnb2=atoi(params);		
			}
			else
			{
				return -1;
			}

			


			base=p[4];
			params=strsep(&base,"=");
				//Printf("set tp9!!%s\n",params);
			if(params==NULL) return -1;
			if(0==strcmp(params,"HV"))
			{
					params=strsep(&base,"=");
						//Printf("set tp10!!%s\n",params);
					if(params==NULL) return -1;
					hv=atoi(params);		
			}
			else
			{
					return -1;
			}	

			base=p[5];
			params=strsep(&base,"=");
				//Printf("set tp9!!%s\n",params);
			if(params==NULL) return -1;
			if(0==strcmp(params,"22K"))
			{
					params=strsep(&base,"=");
						//Printf("set tp10!!%s\n",params);
					if(params==NULL) return -1;
					k22=atoi(params);		
			}
			else
			{
					return -1;
			}	

			base=p[6];
			params=strsep(&base,"=");
				//Printf("set tp7!!%s\n",params);
			if(params==NULL) return -1;
			if(0==strcmp(params,"PORT"))
			{
				params=strsep(&base,"=");
						//Printf("set tp8!!%s\n",params);
				if(params==NULL) return -1;
					port=atoi(params);		
			}
			else
			{
				return -1;
			}
			modify_sat_params(id,lnb1,lnb2,hv,port,k22);
			sprintf((char*)buf,"code:0 message:reply set sat params successfull\r\n");	
			GXCOM_UartSendData(buf,strlen((char*)buf));
			break;


		case CMD_BLIND_START:
			

			base=p[1];	
			

			params=strsep(&base,"=");

			if(0==strcmp(params,"SATID"))
			{
						params=strsep(&base,"=");
						if(params==NULL) return -1;
						satid=atoi(params);
						//Printf("set tp2!!%s\n",params);
				
			}
			else
			{
				return -1;
			}
			set_search_process(0);
			app_satellite_blind_start(satid);
			
			break;

		case CMD_SERACH_GET_PROCESS:
			sprintf((char*)buf,"code:0 message:reply get search process %%%d successfull\r\n",search_process);
			GXCOM_UartSendData(buf,strlen((char*)buf));

			if(search_process==100)
				GXCOM_UartSendData("process over\r\n",strlen("process over\r\n"));

			
				
			break;

		case CMD_GET_SEARCH_PROGRAM_LIST:
			base=p[1];	
			int num;
			int i;

			params=strsep(&base,"=");

			if(0==strcmp(params,"SATID"))
			{
						params=strsep(&base,"=");
						if(params==NULL) return -1;
						satid=atoi(params);
						//Printf("set tp2!!%s\n",params);
				
			}
			else
			{
				return -1;
			}
			
			num=dvbs_get_program_params(satid,program_list,5000,GXBUS_PM_PROG_ALL);
			sprintf((char*)buf,"code:0 message:reply get search  (%d programs) successfull\r\n",num);
			GXCOM_UartSendData(buf,strlen((char*)buf));
			sprintf((char*)buf,"PROGID\t PROG NAME \t\t  VPID\t APID\t  HV\t FTA\t FREQ\t SYM\t \r\n");
			GXCOM_UartSendData(buf,strlen((char*)buf));
			for(i=0;i<num;i++)
			{				
				GxBusPmDataTP tp;
				int string_num;

				dvbs_get_tp_params(program_list[i].tp_id,&tp);

				string_num=20+strlen(program_list[i].prog_name)-get_utf8_string_num( program_list[i].prog_name);

				sprintf(buf2,"%%%d-s",string_num);
				sprintf(buf3,buf2,program_list[i].prog_name);
					
				sprintf((char*)buf,"%02d\t %s\t   %d\t %d\t  %d\t  %d\t  %d  %d \t \r\n",
					    program_list[i].id,
					    buf3,
					    program_list[i].video_pid,
						program_list[i].cur_audio_pid,
						tp.tp_s.polar,
						program_list[i].scramble_flag,
						tp.frequency,
						tp.tp_s.symbol_rate);
				GXCOM_UartSendData(buf,strlen((char*)buf));
						}
			break;

		case CMD_GET_ALL_PROGRAM_LIST:
			
				{
					int num;
						int i;
			
					
						
						num=dvbs_get_program_params(-1,program_list,5000,GXBUS_PM_PROG_ALL);
						sprintf((char*)buf,"code:0 message:reply get all program  (%d programs) successfull\r\n",num);
						GXCOM_UartSendData(buf,strlen((char*)buf));
						sprintf((char*)buf,"PROGID\t PROG NAME \t\t  VPID\t APID\t  HV\t FTA\t FREQ\t SYM\t \r\n");
			GXCOM_UartSendData(buf,strlen((char*)buf));
			for(i=0;i<num;i++)
			{				
				GxBusPmDataTP tp;
				int string_num;

				dvbs_get_tp_params(program_list[i].tp_id,&tp);

				string_num=20+strlen(program_list[i].prog_name)-get_utf8_string_num( program_list[i].prog_name);

				sprintf(buf2,"%%%d-s",string_num);
				sprintf(buf3,buf2,program_list[i].prog_name);
					
				sprintf((char*)buf,"%02d\t %s\t   %d\t %d\t  %d\t  %d\t  %d  %d \t \r\n",
					    program_list[i].id,
					    buf3,
					    program_list[i].video_pid,
						program_list[i].cur_audio_pid,
						tp.tp_s.polar,
						program_list[i].scramble_flag,
						tp.frequency,
						tp.tp_s.symbol_rate);
				GXCOM_UartSendData(buf,strlen((char*)buf));
						}

			}								
			break;

#include "app_epg.h"

		case CMD_GET_PROGRAM_EPG:
			{

				#if 1
				base=p[1];	
				int num;
				int i;
				int prog_id;
				params=strsep(&base,"=");
				if(0==strcmp(params,"PROGID"))
				{
						params=strsep(&base,"=");
						if(params==NULL) return -1;
						prog_id=atoi(params);
										//Printf("set tp2!!%s\n",params);
				}
				else
				{
						return -1;
				}

				
				GxEpgInfo cur_event_info;
				EpgIndex  epg_index;
		   		epg_index.day = 0;
			
				AppEpgOps sp_EpgOps;
            	
				GxEpgInfo *s_cur_event_info=malloc(5000);
				GxEpgInfo *s_nxt_event_info=malloc(5000);
				GxEpgInfo  *s_event_info=malloc(5000);

				time_t cur_time = 0;
				
				EpgProgInfo s_epg_prog_info;
				GxMsgProperty_NodeByIdGet prog_node = {0};
				
				
				prog_node.node_type = NODE_PROG;
				prog_node.id = prog_id;
				epg_ops_init(&sp_EpgOps);
				

				if(GXCORE_SUCCESS == app_send_msg_exec(GXMSG_PM_NODE_BY_ID_GET, (void *)(&prog_node)))
				{
					app_epg_prog_info_set(&s_epg_prog_info, &prog_node);
					sp_EpgOps.prog_info_update(&sp_EpgOps, &s_epg_prog_info);
					sp_EpgOps.event_cnt_update(&sp_EpgOps);
					sprintf((char*)buf,"code:0 message:reply get program id  %d %s events successfull\r\n",prog_id,prog_node.prog_data.prog_name);
					GXCOM_UartSendData(buf,strlen((char*)buf));

					sprintf((char*)buf,"EVENT\t\t\t\t\t   DURATION TIME\t\r\n");
					GXCOM_UartSendData(buf,strlen((char*)buf));
				}
				else
				{
					sprintf((char*)buf,"code:2 message:progid error\r\n",prog_id);
					GXCOM_UartSendData(buf,strlen((char*)buf));
					break;
					
				}
				epg_index.day=0;
				if(sp_EpgOps.get_cur_event_cnt(&sp_EpgOps) > 0)
				{
					epg_index.sel = 0;
					sp_EpgOps.get_day_event_info(&sp_EpgOps, &epg_index, s_cur_event_info);
					{
            			time_t time = 0;
						*s_event_info=*s_cur_event_info;
						char *time_str = NULL;
						char *data_str = NULL;
						char epg_time_str[100]={0,};
            			cur_time=time = get_display_time_by_timezone((*s_event_info).start_time);	
						time_str = app_time_to_hourmin_edit_str(time);
						data_str = app_time_to_date_edit_str(time);
						if(time_str != NULL)
						{
							strcpy((char*)epg_time_str, data_str);
							strcat((char*)epg_time_str, " ");
							strcat((char*)epg_time_str, time_str);
							GxCore_Free(time_str);
							GxCore_Free(data_str);
							time_str = NULL;
						}
						strcat((char*)epg_time_str, " - ");
						time = get_display_time_by_timezone((*s_event_info).start_time+(*s_event_info).duration);	
						time_str = app_time_to_hourmin_edit_str(time);
						data_str = app_time_to_date_edit_str(time);
						
						if(time_str != NULL)
						{
							strcat((char*)epg_time_str, data_str);
							strcat((char*)epg_time_str, " ");
							strcat((char*)epg_time_str, time_str);
							GxCore_Free(time_str);
							GxCore_Free(data_str);
							time_str = NULL;
						}
						strcat((char*)epg_time_str, " ");
						sprintf((char*)buf,"%-50s\t %s\t\r\n",(*s_event_info).event_name,epg_time_str);
						//GXCOM_UartSendData(buf,strlen((char*)buf));
					}
					epg_index.sel = 1;
					sp_EpgOps.get_day_event_info(&sp_EpgOps, &epg_index, s_nxt_event_info);
					{
            			time_t time = 0;
						*s_event_info=*s_nxt_event_info;
						char *time_str = NULL;
						char *data_str = NULL;
						char epg_time_str[100]={0,};
            			time = get_display_time_by_timezone((*s_event_info).start_time);	
					
						time_str = app_time_to_hourmin_edit_str(time);
						data_str = app_time_to_date_edit_str(time);
						if(time_str != NULL)
						{
							strcpy((char*)epg_time_str, data_str);
							strcat((char*)epg_time_str, " ");
							strcat((char*)epg_time_str, time_str);
							GxCore_Free(time_str);
							GxCore_Free(data_str);
							time_str = NULL;
						}
						strcat((char*)epg_time_str, " - ");
						time = get_display_time_by_timezone((*s_event_info).start_time+(*s_event_info).duration);	
						time_str = app_time_to_hourmin_edit_str(time);
						data_str = app_time_to_date_edit_str(time);
						
						if(time_str != NULL)
						{
							strcat((char*)epg_time_str, data_str);
							strcat((char*)epg_time_str, " ");
							strcat((char*)epg_time_str, time_str);
							GxCore_Free(time_str);
							GxCore_Free(data_str);
							time_str = NULL;
						}
						sprintf((char*)buf,"%-50s\t %s\t\r\n",(*s_event_info).event_name,epg_time_str);
						//GXCOM_UartSendData(buf,strlen((char*)buf));
					}
					
				}
				else //no pf no event.
				{


				}
				
				
				num=sp_EpgOps.get_day_event_cnt(&sp_EpgOps,epg_index.day);
				int day;
				for(day=0;day<7;day++)
				for(i=0;i<num;i++)
            	{
            		epg_index.sel=day;
            		epg_index.sel=i+2;
            		if(sp_EpgOps.get_day_event_info(&sp_EpgOps, &epg_index, s_event_info) == GXCORE_SUCCESS)
            		{
            			time_t time = 0;
						char *time_str = NULL;
						char *data_str = NULL;
						char epg_time_str[100]={0,};
            			time = get_display_time_by_timezone((*s_event_info).start_time);

						if(time<cur_time) //before event don't show
							continue;
						time_str = app_time_to_hourmin_edit_str(time);
						data_str = app_time_to_date_edit_str(time);
						if(time_str != NULL)
						{
							strcpy((char*)epg_time_str, data_str);
							strcat((char*)epg_time_str, " ");
							strcat((char*)epg_time_str, time_str);
							GxCore_Free(time_str);
							GxCore_Free(data_str);
							time_str = NULL;
						}
						strcat((char*)epg_time_str, " - ");
						time = get_display_time_by_timezone((*s_event_info).start_time+(*s_event_info).duration);	
						time_str = app_time_to_hourmin_edit_str(time);
						data_str = app_time_to_date_edit_str(time);
						if(time_str != NULL)
						{
							strcat((char*)epg_time_str, data_str);
							strcat((char*)epg_time_str, " ");
							strcat((char*)epg_time_str, time_str);
							GxCore_Free(time_str);
							GxCore_Free(data_str);
							time_str = NULL;
						}
						
						sprintf((char*)buf,"%-50s\t %s\t\r\n",(*s_event_info).event_name,epg_time_str);
						GXCOM_UartSendData(buf,strlen((char*)buf));
						
					}
				}
				free(s_event_info);
				free(s_cur_event_info);
				free(s_nxt_event_info);
				#endif

			}
			break;
		case CMD_GET_AGC_VALUE:
			{
				int i;
				int pol;
				int k22;
				int disqc;
				GxFrontendDiseqc params;
				params.diseqc.type=DiSEQC10;
				GxFrontendGetDis(0,&params);
				pol=params.diseqc.u.params_10.bPolar;
				k22=params.diseqc.u.params_10.b22k;
				disqc=params.diseqc.u.params_10.chDiseqc;
				if(k22==SEC_TONE_ON)//
    			{
					k22=MM_TUNER_22K_ON;
   				}
    			else
   				{
        			k22=MM_TUNER_22K_OFF;
    			}
    			if(pol==SEC_VOLTAGE_18 )
    			{
        			pol=MM_TUNER_POLAR_H;
   				}
   				else if(pol==SEC_VOLTAGE_13)
    			{
        			pol=MM_TUNER_POLAR_V;
    			}
				else
				{
					pol=MM_TUNER_POLAR_OFF;
   				}

				if(disqc==0 )
				{
					disqc=MM_TUNER_DISEQC_OFF;
				}
				AppFrontend_Monitor monitor = FRONTEND_MONITOR_OFF;
				app_ioctl(0, FRONTEND_MONITOR_SET, &monitor);
				sprintf((char*)buf,"code:0 message:reply agc successfull,step %dMhz\r\n",freq_step);
				GXCOM_UartSendData(buf,strlen((char*)buf));	
				set_agc_mode(1);
				for(i=0;i<1200/freq_step+1;i++)
				{
					int onetick;
					int twotick;
					int threetick;
					onetick=dvbfinder_get_tick();
					//set_tp_params(950+freq_step*i,freq_step*1000,pol,k22,disqc);
					Tuner_SetTp(950+freq_step*i,freq_step*1000,pol,k22);
					twotick=dvbfinder_get_tick();	
					agc[i]=GX113x_RDA5815M_Get_Signal_PWR();
					threetick=dvbfinder_get_tick();	
					
					if(i%16==15)
						sprintf((char*)buf,"%02d.%02d\r\n",agc[i]/10,agc[i]%10);
					else
						sprintf((char*)buf,"%02d.%02d\t",agc[i]/10,agc[i]%10);
					GXCOM_UartSendData(buf,strlen((char*)buf));		
				}
				if(i%16!=0)
				{
					GXCOM_UartSendData("\r\n",2);		
				}
				set_agc_mode(0);
			}
			break;

		case CMD_SET_LNB:
			{
				base=p[1];
				params=strsep(&base,"=");
				//Printf("set tp7!!%s\n",params);
				if(params==NULL) return -1;
				if(0==strcmp(params,"HV"))
				{
					params=strsep(&base,"=");
						//Printf("set tp8!!%s\n",params);
					if(params==NULL) return -1;
						hv =atoi(params);		
				}
				else
				{
					return -1;
				}


				base=p[2];
				params=strsep(&base,"=");
				//Printf("set tp9!!%s\n",params);
				if(params==NULL) return -1;
				if(0==strcmp(params,"22K"))
				{
					params=strsep(&base,"=");
						//Printf("set tp10!!%s\n",params);
					if(params==NULL) return -1;
					k22=atoi(params);		
				}
				else
				{
					return -1;
				}	

				base=p[3];
				params=strsep(&base,"=");
				//Printf("set tp9!!%s\n",params);
				if(params==NULL) return -1;
					if(0==strcmp(params,"PORT"))
				{
					params=strsep(&base,"=");
						//Printf("set tp10!!%s\n",params);
					if(params==NULL) return -1;
					port=atoi(params);		
				}
				else
				{
					return -1;
				}	
				set_diseqc_params( hv, k22, port);
			}
			break;


		case CMD_SET_AGC_STEP:

			base=p[1];
			params=strsep(&base,"=");
				//Printf("set tp9!!%s\n",params);
			if(params==NULL) return -1;
			if(0==strcmp(params,"STEP"))
			{
				params=strsep(&base,"=");
						//Printf("set tp10!!%s\n",params);
				if(params==NULL) return -1;
					freq_step=atoi(params);		
			}
			else
			{
					return -1;
			}	
	
			break;

		case CMD_PLAY_PROG_ID:
			base=p[1];
			int progid;
			int pos;
			params=strsep(&base,"=");
				//Printf("set tp9!!%s\n",params);
			if(params==NULL) return -1;
			if(0==strcmp(params,"PROGID"))
			{
				params=strsep(&base,"=");
						//Printf("set tp10!!%s\n",params);
				if(params==NULL) return -1;
					progid=atoi(params);		
			}
			else
			{
					return -1;
			}	

			
			pos=dvbs_get_program_pos(progid);
			if(pos!=-1)
			{
				g_AppPlayOps.program_play(PLAY_TYPE_NORMAL|PLAY_MODE_POINT, pos);
				sprintf((char*)buf,"code:0 message:reply play id %d successfull\r\n",progid);
				GXCOM_UartSendData(buf,strlen((char*)buf));	
			}
			else
			{
				sprintf((char*)buf,"code:2 message:reply play failure,no this id %d\r\n",progid);
				GXCOM_UartSendData(buf,strlen((char*)buf));	

			}
			break;

		case CMD_GET_HELP:


			break;

		default:
			return -1;
	}
	return 0;
}





