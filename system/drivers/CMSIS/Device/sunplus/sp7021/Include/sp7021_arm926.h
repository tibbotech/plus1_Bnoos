
#ifndef __SP7021_ARM926_H
#define __SP7021_ARM926_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/**
  * @brief Bit position definition inside a 32 bits registers
  */
#define  B(x) \
        ((uint32_t) 1 << x)

typedef enum IRQn
{
 /******  SP7021 Q628 specific Interrupt Numbers ************************************************************************/
   TEGN_INIT_FIELD_START_IRQn  = 0,     /*!< TEGN START                                           			      */
   TEGN_INIT_FIELD_END_IRQn    = 1,     /*!< TEGN END                                    						  */ 
   RESERVED_2 				   = 2, 	/*!< RESERVED interrupt                                                   */
   RESERVED_3         		   = 3, 	/*!< RESERVED interrupt                                                   */
   TEGN_INI1_USER_IRQn         = 4,     /*!< TEGN For USER                              						  */
   TEGN_INI2_USER_IRQn         = 5,     /*!< TEGN For USER         												  */
   RESERVED_6                  = 6,     /*!< RESERVED interrupt                                                   */
   RESERVED_7                  = 7,     /*!< RESERVED interrupt                                                   */
   RESERVED_8                  = 8,     /*!< RESERVED interrupt                                                   */
   RESERVED_9                  = 9,     /*!< RESERVED interrupt                                                   */
   USBC1_OTG_IRQn              = 10,    /*!< USB Controller 1 OTG interrupt                                      */
   USBC0_OTG_IRQn              = 11,    /*!< USB Controller 0 OTG interrupt                                      */
   RESERVED_12                 = 12,    /*!< RESERVED interrupt                                                  */
   USBC0_DEVICE_IRQn           = 13,    /*!< USB Controller 0 Device Interrupt                                   */
   USBC0_EHCI_IRQn             = 14,    /*!< USB Controller 0 EHCI Interrupt                                     */
   USBC0_OHCI_IRQn             = 15,    /*!< USB Controller 0 OHCI Interrupt                                     */
   USBC1_DEVICE_IRQn		   = 16,	/*!< USB Controller 1 Device Interrupt								   */
   USBC1_EHCI_IRQn 			   = 17,	/*!< USB Controller 1 EHCI Interrupt									   */
   USBC1_OHCI_IRQn 			   = 18,	/*!< USB Controller 1 OHCI Interrupt									   */
   RESERVED_19                 = 19,    /*!< RESERVED interrupt                                                  */
   EMCC_IRQn                   = 20,    /*!< eMMC interrupt                                                  */
   SDCARD_IRQn                 = 21,    /*!< SD Card interrupt                                                  */
   SDIO_IRQn                   = 22,    /*!< SDIO interrupt                                                  */
   RESERVED_23                 = 23,    /*!< RESERVED interrupt                                                  */
   RESERVED_24                 = 24,    /*!< RESERVED interrupt                                                  */
   RBUS_IRQn                   = 25,    /*!< RBUS interrupt                                                  */
   NOC_IRQn                    = 26,    /*!< NoC interrupt                                                  */
   RESERVED_27                 = 27,    /*!< RESERVED interrupt                                                  */
   SDPORT_IRQn                 = 28,    /*!< Security for protect address                                                  */
   PI_EXTI0_IRQn               = 29,     /*!< EXTI Line0 from FPGA(PIO) Interrupt                                                 */
   PI_EXTI1_IRQn               = 30,     /*!< EXTI Line1  from FPGA(PIO) Interrupt                                                 */
   RESERVED_31                 = 31,    /*!< RESERVED interrupt                                                  */
   RESERVED_32                 = 32,    /*!< RESERVED interrupt                                                  */
   RESERVED_33                 = 33,    /*!< RESERVED interrupt                                                  */
   RESERVED_34                 = 34,    /*!< RESERVED interrupt                                                  */
   RESERVED_35                 = 35,    /*!< RESERVED interrupt                                                  */
   UPHY0_IRQn                  = 36,    /*!< USB PHY 0 interrupt                                                  */
   UPHY1_IRQn                  = 37,     /*!< USB PHY 0 Interrupt                                                 */
   RESERVED_38                 = 38,    /*!< RESERVED interrupt                                                  */
   PFCNT_SENSOR_IRQn           = 39,    /*!< P-chip Sensor interrupt                                                  */
   RESERVED_40                 = 40,    /*!< RESERVED interrupt                                                  */
   IOP_INT0_IRQn               = 41,    /*!< IOP INT0 interrupt                                                  */
   IOP_INT1_IRQn               = 42,    /*!< IOP INT0 interrupt                                                  */
   RESERVED_43                 = 43,    /*!< RESERVED interrupt                                                  */
   RESERVED_44                 = 44,    /*!< RESERVED interrupt                                                  */
   RESERVED_45                 = 45,    /*!< RESERVED interrupt                                                  */
   REMOVE_MBUS_IRQn            = 46,    /*!< MBUS Remove interrupt                                                  */   
   RESERVED_47                 = 47,    /*!< RESERVED interrupt                                                  */
   RESERVED_48                 = 48,    /*!< RESERVED interrupt                                                  */
   CSI0_INT_FIELD_START_IRQn   = 49,    /*!< MIPI CSI0 FIELD Start interrupt                                                  */
   CSI0_INT_FIELD_END_IRQn     = 50,    /*!< MIPI CSI0 FIELD End interrupt                                                  */
   CSI1_INT_FIELD_START_IRQn   = 51,    /*!< MIPI CSI1 FIELD Start interrupt                                                  */
   CSI1_INT_FIELD_END_IRQn     = 52,    /*!< MIPI CSI1 FIELD End interrupt                                                  */
   UART0_IRQn                  = 53,    /*!< UART0 interrupt                                                  */
   UART1_IRQn                  = 54,    /*!< UART1 interrupt                                                  */
   UART2_IRQn                  = 55,    /*!< UART2 interrupt                                                  */
   UART3_IRQn                  = 56,    /*!< UART3 interrupt                                                  */
   RESERVED_57                 = 57,    /*!< RESERVED interrupt                                                  */
   BCH_IRQ_IRQn                = 58,    /*!< SPI Nand BCH interrupt                                                  */
   LOSD_IRQn                   = 59,    /*!< Audio LOSD interrupt                                                  */
   AUD_FIFO_IRQn               = 60,    /*!< Audio FIFO interrupt                                                  */
   RESERVED_61                 = 61,    /*!< RESERVED interrupt                                                  */
   RESERVED_62                 = 62,    /*!< RESERVED interrupt                                                  */
   RESERVED_63                 = 63,    /*!< RESERVED interrupt                                                  */
   RESERVED_64                 = 64,    /*!< RESERVED interrupt                                                  */
   RESERVED_65                 = 65,    /*!< RESERVED interrupt                                                  */
   L2SW_IRQn                   = 66,    /*!< Ethernet Switch interrupt                                                  */
   SPI_COMBO1_DMA_W_IRQn       = 67,    /*!< SPI Combo1 DMA write interrupt                                                  */
   SPI_SLAVE1_IRQn             = 68,    /*!< SPI Slave1 interrupt                                                  */
   SPI_MASTER1_IRQn            = 69,    /*!< SPI Master1 interrupt                                                  */
   SPI_COMBO2_DMA_W_IRQn       = 70,    /*!< SPI Combo2 DMA write interrupt                                                  */
   SPI_SLAVE2_IRQn             = 71,    /*!< SPI Slave2 interrupt                                                  */
   SPI_MASTER2_IRQn            = 72,    /*!< SPI Master2 interrupt                                                  */
   SPI_COMBO3_DMA_W_IRQn	   = 73,	 /*!< SPI Combo3 DMA write interrupt												  */
   SPI_SLAVE3_IRQn 			   = 74,	 /*!< SPI Slave3 interrupt													*/
   SPI_MASTER3_IRQn			   = 75,	 /*!< SPI Master3 interrupt 												 */
   RESERVED_76                 = 76,    /*!< RESERVED interrupt                                                  */	
   RESERVED_77                 = 77,    /*!< RESERVED interrupt                                                  */	
   RESERVED_78                 = 78,    /*!< RESERVED interrupt                                                  */	
   RESERVED_79                 = 79,    /*!< RESERVED interrupt                                                  */	
   RESERVED_80                 = 80,    /*!< RESERVED interrupt                                                  */	
   RESERVED_81                 = 81,    /*!< RESERVED interrupt                                                  */	
   RESERVED_82                 = 82,    /*!< RESERVED interrupt                                                  */	
   RESERVED_83                 = 83,    /*!< RESERVED interrupt                                                  */	
   RESERVED_84                 = 84,    /*!< RESERVED interrupt                                                  */	
   RESERVED_85                 = 85,    /*!< RESERVED interrupt                                                  */	
   RESERVED_86                 = 86,    /*!< RESERVED interrupt                                                  */	
   RESERVED_87                 = 87,    /*!< RESERVED interrupt                                                  */	
   RESERVED_88                 = 88,    /*!< RESERVED interrupt                                                  */	
   RESERVED_89                 = 89,    /*!< RESERVED interrupt                                                  */	
   RESERVED_90                 = 90,    /*!< RESERVED interrupt                                                  */	
   RESERVED_91                 = 91,    /*!< RESERVED interrupt                                                  */	
   ICM0_IRQn                   = 92,    /*!< Input capture 0 interrupt                                                  */	
   ICM1_IRQn                   = 93,    /*!< Input capture 1 interrupt                                                  */	
   ICM2_IRQn                   = 94,    /*!< Input capture 2 interrupt                                                  */	
   ICM3_IRQn                   = 95,    /*!< Input capture 3 interrupt                                                  */	
   RESERVED_96                 = 96,    /*!< RESERVED interrupt                                                  */	
   RESERVED_97                 = 97,    /*!< RESERVED interrupt                                                  */	
   MSI_UART_DMA0_IRQn          = 98,    /*!< MSI UART DMA0 interrupt                                                  */	
   MSI_UART_DMA1_IRQn          = 99,    /*!< MSI UART DMA1 interrupt                                                  */	
   RESERVED_100                = 100,    /*!< RESERVED interrupt                                                  */	
   RESERVED_101                = 101,    /*!< RESERVED interrupt                                                  */	
   RESERVED_102                = 102,    /*!< RESERVED interrupt                                                  */	
   AXI_MON_IRQn                = 103,    /*!< AXI Clobal Monitor interrupt                                                  */	
   RESERVED_104                = 104,    /*!< RESERVED interrupt                                                  */	
   HDMI_TX_IRQn                = 105,    /*!< HDMI TX interrupt                                                  */	
   RESERVED_106                = 106,    /*!< RESERVED interrupt                                                  */	
   RESERVED_107                = 107,    /*!< RESERVED interrupt                                                  */	
   RESERVED_108                = 108,    /*!< RESERVED interrupt                                                  */	
   RESERVED_109                = 109,    /*!< RESERVED interrupt                                                  */	
   RESERVED_110                = 110,    /*!< RESERVED interrupt                                                  */	
   RESERVED_111                = 111,    /*!< RESERVED interrupt                                                  */	
   RESERVED_112                = 112,    /*!< RESERVED interrupt                                                  */	
   RESERVED_113                = 113,    /*!< RESERVED interrupt                                                  */	
   RESERVED_114                = 114,    /*!< RESERVED interrupt                                                  */	
   RESERVED_115                = 115,    /*!< RESERVED interrupt                                                  */	
   RESERVED_116                = 116,    /*!< RESERVED interrupt                                                  */	
   RESERVED_117                = 117,    /*!< RESERVED interrupt                                                  */	
   RESERVED_118                = 118,    /*!< RESERVED interrupt                                                  */	
   RESERVED_119                = 119,    /*!< RESERVED interrupt                                                  */	
   EXTI0_IRQn                  = 120,    /*!< GPIO Externl  interrupt                                                  */	
   EXTI1_IRQn                  = 121,    /*!< GPIO Externl  interrupt                                                  */	
   EXTI2_IRQn                  = 122,    /*!< GPIO Externl  interrupt                                                  */	
   EXTI3_IRQn                  = 123,    /*!< GPIO Externl  interrupt                                                  */	
   EXTI4_IRQn                  = 124,    /*!< GPIO Externl  interrupt                                                  */	
   EXTI5_IRQn                  = 125,    /*!< GPIO Externl  interrupt                                                  */	
   EXTI6_IRQn                  = 126,    /*!< GPIO Externl  interrupt                                                  */	
   EXTI7_IRQn                  = 127,    /*!< GPIO Externl  interrupt                                                  */	
   DMA0_IRQn                   = 128,    /*!< CBDMA0  interrupt                                                  */	
   DMA1_IRQn                   = 129,    /*!< CBDMA1  interrupt                                                  */	
   RESERVED_130                = 130,    /*!< RESERVED interrupt                                                  */	
   RESERVED_131                = 131,    /*!< RESERVED interrupt                                                  */	
   RESERVED_132                = 132,    /*!< RESERVED interrupt                                                  */	
   RESERVED_133                = 133,    /*!< RESERVED interrupt                                                  */	
   UART4_IRQn                  = 134,    /*!< UART4 interrupt                                                  */	
   RESERVED_135                = 135,    /*!< RESERVED interrupt                                                  */	
   RESERVED_136                = 136,    /*!< RESERVED interrupt                                                  */	
   RESERVED_137                = 137,    /*!< RESERVED interrupt                                                  */	
   UART_DMA0_IRQn              = 138,    /*!< UART DMA0  interrupt                                                  */	
   UART_DMA1_IRQn              = 139,    /*!< UART DMA1  interrupt                                                  */	
   RESERVED_140                = 140,    /*!< RESERVED interrupt                                                  */	
   RESERVED_141                = 141,    /*!< RESERVED interrupt                                                  */	
   UADBG_IRQn                  = 142,    /*!< UART to AXI  interrupt                                                  */	
   SPI_FLASH_IRQn              = 143,    /*!< SPI Flash  interrupt                                                  */	
   SPI_COMBO0_DMA_W_IRQn	   = 144,	 /*!< SPI Combo0 DMA write interrupt												  */
   SPI_SLAVE0_IRQn 			   = 145,	 /*!< SPI Slave0 interrupt													*/
   SPI_MASTER0_IRQn			   = 146,	 /*!< SPI Master0 interrupt 												 */
   RESERVED_147                = 147,    /*!< RESERVED interrupt                                                  */	
   SECURIYT_IRQn               = 148,    /*!< Securiyt interrupt                                                  */	
   SPI_NAND_IRQn               = 149,    /*!< SPI Nand interrupt                                                  */	
   STC_TIMER_W_IRQn            = 150,    /*!< STC Wathcdog interrupt                                                  */	
   STC_TIMER0_IRQn             = 151,    /*!< STC TIMER 0 interrupt                                                  */	
   STC_TIMER1_IRQn             = 152,    /*!< STC TIMER 1 interrupt                                                  */	
   STC_TIMER2_IRQn             = 153,    /*!< STC TIMER 2 interrupt                                                  */	
   STC_TIMER3_IRQn             = 154,    /*!< STC TIMER 3 interrupt                                                  */	
   STC_AV0_TIMER0_IRQn 		   = 155,	  /*!< STC TIMER 0 interrupt												  */ 
   STC_AV0_TIMER1_IRQn 		   = 156,	  /*!< STC TIMER 1 interrupt												  */ 
   STC_AV0_TIMER2_IRQn 		   = 157,	  /*!< STC TIMER 2 interrupt												  */ 
   STC_AV0_TIMER3_IRQn 		   = 158,	  /*!< STC TIMER 3 interrupt												  */ 
   STC_AV1_TIMER0_IRQn 		   = 159,	   /*!< STC TIMER 0 interrupt												   */ 
   STC_AV1_TIMER1_IRQn 		   = 160,	   /*!< STC TIMER 1 interrupt												   */ 
   STC_AV1_TIMER2_IRQn 		   = 161,	   /*!< STC TIMER 2 interrupt												   */ 
   STC_AV1_TIMER3_IRQn 		   = 162,	   /*!< STC TIMER 3 interrupt												   */ 
   RTC_IRQn 		           = 163,	   /*!< RTC TIMER  interrupt												   */ 							
   STC_AV2_TIMER0_IRQn 		   = 164,		/*!< STC TIMER 0 interrupt													*/ 
   STC_AV2_TIMER1_IRQn 		   = 165,		/*!< STC TIMER 1 interrupt													*/ 
   STC_AV2_TIMER2_IRQn 		   = 166,		/*!< STC TIMER 2 interrupt													*/ 
   STC_AV2_TIMER3_IRQn 		   = 167,		/*!< STC TIMER 3 interrupt													*/ 
   STC_AV0_TIMER_W_IRQn		   = 168,	  /*!< STC TWatchdog interrupt													*/
   RESERVED_169                = 169,    /*!< RESERVED interrupt                                                  */	
   RESERVED_170                = 170,    /*!< RESERVED interrupt                                                  */	
   RESERVED_171                = 171,    /*!< RESERVED interrupt                                                  */	
   RESERVED_172                = 172,    /*!< RESERVED interrupt                                                  */	
   RESERVED_173                = 173,    /*!< RESERVED interrupt                                                  */	
   I2C_MASTER0_IRQ             = 174,    /*!< I2C 0  interrupt                                                  */	
   I2C_MASTER1_IRQ             = 175,    /*!< I2C 1  interrupt                                                  */	
   I2C_MASTER2_IRQ             = 176,    /*!< I2C 2  interrupt                                                  */	
   I2C_MASTER3_IRQ             = 177,    /*!< I2C 3  interrupt                                                  */	
   I2C_DDC0_IRQ                = 178,    /*!< I2C DDC0 interrupt                                                  */	
   RESERVED_179                = 179,    /*!< RESERVED interrupt                                                  */	
   RESERVED_180                = 180,    /*!< RESERVED interrupt                                                  */	
   RESERVED_181                = 181,    /*!< RESERVED interrupt                                                  */	

   /* Mailbox interrupt*/
   IPC_P2C_DIRECT_INT0_IRQn    = 182,    /*!< P Chip send interrupt 0 to C chip (Mailbox direct transport data0)  */	
   IPC_P2C_DIRECT_INT1_IRQn    = 183,    /*!< P Chip send interrupt 1 to C chip (Mailbox direct transport data1)  */	  
   IPC_P2C_DIRECT_INT2_IRQn    = 184,    /*!< P Chip send interrupt 2 to C chip (Mailbox direct transport data2)  */	  
   IPC_P2C_DIRECT_INT3_IRQn    = 185,    /*!< P Chip send interrupt 3 to C chip (Mailbox direct transport data3)  */	  
   IPC_P2C_DIRECT_INT4_IRQn    = 186,    /*!< P Chip send interrupt 4 to C chip (Mailbox direct transport data4)  */	  
   IPC_P2C_DIRECT_INT5_IRQn    = 187,    /*!< P Chip send interrupt 5 to C chip (Mailbox direct transport data5)  */	  
   IPC_P2C_DIRECT_INT6_IRQn    = 188,    /*!< P Chip send interrupt 6 to C chip (Mailbox direct transport data6)  */	  
   IPC_P2C_DIRECT_INT7_IRQn    = 189,    /*!< P Chip send interrupt 7 to C chip (Mailbox direct transport data7)  */	  
   IPC_C2P_DIRECT_INT0_IRQn    = 190,    /*!< C Chip send interrupt 0 to P chip (Mailbox direct transport data0)  */	
   IPC_C2P_DIRECT_INT1_IRQn	   = 191,	 /*!< C Chip send interrupt 1 to P chip (Mailbox direct transport data1)  */  
   IPC_C2P_DIRECT_INT2_IRQn	   = 192,	 /*!< C Chip send interrupt 2 to P chip (Mailbox direct transport data2)  */	
   IPC_C2P_DIRECT_INT3_IRQn	   = 193,	 /*!< C Chip send interrupt 3 to P chip (Mailbox direct transport data3)  */	
   IPC_C2P_DIRECT_INT4_IRQn	   = 194,	 /*!< C Chip send interrupt 4 to P chip (Mailbox direct transport data4)  */	
   IPC_C2P_DIRECT_INT5_IRQn	   = 195,	 /*!< C Chip send interrupt 5 to P chip (Mailbox direct transport data5)  */	
   IPC_C2P_DIRECT_INT6_IRQn	   = 196,	 /*!< C Chip send interrupt 6 to P chip (Mailbox direct transport data6)  */	
   IPC_C2P_DIRECT_INT7_IRQn	   = 197,	 /*!< C Chip send interrupt 7 to P chip (Mailbox direct transport data7)  */	
   IPC_P2C_NORMAL_IRQn		   = 198,	 /*!< P Chip send interrupt  to C chip (Mailbox normal transport data)    */		
   IPC_C2P_NORMAL_IRQn	       = 199,	 /*!< C Chip send interrupt  to P chip (Mailbox normal transport data)	  */		
   MAX_IRQ_n
} IRQn_Type;


