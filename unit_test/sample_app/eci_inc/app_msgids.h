#ifndef _app_msgids_
#define _app_msgids_

#ifdef SIL_SEND_HK_MID	
#undef SIL_SEND_HK_MID	
#endif

#ifdef SIL_TBL_MANAGE_MID	
#undef SIL_TBL_MANAGE_MID
#endif

#define SIL_SEND_HK_MID                      (0x1800)
#define SIL_TBL_MANAGE_MID                   (0x1801)

#define SA_SEND_HK_MID                       SIL_SEND_HK_MID


#define UNITTESTMODEL_CMD_MID                (0x1802)

#define INPUTCMDPKT_B_INPUTCMDPKT_S_MID      UNITTESTMODEL_CMD_MID

#define UNITTESTMODEL_TICK_MID               (0x1803)






#define OUTPUTTLMPKT1_B_OUTPUTTLMPKT1_S_MID  (0x0800)
#define OUTPUTTLMPKT2_B_OUTPUTTLMPKT2_S_MID  (0x0801)
#define OUTPUTTLMPKT3_B_OUTPUTTLMPKT3_S_MID  (0x0802)
#define OUTPUTTLMPKT4_B_OUTPUTTLMPKT4_S_MID  (0x0808)

#define UNITTESTMODEL_HK_MID                 (0x0803)


#define INPUTTLMPKT1_B_INPUTTLMPKT1_S_MID    (0x0805)
#define INPUTTLMPKT2_B_INPUTTLMPKT2_S_MID    (0x0806)

#define UNITTESTMODEL_FDC_MID                (0x0807)

#endif
