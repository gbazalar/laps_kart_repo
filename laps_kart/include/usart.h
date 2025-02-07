#ifndef USART_H
#define USART_H

void usart_init(void);
void usart_send_char(char c);
void usart_send_string(const char* str);

#endif // USART_H