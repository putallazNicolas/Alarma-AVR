struct hour
{
  int hrs;
  int min;
  int sec;
};

struct hour time;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  led();
}
