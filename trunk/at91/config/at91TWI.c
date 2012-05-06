/* at91TWI.c - AT91RM9200 IIC driver */

/* Copyright 1984-1997 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------


*/

/*
DESCRIPTION

INCLUDES:
at91TWI.h
*/

/* includes */

#include "vxWorks.h"
#include "types\vxTypesBase.h"
#include "types\vxTypes.h"
#include "iv.h"
#include "intLib.h"
#include "sysLib.h"
#include "taskLib.h"
#include "semLib.h"
#include "at91TWI.h"

/* Local data structures */

LOCAL SNDS_IIC at91TWI;

/* forward static declarations */
LOCAL UINT32 setPreScaler(UINT32 sclk);
LOCAL void at91TWIWriteISR();
LOCAL void at91TWIReadISR();

/*******************************************************************************
*
* at91TWIDevInit - initialize the IIC driver
*
* This routine initializes the IIC driver and the device to the operational state.
*
* RETURNS: N/A
*/
void at91TWIDevInit()
	{
	UINT32 nPreScaleValue;

	at91TWI.int_vec = INT_LVL_IIC;
	at91TWI.iiccon = (UINT32 *)SNDS_IICCON;
	at91TWI.iicbuf = (UINT32 *)SNDS_IICBUF;
	at91TWI.iicps = (UINT32 *)SNDS_IICPS;
	at91TWI.iiccnt = (UINT32 *)SNDS_IICCNT;

	/* Reset the IICCON Registers */
	SNDS_IIC_REG_WRITE (iiccon, SNDS_IIC_RESET_IICCON);

	/* Set the Prescaler value */
	nPreScaleValue = setPreScaler(SCLK);
	SNDS_IIC_REG_WRITE (iicps, nPreScaleValue);
	}

/*******************************************************************************
*
* at91TWIDevInit2 - Initialize the semaphores used by IIC driver
*
* This routine creates the semaphores used by the IIC driver.  This function can
* be called only after the kernel is initialized, preferrably from sysHwInit2()
*
* RETURNS: N/A
*/
void at91TWIDevInit2()
	{
	/* Create mutual semaphore to guard the global SNDS_IIC structure */
	at91TWI.IICSemID = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE | SEM_INVERSION_SAFE);
	
	/* Synchronisation semaphores */
	at91TWI.IIC_Txmit.endofWriteSemID = semBCreate (SEM_Q_FIFO, SEM_EMPTY); 
	at91TWI.IIC_Rcv.endofReadSemID = semBCreate (SEM_Q_FIFO, SEM_EMPTY); 
	}

