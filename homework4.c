#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework4.h"

int main(void)
{
    char rChar;
    char *response = "\n\n\r2534 is the best course in the curriculum!\r\n\n";

    // TODO: Declare the variables that main uses to interact with your state machine.
    bool fsmResult;

    // Stops the Watchdog timer.
    initBoard();

    // TODO: Declare a UART config struct as defined in uart.h.
    //       To begin, configure the UART for 9600 baud, 8-bit payload (LSB first), no parity, 1 stop bit.
    eUSCI_UART_Config BAUD9600 = {
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,
    UART_9600_BAUDRATE_PRESCALAR,
    UART_9600_BAUDRATE_FIRST_MOD_REG,
    UART_9600_BAUDRATE_SECOND_MOD_REG,
    EUSCI_A_UART_NO_PARITY,
    EUSCI_A_UART_LSB_FIRST,
    EUSCI_A_UART_ONE_STOP_BIT,
    EUSCI_A_UART_MODE,
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
    };


    // TODO: Make sure Tx AND Rx pins of EUSCI_A0 work for UART and not as regular GPIO pins.
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    // TODO: Initialize EUSCI_A0
    UART_initModule(EUSCI_A0_BASE, &BAUD9600);

    // TODO: Enable EUSCI_A0
    UART_enableModule(EUSCI_A0_BASE);

    while(1)
    {
        // TODO: Check the receive interrupt flag to see if a received character is available.
        //       Return 0xFF if no character is available.
        if (UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) {
            rChar = UART_receiveData(EUSCI_A0_BASE);
        } else {
            rChar = NO_CHARACTER_RECEIVED;
        }

        // TODO: If an actual character was received, echo the character to the terminal AND use it to update the FSM.
        //       Check the transmit interrupt flag prior to transmitting the character.
        if (rChar != NO_CHARACTER_RECEIVED) {
            if (UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) {
                UART_transmitData(EUSCI_A0_BASE, rChar);
            }
            fsmResult = charFSM(rChar);
        }


        // TODO: If the FSM indicates a successful string entry, transmit the response string.
        //       Check the transmit interrupt flag prior to transmitting each character and moving on to the next one.
        //       Make sure to reset the success variable after transmission.
        if (fsmResult) {
            int i = 0;
            while (response[i] != END_OF_STRING) {
                if (UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) {
                    UART_transmitData(EUSCI_A0_BASE, response[i]);
                    i++;
                }
            }
        }
        fsmResult = false;

    }
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: FSM for detecting character sequence.
bool charFSM(char rChar)
{
    bool finished = false;

    typedef enum {NONE, FIRST, SECOND, THIRD} Sequence_Progress;
    static Sequence_Progress sequenceState = NONE;

    switch (sequenceState) {
        case NONE:
            if (rChar == FIRST_CHAR) {
                sequenceState++;
            } else {
                sequenceState = NONE;
            }
            break;
        case FIRST:
            if (rChar == SECOND_CHAR) {
                sequenceState++;
            } else {
                sequenceState = NONE;
            }
            break;
        case SECOND:
            if (rChar == THIRD_CHAR) {
                sequenceState++;
            } else {
                sequenceState = NONE;
            }
            break;
        case THIRD:
            if (rChar == FOURTH_CHAR) {
                sequenceState = NONE;
                finished = true;
            } else {
                sequenceState = NONE;
            }
            break;
        default: // Do nothing.
            break;
    }

    return finished;
}