typedef enum {
	/*group 0*/
	SYSTEM 			= 0,							/* !< System bus ID*/
	RESERVED0 		= 1,
	RTC				= 2,							/* !< RTC moudle ID */
	IOCTL 			= 3,							/* !< IO Ctrl module ID */ 
	IOP				= 4,							/* !< IOP module ID */ 
	OTPRX			= 5, 							/* !< OTP RX moudle ID */
	NOC				= 6, 							/* !< NOC moudle ID */
	BR				= 7, 							/* !< BootROM Wrap moudle ID */
	RBUS_L00		= 8,							/* !< RBUS Level 0 moudle ID */ 
	SPI_FLASH		= 9, 							/* !< SPI Flash moudle ID */
	SDCTRL0			= 10, 							/* !< SD Ctrl moudle ID */
	PERI0			= 11, 							/* !< PREI0 moudle ID */
	RESERVED1		= 12,
	ARM926			= 13, 							/* !< ARM926 moudle ID */			
	UMCTL2			= 14, 							/* !< UMCTRL2 moudle ID */
	PERI1			= 15, 							/* !< PERI1 moudle ID */
	/*group 1*/
	DDR_PHY0		= 16, 							/* !< DDR PHY moudle ID */
	RESERVED2		= 17, 
	C_CHIP			= 18,							/* !< For C Chip module ID */
	RESERVED3		= 19, 									
	STC				= 20, 							/* !< Standard time clock 0 moudle ID */
	STC_AV0			= 21, 							/* !< standard time clock AV0 moudle ID */
	STC_AV1			= 22, 							/* !< standard time clock AV1 moudle ID */
	STC_AV2			= 23, 							/* !< standard time clock AV2 module ID */
	UART0			= 24, 							/* !< UART0 moudle ID */
	UART1 			= 25, 							/* !< UART1 moudle ID */
	UART2			= 26, 							/* !< UART2 moudle ID */ 
	UART3 			= 27, 							/* !< UART3 module ID */
	UART4			= 28, 							/* !< UART4 moudle ID */
	HWUA			= 29, 							/* !< Hardware uart moudle ID */
	DDC0			= 30, 							/* !< DDC0 moudle ID */
	UADMA			= 31, 
	/*group 2*/
	CBDMA0			= 32, 							/* !< CBDMA0 module ID*/
	CBDMA1			= 33,							/* !< CBDMA1 module ID*/
	SPI_COMBO0		= 34, 							/* !< SPI Combo 0 moudle ID */
	SPI_COMBO1		= 35, 							/* !< SPI Combo 1 moudle ID */
	SPI_COMBO2		= 36, 							/* !< SPI Combo 2 moudle ID */
	SPI_COMBO3		= 37, 							/* !< SPI Combo 3 moudle ID */
	AUD				= 38, 							/* !< Audio moudle ID	*/
	RESERVED4       = 39, 
	RESERVED5		= 40, 
	RESERVED6		= 41, 							
	USBCTRL0		= 42, 							/* !< USB Controller 0 moudle ID */	
	USBCTRL1		= 43,							/* !< USB Controller 1 moudle ID */
	RESERVED7		= 44, 						
	USBPHY0			= 45, 							/* !< USB Phy 0 moudle ID */
	USBPHY1			= 46,							/* !< USB Phy 1 moudle ID */
	RESERVED8		= 47,
	/*group 3*/
	I2CM0			= 48, 							/* !< I2C Master0 moudle ID */
	I2CM1			= 49, 							/* !< I2C Master1 moudle ID */
	I2CM2			= 50, 							/* !< I2C Master2 moudle ID */
	I2CM3			= 51, 							/* !< I2C Master3 moudle ID */
	RESERVED9		= 52,
	RESERVED10		= 53,
	RESERVED11		= 54,
	RESERVED12		= 55,
	RESERVED13		= 56,
	RESERVED14		= 57,
	RESERVED15		= 58,
	RESERVED16		= 59,
	RESERVED17		= 60,
	PMC_QCHANEL		= 61,							/* !< PMC Q-chanel moudle ID */
	CARD_CTL0		= 62,							/* !< SD Card Controller moudle ID */
	CARD_CTL1		= 63,							/* !< SD Card Controller moudle ID */
	/*group 4*/
	RESERVED18		= 64,
	RESERVED19		= 65,						
	CARD_CTL4		= 66,							/* !< SD Card Controller moudle ID */					
	RESERVED20		= 67,
	BCH				= 68,							/* !< BCH for spi nand moudle ID */
	RESERVED21		= 69,
	RESERVED22		= 70,
	RESERVED23		= 71,
	RESERVED24		= 72,
	RESERVED25		= 73,
	RESERVED26		= 74,
	DDFCH			= 75,							/* !< DDFCH module ID */
	CSIIW0			= 76, 							/* !< CSI IW 0 moudle ID */
	CSIIW1			= 77, 							/* !< CSI IW 0 moudle ID */
	MIPICSI0 		= 78, 							/* !< MIPICSI 0 moudle ID */
	MIPICSI1		= 79, 							/* !< MIPICSI 1 moudle ID */
	/*group 5*/
	HDMI_TX			= 80, 							/* !< HDMI TX moudle ID */
	RESERVED27		= 81, 
	RESERVED28		= 82,
	RESERVED29		= 83,
	RESERVED30		= 84,
	VPOST			= 85, 							/* !< Video POST moudle ID*/
	RESERVED31		= 86,
	RESERVED32		= 87,
	RESERVED33		= 88,
	RESERVED34		= 89,
	RESERVED35		= 90,
	RESERVED36		= 91,
	RESERVED37		= 92,
	RESERVED38		= 93,
	RESERVED39		= 94,
	RESERVED40		= 95,
	/*group 6*/
	TGEN			= 96, 							/* !< TGEN module ID */
	DMIX			= 97, 							/* !< DMIX moudle ID */
	RESERVED41		= 98,
	RESERVED42		= 99,
	RESERVED43		= 100,
	RESERVED44		= 101,
	RESERVED45		= 102,
	RESERVED46		= 103,
	RESERVED47		= 104,
	RESERVED48		= 105,
	TCON			= 106,							/* !< TCON moudle ID */
	RESERVED49		= 107,
	RESERVED50		= 108,
	RESERVED51		= 109,
	RESERVED52		= 110,
	IRQCTRL			= 111,
	/*group 7*/
	RGST			= 112, 							/* !< Register bus moudle ID */
	RESERVED53		= 113,
	RESERVED54		= 114,
	GPIO			= 115, 							/* !< GPIO moudle ID */
	RBUS_TOP		= 116, 							/* !< Register top bus moudle ID */
	RESERVED55		= 117,
	RESERVED56		= 118,
	RESERVED57		= 119,
	RESERVED58		= 120,
	RESERVED59		= 121,
	RESERVED60		= 122,
	RESERVED61		= 123,
	RESERVED62		= 124,
	RESERVED63		= 125,
	RESERVED64		= 126,
	RESERVED65		= 127,
	/*group 8*/
	RESERVED66		= 128,
	RESERVED67		= 129,
	RESERVED68		= 130,
	RESERVED69		= 131,
	RESERVED70		= 132,
	RESERVED71		= 133,
	MAILBOX			= 134, 							/* !< Mailbox module ID */
	RESERVED72		= 135,
	RESERVED73		= 136,
	RESERVED74		= 137,
	SPI_NAND		= 138, 							/* !< SPI Nand moudle ID */
	I2C2CBUS		= 139, 							/* !< I2C to Bus moudule ID */
	GDMA			= 140,							/* !< GDMA module ID */
	SECURIYT		= 141, 							/* !< Securiyt module ID */
	DVE				= 142, 							/* !< DVE moudle ID */
	GPOST0			= 143, 							/* !< GPOST0 module ID */
	/*group 9*/
	OSD0 			= 144, 							/* !< OSD 0 module ID */
	RESERVED75		= 145,
	PWM				= 146,							/* !< PWM moudle ID */
	UADBG			= 147,							/* !< UART Debug module ID */
	DUMMY_MASTER	= 148,							/* !< Dummy master module ID */
	FIO_CTRL		= 149, 							/* !< FIO controller module ID */
	FPGA			= 150,							/* !< FPGA controller module ID */
	L2SW			= 151, 							/* !< Ethernet switch module ID */
	ICM				= 152, 							/* !< Input capture module ID */
	AXI_GLOBAL		= 153,							/* !< AXI moniter module ID */
	RESERVED76		= 154,	
	RESERVED77		= 155,
	RESERVED78		= 156,
	RESERVED79		= 157,
	RESERVED80		= 158,
	RESERVED81		= 159,
	
}MODULE_ID_Type;

