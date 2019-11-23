#include <stdlib.h>
#include <stdio.h>
#include <sys/io.h>
#include <string.h>
#include <unistd.h>

#include "smbus.h"

typedef unsigned char  UINT8;
typedef unsigned short UINT16;
typedef unsigned int   UINT32;
//iopl(3);
int SmbusExec (UINT8 SlaveAddress, UINT8 Command, UINT8 Operation,  UINT8 Length, UINT8 *Buffer)
{
UINT16 SmbusBase;
int  Status;
UINT8  SmbusCommandReg;
UINT8  SlaveAddressReg;
UINT8  HostCommandReg;
UINT8  *DataBuf;
UINT8  SmbusSts;
UINT8  smbuscmd;
//BOOLEAN SmallBuffer;
//UINT32 Index;
//UINT32 RetryCnt;
//UINT8 Data8;
//UINT32 Count;

Status = 1;

SmbusBase = SMBUS_BASE_ADDRESS;

DataBuf = Buffer;
SmbusSts = 0;
//BlockCount = 0;

//
// Reset before next execute.  for only CHX001
//outb(0x20,SmbusBase+SMIO_SMBUS_HOST_STA); 
//outb(0x00,SmbusBase+SMIO_SMBUS_HOST_STA); 


  //Status = EFI_SUCCESS;
  SmbusCommandReg = SMBCMD_QUICK;
  SlaveAddressReg = (UINT8) ((SlaveAddress << 1) | 1);///default Execute as a READ command 
  HostCommandReg = (UINT8) Command;

  switch (Operation)
  {

   case SmbusWriteByte: 
    SmbusCommandReg = SMBCMD_BYTE_DATA;  
    SlaveAddressReg--;
    outb(DataBuf[0], SmbusBase+ SMIO_SMBUS_HOST_DATA_0); //SMIO Rx05
    break;

   case SmbusWriteWord:
    SmbusCommandReg = SMBCMD_WORD_DATA;
    SlaveAddressReg--;
    outb(DataBuf[1], SmbusBase+ SMIO_SMBUS_HOST_DATA_1); //SMIO Rx06
    outb(DataBuf[0], SmbusBase+ SMIO_SMBUS_HOST_DATA_0); //SMIO Rx05
    //if (Length < 2)
    //{
    //  Status = 0;
    //}
    //Length = 2;
    break;
	
  case SmbusReadByte:
        SmbusCommandReg = SMBCMD_BYTE_DATA;
	SlaveAddressReg |= BIT0;// 1:Execute a READ command 
    //if (Length < 1)
    //{
    //  Status = EFI_BUFFER_TOO_SMALL;
    //}
    //Length = 1;    
       break;

  case SmbusReadWord:
    SmbusCommandReg = SMBCMD_WORD_DATA;
	SlaveAddressReg |= BIT0;// 1:Execute a READ command 
    //if (Length < 2)
    //{
    //  Status = EFI_BUFFER_TOO_SMALL;
    //}
    //Length = 2;
    break;
	
  default:
    Status = 0;
    break;
  }

  if (Status==0)
  {
    printf("not valid command");
    return Status;
  }

 // configure SMbus IO space
 

    smbuscmd=inb(SmbusBase+ SMIO_SMBUS_HOST_CTL); //SMIO Rx02
    outb( 0xDE,SmbusBase+ SMIO_SMBUS_HOST_STA); //SMIO Rx00
    outb( SlaveAddressReg,SmbusBase+ SMIO_SMBUS_HOST_ADR); //SMIO Rx04       
    outb( HostCommandReg,SmbusBase+ SMIO_SMBUS_HOST_CMD); //SMIO Rx03
       
    outb((UINT8)(SMIO_START |smbuscmd | SmbusCommandReg ), SmbusBase+ SMIO_SMBUS_HOST_CTL); //SMIO Rx02    
    //outb((UINT8) SMIO_START, SmbusBase+ SMIO_SMBUS_HOST_CTL);   //Start Execution of Command
  


  // detect device Error
   SmbusSts = inb(SmbusBase+ SMIO_SMBUS_HOST_STA); //SMIO Rx00
   if((SmbusSts & 0x04)==0x04)
   {
     printf(" device error \n");
     exit(1);
   }

  //
  // Wait SMBus process complete
  //
  SmbusSts = inb(SmbusBase+ SMIO_SMBUS_HOST_STA); //SMIO Rx00
  while ((SmbusSts & 0x1)==0x1)
  {
    SmbusSts = inb(SmbusBase+ SMIO_SMBUS_HOST_STA);
  }
  
  switch (Operation)
  {
	  case SmbusReadByte:
		DataBuf[0] = inb(SmbusBase+ SMIO_SMBUS_HOST_DATA_0); //SMIO Rx05
		break;
	  case SmbusReadWord:
		DataBuf[0] = inb(SmbusBase+ SMIO_SMBUS_HOST_DATA_0); //SMIO Rx05
		DataBuf[1] = inb(SmbusBase+ SMIO_SMBUS_HOST_DATA_1); //SMIO Rx06
		break;
	  default:
		break;
  }

outb(0xDE, SmbusBase+ SMIO_SMBUS_HOST_STA); //SMIO Rx00
return Status; 
}

