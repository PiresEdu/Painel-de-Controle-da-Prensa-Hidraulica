#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTPIN A5
#define DHTTYPE DHT11

#define botaoPin 7
#define ledPin 9

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Símbolo do grau
byte grau[8] = {B00001100,
                B00010010,
                B00010010,
                B00001100,
                B00000000,
                B00000000,
                B00000000,
                B00000000};

int contador = 0;
bool contagemIniciada = false;
bool ledAceso = false;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.createChar(0, grau);
  dht.begin();
  pinMode(botaoPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Mostra temperatura na primeira linha
  lcd.setCursor(0, 0);
  lcd.print("Graus: ");
  lcd.print(t, 1);
  lcd.write((byte)0);
  lcd.print("C");

  // Mostra tempo na segunda linha
  lcd.setCursor(0, 1);
  lcd.print("Tempo: ");
  lcd.print("00:");
  lcd.print(contador < 10 ? "0" : "");
  lcd.print(contador);

  // Lê o estado atual do botão
  bool botaoAtual = digitalRead(botaoPin);

  // Verifica se o botão foi pressionado e a contagem não está em andamento
  if (botaoAtual == HIGH && !contagemIniciada) {
    contagemIniciada = true;  // Inicia a contagem
  }

  // Verifica se a contagem está em andamento
  if (contagemIniciada) {
    delay(1000);  // Aguarda 1 segundo
    contador++;  // Incrementa o contador

    // Verifica se atingiu 10 segundos
    if (contador > 10) {
      contagemIniciada = false;
      contador = 0;  // Reinicia o contador
      digitalWrite(ledPin, HIGH);  // Acende o LED
      ledAceso = true;  // Atualiza a flag do LED aceso
    }
  }

  // Verifica se o botão foi pressionado novamente após o LED ser aceso
  if (ledAceso && botaoAtual == HIGH) {
    digitalWrite(ledPin, LOW);  // Desliga o LED
    ledAceso = false;  // Atualiza a flag do LED apagado
  }
}