typedef enum{
/*_G1.1_*/
PINMUX_SPI_FLASH=0x0,
PINMUX_SPI_FLASH_4BIT=0x2,
PINMUX_SPI_NAND=0x4,
PINMUX_CARD0_EMMC,
PINMUX_SD_CARD,
PINMUX_UART0,
PINMUX_ACHIP_DEBUG,
PINMUX_ACHIP_UA2AXI=0xA,
PINMUX_FPGA_IFX=0xC,
PINMUX_HDMI_TX,
PINMUX_AUD_EXT_ADC_IFX0=0x0F,

/*_G1.2_*/
PINMUX_AUD_EXT_DAC_IFX0=0x10,
PINMUX_AUD_TEST,
PINMUX_AUD_IEC_RX0,
PINMUX_IEC_TX,
PINMUX_TDMTX_IFX0,
PINMUX_TDMRX_IFX0,
PINMUX_PDMRX_IFX0,
PINMUX_PCM_IEC_TX,
PINMUX_CLKGENA,
PINMUX_DGB_I2C,
PINMUX_SLV_I2C,
PINMUX_IR_IN,
PINMUX_UADGB,
PINMUX_UPHY0_IF=0x1E,

/*_G1.3_*/
PINMUX_UPHY1_IF=0x20,
PINMUX_USBC0_OTG_EN=0x22,
PINMUX_USBC1_OTG_EN,
PINMUX_UPHY0_DEBUG,
PINMUX_UPHY1_DEBUG,
PINMUX_UPHY0_EXT,
PINMUX_PROBE_PORT,
PINMUX_MO1_CLK27_O=0x29,
PINMUX_MO1_PLLA_135_147_O,
PINMUX_XCK_OUT,
PINMUX_CLK27_OUT,
PINMUX_HWCFG_PROB,
PINMUX_8051_JTAG,
/*_G1.4_*/
PINMUX_ARM926_JTAG=0x30,

/*_G2.0_*/
PINMUX_L2SW_CLK_OUT=0x100,
PINMUX_L2SW_MAC_SMI_MDC,
PINMUX_L2SW_LED_FLASH0,
PINMUX_L2SW_LED_FLASH1,
PINMUX_L2SW_LED_ON0,
PINMUX_L2SW_LED_ON1,
PINMUX_L2SW_MAC_SMI_MDIO,
PINMUX_L2SW_P0_MAC_RMII_T1,
PINMUX_L2SW_P0_MAC_RMII_TXD0,
PINMUX_L2SW_P0_MAC_RMII_TXD1,
PINMUX_L2SW_P0_MAC_RMII_CRSDV,
PINMUX_L2SW_P0_MAC_RMII_RXD0,
PINMUX_L2SW_P0_MAC_RMII_RXD1,
PINMUX_L2SW_P0_MAC_RMII_RXER,
PINMUX_L2SW_P1_MAC_RMII_TXEN,
PINMUX_L2SW_P1_MAC_RMII_TXD0,
/*_G2.8_*/
PINMUX_L2SW_P1_MAC_RMII_TXD1,
PINMUX_L2SW_P1_MAC_RMII_CRSDV,
PINMUX_L2SW_P1_MAC_RMII_RXD0,
PINMUX_L2SW_P1_MAC_RMII_RXD1,
PINMUX_L2SW_P1_MAC_RMII_RXER,
PINMUX_DAISY_MODE,
PINMUX_SDIO_CLK,
PINMUX_SDIO_CMD,
PINMUX_SDIO_D0,
PINMUX_SDIO_D1,
PINMUX_SDIO_D2,
PINMUX_SDIO_D3,
PINMUX_PWM0,
PINMUX_PWM1,
PINMUX_PWM2,
PINMUX_PWM3,
/*_G2.16_*/
PINMUX_PWM4,
PINMUX_PWM5,
PINMUX_PWM6,
PINMUX_PWM7,
PINMUX_ICM0_D,
PINMUX_ICM1_D,
PINMUX_ICM2_D,
PINMUX_ICM3_D,
PINMUX_ICM0_CLK,
PINMUX_ICM1_CLK,
PINMUX_ICM2_CLK,
PINMUX_ICM3_CLK,
PINMUX_SPIM0_INT,
PINMUX_SPIM0_CLK,
PINMUX_SPIM0_EN,
PINMUX_SPIM0_DO,
/*_G2.24_*/
PINMUX_SPIM0_DI,
PINMUX_SPIM1_INT,
PINMUX_SPIM1_CLK,
PINMUX_SPIM1_CEN,
PINMUX_SPIM1_DO,
PINMUX_SPIM1_DI,
PINMUX_SPIM2_INT,
PINMUX_SPIM2_CLK,
PINMUX_SPIM2_CEN,
PINMUX_SPIM2_DO,
PINMUX_SPIM2_DI,
PINMUX_SPIM3_INT,
PINMUX_SPIM3_CLK,
PINMUX_SPIM3_CEN,
/*_G2.31_*/
PINMUX_SPIM3_DO,
PINMUX_SPIM3_DI,

/*_G3.0_*/
PINMUX_SPI0S_INT=0x200,
PINMUX_SPI0S_CLK,
PINMUX_SPI0S_EN,
PINMUX_SPI0S_DO,
PINMUX_SPI0S_DI,
PINMUX_SPI1S_INT,
PINMUX_SPI1S_CLK,
PINMUX_SPI1S_EN,
PINMUX_SPI1S_DO,
PINMUX_SPI1S_DI,
PINMUX_SPI2S_INT,
PINMUX_SPI2S_CLK,
PINMUX_SPI2S_EN,
PINMUX_SPI2S_DO,
PINMUX_SPI2S_DI,
PINMUX_SPI3S_INT,
/*_G3.8_*/
PINMUX_SPI3S_CLK,
PINMUX_SPI3S_EN,
PINMUX_SPI3S_DO,
PINMUX_SPI3S_DI,
PINMUX_I2CM0_SCL,
PINMUX_I2CM0_SDA,
PINMUX_I2CM1_SCL,
PINMUX_I2CM1_SDA,
PINMUX_I2CM2_SCL,
PINMUX_I2CM2_SDA,
PINMUX_I2CM3_SCL,
PINMUX_I2CM3_SDA,
PINMUX_UA1_TX,
PINMUX_UA1_RX,
PINMUX_UA1_CTS,
PINMUX_UA1_RTS,
/*_G3.16_*/
PINMUX_UA2_TX,
PINMUX_UA2_RX,
PINMUX_UA2_CTS,
PINMUX_UA2_RTS,
PINMUX_UA3_TX,
PINMUX_UA3_RX,
PINMUX_UA3_CTS,
PINMUX_UA3_RTS,
PINMUX_UA4_TX,
PINMUX_UA4_RX,
PINMUX_UA4_CTS,
PINMUX_UA4_RTS,
PINMUX_TIMER0_INT,
PINMUX_TIMER1_INT,
PINMUX_TIMER2_INT,
PINMUX_TIMER3_INT,
/*_G3.24_*/
PINMUX_GPIO_INT0,
PINMUX_GPIO_INT1,
PINMUX_GPIO_INT2,
PINMUX_GPIO_INT3,
PINMUX_GPIO_INT4,
PINMUX_GPIO_INT5,
PINMUX_GPIO_INT6,
PINMUX_GPIO_INT7,

}PINMUX_Type;
 
