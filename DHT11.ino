// Folosim libraria pentru Dht11
#include <DHT.h>

// Senzorul Dht11 este conectat la pinul digital D6
#define DHTPIN D6

// Definim tipul senzorului pe care il folosim ("DHT11")
#define DHTTYPE DHT11

// Initiem un senzor Dht
DHT dht(DHTPIN, DHTTYPE);

void setup(void) {
  // Initiem interfata serial
  Serial.begin(9600);

  // Pornim senzorul
  dht.begin();
}

void loop(void)
{
  // Atribuim variabilei "h" valoarea umiditatii, citita de functia "dht.readHumidity()"
  float h = dht.readHumidity();

  // Atribuim variabilei "t" valoarea temperaturii, citita de functia "dht.readTemperature()"
  float t = dht.readTemperature();

  // Afisam in consola mesajul "Exterior Temperature (C) = "
  Serial.print("Exterior Temperature (C) = ");

  // Afisam temperatura din camera, citita cu ajutorul senzorului Dht11
  Serial.println(t);

  // Afisam in consola mesajul "Exterior Humidity = "
  Serial.print("Exterior Humidity = ");

  // Afisam umiditatea din camera, citita cu ajutorul senzorului Dht11
  Serial.println(h);

  delay(3000);
}
