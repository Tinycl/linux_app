#include <stdlib.h>
#include <stdio.h>

#define BIT7 0x80
#define BIT6 0x40
#define BIT5 0x20
#define BIT4 0x10
#define BIT3 0x08
#define BIT2 0x04 
#define BIT1 0x02
#define BIT0 0x01
 
#define SMBUS_BASE_ADDRESS 0x400

#define SMIO_SMBUS_HOST_STA 0x0   // SMBus Host Status
    #define SMIO_HT_PECERR BIT7 // 0/0/x/x SMB Host PEC Error
    #define SMIO_INUSE BIT6 // 0/x/x/x SMB Semaphore
    #define SMIO_NSMBSRST BIT5 // 0/0/x/x New SMBus Softwate Software Reset
    #define SMIO_FAILED BIT4 // 0/0/x/x Failed Bus Transaction
    #define SMIO_BUS_ERR BIT3 // 0/0/x/x Bus Collision
    #define SMIO_DEV_ERR BIT2 // 0/0/x/x Device Error
    #define SMIO_INTER BIT1 // 0/0/x/x SMBus Interrupt



#define SMIO_HOST_BUSY BIT0 // 0/R/x/x Host Busy
	
#define SMIO_SMBUS_SLAVE_STA 0x1 // SMBus Slave Status
    #define SMIO_GP_PECERR BIT7 // 0/0/x/x SMB GPIO Slave PEC Error
    #define SMIO_SL_PECERR BIT6 // 0/0/x/x SMB Host Slave PEC Error
    #define SMIO_ALERT_STS BIT5 // 0/0/x/x Alert Status
    #define SMIO_SHDW2_STS BIT4 // 0/0/x/x Shadow 2 Status
    #define SMIO_SHDW1_STS BIT3 // 0/0/x/x Shadow 1 Status
    #define SMIO_SLV_STS BIT2 // 0/0/x/x Slave Status
    #define SMIO_RS01_1 BIT1 // 0/R/x/x Reserved
    #define SMIO_SLV_BSY BIT0 // 0/R/x/x Slave Busy
	
#define SMIO_SMBUS_HOST_CTL 0x2 // SMBus Host Control
    #define SMIO_HT_PECEN BIT7 // 0/x/x/x PEC Enable
    #define SMIO_START BIT6 // 0/x/x/x Start
    #define SMIO_SMB_CMD_PROT 0x3C // 0000b/x/x/x SMBus Command Protocol
    #define SMIO_KILL BIT1 // 0/x/x/x Kill Transaction in Progress
    #define SMIO_INTEREN BIT0 // 0/x/x/x Interrupt Enable
	
#define SMIO_SMBUS_HOST_CMD 0x3 // SMBus Host Command
    #define SMIO_HST_CMD SMIO_SMBUS_HOST_CMD // 0/x/x/x SMBus Host Command
	
#define SMIO_SMBUS_HOST_ADR 0x4 // SMBus Host Address
    #define SMIO_SMB_ADRESS 0xFE // 0/x/x/x SMBus Address
    #define SMIO_SMB_RW BIT0 // 0/x/x/x SMBus Read or Write
	
#define SMIO_SMBUS_HOST_DATA_0 0x5 // SMBus Host Data 0
    #define SMIO_SMBD0 SMIO_SMBUS_HOST_DATA_0 // 0/x/x/x SMBus Data 0
	
#define SMIO_SMBUS_HOST_DATA_1 0x6 // SMBus Host Data 1
    #define SMIO_SMBD1 SMIO_SMBUS_HOST_DATA_1 // 0/x/x/x SMBus Data 1
	
#define SMIO_SMBUS_BLOCK_DATA 0x7 // SMBus Block Data
    #define SMIO_BLK_DATA SMIO_SMBUS_BLOCK_DATA // 0/x/x/x SMBus Block Data Byte
	
#define SMIO_SMBUS_SLAVE_CTL 0x8 // SMBus Slave Control
    #define SMIO_GP_PECEN BIT7 // 0/x/x/x SMBus GPIO Slave PEC Enable
    #define SMIO_SL_PECEN BIT6 // 0/x/x/x SMBus Host Slave PEC Enable
    #define SMIO_PECABORT BIT5 // 0/x/x/x PEC Abort
    #define SMIO_GPIO_EN BIT4 // 0/x/x/x SMBus GPIO Slave Enable 
    #define SMIO_ALERT_EN BIT3 // 0/x/x/x SMBus Alert Enable
    #define SMIO_SHDW2_EN BIT2 // 0/x/x/x SMBus Shadow Port 2 Enable
    #define SMIO_SHDW1_EN BIT1 // 0/x/x/x SMBus Shadow Port 1 Enable
    #define SMIO_SLV_EN BIT0 // 0/x/x/x SMBus Slave Enable
	
#define SMIO_SMBUS_SHADOW_CMD 0x9  // SMBus Shadow Command
    #define SMIO_SHDW_CMD SMIO_SMBUS_SHADOW_CMD // 0/R/x/x Shadow Command
	
#define SMIO_SMBUS_SLAVE_EVENT 0xA // SMBus Slave Event
    #define SMIO_SMB_SLV_EVT SMIO_SMBUS_SLAVE_EVENT // 0/x/x/x SMBus Slave Event
	
#define SMIO_SMBUS_SLAVE_DATA 0xC // SMBus Slave Data
    #define SMIO_SMB_SLV_DATA SMIO_SMBUS_SLAVE_DATA // 0/R/x/x SMBus Slave Data
	
#define SMIO_SMBUS_GPIO_SLAVE_ADR 0xF // SMBus GPIO Slave Address
    #define SMIO_GPIOADD 0xFE // 0/x/x/x SMBus GPIO Slave Address
    #define SMIO_RS0F_0 BIT0 // 0/R/x/x Reserved
	
#define SmbusWriteByte  0x1	
#define SmbusWriteWord  0x2
#define SmbusReadByte  0x3
#define SmbusReadWord  0x4
	
#define SMBCMD_QUICK 0x00
#define SMBCMD_BYTE 0x04
#define SMBCMD_BYTE_DATA 0x08
#define SMBCMD_WORD_DATA 0x0C
#define SMBCMD_PROCESS_CALL 0x10
#define SMBCMD_BLOCK 0x14
#define SMBCMD_IIC_10BIT 0x18
#define SMBCMD_IIC_PCALL 0x30
#define SMBCMD_IIC_BLOCK 0x34
#define SMBCMD_IIC_7BIT 0x38

#define SMIO_START BIT6 // 0/x/x/x Start