#define __FPU_PRESENT             0       
#define __TIM_PRESENT 			  0		  
#define __GIC_PRESENT             0
#define __L2C_PRESENT	          0

#define IRQ_CTRL_BASE    0x9C000480

#include "core_armv5.h"

#include <stdint.h>




typedef struct{
	__IOM uint32_t clock_enable[10];				/*!< \brief devices clock enable bit*/
}Module_Clock_En_type;

typedef struct{
	__IOM uint32_t clock_gate_enable[10];			/*!< \brief devices clock gate enable bit*/
}Module_Clock_Gate_Type;

typedef struct{
	__IOM uint32_t reset[10];			/*!< \brief devices clock gate enable bit*/
}Module_Reset_Type;



typedef struct{
	__IM uint32_t stamp_id; 						   /*!< \brief chip reversion and stamp id*/
	__IOM uint32_t clock_enable[10]; 				/*!< \brief devices clock enable bit*/
	__IOM uint32_t clock_gate_enable[10];			/*!< \brief devices clock gate enable bit*/
	__IOM uint32_t reset[10];						   /*!< \brief devices reset bit*/
	__IOM uint32_t hw_config;							/*!< \brief hardware config for boot mode*/
}GROUP0_Type;

typedef struct{
	__IM uint32_t test_mode_config; 				  /*!< \brief chip test mode config*/
	__IOM uint32_t pinmux_cfg[4]; 				  /*!< \brief pinmux config */
	RESERVED(0[17], uint32_t)                    /*!< reserved */
}GROUP1_Type;

typedef struct{
   __IOM uint32_t pinmux_cfg[32];             /*!< \brief  pinmux config */
}GROUP2_Type;

typedef struct{
   __IOM uint32_t pinmux_cfg[28];             /*!< \brief  pinmux config */
   RESERVED(0[4], uint32_t)                    /*!< reserved */
}GROUP3_Type;

typedef struct
{
  __IOM uint32_t type[7];              /*!< \brief  interrupt type 0:level-type, 1: edge-type */
  __IOM uint32_t polarity[7];          /*!< \brief  interrupt polarity 0:high-active(default) 1:low-active*/
  __IOM uint32_t priority[7];          /*!< \brief  interrupt piority 0:fiq(default), 1:irq */
  __IOM  uint32_t mask[7];              /*!< \brief  mask interrupt bit 0:masked(default) 1:pass */
  RESERVED(0[4], uint32_t)
  	__OM uint32_t clear[7]; 		   /*!< \brief  clear interrupt bit */
  __IM uint32_t masked_fiq[7];		   /*!< \brief  read masked fiq bit */
  __IM uint32_t masked_irq[7];		   /*!< \brief  read masked irq bit */
  __IOM uint32_t dsp_mask[7];		   /*!< \brief  mask dsp interrupt bit 0:masked(default) 1:pass */
  RESERVED(1[3], uint32_t)
  __IM uint32_t group; 
}IRQ_Ctrl_Type;

typedef struct{
	__IOM uint32_t stc_15_0;				 /*!< \brief  standard time clock counter, 0~15 bit */
	__IOM uint32_t stc_31_16;				 /*!< \brief  standard time clock counter, 16~31 bit */
	__IOM uint32_t stc_64;					 /*!< \brief  standard time clock counter, the MSB 64 bit, when write the bit ,clear the stc counter at once */
	__IOM uint32_t stc_prescale_val;		
	RESERVED(0[3], uint32_t)
	__IOM uint32_t stc_config;	
	RESERVED(1[6], uint32_t)
	__IOM uint32_t stc_47_32;	
	__IOM uint32_t stc_63_48;
	RESERVED(2[8], uint32_t)
	__IOM uint32_t stcl_15_0;	
	__IOM uint32_t stcl_31_16;
	__IOM uint32_t stcl_32;
	__IOM uint32_t atc_15_0;
	__IOM uint32_t atc_31_16;
	__IOM uint32_t atc_33_32;
	RESERVED(3[2], uint32_t)
}STC_TypeDef;


typedef struct 
{
	__IOM uint32_t control;   			/*!< \brief timer control register for >*/
	__IOM uint32_t prescale_val;			/*!< \brief 16-bit prescale for timer>*/
	__IOM uint32_t reload_val;			/*!< \brief when count reaches to 0, the value will reload to the count >*/
	__IOM uint32_t counter_val;					/*!< \brief 16-bit counter , it down to 0, the timer will generate the interrupt*/
}TIM_TypeDef;



typedef struct{
    __IOM uint32_t dr;  /* data register */
    __IOM uint32_t lsr; /* line status register */
    __IOM uint32_t msr; /* modem status register */
    __IOM uint32_t lcr; /* line control register */
    __IOM uint32_t mcr; /* modem control register */
    __IOM uint32_t div_l;
    __IOM uint32_t div_h;
    __IOM uint32_t isc; /* interrupt status/control */
    __IOM uint32_t txr; /* tx residue */
    __IOM uint32_t rxr; /* rx residue */
    __IOM uint32_t thr; /* rx threshold */
    __IOM uint32_t clk_src;
}UART_CTRL_Type;

typedef struct {
   __IOM uint32_t rxdma_enable_sel;
   __IOM uint32_t rxdma_start_addr;
   __IOM uint32_t rxdma_timeout_set;
   __IOM uint32_t rxdma_reserved;
   __IOM uint32_t rxdma_wr_adr;
   __IOM uint32_t rxdma_rd_adr;
   __IOM uint32_t rxdma_length_thr;
   __IOM uint32_t rxdma_end_addr;
   __IOM uint32_t rxdma_databytes;
   __IOM uint32_t rxdma_debug_info;
}UART_Rxdma;

typedef struct {
   __IOM uint32_t txdma_enable;
   __IOM uint32_t txdma_sel;
   __IOM uint32_t txdma_start_addr;
   __IOM uint32_t txdma_end_addr;
   __IOM uint32_t txdma_wr_adr;
   __IOM uint32_t txdma_rd_adr;
   __IOM uint32_t txdma_status;
   __IOM uint32_t txdma_tmr_unit;
   __IOM uint32_t txdma_tmr_cnt;
   __IOM uint32_t txdma_rst_done;
}UART_Txdma;

typedef struct {
   __IOM uint32_t gdma_hw_ver;
   __IOM uint32_t gdma_config;
   __IOM uint32_t gdma_length;
   __IOM uint32_t gdma_addr;
   __IOM uint32_t gdma_port_mux;
   __IOM uint32_t gdma_int_flag;
   __IOM uint32_t gdma_int_en;
   __IOM uint32_t gdma_software_reset_status;
   __IOM uint32_t txdma_tmr_cnt;
}UART_TxGdma;


typedef struct{
   // SPI_MASTER
   __IOM uint32_t mst_tx_data_addr;
   __IOM uint32_t mst_tx_data_3_2_1_0;
   __IOM uint32_t mst_tx_data_7_6_5_4;
   __IOM uint32_t mst_tx_data_11_10_9_8;
   __IOM uint32_t mst_tx_data_15_14_13_12;
   RESERVED(0[4], uint32_t);
   __IOM uint32_t mst_rx_data_3_2_1_0;
   __IOM uint32_t mst_rx_data_7_6_5_4;
   __IOM uint32_t mst_rx_data_11_10_9_8;
   __IOM uint32_t mst_rx_data_15_14_13_12;
   __IOM uint32_t fifo_data;
   __IOM uint32_t spi_status;
   __IOM uint32_t spi_config;
  RESERVED(1[1], uint32_t);
   __IOM uint32_t spi_ctl_clk_sel;
   __IOM uint32_t spi_byte_no;
   __IOM uint32_t spi_int_busy;
   __IOM uint32_t spi_dma_ctrl;
   __IOM uint32_t spi_dma_length;
   __IOM uint32_t spi_dma_addr;
   RESERVED(2[1], uint32_t);
   __IOM uint32_t spi_dma_status;
   RESERVED(3[1], uint32_t);
   __IOM uint32_t uart_dma_ctrl;
   RESERVED(4[1], uint32_t);
   __IOM uint32_t spi_mst_debug_sel;
   __IOM uint32_t spi_combo_debug_sel;
   __IOM uint32_t spi_extra_cycle;
   __IOM uint32_t spi_dma_data_rdy;
}SPI_TypeDef;

typedef struct
{
	__IOM uint32_t control0;      /* 00 */
	__IOM uint32_t control1;      /* 01 */
	__IOM uint32_t control2;      /* 02 */
	__IOM uint32_t control3;      /* 03 */
	__IOM uint32_t control4;      /* 04 */
	__IOM uint32_t control5;      /* 05 */
	__IOM uint32_t i2cm_status0;  /* 06 */
	__IOM uint32_t interrupt;     /* 07 */
	__IOM uint32_t int_en0;       /* 08 */
	__IOM uint32_t i2cm_mode;     /* 09 */
	__IOM uint32_t i2cm_status1;  /* 10 */
	__IOM uint32_t i2cm_status2;  /* 11 */
	__IOM uint32_t control6;      /* 12 */
	__IOM uint32_t int_en1;       /* 13 */
	__IOM uint32_t i2cm_status3;  /* 14 */
	__IOM uint32_t i2cm_status4;  /* 15 */
	__IOM uint32_t int_en2;       /* 16 */
	__IOM uint32_t control7;      /* 17 */
	__IOM uint32_t control8;      /* 18 */
	__IOM uint32_t control9;      /* 19 */
	RESERVED(0[3], uint32_t)      /* 20~22 */
	__IOM uint32_t version;       /* 23 */
	__IOM uint32_t data00_03;     /* 24 */
	__IOM uint32_t data04_07;     /* 25 */
	__IOM uint32_t data08_11;     /* 26 */
	__IOM uint32_t data12_15;     /* 27 */
	__IOM uint32_t data16_19;     /* 28 */
	__IOM uint32_t data20_23;     /* 29 */
	__IOM uint32_t data24_27;     /* 30 */
	__IOM uint32_t data28_31;     /* 31 */
} I2C_TypeDef;