void help(char *s)
{
	printf("\n%s\n\n",s);
	printf(" -rdbyte  device_address  offset               \n");
	printf(" -wrbyte  device_address  offset  data         \n");
	printf(" -rdword  device_address  offset               \n");
	printf(" -wrword  device_address  offset  data0 data1  \n");
    exit(1);
}

main(int argc, char* argv[])
{
	
	UINT8 device_address, offset, data_low, data_high;
	UINT8 *data;
	UINT8 operation;

	iopl(3);	

	data=(UINT8 *)malloc(2);	
	if(data==NULL) {printf("not success malloc\n"); exit(1);}

	memset(data,0x0,2);

	for(int i=1; i<argc; i++)
	{
		
		if (0 == strcmp("-h",argv[i]))                
		{
			help("");
		} 
		else if (0 == strcmp("-rdbyte",argv[i]))           
		{   
			if(argc!=4)
			{
				help("error parameter, please input the correct parameters");
			}
			device_address=strtoul (argv[++i],NULL,16);
			offset=strtoul (argv[++i],NULL,16);
			operation=0x3;   //#define SmbusReadByte  0x3
			
		} 
		else if (0 == strcmp("-wrbyte",argv[i]))           
		{ 
			if(argc!=5)
			{
				help("error parameter, please input the correct parameters");
			}
			device_address=strtoul (argv[++i],NULL,16);
			offset=strtoul (argv[++i],NULL,16);
			data_low=strtoul (argv[++i],NULL,16);
			operation=0x1;   ///#define SmbusWriteByte  0x1	
		} 
		else if (0 == strcmp("-rdword",argv[i]))           
		{ 
			if(argc!=4)
			{
				help("error parameter, please input the correct parameters");
			}
			device_address=strtoul (argv[++i],NULL,16);
			offset=strtoul (argv[++i],NULL,16);
			operation=0x4;    //#define SmbusReadWord  0x4 
		} 
		else if (0 == strcmp("-wrword",argv[i]))           
		{ 
			if(argc!=6)
			{
				help("error parameter, please input the correct parameters");
			}
			device_address=strtoul (argv[++i],NULL,16);
			offset=strtoul (argv[++i],NULL,16);
			data_low=strtoul (argv[++i],NULL,16);
			data_high=strtoul (argv[++i],NULL,16);
			operation=0x2;     //#define SmbusWriteWord  0x2
		} 
		else
		{
			help("error command, please input correct command");	
		}		
	}
	 
	 
	 if(operation==1)   ///#define SmbusWriteByte  0x1	
	 {
		 data[0]=data_low;
		 SmbusExec (device_address, offset, SmbusWriteByte, 0x1, data);
		 sleep(1);
		 printf("Success write smbus\n");
		 memset(data,0x0,2);
		 SmbusExec (device_address, offset, SmbusReadByte, 0x1, data);
		 printf("SmbusWriteByte: device(%2X) offset(%2X) data(%2X)\n",device_address,offset,data[0]);
		 	 
	 }
	 else if(operation==2)   //#define SmbusWriteWord  0x2
	 {
		 data[0]=data_low;
		 data[1]=data_high;
		 SmbusExec (device_address, offset, SmbusWriteWord, 0x2, data);
		 sleep(1);
		 printf("Success write smbus\n");
		 memset(data,0x0,2);
		 SmbusExec (device_address, offset, SmbusReadWord, 0x1, data);
		 printf("SmbusWriteWord: device(%2X) offset(%2X) data(%2X%2X)\n",device_address,offset,data[1],data[0]);
		 
	 }
	 else if(operation==3)   //#define SmbusReadByte  0x3
	 {
		 SmbusExec (device_address, offset, SmbusReadByte, 0x1, data);
		 printf("SmbusReadByte: device(%2X) offset(%2X) data(%2X)\n",device_address,offset,data[0]);	 
	 }
	 else if(operation==4)  //#define SmbusReadWord  0x4
	 {
		 SmbusExec (device_address, offset, SmbusReadWord, 0x2, data);
		 printf("SmbusReadWord: device(%2X) offset(%2X) data(%2X%2X)\n",device_address,offset,data[1],data[0]);		 
	 }
	 else
	 {
		 help("error format, please input the correct format");
	 }

	

	return 0;
}
