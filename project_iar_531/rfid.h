/* RFID.h - Library to use ARDUINO RFID MODULE KIT 13.56 MHZ WITH TAGS SPI W AND R BY COOQROBOT.
 * Based on code Dr.Leong   ( WWW.B2CQSHOP.COM )
 * Created by Miguel Balboa (circuitito.com), Jan, 2012. 
 */
#ifndef RFID_h
#define RFID_h

#include "spi.h"
/******************************************************************************
 * Definitions
 ******************************************************************************/

void rfid_init(void);
unsigned char isCard(void);
unsigned char readMFRC522( unsigned char addr);
void selftest (void);
unsigned char getVersion (void);

#define MAX_LEN 16   // Largo m�ximo de la matriz

//MF522 �������
#define PCD_IDLE              0x00               // NO action; Y cancelar el comando
#define PCD_AUTHENT           0x0E               // �������������� �����
#define PCD_RECEIVE           0x08               // ����� ������
#define PCD_TRANSMIT          0x04               // ��������� ������
#define PCD_TRANSCEIVE        0x0C               // ���������� � �������� ������
#define PCD_RESETPHASE        0x0F               // �����
#define PCD_CALCCRC           0x03               // ������ CRC

//Mifare_One  Tarjeta Mifare_One comando palabra
#define PICC_REQIDL           0x26               // �rea de la antena no est� tratando de entrar en el estado de reposo
#define PICC_REQALL           0x52               // Todas las cartas para encontrar el �rea de la antena
#define PICC_ANTICOLL         0x93               // anti-colisi�n
#define PICC_SElECTTAG        0x93               // elecci�n de tarjeta
#define PICC_AUTHENT1A        0x60               // verificaci�n key A
#define PICC_AUTHENT1B        0x61               // verificaci�n Key B
#define PICC_READ             0x30               // leer bloque
#define PICC_WRITE            0xA0               // Escribir en el bloque 
#define PICC_DECREMENT        0xC0               // cargo
#define PICC_INCREMENT        0xC1               // recargar
#define PICC_RESTORE          0xC2               // Transferencia de datos de bloque de buffer
#define PICC_TRANSFER         0xB0               // Guardar los datos en el b�fer
#define PICC_HALT             0x50               // inactividad

//MF522 C�digo de error de comunicaci�n cuando regres�
#define MI_OK                 0
#define MI_NOTAGERR           1
#define MI_ERR                2

//------------------ MFRC522 ��������---------------
//Page 0:Command and Status
#define     Reserved00            0x00    
#define     CommandReg            0x01    
#define     CommIEnReg            0x02    
#define     DivlEnReg             0x03    
#define     CommIrqReg            0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06    
#define     Status1Reg            0x07    
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     Reserved01            0x0F
//Page 1:Command     
#define     Reserved10            0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     Reserved11            0x1A
#define     Reserved12            0x1B
#define     MifareReg             0x1C
#define     Reserved13            0x1D
#define     Reserved14            0x1E
#define     SerialSpeedReg        0x1F
//Page 2:CFG    
#define     Reserved20            0x20  
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     Reserved21            0x23
#define     ModWidthReg           0x24
#define     Reserved22            0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsPReg	          0x28
#define     ModGsPReg             0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
//Page 3:TestRegister     
#define     Reserved30            0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39  
#define     TestDAC2Reg           0x3A   
#define     TestADCReg            0x3B   
#define     Reserved31            0x3C   
#define     Reserved32            0x3D   
#define     Reserved33            0x3E   
#define     Reserved34			  0x3F
//-----------------------------------------------


#endif