#ifdef CONFIG_VIDEO_RK29
/*---------------- Camera Sensor Macro Define Begin  -------------------------------*/
/*---------------- Camera Sensor Configuration Macro Begin ------------------------*/
#define CONFIG_SENSOR_FALSH_PIN		    INVALID_GPIO
#define CONFIG_SENSOR_TORCH_PIN        	    INVALID_GPIO
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL     RK29_CAM_FLASHACTIVE_L
#define CONFIG_SENSOR_TORCHACTIVE_LEVEL     RK29_CAM_TORCHACTIVE_L

#define CONFIG_SENSOR_0                     RK29_CAM_SENSOR_GC0308 /* front camera sensor 0 */
#define CONFIG_SENSOR_IIC_ADDR_0 	    0x42
#define CONFIG_SENSOR_IIC_ADAPTER_ID_0      3
#define CONFIG_SENSOR_CIF_INDEX_0           0
#define CONFIG_SENSOR_ORIENTATION_0         270
#define CONFIG_SENSOR_POWER_PIN_0           INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_0           INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_0         RK30_PIN1_PB7
#define CONFIG_SENSOR_FALSH_PIN_0           INVALID_GPIO
#define CONFIG_SENSOR_TORCH_PIN_0           INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_0   RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_0   RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_0 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_0   RK29_CAM_FLASHACTIVE_L
#define CONFIG_SENSOR_TORCHACTIVE_LEVEL_0   RK29_CAM_TORCHACTIVE_L
#define CONFIG_SENSOR_DVDD_VALUE_0          VDD_18V
#define FOV_V_0                             60
#define FOV_H_0                             60

#define CONFIG_SENSOR_QCIF_FPS_FIXED_0      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_0   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_0      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_0       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_0       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_0      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_0      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_0      30000
#endif //#ifdef CONFIG_VIDEO_RK29

/*---------------- Camera Sensor Configuration Macro End------------------------*/
#include "../../../drivers/media/video/rk30_camera.c"
/*---------------- Camera Sensor Macro Define End  ----------------------------*/

#define PMEM_CAM_SIZE PMEM_CAM_NECESSARY
/*****************************************************************************************
 * camera  devices
 * author: ddl@rock-chips.com
 *****************************************************************************************/
#ifdef CONFIG_VIDEO_RK29
#define CONFIG_SENSOR_POWER_IOCTL_USR	   1 //define this refer to your board layout
#define CONFIG_SENSOR_RESET_IOCTL_USR	   0
#define CONFIG_SENSOR_POWERDOWN_IOCTL_USR  0
#define CONFIG_SENSOR_FLASH_IOCTL_USR	   0
static void all_rk_cif_power(int on,enum rk29camera_vdd_val vdd_val)
{
    struct regulator *ldo_18,*ldo_28;

	ldo_28 = regulator_get(NULL, "act_ldo8"); //vcc28_cif
	ldo_18 = regulator_get(NULL, "act_ldo3"); //vcc18_cif
	if (ldo_28 == NULL || IS_ERR(ldo_28) || ldo_18 == NULL || IS_ERR(ldo_18)){
            //printk("get cif ldo failed!\n");
		return;
	    }
    if(on == 0){	
		while(regulator_is_enabled(ldo_28)>0)
    		regulator_disable(ldo_28);
    	regulator_put(ldo_28);

		while(regulator_is_enabled(ldo_18)>0)
    		regulator_disable(ldo_18);
    	regulator_put(ldo_18);
		mdelay(150);
		}
	else{
		regulator_set_voltage(ldo_28, 2800000, 2800000);
		regulator_enable(ldo_28);
		regulator_put(ldo_28);

    	if(vdd_val == VDD_18V)
		regulator_set_voltage(ldo_18, 1800000, 1800000);
	else
		regulator_set_voltage(ldo_18, 1500000, 1500000);
                //regulator_set_suspend_voltage(ldo, 1800000);
    	regulator_enable(ldo_18);
    	regulator_put(ldo_18);
		mdelay(150);
	}
}