typedef struct
{
	__IOM uint32_t hw_version;                /* 00 */
	__IOM uint32_t dma_config;                /* 01 */
	__IOM uint32_t dma_length;                /* 02 */
	__IOM uint32_t dma_addr;                  /* 03 */
	__IOM uint32_t port_mux;                  /* 04 */
	__IOM uint32_t int_flag;                  /* 05 */
	__IOM uint32_t int_en;                    /* 06 */
	__IOM uint32_t sw_reset_state;            /* 07 */
	RESERVED(0[2], uint32_t)                  /* 08~09 */
	__IOM uint32_t sg_dma_index;              /* 10 */
	__IOM uint32_t sg_dma_config;             /* 11 */
	__IOM uint32_t sg_dma_length;             /* 12 */
	__IOM uint32_t sg_dma_addr;               /* 13 */
	RESERVED(1, uint32_t)                     /* 14 */
	__IOM uint32_t sg_setting;                /* 15 */
	__IOM uint32_t threshold;                 /* 16 */
	RESERVED(2, uint32_t)                     /* 17 */
	__IOM uint32_t gdma_read_timeout;         /* 18 */
	__IOM uint32_t gdma_write_timeout;        /* 19 */
	__IOM uint32_t ip_read_timeout;           /* 20 */
	__IOM uint32_t ip_write_timeout;          /* 21 */
	__IOM uint32_t write_cnt_debug;           /* 22 */
	__IOM uint32_t w_byte_en_debug;           /* 23 */
	__IOM uint32_t sw_reset_write_cnt_debug;  /* 24 */
	__IOM uint32_t sw_reset_read_cnt_debug;   /* 25 */
	RESERVED(3[6], uint32_t)                  /* 26~31 */
} GDMA_TypeDef;

typedef struct
{
	__IOM uint32_t pll_ctrl;			/*!< \brief sp7021 q628 system PLL set >*/
	__IOM uint32_t clk_setting;			/*!< \brief sp7021 q628 system clock set and selection >*/

}SYS_CLKDef;


#define STATIC_ASSERT(b) extern int _static_assert[b ? 1 : -1]

struct _PWM_DD_REG_ {
   union {
      struct {
         uint32_t dd             :16;//b(0-15)
         uint32_t :16;
      };
      uint32_t idx_all;
   };
};
STATIC_ASSERT(sizeof(struct _PWM_DD_REG_) == 4);

struct _PWM_DU_REG_ {
   union {
      struct {
         uint32_t pwm_du            :8;   //b(0-7)
         uint32_t pwm_du_dd_sel     :2;   //b(8-9)
         uint32_t                :6;   //b(10-15)
         uint32_t :16;
      };
      uint32_t idx_all;
   };
};
STATIC_ASSERT(sizeof(struct _PWM_DU_REG_) == 4);

typedef struct {
   //GROUP 244
   union {
      struct {
         uint32_t pwm_en            :8;   //b(0-7)
         uint32_t pwm_bypass        :8;   //b(8-15)
         uint32_t :16;
      };
      uint32_t grp244_0;
   };
   union {
      struct {
         uint32_t pwm_cnt0_en       :1;   //b(0)
         uint32_t pwm_cnt1_en       :1;   //b(1)
         uint32_t pwm_cnt2_en       :1;   //b(2)
         uint32_t pwm_cnt3_en       :1;   //b(3)
         uint32_t pwm_clk54_en      :1;   //b(4)
         uint32_t                :3;   //b(5-7)
         uint32_t pwm_dd0_sync_off  :1;   //b(8)
         uint32_t pwm_dd1_sync_off  :1;   //b(9)
         uint32_t pwm_dd2_sync_off  :1;   //b(10)
         uint32_t pwm_dd3_sync_off  :1;   //b(11)
         uint32_t                :4;   //b(12-15)
         uint32_t :16;
      };
      uint32_t grp244_1;
   };
   struct _PWM_DD_REG_ pwm_dd[4];      //G244.2~5
   struct _PWM_DU_REG_ pwm_du[8];      //G244.6~13
   uint32_t grp244_14_31[18];
}PWM_TypeDef;


typedef struct {
	__OM uint32_t C2P_normal_trigger;		//CA7 tiger the INT to ARM926
	__IOM uint32_t C2P_write_monitor_status;
	__IOM uint32_t C2P_over_write_monitor_status;
	RESERVED(0, uint32_t);
	__IOM uint32_t C2P_normal_data[20];
	__IOM uint32_t C2P_direct_data[8];	
	
	__OM uint32_t P2C_normal_trigger;		//ARM926 tiger the INT to CA7
	__IOM uint32_t P2C_write_monitor_status;
	__IOM uint32_t P2C_over_write_monitor_status;
	RESERVED(1, uint32_t);
	__IOM uint32_t P2C_normal_data[20];
	__IOM uint32_t P2C_direct_data[8];	
}IPCC_Typedef;

typedef struct
{
   __IOM uint32_t cfg0;
   __IOM uint32_t cfg1;
   __IOM uint32_t cntscl;
   __IOM uint32_t tstscl;
   __IOM uint32_t cnt;
   __IOM uint32_t pwh;
   __IOM uint32_t pwl;
}ICM_CTRL_REG;

#define SP_IRQ_CTRL  ((IRQ_Ctrl_Type*) IRQ_CTRL_BASE)


#define DRAM_BASE 			0x00000000
#define EXTERNAL_ROM_BASE 	0x98000000
#define PERIPH_BASE 		0x9C000000
#define RBUS_BASE			(PERIPH_BASE+0)
#define AXI_BUS_BASE		(PERIPH_BASE+0x101000)
#define INTERNEL_ROM_BASE 	0x9E000000
#define SRAM_BASE 			0x9E800000

#define RF_GRP(_grp, _reg) ((((_grp) * 32 + (_reg)) * 4) + RBUS_BASE)
#define RF_AXI_GRP(_grp, _reg) ((((_grp) * 4096 + (_reg)) * 4) + AXI_BUS_BASE)

#define RF_MASK_V(_mask, _val)       (((_mask) << 16) | (_val))
#define RF_MASK_V_SET(_mask)         (((_mask) << 16) | (_mask))
#define RF_MASK_V_CLR(_mask)         (((_mask) << 16) | 0)

/*APB RBUS register group*/
#define MOON0_GROUP_NO 		(0)
#define MOON1_GROUP_NO     (1)
#define MOON2_GROUP_NO		(2)
#define MOON3_GROUP_NO		(3)
#define MOON4_GROUP_NO		(4)
#define MOON5_GROUP_NO		(5)
#define GPIOXT0_GROUP_NO	(6)
#define GPIOXT1_GROUP_NO	(7)
#define IOP_GROUP_NO		(8)
#define INT_CTRL0_GROUP_NO	(9)  //for ARM926
#define INT_CTRL1_GROUP_NO	(10)
#define TSENT_GROUP_NO		(11)

#define INT_CTRL2_GROUP_NO	(15) //for C-Chip
#define INT_CTRL3_GROUP_NO	(23)


/*SP7021 module clock enable bit*/
#define MODULE_CLK_EN_BASE	RF_GRP(0, 1)
#define CLK_EN       ((Module_Clock_En_type*) MODULE_CLK_EN_BASE)

/*SP7021 module clock enable bit*/
#define MODULE_CLK_GATE_BASE RF_GRP(0, 11)
#define CLK_GATE     ((Module_Clock_Gate_Type*) MODULE_CLK_GATE_BASE)

/*SP7021 module rest bit*/
#define MODULE_RESET_BASE RF_GRP(0, 21)
#define MODULE_REST     ((Module_Reset_Type*) MODULE_RESET_BASE)

#define MOON1_GROUP_BASE    RF_GRP(1,0)
#define PINMUX_1_CFG    ((GROUP1_Type*) MOON1_GROUP_BASE)

#define MOON2_GROUP_BASE    RF_GRP(2,0)
#define PINMUX_2_CFG    ((GROUP2_Type*) MOON2_GROUP_BASE)

#define MOON3_GROUP_BASE    RF_GRP(3,0)
#define PINMUX_3_CFG    ((GROUP3_Type*) MOON3_GROUP_BASE)

#define PWM_BASE            RF_GRP(244, 0)
#define PWM_CTRL_REG    ((PWM_TypeDef *)PWM_BASE)


/*TIMER0 & TIMER1 is a group*/
#define TIM0_BASE RF_GRP(12, 16)
#define TIM1_BASE RF_GRP(12, 20)

/*TIMER2 & TIMER3 is a group*/
#define TIM2_BASE RF_GRP(96, 16)
#define TIM3_BASE RF_GRP(96, 20)

/*TIMER4 & TIMER5 is a group*/
#define TIM4_BASE RF_GRP(97, 16)
#define TIM5_BASE RF_GRP(97, 20)

/*TIMER6 & TIMER7 is a group*/
#define TIM6_BASE RF_GRP(99, 16)
#define TIM7_BASE RF_GRP(99, 20)


/*Standard Time Clock (STC)*/
#define STC0_BASE  RF_GRP(12, 0)
#define STC1_BASE  RF_GRP(96, 0)
#define STC2_BASE  RF_GRP(97, 0)
#define STC3_BASE  RF_GRP(99, 0)

#define SYSCLK_BASE RF_GRP(4, 26)
#define SYS_CLK			((SYS_CLKDef*)SYSCLK_BASE)

#define TIM0			((TIM_TypeDef *) TIM0_BASE)
#define TIM1			((TIM_TypeDef *) TIM1_BASE)
#define TIM2			((TIM_TypeDef *) TIM2_BASE)
#define TIM3			((TIM_TypeDef *) TIM3_BASE)
#define TIM4			((TIM_TypeDef *) TIM4_BASE)
#define TIM5			((TIM_TypeDef *) TIM5_BASE)
#define TIM6			((TIM_TypeDef *) TIM6_BASE)
#define TIM7			((TIM_TypeDef *) TIM7_BASE)

#define STC0		((STC_TypeDef *)STC0_BASE)
#define STC1		((STC_TypeDef *)STC1_BASE)
#define STC2		((STC_TypeDef *)STC2_BASE)
#define STC3		((STC_TypeDef *)STC3_BASE)


/*  gpio register */

#define GPIO_FIRST(X)   (RF_GRP(101, (25+X)))
#define GPIO_MASTER(X)  (RF_GRP(6, (0+X)))
#define GPIO_OE(X)      (RF_GRP(6, (8+X)))
#define GPIO_OUT(X)     (RF_GRP(6, (16+X)))
#define GPIO_IN(X)      (RF_GRP(6, (24+X)))
#define GPIO_I_INV(X)   (RF_GRP(7, (0+X)))
#define GPIO_O_INV(X)   (RF_GRP(7, (8+X)))
#define GPIO_OD(X)      (RF_GRP(7, (16+X)))


/* uart register */

#define UART0_BASE      RF_GRP(18, 0)
#define UART1_BASE      RF_GRP(19, 0)
#define UART2_BASE      RF_GRP(16, 0)
#define UART3_BASE      RF_GRP(17, 0)
#define UART4_BASE      RF_GRP(271, 0)


#define SP_UART0        ((UART_CTRL_Type *)UART0_BASE)
#define SP_UART1        ((UART_CTRL_Type *)UART1_BASE)
#define SP_UART2        ((UART_CTRL_Type *)UART2_BASE)
#define SP_UART3        ((UART_CTRL_Type *)UART3_BASE)
#define SP_UART4        ((UART_CTRL_Type *)UART4_BASE)


#define UART_RXDMA0_BASE      RF_GRP(275, 0)
#define UART_RXDMA1_BASE      RF_GRP(275, 16)

#define SP_UART_RXDMA0_CTRL   ((volatile UART_Rxdma*) UART_RXDMA0_BASE)
#define SP_UART_RXDMA1_CTRL   ((volatile UART_Rxdma*) UART_RXDMA1_BASE)