/*******************************************************************************
*
* at91TWIWrite - Write data into the IIC device
*
* This routine writes the data given by the application into the IIC device.
*
* RETURNS: OK if write is successful, else ERROR
*/
STATUS at91TWIWrite (UINT8 slaveAddr, UINT16 writeAddr, UINT8 *data, UINT32 sizeOfData)
	{
	UINT32 page = 0;
	UINT32 numPages = 0;
	UINT32 remain_byte = 0;
	UINT16 pageAccessAddr = 0;
	UINT8 bEndWrite = 0;
	UINT32 status;
	STATUS nRetVal = OK;
	UINT32 j=0;

	/* Validation of Input Parameters */
	if (slaveAddr != EEPROM_SLAVE_ADDR)	/* At present, only EEPROM is supported on IIC bus on SNDS board */
		return(ERROR);
	if (writeAddr >= EEPROM_MAX_SIZE)
		return(ERROR);
	if (data == NULL)		
		return(ERROR);
	
	/* Connect the IIC Bus interrupt to WriteISR */
   	if (intConnect (INUM_TO_IVEC (at91TWI.int_vec), at91TWIWriteISR, NULL) == ERROR)
		return(ERROR);
   	if (intEnable (at91TWI.int_vec) == ERROR) /*Enable the IIC bus interrupt */
		return(ERROR);

	pageAccessAddr = writeAddr;
	at91TWI.IIC_Txmit.slaveAddress = slaveAddr;
	at91TWI.IIC_Txmit.totalBytesWritten = 0;

	/* semTake */
	if (semTake (at91TWI.IICSemID, WAIT_FOREVER) == ERROR)
		return(ERROR);

	if ((sizeOfData % EEPROM_PAGE_SIZE) != 0)				/* Calculate no of page write operations */
		numPages = (sizeOfData / EEPROM_PAGE_SIZE) + 1;
	else
		numPages = sizeOfData / EEPROM_PAGE_SIZE;

	remain_byte = (UINT32)(sizeOfData % EEPROM_PAGE_SIZE); /* Calculate size of data in last page */
	if (!remain_byte)
		remain_byte = EEPROM_PAGE_SIZE;

	/* write the data page by page with 20ms delay provided for internal write operation by 
	 * the slave device after each page
	 */
	for (page = 1; (page <= numPages) && (bEndWrite == 0); page++)
		{
		if (pageAccessAddr > EEPROM_MAX_SIZE)
			{
			break;
			}

		if (sizeOfData < EEPROM_PAGE_SIZE) /* if only one page to be written */
			{
			for(j = 0; j < sizeOfData; j++)	
				at91TWI.IIC_Txmit.pageBuff[j] = *data++;
			at91TWI.IIC_Txmit.dataSize = sizeOfData;

			/* If (current write pointer address + no. of data) > max offset, 
			 * omit out of range data 
			 */
			if ((at91TWI.IIC_Txmit.dataSize + pageAccessAddr) > EEPROM_MAX_SIZE)
				{
				at91TWI.IIC_Txmit.dataSize = (EEPROM_MAX_SIZE - pageAccessAddr);
				bEndWrite = 1;
				}
			}
		else 
			{
			/* if last page */
			if(page == numPages)
				{
				for(j=0; j < remain_byte; j++)
					{
					at91TWI.IIC_Txmit.pageBuff[j] = *data++;
					}
				at91TWI.IIC_Txmit.dataSize = remain_byte;

				/* If (current write pointer address + no. of data) > max offset, 
			  	 *  omit out of range data 
				 */
				if ((at91TWI.IIC_Txmit.dataSize + pageAccessAddr) > EEPROM_MAX_SIZE)
					{
					at91TWI.IIC_Txmit.dataSize = (EEPROM_MAX_SIZE - pageAccessAddr);
					bEndWrite = 1;
					}
				}
			/* if still more pages */
			else 
				{
				for(j = 0; j < EEPROM_PAGE_SIZE; j++)
					at91TWI.IIC_Txmit.pageBuff[j] = *data++;
				at91TWI.IIC_Txmit.dataSize = EEPROM_PAGE_SIZE;

				/* If (current write pointer address + no. of data) > max offset, 
				 *  omit out of range data 
				 */
				if ((at91TWI.IIC_Txmit.dataSize + pageAccessAddr) > EEPROM_MAX_SIZE)
					{
					at91TWI.IIC_Txmit.dataSize = (EEPROM_MAX_SIZE - pageAccessAddr);
					bEndWrite = 1;
					}
				}
			}
		at91TWI.IIC_Txmit.flag = 0x0;
		at91TWI.IIC_Txmit.byteWriteCount = 0;
		at91TWI.IIC_Txmit.writeAddrMSB = (UINT8)((pageAccessAddr>>8) & 0xff);
		at91TWI.IIC_Txmit.writeAddrLSB = (UINT8)(pageAccessAddr & 0xff);
		do 
			{
			SNDS_IIC_REG_READ (iiccon, status);
			} while (status & (UINT8)SNDS_IIC_ISBUSY); /* Wait until the bus is busy */

		/* Send Start  bit. Enable the interrupt mode and Enable Ack */
		SNDS_IIC_REG_WRITE (iiccon, (SNDS_IIC_START | SNDS_IIC_ACK | SNDS_IIC_IEN));

		/* Send the slave address */
		SNDS_IIC_REG_WRITE (iicbuf, (at91TWI.IIC_Txmit.slaveAddress & S_WRITE));

		/* Wait till one page of data is written */
		if (semTake (at91TWI.IIC_Txmit.endofWriteSemID, WAIT_FOREVER) == ERROR)
			{
			nRetVal = ERROR;
			break;
			}

		if (taskDelay(sysClkRateGet() / 50 ) == ERROR) /* to give 5ms delay used for internal write operation */
			{
			nRetVal = ERROR;
			break;
			}
		pageAccessAddr += EEPROM_PAGE_SIZE;
		}

	if (intDisable (at91TWI.int_vec) == ERROR) /*Enable the IIC bus interrupt */
		return(ERROR);

	/* semGive */
	if (semGive (at91TWI.IICSemID) == ERROR)
		return(ERROR);

	if (nRetVal == ERROR)
		return(ERROR);
	else
		return (at91TWI.IIC_Txmit.totalBytesWritten);
	}

