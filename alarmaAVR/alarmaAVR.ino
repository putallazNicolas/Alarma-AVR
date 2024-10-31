int main(void)
{
  Serial.begin(9600);
  tiempoDeclare();
  setupKeypad();
  while(1)
  {
    char key = checkKeypad();
    if(key)
    {
      Serial.println(key);
    }
  }
}