static void rk_cif_power(struct rk29camera_gpio_res *res,int on,enum rk29camera_vdd_val vdd_val)
{
    struct regulator *ldo_18,*ldo_28;
	int camera_power = res->gpio_power;
	int camera_ioflag = res->gpio_flag;
	int camera_io_init = res->gpio_init;
            //printk("honghaishen_0626 camera power is %d",on);
	ldo_28 = regulator_get(NULL, "act_ldo8"); // vcc28_cif
	ldo_18 = regulator_get(NULL, "act_ldo3"); // vcc18_cif
	if (ldo_28 == NULL || IS_ERR(ldo_28) || ldo_18 == NULL || IS_ERR(ldo_18)){
            //printk("get cif ldo failed!\n");
		return;
	    }
    if(on == 0) {	
		while(regulator_is_enabled(ldo_28)>0)
    		regulator_disable(ldo_28);
    	regulator_put(ldo_28);

		while(regulator_is_enabled(ldo_18)>0)
    		regulator_disable(ldo_18);
    	regulator_put(ldo_18);
    	
		mdelay(150);
	if (camera_power != INVALID_GPIO) {
		  if (camera_io_init & RK29_CAM_POWERACTIVE_MASK) {
			  gpio_set_value(camera_power, (((~camera_ioflag)&RK29_CAM_POWERACTIVE_MASK)>>RK29_CAM_POWERACTIVE_BITPOS));
			}
		}
		}
	else {
		regulator_set_voltage(ldo_28, 2800000, 2800000);
		regulator_enable(ldo_28);
		regulator_put(ldo_28);

    	if(vdd_val == VDD_18V)
		regulator_set_voltage(ldo_18, 1800000, 1800000);
	else
		regulator_set_voltage(ldo_18, 1500000, 1500000);
                //regulator_set_suspend_voltage(ldo, 1800000);
    	regulator_enable(ldo_18);
    	regulator_put(ldo_18);
		mdelay(150);
	if (camera_power != INVALID_GPIO)  {
		  if (camera_io_init & RK29_CAM_POWERACTIVE_MASK) {
			gpio_set_value(camera_power, ((camera_ioflag&RK29_CAM_POWERACTIVE_MASK)>>RK29_CAM_POWERACTIVE_BITPOS));
		  }
}
	}
}

#if CONFIG_SENSOR_POWER_IOCTL_USR
static int sensor_power_usr_cb (struct rk29camera_gpio_res *res,int on,enum rk29camera_vdd_val vdd_val)
{
	rk_cif_power(res,on, vdd_val);
	return 0;
}
#endif

#if CONFIG_SENSOR_FLASH_IOCTL_USR
static int sensor_flash_usr_cb (struct rk29camera_gpio_res *res,int on)
{
	#error "CONFIG_SENSOR_FLASH_IOCTL_USR is 1, sensor_flash_usr_cb function must be writed!!";
}
#endif

static struct rk29camera_platform_ioctl_cb sensor_ioctl_cb = {
	#if CONFIG_SENSOR_POWER_IOCTL_USR
	.sensor_power_cb = sensor_power_usr_cb,
	#else
	.sensor_power_cb = NULL,
	#endif

	#if CONFIG_SENSOR_RESET_IOCTL_USR
	.sensor_reset_cb = sensor_reset_usr_cb,
	#else
	.sensor_reset_cb = NULL,
	#endif

	#if CONFIG_SENSOR_POWERDOWN_IOCTL_USR
	.sensor_powerdown_cb = sensor_powerdown_usr_cb,
	#else
	.sensor_powerdown_cb = NULL,
	#endif

	#if CONFIG_SENSOR_FLASH_IOCTL_USR
	.sensor_flash_cb = sensor_flash_usr_cb,
	#else
	.sensor_flash_cb = NULL,
	#endif
};

#if CONFIG_SENSOR_IIC_ADDR_0
static struct reginfo_t rk_init_data_sensor_reg_0[] =
{
	{0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_0[] =
{
	{0x0000, 0x00,0,0}
};
#endif

static rk_sensor_user_init_data_s rk_init_data_sensor[RK_CAM_NUM] = 
{
    #if CONFIG_SENSOR_IIC_ADDR_0
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_0,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_0,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_0) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_0) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
};

#include "../../../drivers/media/video/rk30_camera.c"

#endif /* CONFIG_VIDEO_RK29 */
