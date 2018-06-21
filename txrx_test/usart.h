#ifndef USART_H
#define USART_H

/* From console:
 * 
 * Setup USART:
 * stty -F /dev/<arduino> cs8 9600 ignbrk -brkint -icrnl -imaxbel \
 * -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl \
 * -echoke noflsh -ixon -crtscts
 * 
 * Reading:
 * cat < /dev/<arduino>
 * 
 * Write char 'c':
 * echo -n "c" > /dev/<arduino>
 * 
 */

namespace USART {
typedef void (*CallbackFun)(unsigned char data);

/* Callback will be called on _received_ character */
void init(CallbackFun cb = nullptr);

void transmit(unsigned char data);
void transmit(char const *data);
void transmit(uint8_t const *data, uint8_t len);
}

#endif
