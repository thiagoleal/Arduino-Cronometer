#include "sevenSegmentDisplay.h"

// Pinos do display 1
int pinG_1 = 30;
int pinF_1 = 32;
int pinA_1 = 34;
int pinB_1 = 36;
int pinE_1 = 31;
int pinD_1 = 33;
int pinC_1 = 35;
int pinDP_1 = 37;

// Pinos do display 1
int pinG_2 = 40;
int pinF_2 = 42;
int pinA_2 = 44;
int pinB_2 = 46;
int pinE_2 = 41;
int pinD_2 = 43;
int pinC_2 = 45;
int pinDP_2 = 47;

// Configura os displays utilizando a biblioteca
sevenSegmentDisplay display1("COMMON_ANODE", pinA_1, pinB_1, pinC_1, pinD_1, pinE_1, pinF_1, pinG_1, pinDP_1);
sevenSegmentDisplay display2("COMMON_ANODE", pinA_2, pinB_2, pinC_2, pinD_2, pinE_2, pinF_2, pinG_2, pinDP_2);

unsigned int contador = 0;

void setup()
{
  confTimer1();
  confTimer2();
}

void loop()
{
}

void imprimir(int numero)
{  
  int num1, num2;
  
  if (numero < 10)
  {
    num1 = 0;
    num2 = numero;
  }
  else
  {
    num1 = numero / 10;
    num2 = numero % 10;
  }

  display1.set(num1);
  display2.set(num2);
}

void confTimer1()
{
  // Configuração do timer1 
  TCCR1A = 0; // Confira timer para operação normal
  TCCR1B = 0; // Limpa registrador
  TIMSK1 = 1; // Habilita a interrupção do TIMER1
  TCCR1B = 5; // Configura prescaler para 1024
  TCNT1 = 49911; // Valor para que estouro ocorra em 1 segundo

  /*
   * Overlfow a cada 65536 ciclos (16bits)
   * Clock = 16MHz
   * Frequência = 1/16000000 => 6,25*10^-8s/ciclo
   * Com Prescaler * 1024 => 6,25*10^-8s * 1024 = 6,4ms/ciclo
   * 1S = 1000ms
   * 1000ms/6,4ms => 15625
   * 15625 ciclos para 1 segundo
   * 65536 - 15625 = 49911
   * Contador inicia em 49911 para que a interrupção ocorra a cada 1 segundo
  */
}


void confTimer2()
{
  TCCR2A = 0; // Confira timer para operação normal
  TCCR2B = 7; // Prescaler 1024
  TIMSK2 = 1;// Habilita interrupçao do timer2
  TCNT2 = 0; // Inicia contador em zero

  /*
   * Overlfow a cada 256 ciclos (8bits)
   * Clock = 16MHz
   * Frequência = 1/16000000 => 6,25*10^-8s/ciclo
   * Com Prescaler * 1024 => 6,25*10^-8s * 1024 = 6,4ms/ciclo
   * 256 * 6,4ms = 16,38ms
   * A interrupção ocorre a cada 16,38ms (frequência de atualização do valor nos displays)
  */
}

//interrupção do TIMER1 por overflow
ISR(TIMER1_OVF_vect)
{
  if ( contador+1 > 99) contador = 0;
  else contador++;
  
  TCNT1 = 49911; // Reinicia TIMER
}

//interrupção do TIMER2 por overflow
ISR(TIMER2_OVF_vect)
{
  imprimir(contador);
  TCNT2 = 0; // Reinicia TIMER
}