#define UART_TXDMA0_BASE      RF_GRP(276, 0)
#define UART_TXDMA1_BASE      RF_GRP(276, 16)

#define SP_UART_TXDMA0_CTRL   ((volatile UART_Txdma*) UART_TXDMA0_BASE)
#define SP_UART_TXDMA1_CTRL   ((volatile UART_Txdma*) UART_TXDMA1_BASE)

#define UART_TXGDMA0_BASE     RF_GRP(273, 0)
#define UART_TXGDMA1_BASE     RF_GRP(273, 16)

#define SP_UART_TXGDMA0_CTRL     ((volatile UART_TxGdma*) UART_TXGDMA0_BASE)
#define SP_UART_TXGDMA1_CTRL     ((volatile UART_TxGdma*) UART_TXGDMA1_BASE)

/* spi register */

#define SPI0_BASE      RF_GRP(91, 0)
#define SPI1_BASE      RF_GRP(489, 0)
#define SPI2_BASE      RF_GRP(492, 0)
#define SPI3_BASE      RF_GRP(495, 0)


#define SPI0        ((volatile SPI_TypeDef *)SPI0_BASE)
#define SPI1        ((volatile SPI_TypeDef *)SPI1_BASE)
#define SPI2        ((volatile SPI_TypeDef *)SPI2_BASE)
#define SPI3        ((volatile SPI_TypeDef *)SPI3_BASE)

/* i2c register */
#define I2CM0_BASE      RF_GRP(140, 0)
#define I2CM1_BASE      RF_GRP(142, 0)
#define I2CM2_BASE      RF_GRP(144, 0)
#define I2CM3_BASE      RF_GRP(146, 0)

#define GDMA0_BASE     RF_GRP(141, 0)
#define GDMA1_BASE     RF_GRP(143, 0)
#define GDMA2_BASE     RF_GRP(145, 0)
#define GDMA3_BASE     RF_GRP(147, 0)


#define SP_I2CM0 	((volatile I2C_TypeDef *)I2CM0_BASE)
#define SP_I2CM1 	((volatile I2C_TypeDef *)I2CM1_BASE)
#define SP_I2CM2 	((volatile I2C_TypeDef *)I2CM2_BASE)
#define SP_I2CM3 	((volatile I2C_TypeDef *)I2CM3_BASE)

#define SP_GDMA0 	((volatile GDMA_TypeDef *)GDMA0_BASE)
#define SP_GDMA1 	((volatile GDMA_TypeDef *)GDMA1_BASE)
#define SP_GDMA2 	((volatile GDMA_TypeDef *)GDMA2_BASE)
#define SP_GDMA3 	((volatile GDMA_TypeDef *)GDMA3_BASE)

/* IPCC Mailbox */
#define IPC_MAILBOX_BASE	RF_GRP(258, 0)
#define IPC_MAILBOX	((volatile IPCC_Typedef *)IPC_MAILBOX_BASE)

/*Input Capture*/
#define SP_ICM0 	((volatile ICM_CTRL_REG *)RF_GRP(81, 0))
#define SP_ICM1 	((volatile ICM_CTRL_REG *)RF_GRP(81, 7))
#define SP_ICM2 	((volatile ICM_CTRL_REG *)RF_GRP(81, 14))
#define SP_ICM3 	((volatile ICM_CTRL_REG *)RF_GRP(81, 21))

/*
UART LCR register BIT
*/
#define LCR_VOTE_BIT                (0x3 << 6)
#define LCR_BREAK_CONDITION_BIT     (0x1 << 5)
#define LCR_PARITY_BIT              (0x1 << 4)
#define LCR_PARITY_ENABLE_BIT       (0x1 << 3)
#define LCR_STOP_BIT_BIT            (0x1 << 2)
#define LCR_WORD_LEN_BIT            (0x3 << 0)

/* UART MCR register BIT */
#define MCR_AUTO_CALIBRATE_BIT         (0x1 << 7)
#define MCR_AUTO_CTS_BIT               (0x1 << 6)
#define MCR_AUTO_RTS_BIT               (0x1 << 5)
#define MCR_LOOP_BACK_BIT              (0x1 << 4)
#define MCR_RING_INDICATOR_BIT         (0x1 << 3)
#define MCR_DCD_BIT                    (0x1 << 2)
#define MCR_RTS_BIT                    (0x1 << 1)
#define MCR_DTS_BIT                    (0x1 << 0)

/* interrupt */
#define SP_UART_LSR_BC     (1 << 5) /* break condition */
#define SP_UART_LSR_FE     (1 << 4) /* frame error */
#define SP_UART_LSR_OE     (1 << 3) /* overrun error */
#define SP_UART_LSR_PE     (1 << 2) /* parity error */
#define SP_UART_LSR_RX     (1 << 1) /* 1: receive fifo not empty */
#define SP_UART_LSR_TX     (1 << 0) /* 1: transmit fifo is not full */

/* isc */
#define SP_UART_ISC_MSM    (1 << 7) /* Modem status ctrl */
#define SP_UART_ISC_LSM    (1 << 6) /* Line status interrupt */
#define SP_UART_ISC_RXM    (1 << 5) /* RX interrupt, when got some input data */
#define SP_UART_ISC_TXM    (1 << 4) /* TX interrupt, when trans start */
#define HAS_UART_ISC_FLAGMASK 0x0F
#define SP_UART_ISC_MS     (1 << 3)
#define SP_UART_ISC_LS     (1 << 2)
#define SP_UART_ISC_RX     (1 << 1)
#define SP_UART_ISC_TX     (1 << 0)

/* DMA-RX, dma_enable_sel */
#define DMA_INT            (1 << 31)
#define DMA_MSI_ID_SHIFT   12
#define DMA_MSI_ID_MASK    (0x7F << DMA_MSI_ID_SHIFT)
#define DMA_SEL_UARTX_SHIFT   8
#define DMA_SEL_UARTX_MASK (0x07 << DMA_SEL_UARTX_SHIFT)
#define DMA_SW_RST_B    (1 << 7)
#define DMA_INIT        (1 << 6)
#define DMA_GO          (1 << 5)
#define DMA_AUTO_ENABLE    (1 << 4)
#define DMA_TIMEOUT_INT_EN (1 << 3)
#define DMA_P_SAFE_DISABLE (1 << 2)
#define DMA_PBUS_DATA_SWAP (1 << 1)
#define DMA_ENABLE         (1 << 0)

#define UART_LSR_TRANSMIT_FIFO_STATUS        (1 << 0)
#define UART_LSR_RECEIVE_FIFO_STATUS         (1 << 1)



/* SPI MST CONFIG */
#define CLK_DIVIDER(x)         (x<<16)
#define FINISH_FLAG_MASK       (1<<15)
#define RX_FULL_FLAG_MASK      (1<<14)
#define RX_EMP_FLAG_MASK       (1<<13)
#define TX_FULL_FLAG_MASK      (1<<12)
#define TX_EMP_FLAG_MASK       (1<<11)
#define WRITE_BYTE(x)          (x<<9)
#define READ_BYTE(x)           (x<<7)
#define FD_SEL                 (1<<6)
#define CS_SEL                 (1<<5)
#define LSB_SEL                (1<<4)
#define DELAY_ENABLE           (1<<3)
#define CPHA_W                 (1<<2)
#define CPHA_R                 (1<<1)
#define CPOL                   (1<<0)

#define CLEAR_MASTER_INT      (1<<6)

#define GET_LEN(x)            ((x>>24) & 0xFF)
#define GET_TX_LEN(x)         ((x>>16) & 0xFF)
#define GET_RX_CNT(x)         ((x>>12) & 0x0F)
#define GET_TX_CNT(x)         ((x>>8)  & 0x0F)

#define TOTAL_LENGTH(x)        (x<<24)
#define TX_LENGTH(x)           (x<<16)
#define RX_CNT                 (0x0F<<12)
#define TX_CNT                 (0x0F<<12)
#define SPI_BUSY               (1<<7)
#define FINISH_FLAG            (1<<6)
#define RX_FULL_FLAG           (1<<5)
#define RX_EMP_FLAG            (1<<4)
#define TX_FULL_FLAG           (1<<3)
#define TX_EMP_FLAG            (1<<2)
#define SPI_SW_RST             (1<<1)
#define SPI_START_FD           (1<<0)

#define SPI_TOTAL_SIZE         TOTAL_LENGTH(0xFF)     
#define SPI_TX_SIZE            TX_LENGTH(0xFF)         

/******************************************************************************/
/*                                                                            */
/*                        Standard time clock                                 */
/*                                                                            */
/******************************************************************************/


/********************  Bit definition for STC register  ********************/

#define STC_PRESCALER_Pos 	(0U)
#define STC_PRESCALER_Msk	(0x7FFFU << STC_PRESCALER_Pos) 	/*0x000007FF*/
#define STC_PRESCALER		STC_PRESCALER_Msk				/*STC prescale value */
#define STC_TRIG_SRC_Pos 	(15U)							
#define STC_TRIG_SRC_Msk	(1U << STC_TRIG_SRC_Pos)		/*0x00008000*/
#define STC_TRIG_SRC		STC_TRIG_SRC_Msk				/*The triger source of the STC counter. 0:system clock; 1:externl clock*/
#define STC_EXT_DIV_Pos		(0U)	
#define STC_EXT_DIV_Msk		(0xFFU << STC_EXT_DIV_Pos)		/*0x000000FF*/
#define STC_EXT_DIV			(STC_EXT_DIV_Msk)				/*The external clock divder for STC*/		

/****************** STC Instances : All supported instances *******************/
#define IS_STC_INSTANCE(INSTANCE)       (((INSTANCE) == STC0)   || \
										           ((INSTANCE) == STC1)   || \
                                         ((INSTANCE) == STC2)   || \
                                         ((INSTANCE) == STC3))

#define IS_SPI_ALL_INSTANCE(__INSTANCE__) (((__INSTANCE__) == SPI0) || \
                                           ((__INSTANCE__) == SPI1) || \
                                           ((__INSTANCE__) == SPI2) || \
                                           ((__INSTANCE__) == SPI3))

#define IS_UART_INSTANCE(__INSTANCE__) (((__INSTANCE__) == SP_UART0) || \
                                    ((__INSTANCE__) == SP_UART1) || \
                                    ((__INSTANCE__) == SP_UART2) || \
                                    ((__INSTANCE__) == SP_UART3)  || \
                                    ((__INSTANCE__) == SP_UART4))

/******************************************************************************/
/*                                                                            */
/*                        timer/counter			                              */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for timer register  ********************/
#define TIMER_GO_Pos 			(0)
#define TIMER_GO_Msk 			(1U << TIMER_GO_Pos)			/*0x00000001*/
#define TIMER_GO				TIMER_GO_Msk					/*timer go. 1:start;0:stop*/	
#define TIMER_RPT_pos			(1U)
#define TIMER_RPT_Msk			(1U << TIMER_RPT_pos)			/*0x00000002*/	
#define TIMER_RPT				TIMER_RPT_Msk					/*timer repeat operation. 0:one shot;1: repeation*/
#define TIMER_TRIG_SRC_Pos		(2U)
#define TIMER_TRIG_SRC_Msk		(0xF << TIMER_TRIG_SRC_Pos)		/*0x0000003C*/
#define TIMER_TRIG_SRC			TIMER_TRIG_SRC_Msk				/*timer triger source selection. 0:systemclock triger;1:stc triger;2:rtc_triger;3:external clk triger;4:in a group timer prescale triger;5:in a group timer triger*/