/*******************************************************************************
*
* at91TWIWriteISR() - Handle IIC write interrupt
*
* This ISR writes one page of data (32 bytes) into the serial EEPROM.  After the 
* write of a page is complete, the ISR gives the semaphore.
*
* RETURNS: N/A
*/
LOCAL void at91TWIWriteISR()
	{
	if (!(at91TWI.IIC_Txmit.flag & (UINT8)IIC_TX_BYTE_ADDR_MSB))
		{
		SNDS_IIC_REG_WRITE (iicbuf, at91TWI.IIC_Txmit.writeAddrMSB);
		at91TWI.IIC_Txmit.flag |= (UINT8)IIC_TX_BYTE_ADDR_MSB;
		}
	else if (!(at91TWI.IIC_Txmit.flag & (UINT8)IIC_TX_BYTE_ADDR_LSB))
		{
		SNDS_IIC_REG_WRITE (iicbuf, at91TWI.IIC_Txmit.writeAddrLSB);		
		at91TWI.IIC_Txmit.flag |= (UINT8) IIC_TX_BYTE_ADDR_LSB;
		}
	else if ((at91TWI.IIC_Txmit.byteWriteCount < (UINT32) at91TWI.IIC_Txmit.dataSize))
		{
		SNDS_IIC_REG_WRITE (iicbuf, at91TWI.IIC_Txmit.pageBuff[at91TWI.IIC_Txmit.byteWriteCount++]);
		at91TWI.IIC_Txmit.totalBytesWritten++;
		}
	else
		{
		SNDS_IIC_REG_WRITE (iiccon, SNDS_IIC_STOP);
		semGive (at91TWI.IIC_Txmit.endofWriteSemID);
		}
	}

/*******************************************************************************
*
* at91TWIRead - Read data from the IIC device
*
* This routine reads the required amount of data requested by the application from 
* the IIC device.  The data is copied into the buffer provided by the application
*
* RETURNS: OK if read is successful, else ERROR
*/
STATUS at91TWIRead (UINT8 slaveAddr, UINT16 readAddr, UINT8 *pBuff, UINT32 sizeOfData)
	{
	UINT32 status = 0;

	/* Validation of Input Parameters */
	if (slaveAddr != EEPROM_SLAVE_ADDR)	/* Only EEPROM supported on IIC bus on SNDS board */
		return (ERROR);
	if (readAddr >= EEPROM_MAX_SIZE)
		return (ERROR);
	if (pBuff == NULL)		
		return (ERROR);
	
	if ((readAddr + sizeOfData) >= EEPROM_MAX_SIZE)
		sizeOfData = EEPROM_MAX_SIZE - readAddr;

	/* semTake */
	if (semTake (at91TWI.IICSemID, WAIT_FOREVER) == ERROR)
		return (ERROR);

    /* Connecting the IIC bus interrupt to Read ISR */
	if (intConnect (INUM_TO_IVEC(at91TWI.int_vec), at91TWIReadISR, NULL) == ERROR)
		return(ERROR);
	if (intEnable (at91TWI.int_vec) == ERROR)
		return(ERROR);

	/* Initialising the IIC Rx Structure */
	at91TWI.IIC_Rcv.rxBuff = pBuff;
	at91TWI.IIC_Rcv.flag = 0x0;
	at91TWI.IIC_Rcv.byteReadCount = 0x0;
	at91TWI.IIC_Rcv.dataSize = sizeOfData;
	at91TWI.IIC_Rcv.slaveAddress = slaveAddr; 
	at91TWI.IIC_Rcv.readAddrMSB = (UINT8)((readAddr>>8) & 0xff);
	at91TWI.IIC_Rcv.readAddrLSB = (UINT8)(readAddr & 0xff);
	at91TWI.IIC_Rcv.totalBytesRead = 0;
	do 
		{
		SNDS_IIC_REG_READ(iiccon, status);
		} while (status & SNDS_IIC_ISBUSY); /* Wait! the iic bus is busy */

	/* Dummy Write */
	SNDS_IIC_REG_WRITE (iiccon, (SNDS_IIC_START|SNDS_IIC_ACK | SNDS_IIC_IEN));
	SNDS_IIC_REG_WRITE (iicbuf, (at91TWI.IIC_Rcv.slaveAddress & S_WRITE));

	/* Wait till read operation is finished in ISR*/
	semTake (at91TWI.IIC_Rcv.endofReadSemID, WAIT_FOREVER);

   	if (intDisable (at91TWI.int_vec) == ERROR) /*Disable the IIC bus interrupt */
		return(ERROR);
	/* semGive */
	if (semGive (at91TWI.IICSemID) == ERROR)
		return(ERROR);

   	return(at91TWI.IIC_Rcv.totalBytesRead); 
	}

