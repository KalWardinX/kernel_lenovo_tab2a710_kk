#ifdef MTK_INTERNAL_MHL_SUPPORT
#ifndef _INTER_MHL_DRV_H_
#define _INTER_MHL_DRV_H_

#include "mhl_struct.h"

#include "mhl_table.h"
#include "mhl_edid.h"
#include "mhl_ctrl.h"

//5ms
#define MHL_LINK_TIME	(10) 
#define MHL_POWER_OFF_TIME	(200) 

typedef enum
{
 HDMI_CEC_CMD=0,
 HDMI_PLUG_DETECT_CMD,
 HDMI_HDCP_PROTOCAL_CMD,	
 HDMI_DISABLE_HDMI_TASK_CMD,
 MAX_HDMI_TMR_NUMBER
	
}HDMI_TASK_COMMAND_TYPE_T;



#ifndef ARY_SIZE
#define ARY_SIZE(x) (sizeof((x)) / sizeof((x[0])))
#endif

typedef enum
{
    HDMI_POLARITY_RISING  = 0,
    HDMI_POLARITY_FALLING = 1
}HDMI_POLARITY;

typedef enum
{
    HDMI_CLOCK_PHASE_0  = 0,
    HDMI_CLOCK_PHASE_90 = 1
}HDMI_CLOCK_PHASE;

typedef enum
{
    HDMI_COLOR_ORDER_RGB = 0,
    HDMI_COLOR_ORDER_BGR = 1
}HDMI_COLOR_ORDER;

typedef enum
{    
   IO_DRIVING_CURRENT_8MA       = (1 << 0),
   IO_DRIVING_CURRENT_4MA       = (1 << 1),
   IO_DRIVING_CURRENT_2MA       = (1 << 2),
   IO_DRIVING_CURRENT_SLEW_CNTL = (1 << 3),
}IO_DRIVING_CURRENT;


typedef enum
{
    HDMI_VIN_FORMAT_RGB565,
    HDMI_VIN_FORMAT_RGB666,
    HDMI_VIN_FORMAT_RGB888,
} HDMI_VIDEO_INPUT_FORMAT;

typedef enum
{
	HDMI_VOUT_FORMAT_RGB888,
	HDMI_VOUT_FORMAT_YUV422,
	HDMI_VOUT_FORMAT_YUV444,
}HDMI_VIDEO_OUTPUT_FORMAT;

typedef enum
{
	HDMI_AUDIO_PCM_16bit_48000,
	HDMI_AUDIO_PCM_16bit_44100,
	HDMI_AUDIO_PCM_16bit_32000,
	HDMI_AUDIO_SOURCE_STREAM,
}HDMI_AUDIO_FORMAT; 

typedef struct
{
	HDMI_VIDEO_RESOLUTION		vformat;
	HDMI_VIDEO_INPUT_FORMAT 	vin;
	HDMI_VIDEO_OUTPUT_FORMAT 	vout;
	HDMI_AUDIO_FORMAT			aformat;
}HDMI_CONFIG;

typedef enum{
    HDMI_OUTPUT_MODE_LCD_MIRROR,
    HDMI_OUTPUT_MODE_VIDEO_MODE,
    HDMI_OUTPUT_MODE_DPI_BYPASS
}HDMI_OUTPUT_MODE;
typedef enum{
    HDMI_CABLE,
    MHL_CABLE,
    MHL_SMB_CABLE,
    MHL_2_CABLE ///MHL 2.0
}HDMI_CABLE_TYPE;

typedef struct
{
	unsigned int width;
	unsigned int height;

	HDMI_CONFIG init_config;

    /* polarity parameters */
    HDMI_POLARITY clk_pol;
    HDMI_POLARITY de_pol;
    HDMI_POLARITY vsync_pol;
    HDMI_POLARITY hsync_pol;

    /* timing parameters */
    unsigned int hsync_pulse_width;
    unsigned int hsync_back_porch;
    unsigned int hsync_front_porch;
    unsigned int vsync_pulse_width;
    unsigned int vsync_back_porch;
    unsigned int vsync_front_porch;
    
    /* output format parameters */
    HDMI_COLOR_ORDER rgb_order;

    /* intermediate buffers parameters */
    unsigned int intermediat_buffer_num; // 2..3

    /* iopad parameters */
    IO_DRIVING_CURRENT io_driving_current;
    HDMI_OUTPUT_MODE    output_mode;

    int is_force_awake;
    int is_force_landscape;

    unsigned int scaling_factor; // determine the scaling of output screen size, valid value 0~10
                                 // 0 means no scaling, 5 means scaling to 95%, 10 means 90%
     bool NeedSwHDCP;
     HDMI_CABLE_TYPE cabletype;     
}HDMI_PARAMS;

typedef struct 
{
	unsigned char	e_hdmi_aud_in;
	unsigned char	e_iec_frame;
	unsigned char	e_hdmi_fs;
	unsigned char	e_aud_code;
	unsigned char	u1Aud_Input_Chan_Cnt;
	unsigned char	e_I2sFmt;
	unsigned char	u1HdmiI2sMclk;
	unsigned char	bhdmi_LCh_status[5];
	unsigned char	bhdmi_RCh_status[5];
}	HDMIDRV_AUDIO_PARA;

