#ifndef MAIN_C
#define MAIN_C

#define _XTAL_FREQ = 48000000;

// Todos os includes
#include <htc.h>

// Includes de arquivos locais
#include "usb.h"
#include "HardwareProfile.h"
#include "usb_function_hid.h"
#include "genericHID.h"

// PIC 18F4550 configuração dos Bits
// Cristal de 20Mhz --> 96MHz --> 48MHz e configurado para USB
__CONFIG(1, USBPLL & IESODIS & FCMDIS & HSPLL & CPUDIV1 & PLLDIV5);
// Palavra de configuração 2
__CONFIG(2, VREGEN & PWRTDIS & BOREN & BORV20 & WDTDIS & WDTPS32K);
// Palavra de configuração 3
__CONFIG(3, PBDIGITAL & LPT1DIS & MCLREN);
// Palavra de configuração 4
__CONFIG(4, XINSTDIS & STVREN & LVPDIS & ICPORTDIS & DEBUGDIS);
// Palavra de configuração 5,6 e 7
__CONFIG(5, UNPROTECT);
__CONFIG(6, UNPROTECT);
__CONFIG(7, UNPROTECT);

// Protótipos locais
static void InitialiseSystem(void);
void ProcessIO(void);

// Função Principal
void main(void)
{
    //Inicializa o sistema
    InitialiseSystem();

    LED0=1;

    while(1)
    {
	// Checa o status da USB e algumas interrupções
        USBDeviceTasks();
	// Tarefas específicas da aplicação
        ProcessIO();

    }
}

// Inicializa o sistema
static void InitialiseSystem(void)
{
        ADCON1 = 0x0F; // Todos os pinos digitais

	// Configurar portas para (1)nput ou (0)utput
	TRISA = 0b00000000;
	TRISB = 0b00000000;
	TRISC = 0b00000000;
	TRISD = 0b00000000;
	TRISE = 0b00000000;

	// Apaga todas as portas
	PORTA = 0b00000000;
	PORTB = 0b00000000;
	PORTC = 0b00000000;
	PORTD = 0b00000000;
	PORTE = 0b00000000;

    #if defined(USE_SELF_POWER_SENSE_IO)
        tris_self_power = INPUT_PIN;
    #endif
    
    //Inicializa a variável deixando o Handle para o final
    // Transmissão
    USBOutHandle = 0;
    USBInHandle = 0;

    blinkStatusValid = TRUE;
    // Inicia o dispositivo USB
    USBDeviceInit();
    LED0 = 1;
}

// Process input and output
void ProcessIO(void)
{   
    // Se não estiver configurado somente retorna
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;
    
	// O processo atual suporta os seguintes comandos
	//
	// 0x80 (128) Muda o LED
	// 0x81 (129) Le o status do Pushbutton
	// 0x82 (130) Le o status do LED

    //Checa se recebeu as informações do HOST
    if(!HIDRxHandleBusy(USBOutHandle))
    {   
        switch(ReceivedDataBuffer[0]) {
            case 0x80:  // (128) Troca o LED
                if (LED0 == 1) LED0 = 0; else LED0 = 1;
                break;
            case 0x81:  // (129) Manipula a Valvula que controla o meio de cultura
                    RELE_MEIO = 1;
                    RELE_PLTS = 0;
                break;
            case 0x82:  // (130) Manipula a Valvula que controla as plantas
                    RELE_MEIO = 0;
                    RELE_PLTS = 1;
                break;
            case 0x83:  // (131) Desliga todos os reles
                    RELE_MEIO = 0;
                    RELE_PLTS = 0;
                break;
            /*case 0x81:  // Le o status do Push Button
		ToSendDataBuffer[0] = 0x81;
		ToSendDataBuffer[1] = SWITCH0;	// return the current switch state
            
		// Transmite a resposta para o computador
                if(!HIDTxHandleBusy(USBInHandle))
                    {
                            USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
                    }
                break;
             */
            /*case 0x82:  // Read the LED status
				ToSendDataBuffer[0] = 0x82;
				ToSendDataBuffer[1] = LED0;	// return the current LED state

				// Transmit the response to the host
                if(!HIDTxHandleBusy(USBInHandle))
				{
					USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
				}
                break;
             */
        }
        // Re-Arma o sistema de saída para o próximo pacota
        USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&ReceivedDataBuffer,64);
    }  
}

#endif