/****************** TIM Instances : All supported instances *******************/
#define IS_TIM_INSTANCE(INSTANCE)       (((INSTANCE) == TIM0)   || \
										 ((INSTANCE) == TIM1)   || \
                                         ((INSTANCE) == TIM2)   || \
                                         ((INSTANCE) == TIM3)   || \
                                         ((INSTANCE) == TIM4)   || \
                                         ((INSTANCE) == TIM5)   || \
                                         ((INSTANCE) == TIM6)   || \
                                         ((INSTANCE) == TIM7))


/******************************************************************************/
/*                                                                            */
/*                       System clock 		                                  */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for system clock register ***************/
#define PLLSYS_NS_Pos				(0)
#define PLLSYS_NS_Msk				(0xF << PLLSYS_NS_Pos)				/*0x0000000F*/
#define PLLSYS_NS					PLLSYS_NS_Msk						/*PLL Feedback Divder control*/
#define PLLSYS_ICP_Pos				(4)
#define PLLSYS_ICP_Msk				(7U << PLLSYS_ICP_Pos)				/*0x00000070*/
#define PLLSYS_ICP					PLLSYS_ICP_Msk						/*PLL charge pump current adjust*/
#define PLLSYS_GAIN_Pos				(7)
#define PLLSYS_GAIN_Msk				(3U << PLLSYS_GAIN_Pos)				/*0x00000180*/
#define PLLSYS_BYPASS_Pos			(9)
#define PLLSYS_BYPASS_Msk			(1 << PLLSYS_BYPASS_Pos)			/*PLL Bypass 0:normal mode; 1:output 27MHz*/
#define PLLSYS_BYPASS				(PLLSYS_BYPASS_Msk)	
#define PLLSYS_POWER_EN_Pos			(10)
#define PLLSYS_POWER_EN_Msk			(1 << PLLSYS_POWER_EN_Pos)			/*0x00000400*/
#define PLLSYS_OUTCLK_DIV_Pos		(11)
#define PLLSYS_OUTCLK_DIV_Msk		(3U << PLLSYS_OUTCLK_DIV_Pos)		/*it only for pinmux or for pll system test mode*/
#define PLLSYS_OUTCLK_DIV			(PLLSYS_OUTCLK_DIV_Msk)
#define PLLSYS_CLK_BYPASS_Pos		(13)
#define PLLSYS_CLK_BYPASS_Msk		(1U << PLLSYS_CLK_BYPASS_Pos)
#define PLLSYS_CLK_BYPASS			(PLLSYS_CLK_BYPASS_Msk)

#define CLK_27M_POLARITY_Pos			(0)
#define CLK_27M_POLARITY_Msk			(1U << CLK_27M_POLARITY_Pos)			/*0x00000001*/
#define CLK_27M_POLARITY				(CLK_27M_POLARITY_Msk)				/*27MHz clock polarity adjut 0: phase same as crystal, 1:phase is inversed with crystal*/
#define CPIOCLK_POLARITY_Pos		(1)
#define CPIOCLK_POLARITY_Msk		(1U << CPIOCLK_POLARITY_Pos)
#define CPIOCLK_POLARITY			(CPIOCLK_POLARITY_Msk)
#define SPINAND_CLK_SRC_Pos			(2)
#define SPINAND_CLK_SRC_Msk			(3U << SPINAND_CLK_SRC_Pos)
#define SPINAND_CLK_SRC				(SPINAND_CLK_SRC_Msk)
#define SYSCLK_DIV_Pos				(4)
#define SYSCLK_DIV_Msk				(0xF << SYSCLK_DIV_Pos)
#define SYSCLK_DIV					(SYSCLK_DIV_Msk)
#define	SPICLK_SRC_Pos				(8)
#define SPICLK_SRC_Msk				(3U << SPICLK_SRC_Pos)
#define SPICLK_SRC					(SPICLK_SRC_Msk)
#define EXT27MCLK_EN_Pos			(10)
#define EXT27MCLK_EN_Msk			(1U << EXT27MCLK_EN_Pos)
#define EXT27MCLK_EN				(EXT27MCLK_EN_Msk)
#define ARM926CLK_DIV_Pos			(11)
#define	ARM926CLK_DIV_Msk			(0xFF << ARM926CLK_DIV_Pos)
/******************************************************************************/
/*                                                                            */
/*                        gpio/pinmux define                                  */
/*                                                                            */
/******************************************************************************/
#define GPIO_P0_00    0
#define GPIO_P0_01    1
#define GPIO_P0_02    2
#define GPIO_P0_03    3
#define GPIO_P0_04    4
#define GPIO_P0_05    5
#define GPIO_P0_06    6
#define GPIO_P0_07    7
#define GPIO_P1_00    8
#define GPIO_P1_01    9
#define GPIO_P1_02    10
#define GPIO_P1_03    11
#define GPIO_P1_04    12
#define GPIO_P1_05    13
#define GPIO_P1_06    14
#define GPIO_P1_07    15
#define GPIO_P2_00    16
#define GPIO_P2_01    17
#define GPIO_P2_02    18
#define GPIO_P2_03    19
#define GPIO_P2_04    20
#define GPIO_P2_05    21
#define GPIO_P2_06    22
#define GPIO_P2_07    23
#define GPIO_P3_00    24
#define GPIO_P3_01    25
#define GPIO_P3_02    26
#define GPIO_P3_03    27
#define GPIO_P3_04    28
#define GPIO_P3_05    29
#define GPIO_P3_06    30
#define GPIO_P3_07    31
#define GPIO_P4_00    32
#define GPIO_P4_01    33
#define GPIO_P4_02    34
#define GPIO_P4_03    35
#define GPIO_P4_04    36
#define GPIO_P4_05    37
#define GPIO_P4_06    38
#define GPIO_P4_07    39
#define GPIO_P5_00    40
#define GPIO_P5_01    41
#define GPIO_P5_02    42
#define GPIO_P5_03    43
#define GPIO_P5_04    44
#define GPIO_P5_05    45
#define GPIO_P5_06    46
#define GPIO_P5_07    47
#define GPIO_P6_00    48
#define GPIO_P6_01    49
#define GPIO_P6_02    50
#define GPIO_P6_03    51
#define GPIO_P6_04    52
#define GPIO_P6_05    53
#define GPIO_P6_06    54
#define GPIO_P6_07    55
#define GPIO_P7_00    56
#define GPIO_P7_01    57
#define GPIO_P7_02    58
#define GPIO_P7_03    59
#define GPIO_P7_04    60
#define GPIO_P7_05    61
#define GPIO_P7_06    62
#define GPIO_P7_07    63
#define GPIO_P8_00    64
#define GPIO_P8_01    65
#define GPIO_P8_02    66
#define GPIO_P8_03    67
#define GPIO_P8_04    68
#define GPIO_P8_05    69
#define GPIO_P8_06    70
#define GPIO_P8_07    71

#define IS_GPIO_PIN(pin)         (((pin) >= GPIO_P0_00) && ((pin) <= GPIO_P8_07))

#define IS_PINMUX_PIN(pin)       (((pin) >= GPIO_P1_00) && ((pin) <= GPIO_P8_07))

#define IS_VALID_PINMUX(pin)     (((pin) >= (GPIO_P1_00-7)) && ((pin) <= (GPIO_P8_07-7)))

#define GPIO_TO_PINMUX(x)        IS_PINMUX_PIN(x)?((x)-7):-1




/******************************************************************************/
/*                                                                            */
/*                       IPC Mailbox  		                                  */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for IPC Mailbox register  ***************/


#define IS_IPCC_INSTANCE(INSTANCE)       (((INSTANCE) == IPC_MAILBOX))  


#define NORMAL_LOCK_DATA0_Pos	(4)
#define NORMAL_LOCK_DATA0_Msk	(1<< NORMAL_LOCK_DATA0_Pos)
#define NORMAL_LOCK_DATA0		NORMAL_LOCK_DATA0_Msk

#define NORMAL_LOCK_DATA1_Pos	(5)
#define NORMAL_LOCK_DATA1_Msk	(1<< NORMAL_LOCK_DATA1_Pos)
#define NORMAL_LOCK_DATA1		NORMAL_LOCK_DATA1_Msk

#define NORMAL_LOCK_DATA2_Pos	(6)
#define NORMAL_LOCK_DATA2_Msk	(1<< NORMAL_LOCK_DATA2_Pos)
#define NORMAL_LOCK_DATA2		NORMAL_LOCK_DATA2_Msk

#define NORMAL_LOCK_DATA3_Pos	(7)
#define NORMAL_LOCK_DATA3_Msk	(1<< NORMAL_LOCK_DATA3_Pos)
#define NORMAL_LOCK_DATA3		NORMAL_LOCK_DATA3_Msk

#define NORMAL_LOCK_DATA4_Pos	(8)
#define NORMAL_LOCK_DATA4_Msk	(1<< NORMAL_LOCK_DATA4_Pos)
#define NORMAL_LOCK_DATA4		NORMAL_LOCK_DATA4_Msk

#define NORMAL_LOCK_DATA5_Pos	(9)
#define NORMAL_LOCK_DATA5_Msk	(1<< NORMAL_LOCK_DATA5_Pos)
#define NORMAL_LOCK_DATA5		NORMAL_LOCK_DATA5_Msk

#define NORMAL_LOCK_DATA6_Pos	(10)
#define NORMAL_LOCK_DATA6_Msk	(1<< NORMAL_LOCK_DATA6_Pos)
#define NORMAL_LOCK_DATA6		NORMAL_LOCK_DATA6_Msk

#define NORMAL_LOCK_DATA7_Pos	(11)
#define NORMAL_LOCK_DATA7_Msk	(1<< NORMAL_LOCK_DATA7_Pos)
#define NORMAL_LOCK_DATA7		NORMAL_LOCK_DATA7_Msk

#define NORMAL_LOCK_DATA8_Pos	(12)
#define NORMAL_LOCK_DATA8_Msk	(1<< NORMAL_LOCK_DATA8_Pos)
#define NORMAL_LOCK_DATA8		NORMAL_LOCK_DATA8_Msk

#define NORMAL_LOCK_DATA9_Pos	(13)
#define NORMAL_LOCK_DATA9_Msk	(1<< NORMAL_LOCK_DATA9_Pos)
#define NORMAL_LOCK_DATA9		NORMAL_LOCK_DATA9_Msk

#define NORMAL_LOCK_DATA10_Pos	(14)
#define NORMAL_LOCK_DATA10_Msk	(1<< NORMAL_LOCK_DATA10_Pos)
#define NORMAL_LOCK_DATA10		NORMAL_LOCK_DATA10_Msk

#define NORMAL_LOCK_DATA11_Pos	(15)
#define NORMAL_LOCK_DATA11_Msk	(1<< NORMAL_LOCK_DATA11_Pos)
#define NORMAL_LOCK_DATA11		NORMAL_LOCK_DATA11_Msk

#define NORMAL_LOCK_DATA12_Pos	(16)
#define NORMAL_LOCK_DATA12_Msk	(1<< NORMAL_LOCK_DATA12_Pos)
#define NORMAL_LOCK_DATA12		NORMAL_LOCK_DATA12_Msk

#define NORMAL_LOCK_DATA13_Pos	(17)
#define NORMAL_LOCK_DATA13_Msk	(1<< NORMAL_LOCK_DATA13_Pos)
#define NORMAL_LOCK_DATA13		NORMAL_LOCK_DATA13_Msk

#define NORMAL_LOCK_DATA14_Pos	(18)
#define NORMAL_LOCK_DATA14_Msk	(1<< NORMAL_LOCK_DATA14_Pos)
#define NORMAL_LOCK_DATA14		NORMAL_LOCK_DATA14_Msk

#define NORMAL_LOCK_DATA15_Pos	(19)
#define NORMAL_LOCK_DATA15_Msk	(1<< NORMAL_LOCK_DATA15_Pos)
#define NORMAL_LOCK_DATA15		NORMAL_LOCK_DATA15_Msk