/*******************************************************************************
*
* at91TWIReadISR() - Handle IIC read interrupt
*
* This ISR reads the required number of bytes from the serial EEPROM.  After the 
* reading is complete, the ISR gives the semaphore.
*
* RETURNS: N/A
*/
LOCAL void at91TWIReadISR()
	{
	/* Dummy write to send read address */
	if (!(at91TWI.IIC_Rcv.flag & (UINT8)IIC_RX_BYTE_ADDR_MSB))
		{
		/* Send read address MSB */
		SNDS_IIC_REG_WRITE (iicbuf, at91TWI.IIC_Rcv.readAddrMSB); 
		at91TWI.IIC_Rcv.flag |= (UINT8)IIC_RX_BYTE_ADDR_MSB; 
		}
	else if (!(at91TWI.IIC_Rcv.flag & (UINT8) IIC_RX_BYTE_ADDR_LSB))
		{
		/* Send read address LSB */
		SNDS_IIC_REG_WRITE (iicbuf, at91TWI.IIC_Rcv.readAddrLSB);
		at91TWI.IIC_Rcv.flag |= (UINT8) IIC_RX_BYTE_ADDR_LSB; 
		}
	else if (!(at91TWI.IIC_Rcv.flag & (UINT8) IIC_REPEAT_START))
		{
		/* Repeat Start */
		SNDS_IIC_REG_WRITE (iiccon,  SNDS_IIC_REPEAT_START);	

		/* Set IIC master in receiver mode */
		SNDS_IIC_REG_WRITE (iiccon,  (SNDS_IIC_START | SNDS_IIC_ACK | SNDS_IIC_IEN));
		SNDS_IIC_REG_WRITE (iicbuf, at91TWI.IIC_Rcv.slaveAddress | (UINT8)S_READ);

		at91TWI.IIC_Rcv.flag |= (UINT8) IIC_REPEAT_START;
		}
	else if (!(at91TWI.IIC_Rcv.flag & (UINT8)IIC_MULTI_RCV))
		{
		/* This block is used to skip the interrupt raised immediately after writing 
	       slave address. No useful data is available by this time. But this block is used
		   for setting the IIC master in NOACK or ACK mode depending on the condition that
		   only single byte is to be read or Multiple bytes */
		if(at91TWI.IIC_Rcv.dataSize == 1)
			{
			/* Single byte read. Set to NOACK mode  */
			SNDS_IIC_REG_WRITE(iiccon,  (SNDS_IIC_NOACK | SNDS_IIC_IEN));
			}
		else
			{
			/* Multi bytes read.Send ACK after every byte */
		   	SNDS_IIC_REG_WRITE(iiccon,  (SNDS_IIC_ACK | SNDS_IIC_IEN));
			}
   		at91TWI.IIC_Rcv.flag |= (UINT8) IIC_MULTI_RCV;
		}

	/* for multibyte read operation.. */
	else if (at91TWI.IIC_Rcv.byteReadCount < (at91TWI.IIC_Rcv.dataSize - 1))
		{
		/* Read a byte to RxBuff till last but one character */
		SNDS_IIC_REG_READ (iicbuf,  at91TWI.IIC_Rcv.rxBuff[at91TWI.IIC_Rcv.byteReadCount++]);
		at91TWI.IIC_Rcv.totalBytesRead++;

		/* if only last byte is remaining, set NOACK mode so that no ACK is sent after 
		 * last byte 
		 */
		if (at91TWI.IIC_Rcv.byteReadCount == (at91TWI.IIC_Rcv.dataSize - 1))
			{
			SNDS_IIC_REG_WRITE (iiccon, (SNDS_IIC_NOACK | SNDS_IIC_IEN));
			}
		}
	else
		{  
		/* Receive last data */
		SNDS_IIC_REG_READ (iicbuf, at91TWI.IIC_Rcv.rxBuff[at91TWI.IIC_Rcv.byteReadCount]);
		at91TWI.IIC_Rcv.totalBytesRead++;

        /* STOP IIC Controller */
		SNDS_IIC_REG_WRITE (iiccon, (SNDS_IIC_STOP));
		semGive (at91TWI.IIC_Rcv.endofReadSemID);
		}
	}

/*******************************************************************************
*
* at91TWIIoctl - Control the IIC device
*
* This routine can be used by the application to set the prescaler value of the 
* IIC device.
*
* RETURNS: OK if successful, else ERROR
*/
STATUS at91TWIIoctl(UINT32 cmd, void* arg)
	{
	UINT32 *pSClk; 
	UINT32 nPreScaleValue;

	pSClk = (UINT32 *) arg;

	if (cmd == SET_PRE_SCALER)
		{
		SNDS_IIC_REG_WRITE(iiccon, SNDS_IIC_RESET_IICCON);
		nPreScaleValue = setPreScaler (*pSClk);
		SNDS_IIC_REG_WRITE (iicps, nPreScaleValue);
		return (OK);
		}
	else
		return (ERROR);
	}

/*******************************************************************************
*
* setPreScaler - Calculate the IIC prescaler value for the given clock speed
*
* RETURNS: Prescaler value
*/
LOCAL UINT32 setPreScaler(UINT32 sclk)
	{
	return ((UINT32) ((((fMCLK_MHz / sclk) - 3) / 16)));
	}
