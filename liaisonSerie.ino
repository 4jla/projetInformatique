/**
* @file serial_communication.c
* @brief Implémentation de la communication série à l'aide de l'USART sur un microcontrôleur AVR.
*
* Ce fichier contient l'implémentation complète de la communication série à l'aide de l'USART
* sur un microcontrôleur AVR. Il inclut l'initialisation, la transmission, la réception, et
* les routines de service d'interruption pour deux modules USART.
*
* @par Note d'application :
* Arduino UNO : Commande
*
* @par Documentation :
* Pour une documentation complète du code, des compilateurs pris en charge, des paramètres du compilateur,
* et des dispositifs pris en charge, référez-vous au dépôt GitHub : https://github.com/4jla
*
* @author
* Atmel Corporation: ajvlic@hotmail.com
*
* @version $Name: VELIC_Ajla $
* @version $Revision: 1.8 $
* @version $RCSfile: main.c,v $
* @version $Date: 2023/12/7 07:20:51 $
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*! \brief Fréquence du cristal quartz du microcontrôleur */
#define FOSC 16000000
/*! \brief Nombre de changements d'état par seconde (vitesse de transmission des données) */
#define BAUD 9600
/*! \brief Calcul pour trouver le numéro du registre */
#define MYUBRR FOSC / 16 / BAUD - 1

volatile unsigned char flag0 = 0;
volatile unsigned char flag3 = 0;

/*! \brief Initialisation de l'USART0 */
void USART0_Init(unsigned int ubrr) {
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;

  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  UCSR0C = (3 << UCSZ00);
}

/*! \brief Initialisation de l'USART3 */
void USART3_Init(unsigned int ubrr) {
  UBRR3H = (unsigned char)(ubrr >> 8);
  UBRR3L = (unsigned char)ubrr;

  UCSR3B = (1 << RXEN3) | (1 << TXEN3) | (1 << RXCIE3);
  UCSR3C = (3 << UCSZ30);
}

/*! \brief Fonction de transmission de l'USART0 */
void USART0_Transmit(unsigned char data) {
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = data;
}

/*! \brief Fonction de transmission de l'USART3 */
void USART3_Transmit(unsigned char data) {
  while (!(UCSR3A & (1 << UDRE3)));
  UDR3 = data;
}

/*! \brief Fonction de réception de l'USART0 */
unsigned char USART0_Receive(void) {
  while (!(UCSR0A & (1 << RXC0)));
  return UDR0;
}

/*! \brief Fonction de réception de l'USART3 */
unsigned char USART3_Receive(void) {
  while (!(UCSR3A & (1 << RXC3)));
  return UDR3;
}

/*! \brief Routine de service d'interruption de réception de l'USART0 */
ISR(USART0_RX_vect) {
  PORTB = 0B10000000;
  flag0 = 1;
}

/*! \brief Routine de service d'interruption de réception de l'USART3 */
ISR(USART3_RX_vect) {
  PORTB = 0B10000000;
  flag3 = 1;
}

/*! \brief Fonction de transmission de chaîne de l'USART0 */
void USART0_puts(unsigned char *str) {
  while (*str != 0) {
    USART0_Transmit(*str);
    ++str;
  }
}

/*! \brief Fonction de transmission de chaîne de l'USART3 */
void USART3_puts(unsigned char *str) {
  while (*str != 0) {
    USART3_Transmit(*str);
    ++str;
  }
}

/*! \brief Fonction principale */
int main(void) {
  USART0_Init(MYUBRR);
  USART3_Init(MYUBRR);
  DDRB = 0B10000000;
  PORTB = 0B00000000;
  sei();

  while (1) {
    if (flag0 == 1) {
      USART3_Transmit(USART0_Receive());
      flag0 = 0;
    }

    if (flag3 == 1) {
      USART0_Transmit(USART3_Receive());
      flag3 = 0;
    }

    _delay_ms(1);
  }
  return 0;
}
