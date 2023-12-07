/* Ce fichier a été préparé pour la génération automatique de documentation avec Doxygen. /
/*! \file *********************************************************************
*
* \brief
*      Liaison port série
*
*      Ce fichier contient l'implémentation complète du contrôle moteur,
 à l'exception du régulateur PID.
*
* \par Application note:
*     
 AVR447 : Commande sinusoïdale d'un moteur permanent triphasé à l'aide
*      du ATmega48/88/168
*
* \par Documentation
*       Pour une documentation complète du code, des compilateurs pris en charge, des paramètres du compilateur
*       et des dispositifs pris en charge, consultez mon github : 4jla


*
* \auteur
*      Atmel Corporation: ajvlic@hotmail.com \n
*
* $Name: VELIC_Ajla $
* $Revision: 1.8 $
* $RCSfile: main.c,v $
* $Date: 2023/12/7 07:20:51 $  \n
******************************************************************************/


/*! \brief fréquence du qwark du µcontroller*/
#define FOSC 16000000
/*! \brief nombre de changement d'état par seconde(vitesse d'envoi de données)*/
#define BAUD 9600
/*! \brief calcul servant à trouver le numéro du registre*/
#define MYUBRR FOSC / 16 / BAUD - 1
unsigned char flag = 0;
unsigned char data;


void USART_Init(unsigned int ubrr) {
/* Définir le taux de bauds */
UBRR0H = (unsigned char)(ubrr >> 8);
UBRR0L = (unsigned char)ubrr;


/*autoriser  transmitter allumer le bit n3 */
UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);


/* Définir le format de trame : 8 bits de données, 1 bit d'arrêt */
UCSR0C = (3 << UCSZ00);
DDRB = 0B00100000;
PORTB = 0B00000000;
}


/* Fonction Transmettre un message */
void USART_Transmit(unsigned char data) {


/* Attendre que le tampon de transmission soit vide */
while (!(UCSR0A & (1 << UDRE0)))
 ;
/* Placer les données dans le tampon, envoyer les données */
UDR0 = data;
}


void USART_puts(unsigned char *str)
{
   // Boucle tant que le caractère pointé par 'str' n'est pas nul (fin de la chaîne)
   do
   {
       // Transmettre le caractère actuel pointé par 'str'
       USART_Transmit(*str);
   } while (*++str != 0); // Passer au caractère suivant dans la chaîne
}


int main(void) {


USART_Init(MYUBRR);
sei();
while (1) {
 if (flag == 1) {
   /* PORTB=0B00100000;//Allumer la led quand l'interruption est réalisée*/
   //USART_Transmit(data + 1);
   flag = 0;
 }
 _delay_ms(1);
}
}
