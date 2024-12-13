# Repositório da prova módulo 4 - Inteli

## Aluno: Davi Abreu da Silveira - T14

### Link para o Wokwi [aqui](https://wokwi.com/projects/417159999818676225)

## Avaliação:

1) Todo o código deve estar padronizado para inglês ou português, incluindo comentários (1,0 ponto);

O código foi todo padronizado em ingles, tanto comentários, quanto variáveis.

2) Todas as entradas e saídas devem estar declaradas corretamente na inicialização (1,0 ponto);

Foram oganizadas no início do código.
```c++
//LIBS
#include <WiFi.h>
#include <HTTPClient.h>

//LEDS
#define led_yellow 13 // Pin for yellow led
#define led_green 41 // Pin for green led
#define led_red 40 // Pin for red led

//LDR
const int ldrPin = 4;  // the number of the pushbutton pin
int threshold=600;

//BUTTON
const int buttonPin = 18;  // the number of the pushbutton pin
int buttonState = LOW; //this variable tracks the state of the button, low if not pressed, high if pressed

//DEBOUNCE
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

int buttonPressCount = 0;
```

3) Todos os leds devem estar apagados na inicialização (até 1,0 ponto);

Estão apagados, como pode ser visto aqui:

```c++
void setup() {
  //Initial configuration of the pins for led control as OUTPUTS 
  pinMode(led_yellow,OUTPUT);
  pinMode(led_green,OUTPUT);
  pinMode(led_red,OUTPUT);

  // Inicialization of the inputs
  pinMode(buttonPin, INPUT); // Initialize the pushbutton pin as an input

  digitalWrite(led_yellow, LOW);
  digitalWrite(led_green, LOW);
  digitalWrite(led_red, LOW);
```

4) Quando estiver escuro (segundo a leitura analógica do sensor LDR), o protótipo deve ativar o modo noturno e piscar o led amarelo a cada segundo (até 1,0 ponto);

<br>
<div align="center">
<sub>Figura 1 - Led Amarelo Aceso</sub>
<br>
<br>
<img src="assets\led_amarelo.png" alt="Cinco Forças de Porter">
<br>
<br>
<sup>Fonte: Material produzido pelo grupo BiMu (2024)</sup>
</div>
<br>

Quando escuro, o ldrstatus fica menor que o threshold e cai no if abaixo, que pisca o led amarelo a cada segundo.

```c++
  if (ldrstatus <= threshold) {
    digitalWrite(led_yellow, HIGH);
    digitalWrite(led_green, LOW);
    digitalWrite(led_red, LOW);
    
    delay(1000);
    digitalWrite(led_yellow, LOW);
    digitalWrite(led_green, LOW);
    digitalWrite(led_red, LOW);

    delay(1000);
  }
```

5) Quando estiver claro (segundo a leitura analógica do sensor LDR), o protótipo deve ativar o modo convencional e fazer a temporização alternando entre verde (3 segundos), amarelo (2 segundos) e vermelho (5 segundos) (até 2,0 pontos);

Quando claro, o ldrstatus fica maior que o threshold e cai no if abaixo, que os leds piscam na ordem e tempo estipulado.

<br>
<div align="center">
<sub>Figura 2 - Led Vermelho Aceso</sub>
<br>
<br>
<img src="assets\vermelho.png" alt="Cinco Forças de Porter">
<br>
<br>
<sup>Fonte: Material produzido pelo grupo BiMu (2024)</sup>
</div>
<br>

```c++
//5
  if (ldrstatus >= threshold) {
    //3 seconds green
    digitalWrite(led_yellow, LOW);
    digitalWrite(led_green, HIGH);
    digitalWrite(led_red, LOW);
    delay(3000);

    //2 seconds yellow
    digitalWrite(led_yellow, HIGH);
    digitalWrite(led_green, LOW);
    digitalWrite(led_red, LOW);
    delay(2000);

    //5 seconds red
    digitalWrite(led_yellow, LOW);
    digitalWrite(led_green, LOW);
    digitalWrite(led_red, HIGH);
    delay(5000);
  }
```

6) Quando estiver claro (segundo a leitura analógica do sensor LDR) e o semáforo estiver no estado fechado (somente led vermelho aceso) e o botão for pressionado, o semáforo deve abrir 1 segundo após o pressionamento do botão (até 2,0 pontos);

```c++
 //6
  //filter out any noise by setting a time buffer
  if ( (millis() - lastDebounceTime) > debounceDelay) {

    if ( (buttonState == HIGH) && (ldrstatus >= threshold) && digitalRead(led_red) == HIGH) {

      delay(1000);
      digitalWrite(led_red, LOW);
      digitalWrite(led_yellow, LOW);
      digitalWrite(led_green, HIGH);
      lastDebounceTime = millis(); //set the current time
    }
```

8) Quando estiver claro (segundo a leitura analógica do sensor LDR) e o semáforo estiver no estado fechado (somente led vermelho aceso) e o botão for pressionado 3 vezes, você deve enviar uma requisição HTTP para implementar um alerta (realize uma requisição para o http://www.google.com.br/ para fins de teste) (até 1,0 ponto).

```c++
int buttonPressCount = 0;

void sendHttpRequest() {
  if (WiFi.status() == WL_CONNECTED) {  // If esp connected to the internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint of HTTP requisition

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) { // Result of HTTP Requisition
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {

...

 if ( (ldrstatus >= threshold) && digitalRead(led_red) == HIGH) {

      if ( (millis() - lastDebounceTime) > debounceDelay) {
        if (buttonState == HIGH) {
          buttonPressCount++;
          lastDebounceTime = millis();
        }
      }

      //8
      if (buttonPressCount >= 3) {
        sendHttpRequest();
        buttonPressCount = 0;  // reset the counter
      }
```

7) O protótipo deve suportar debounce na leitura do botão (até 1,0 ponto);

Como se pode ver nos códigos acima, o botão foi implementado com debounce por meio de código. Utilizando do milis e duas variáveis: lastDebounceTime e debounceDelay.