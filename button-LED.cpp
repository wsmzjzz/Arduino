boolean state = false;
boolean minusBtnReleased = true;
void setup()
{
    Serial.begin(9600);
    pinMode(2, INPUT);
    digitalWrite(2, HIGH);
    pinMode(11, OUTPUT);
    digitalWrite(11, LOW);
}

void loop()
{
    if (Serial.available())
    {
        char ch = Serial.read();

        if (ch == 'o')
        {
            digitalWrite(11, HIGH);
        }
        else if (ch == 'c')
        {
            digitalWrite(11, LOW);
        }
        else
        {
            Serial.println(" Invalid character. ");
        }
    }
    // 2--Button--GND
    // !HIGH === pressed
    if (digitalRead(2) != HIGH && minusBtnReleased)
    {
        state = !state; // on <-> off
        minusBtnReleased = false;
        digitalWrite(11, state);
        // delay(200);
    }
    else if (digitalRead(2) == HIGH) {
        minusBtnReleased = true;
    }
}