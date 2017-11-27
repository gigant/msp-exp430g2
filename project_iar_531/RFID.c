/*
Драйвер работы с рфид
*/

#include <msp430g2231.h>
#include "rfid.h"


// defines

unsigned char MFRC522ToCard(unsigned char command, unsigned char *sendData, unsigned char sendLen, unsigned char *backData, unsigned int *backLen);


void writeMFRC522( unsigned char addr, unsigned char val)
{
  P2OUT &=~BIT4; // чип селект в ноль
  
  spiWrite((addr));
  spiWrite(val);
  P2OUT |= BIT4; // чип селект высокий
};

unsigned char readMFRC522( unsigned char addr)
{
  
  unsigned char val;
  P2OUT&=~BIT4; // чип селект в ноль
  
  spiWrite(((addr<<1)&0x7E)| 0x80);
  val = spiRead();
  P2OUT&=~BIT4; // чип селект высокий
  return val;
};

void clearBitMask(unsigned char reg, unsigned char mask)  
{
    unsigned char tmp;
    tmp = readMFRC522(reg);
    writeMFRC522(reg, tmp & (~mask));  // clear bit mask
};

void setBitMask(unsigned char reg, unsigned char mask)  
{
    unsigned char tmp;
    tmp = readMFRC522(reg);
    writeMFRC522(reg, tmp | mask);  // set bit mask
};

void antennaOn(void)
{

  unsigned char temp;
  temp = readMFRC522(TxControlReg);
  if (!(temp & 0x03))  {
	  setBitMask(TxControlReg, 0x03);
  }
};
		  
void rfid_init(void)
{
   P2OUT |= BIT5; // ресет рфида происходит по положительному перепаду. один
   // раз дергаем и так и оставляем
  writeMFRC522(CommandReg, PCD_RESETPHASE); // софтовый ресет
  //Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
  writeMFRC522(TModeReg, 0x8D);		//Tauto=1; f(Timer) = 6.78MHz/TPreScaler
  writeMFRC522(TPrescalerReg, 0x3E);	//TModeReg[3..0] + TPrescalerReg
  writeMFRC522(TReloadRegL, 30);    
  writeMFRC522(TReloadRegH, 0);
  writeMFRC522(TxAutoReg, 0x40);		//100%ASK
  writeMFRC522(ModeReg, 0x3D);
  antennaOn();		
  //включаем антенну
	
};

/*
 *  Functin name: MFRC522_Request
 *  Descripcion: find tag, check card number
 *  parameters: reqMode - card mode
 *			   Tagtype - type of the tag
 *			 	0x4400 = Mifare_UltraLight
 *				0x0400 = Mifare_One(S50)
 *				0x0200 = Mifare_One(S70)
 *				0x0800 = Mifare_Pro(X)
 *				0x4403 = Mifare_DESFire
 *  Valor de retorno: el retorno exitoso MI_OK
 */
unsigned char MFRC522Request(unsigned char reqMode, unsigned char *TagType)
{
	unsigned char status;  
	unsigned int backBits;			//   Recibio bits de datos

	writeMFRC522(BitFramingReg, 0x07);		//TxLastBists = BitFramingReg[2..0]	???
	
	TagType[0] = reqMode;
	status = MFRC522ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);

	if ((status != MI_OK) || (backBits != 0x10))
	{    
		status = MI_ERR;
	}
   
	return status;
}

unsigned char isCard(void) 
 {
	unsigned char status;
	unsigned char str[MAX_LEN];
	
	status = MFRC522Request(PICC_REQIDL, str);	
    if (status == MI_OK) {
		return 1;
	} else { 
		return 0; 
	}
 }

unsigned char MFRC522ToCard(unsigned char command, unsigned char *sendData, unsigned char sendLen, unsigned char *backData, unsigned int *backLen)
{
    unsigned char status = MI_ERR;
    unsigned char irqEn = 0x00;
    unsigned char waitIRq = 0x00;
	unsigned char lastBits;
    unsigned char n;
    unsigned int i;

    switch (command)
    {
        case PCD_AUTHENT:		// Tarjetas de certificacion cerca
		{
			irqEn = 0x12;
			waitIRq = 0x10;
			break;
		}
		case PCD_TRANSCEIVE:	//La transmision de datos FIFO
		{
			irqEn = 0x77;
			waitIRq = 0x30;
			break;
		}
		default:
			break;
    }
   
    writeMFRC522(CommIEnReg, irqEn|0x80);	//De solicitud de interrupcion
    clearBitMask(CommIrqReg, 0x80);			// Borrar todos los bits de peticion de interrupcion
    setBitMask(FIFOLevelReg, 0x80);			//FlushBuffer=1, FIFO de inicializacion
    
	writeMFRC522(CommandReg, PCD_IDLE);	//NO action;Y cancelar el comando

	//Escribir datos en el FIFO
    for (i=0; i<sendLen; i++)
    {   
		writeMFRC522(FIFODataReg, sendData[i]);    
	}

	//???? ejecutar el comando
	writeMFRC522(CommandReg, command);
    if (command == PCD_TRANSCEIVE)
    {    
		setBitMask(BitFramingReg, 0x80);		//StartSend=1,transmission of data starts  
	}

	// A la espera de recibir datos para completar
	i = 5000;	//i????????,??M1???????25ms	??? i De acuerdo con el ajuste de frecuencia de reloj, el tiempo maximo de espera operacion M1 25ms tarjeta??
    clearBitMask (CommIrqReg,0x4);
    do 
    {
		//CommIrqReg[7..0]
		//Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
        n = readMFRC522(CommIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitIRq));

    clearBitMask(BitFramingReg, 0x80);			//StartSend=0
	
    if (i != 0)
    {    
       if(!(readMFRC522(ErrorReg) & 0x1B))	//BufferOvfl Collerr CRCErr ProtecolErr
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   
				status = MI_NOTAGERR;			//??   
			}

            if (command == PCD_TRANSCEIVE)
            {
               	n = readMFRC522(FIFOLevelReg);
              	lastBits = readMFRC522(ControlReg) & 0x07;
                if (lastBits)
                {   
					*backLen = (n-1)*8 + lastBits;   
				}
                else
                {   
					*backLen = n*8;   
				}

                if (n == 0)
                {   
					n = 1;    
				}
                if (n > MAX_LEN)
                {   
					n = MAX_LEN;   
				}
				
				//??FIFO??????? Lea los datos recibidos en el FIFO
                for (i=0; i<n; i++)
                {   
					backData[i] = readMFRC522(FIFODataReg);    
				}
            }
        }
        else
        {   
			status = MI_ERR;  
		}
        
    }
	
    //SetBitMask(ControlReg,0x80);           //timer stops
    //Write_MFRC522(CommandReg, PCD_IDLE); 

    return status;
}

unsigned char buf [25];

unsigned char getVersion (void)
{
  return readMFRC522(0x37);
}
void selftest(void)
{
    writeMFRC522(CommandReg, PCD_RESETPHASE); // софтовый ресет
    // очистить буфер записав 25 байт 0x00
    int i;
    unsigned char *p = buf;

    writeMFRC522(CommandReg, 1); // читаем 25 байт буфера
    for (i=0;i<24;i++)
    {
	*p =  readMFRC522(0x35);
	p++;
    }
    
    writeMFRC522(AutoTestReg,0x9); // enable self test
    writeMFRC522(CommandReg, PCD_CALCCRC); // считаем црц
        for (i=0;i<24;i++)
    {
	buf[i] =  spiRead();
    }

};