#define NORMAL_LOCK_DATA16_Pos	(20)
#define NORMAL_LOCK_DATA16_Msk	(1<< NORMAL_LOCK_DATA16_Pos)
#define NORMAL_LOCK_DATA16		NORMAL_LOCK_DATA16_Msk

#define NORMAL_LOCK_DATA17_Pos	(21)
#define NORMAL_LOCK_DATA17_Msk	(1<< NORMAL_LOCK_DATA17_Pos)
#define NORMAL_LOCK_DATA17		NORMAL_LOCK_DATA17_Msk

#define NORMAL_LOCK_DATA18_Pos	(22)
#define NORMAL_LOCK_DATA18_Msk	(1<< NORMAL_LOCK_DATA18_Pos)
#define NORMAL_LOCK_DATA18		NORMAL_LOCK_DATA18_Msk

#define NORMAL_LOCK_DATA19_Pos	(23)
#define NORMAL_LOCK_DATA19_Msk	(1<< NORMAL_LOCK_DATA19_Pos)
#define NORMAL_LOCK_DATA19		NORMAL_LOCK_DATA19_Msk


#define DIRECT_LOCK_DATA0_Pos	(24)
#define DIRECT_LOCK_DATA0_Msk	(1<< DIRECT_LOCK_DATA0_Pos)
#define DIRECT_LOCK_DATA0		DIRECT_LOCK_DATA0_Msk

#define DIRECT_LOCK_DATA1_Pos	(25)
#define DIRECT_LOCK_DATA1_Msk	(1<< DIRECT_LOCK_DATA1_Pos)
#define DIRECT_LOCK_DATA1		DIRECT_LOCK_DATA1_Msk

#define DIRECT_LOCK_DATA2_Pos	(26)
#define DIRECT_LOCK_DATA2_Msk	(1<< DIRECT_LOCK_DATA2_Pos)
#define DIRECT_LOCK_DATA2		DIRECT_LOCK_DATA2_Msk

#define DIRECT_LOCK_DATA3_Pos	(27)
#define DIRECT_LOCK_DATA3_Msk	(1<< DIRECT_LOCK_DATA3_Pos)
#define DIRECT_LOCK_DATA3		DIRECT_LOCK_DATA3_Msk

#define DIRECT_LOCK_DATA4_Pos	(28)
#define DIRECT_LOCK_DATA4_Msk	(1<< DIRECT_LOCK_DATA4_Pos)
#define DIRECT_LOCK_DATA4		DIRECT_LOCK_DATA4_Msk

#define DIRECT_LOCK_DATA5_Pos	(29)
#define DIRECT_LOCK_DATA5_Msk	(1<< DIRECT_LOCK_DATA5_Pos)
#define DIRECT_LOCK_DATA5		DIRECT_LOCK_DATA5_Msk

#define DIRECT_LOCK_DATA6_Pos	(30)
#define DIRECT_LOCK_DATA6_Msk	(1<< DIRECT_LOCK_DATA6_Pos)
#define DIRECT_LOCK_DATA6		DIRECT_LOCK_DATA6_Msk

#define DIRECT_LOCK_DATA7_Pos	(31)
#define DIRECT_LOCK_DATA7_Msk	(1<< DIRECT_LOCK_DATA7_Pos)
#define DIRECT_LOCK_DATA7		DIRECT_LOCK_DATA7_Msk

#define NORMAL_OVERWRITE_DATA0_Pos	(4)
#define NORMAL_OVERWRITE_DATA0_Msk	(1<< NORMAL_OVERWRITE_DATA0_Pos)
#define NORMAL_OVERWRITE_DATA0		NORMAL_LOCK_DATA0_Msk

#define NORMAL_OVERWRITE_DATA1_Pos	(5)
#define NORMAL_OVERWRITE_DATA1_Msk	(1<< NORMAL_OVERWRITE_DATA1_Pos)
#define NORMAL_OVERWRITE_DATA1		NORMAL_LOCK_DATA1_Msk

#define NORMAL_OVERWRITE_DATA2_Pos	(6)
#define NORMAL_OVERWRITE_DATA2_Msk	(1<< NORMAL_OVERWRITE_DATA2_Pos)
#define NORMAL_OVERWRITE_DATA2		NORMAL_LOCK_DATA2_Msk

#define NORMAL_OVERWRITE_DATA3_Pos	(7)
#define NORMAL_OVERWRITE_DATA3_Msk	(1<< NORMAL_OVERWRITE_DATA3_Pos)
#define NORMAL_OVERWRITE_DATA3		NORMAL_LOCK_DATA3_Msk

#define NORMAL_OVERWRITE_DATA4_Pos	(8)
#define NORMAL_OVERWRITE_DATA4_Msk	(1<< NORMAL_OVERWRITE_DATA4_Pos)
#define NORMAL_OVERWRITE_DATA4		NORMAL_LOCK_DATA4_Msk

#define NORMAL_OVERWRITE_DATA5_Pos	(9)
#define NORMAL_OVERWRITE_DATA5_Msk	(1<< NORMAL_OVERWRITE_DATA5_Pos)
#define NORMAL_OVERWRITE_DATA5		NORMAL_LOCK_DATA5_Msk

#define NORMAL_OVERWRITE_DATA6_Pos	(10)
#define NORMAL_OVERWRITE_DATA6_Msk	(1<< NORMAL_OVERWRITE_DATA6_Pos)
#define NORMAL_OVERWRITE_DATA6		NORMAL_LOCK_DATA6_Msk

#define NORMAL_OVERWRITE_DATA7_Pos	(11)
#define NORMAL_OVERWRITE_DATA7_Msk	(1<< NORMAL_OVERWRITE_DATA7_Pos)
#define NORMAL_OVERWRITE_DATA7		NORMAL_LOCK_DATA7_Msk

#define NORMAL_OVERWRITE_DATA8_Pos	(12)
#define NORMAL_OVERWRITE_DATA8_Msk	(1<< NORMAL_OVERWRITE_DATA8_Pos)
#define NORMAL_OVERWRITE_DATA8		NORMAL_LOCK_DATA8_Msk

#define NORMAL_OVERWRITE_DATA9_Pos	(13)
#define NORMAL_OVERWRITE_DATA9_Msk	(1<< NORMAL_OVERWRITE_DATA9_Pos)
#define NORMAL_OVERWRITE_DATA9		NORMAL_LOCK_DATA9_Msk

#define NORMAL_OVERWRITE_DATA10_Pos	(14)
#define NORMAL_OVERWRITE_DATA10_Msk	(1<< NORMAL_OVERWRITE_DATA10_Pos)
#define NORMAL_OVERWRITE_DATA10		NORMAL_LOCK_DATA10_Msk

#define NORMAL_OVERWRITE_DATA11_Pos	(15)
#define NORMAL_OVERWRITE_DATA11_Msk	(1<< NORMAL_OVERWRITE_DATA11_Pos)
#define NORMAL_OVERWRITE_DATA11		NORMAL_LOCK_DATA11_Msk

#define NORMAL_OVERWRITE_DATA12_Pos	(16)
#define NORMAL_OVERWRITE_DATA12_Msk	(1<< NORMAL_OVERWRITE_DATA12_Pos)
#define NORMAL_OVERWRITE_DATA12		NORMAL_LOCK_DATA12_Msk

#define NORMAL_OVERWRITE_DATA13_Pos	(17)
#define NORMAL_OVERWRITE_DATA13_Msk	(1<< NORMAL_OVERWRITE_DATA13_Pos)
#define NORMAL_OVERWRITE_DATA13		NORMAL_LOCK_DATA13_Msk

#define NORMAL_OVERWRITE_DATA14_Pos	(18)
#define NORMAL_OVERWRITE_DATA14_Msk	(1<< NORMAL_OVERWRITE_DATA14_Pos)
#define NORMAL_OVERWRITE_DATA14		NORMAL_LOCK_DATA14_Msk

#define NORMAL_OVERWRITE_DATA15_Pos	(19)
#define NORMAL_OVERWRITE_DATA15_Msk	(1<< NORMAL_OVERWRITE_DATA15_Pos)
#define NORMAL_OVERWRITE_DATA15		NORMAL_LOCK_DATA15_Msk

#define NORMAL_OVERWRITE_DATA16_Pos	(20)
#define NORMAL_OVERWRITE_DATA16_Msk	(1<< NORMAL_OVERWRITE_DATA16_Pos)
#define NORMAL_OVERWRITE_DATA16		NORMAL_LOCK_DATA16_Msk

#define NORMAL_OVERWRITE_DATA17_Pos	(21)
#define NORMAL_OVERWRITE_DATA17_Msk	(1<< NORMAL_OVERWRITE_DATA17_Pos)
#define NORMAL_OVERWRITE_DATA17		NORMAL_LOCK_DATA17_Msk

#define NORMAL_OVERWRITE_DATA18_Pos	(22)
#define NORMAL_OVERWRITE_DATA18_Msk	(1<< NORMAL_OVERWRITE_DATA18_Pos)
#define NORMAL_OVERWRITE_DATA18		NORMAL_LOCK_DATA18_Msk

#define NORMAL_OVERWRITE_DATA19_Pos	(23)
#define NORMAL_OVERWRITE_DATA19_Msk	(1<< NORMAL_OVERWRITE_DATA19_Pos)
#define NORMAL_OVERWRITE_DATA19		NORMAL_LOCK_DATA19_Msk


#define DIRECT_OVERWITE_DATA0_Pos	(24)
#define DIRECT_OVERWITE_DATA0_Msk	(1<< DIRECT_OVERWITE_DATA0_Pos)
#define DIRECT_OVERWITE_DATA0		DIRECT_OVERWITE_DATA0_Msk

#define DIRECT_OVERWITE_DATA1_Pos	(25)
#define DIRECT_OVERWITE_DATA1_Msk	(1<< DIRECT_OVERWITE_DATA1_Pos)
#define DIRECT_OVERWITE_DATA1		DIRECT_OVERWITE_DATA1_Msk

#define DIRECT_OVERWITE_DATA2_Pos	(26)
#define DIRECT_OVERWITE_DATA2_Msk	(1<< DIRECT_OVERWITE_DATA2_Pos)
#define DIRECT_OVERWITE_DATA2		DIRECT_OVERWITE_DATA2_Msk

#define DIRECT_OVERWITE_DATA3_Pos	(27)
#define DIRECT_OVERWITE_DATA3_Msk	(1<< DIRECT_OVERWITE_DATA3_Pos)
#define DIRECT_OVERWITE_DATA3		DIRECT_OVERWITE_DATA3_Msk

#define DIRECT_OVERWITE_DATA4_Pos	(28)
#define DIRECT_OVERWITE_DATA4_Msk	(1<< DIRECT_OVERWITE_DATA4_Pos)
#define DIRECT_OVERWITE_DATA4		DIRECT_OVERWITE_DATA4_Msk

#define DIRECT_OVERWITE_DATA5_Pos	(29)
#define DIRECT_OVERWITE_DATA5_Msk	(1<< DIRECT_OVERWITE_DATA5_Pos)
#define DIRECT_OVERWITE_DATA5		DIRECT_OVERWITE_DATA5_Msk

#define DIRECT_OVERWITE_DATA6_Pos	(30)
#define DIRECT_OVERWITE_DATA6_Msk	(1<< DIRECT_OVERWITE_DATA6_Pos)
#define DIRECT_OVERWITE_DATA6		DIRECT_OVERWITE_DATA6_Msk

#define DIRECT_OVERWITE_DATA7_Pos	(31)
#define DIRECT_OVERWITE_DATA7_Msk	(1<< DIRECT_OVERWITE_DATA7_Pos)
#define DIRECT_OVERWITE_DATA7		DIRECT_OVERWITE_DATA7_Msk

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SP7021_ARM926_H */