typedef enum{
	HDMI_STATE_NO_DEVICE,
	HDMI_STATE_ACTIVE,
	#if defined(MTK_MT8193_HDMI_SUPPORT)
	HDMI_STATE_PLUGIN_ONLY,
	HDMI_STATE_EDID_UPDATE,
	HDMI_STATE_CEC_UPDATE
	#endif
}HDMI_STATE;

// ---------------------------------------------------------------------------

typedef struct
{
    void (*set_reset_pin)(unsigned int value);
    int  (*set_gpio_out)(unsigned int gpio, unsigned int value);
    void (*udelay)(unsigned int us);
    void (*mdelay)(unsigned int ms);
    void (*wait_transfer_done)(void);
	void (*state_callback)(HDMI_STATE state);
}HDMI_UTIL_FUNCS;


typedef struct
{
	void (*set_util_funcs)(const HDMI_UTIL_FUNCS *util);
	void (*get_params)(HDMI_PARAMS *params);

	int (*init)(void);
	int (*enter)(void);
	int (*exit)(void);
	void (*suspend)(void);
	void (*resume)(void);
	int  (*audio_config)(HDMI_AUDIO_FORMAT aformat);
	int  (*video_config)(HDMI_VIDEO_RESOLUTION vformat, HDMI_VIDEO_INPUT_FORMAT vin, HDMI_VIDEO_OUTPUT_FORMAT vou);
	int  (*video_enable)(bool enable);
	int  (*audio_enable)(bool enable);
	int	 (*irq_enable)(bool enable);
	int  (*power_on)(void);
	void (*power_off)(void);
	HDMI_STATE (*get_state)(void);
	void (*set_mode)(unsigned char ucMode);
	void (*dump)(void);
	#if (!defined(MTK_MT8193_HDMI_SUPPORT))&&(!defined(MTK_INTERNAL_HDMI_SUPPORT)&&(!defined(MTK_INTERNAL_MHL_SUPPORT)	))
    void (*read)(unsigned char u8Reg);
    void (*write)(unsigned char u8Reg, unsigned char u8Data);
    void (*log_enable)(bool enable);
#else
#if defined(MTK_INTERNAL_MHL_SUPPORT)	
	void (*read)(unsigned int u4Reg, unsigned int *pdata);
	void (*write)(unsigned int u4Reg, unsigned int u4Data);
	void (*log_enable)(unsigned short enable);
	void (*InfoframeSetting)(unsigned char i1typemode, unsigned char i1typeselect);
	void (*checkedid)(unsigned char i1noedid);
	void (*enablehdcp)(unsigned char u1hdcponoff);
	void (*hdmistatus)(void);
	void (*hdcpkey)(unsigned char *pbhdcpkey);
	void (*getedid)(HDMI_EDID_INFO_T *pv_get_info);
	void (*mhl_cmd)(unsigned int u4Cmd,unsigned int u4Para,unsigned int u4Para1,unsigned int u4Para2);

	void (*dump6397)(void);
	void (*write6397)(unsigned int u4Addr,unsigned int u4Data);	
	void (*read6397)(unsigned int u4Addr, unsigned int *pdata);
	void (*cbusstatus)(void);
	void (*getdcapdata)(unsigned char *pdacp);
	void (*get3dinfo)(MHL_3D_INFO_T *pdacp);
	void (*colordeep)(unsigned char u1colorspace);
#else
	void (*read)(u16 u2Reg, u32 *p4Data);
    void (*write)(u16 u2Reg, u32 u4Data);
    void (*log_enable)(u16 enable);
	void (*InfoframeSetting)(u8 i1typemode, u8 i1typeselect);
	void (*checkedid)(u8 i1noedid);
	void (*colordeep)(u8 u1colorspace, u8 u1deepcolor);
	void (*enablehdcp)(u8 u1hdcponoff);
	void (*setcecrxmode)(u8 u1cecrxmode);
	void (*hdmistatus)(void);
	void (*hdcpkey)(u8 *pbhdcpkey);
	void (*getedid)(HDMI_EDID_INFO_T *pv_get_info);
	void (*setcecla)(CEC_DRV_ADDR_CFG_T* prAddr);
	void (*sendsltdata)(u8 *pu1Data);
	void (*getceccmd)(CEC_FRAME_DESCRIPTION* frame);
	void (*getsltdata)(CEC_SLT_DATA* rCecSltData);
	void (*setceccmd)(CEC_SEND_MSG_T* msg);
	void (*cecenable)(u8 u1EnCec);
	void (*getcecaddr)(CEC_ADDRESS *cecaddr);
#endif
	#endif
	#if defined(MTK_INTERNAL_HDMI_SUPPORT) ||defined(MTK_INTERNAL_MHL_SUPPORT)
	int (*audiosetting)(HDMIDRV_AUDIO_PARA* audio_para);
	#endif
	void (*mutehdmi)(unsigned char u1flagvideomute, unsigned char u1flagaudiomute);
	int (*tmdsonoff)(unsigned char u1ionoff);
	void (*svpmutehdmi)(unsigned char u1svpvideomute, unsigned char u1svpaudiomute);
} HDMI_DRIVER;


// ---------------------------------------------------------------------------
//  HDMI Driver Functions
// ---------------------------------------------------------------------------

const HDMI_DRIVER* HDMI_GetDriver(void);

#endif // __HDMI_DRV_H__
#endif
