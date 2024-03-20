const int symbolSpace = 300;
const int letterSpace = 900;
const int wordSpace = 2100;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //R . - .
  dot();
  dash();
  dot();
  delay(letterSpace);
  //I . .
  dot();
  dot();
  delay(letterSpace);
  //C - . - .
  dash();
  dot();
  dash();
  dot();
  delay(letterSpace);
  //K - . -
  dash();
  dot();
  dash();
  delay(letterSpace);

  delay(wordSpace);
}

void dot() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);
  delay(symbolSpace);
}

void dash() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(900);
  digitalWrite(LED_BUILTIN, LOW);
  delay(symbolSpace);
}

/*The length of a dot is 1 time unit.

A dash is 3 time units.

The space between symbols (dots and dashes) of the same letter is 1 time unit.

The space between letters is 3 time units.

The space between words is 7 time units.
https://www.codebug.org.uk/learn/step/541/morse-code-timing-rules/*/
