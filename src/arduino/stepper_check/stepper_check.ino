

// vars for steppers
int rev_steps = 200;
int left_step_pin = 8;
int left_dir_pin = 9;
int right_step_pin = 10;
int right_dir_pin = 12;

// vars for stepper speed
unsigned long last_left = 0;
unsigned long last_right = 0;
unsigned long step_delay_left = 4;
unsigned long step_delay_right = 4;

// variables for serial communication
int state = 0;
String serial_command = "";

void setup()
{
    // put your setup code here, to run once:
    // pinmodes
    pinMode(left_step_pin, OUTPUT);
    pinMode(right_step_pin, OUTPUT);

    pinMode(left_dir_pin, OUTPUT);
    pinMode(right_dir_pin, OUTPUT);
    Serial.begin(9600);

    // set the direction to be something
    digitalWrite(left_dir_pin, HIGH);
    digitalWrite(right_dir_pin, HIGH);
}

void left_step(bool fwd = true)
{
    if (millis() - last_left > step_delay_left)
    {

        digitalWrite(left_dir_pin, fwd);

        digitalWrite(left_step_pin, HIGH);
        digitalWrite(left_step_pin, LOW);
        last_left = millis();
    }
}

void right_step(bool fwd = true)
{
    if (millis() - last_right > step_delay_right)
    {

        digitalWrite(right_dir_pin, fwd); //pin 11 does not go low

        digitalWrite(right_step_pin, HIGH);
        digitalWrite(right_step_pin, LOW);
        last_right = millis();
    }
}

void move_steps(int steps, bool right_dir = true, bool left_dir = false, int slow = 5)
{

    digitalWrite(right_dir_pin, right_dir);
    digitalWrite(left_dir_pin, left_dir);

    for (int i = 0; i < steps; i++)
    {
        left_step();
        right_step();
        delay(slow);
    }
}

void loop()
{
    //  move_steps(400, true, true);
    //  delay(1000);
    //  move_steps(120, true, false);
    //  delay(1000);

    // model as state machine

    switch (state)
    {

    // state 0, wait for serial information
    case 0:
        // if we have something to read, read it until getting endline
        if (Serial.available() != 0)
        {
            char in = Serial.read();
            // if the char we got is a new line, then continue to the next state and work with the command
            if (in == '\n')
            {
                Serial.println("serial command= " + serial_command);

                state = 1;
            }
            else
            {
                // otherwise, put what we got in the string and continue getting chars
                serial_command += in;
            }
        }
        break;

    // do stuff with the command
    case 1:
        //      Serial.print("got from serial=");
        //      Serial.println(serial_command);

        //if the command is 'a' turn left
        if (serial_command == "a")
        {
            //        Serial.println("left");
            right_step();
            left_step(false);
        }
        else if (serial_command == "d")
        {
            //        Serial.println("right");
            right_step(false);
            left_step();
        }
        else if (serial_command == "w")
        {
            //        Serial.println("fwd");
            right_step();
            left_step();
        }

        else if (serial_command == "s")
        {
            //        Serial.println("rvs");
            right_step(false);
            left_step(false);
        }
        else if (serial_command == "W")
        {
            right_step();
            left_step();
            serial_command = "";
        }

        else if (serial_command == "S")
        {
            right_step(false);
            left_step(false);
            serial_command = "";
        }

        else if (serial_command == "D")
        {
            right_step(false);
            left_step();
            serial_command = "";
        }

        else if (serial_command == "A")
        {
            right_step();
            left_step(false);
            serial_command = "";
        }

        else
        {
            serial_command = "";
        }

        if (Serial.available() > 0)
        {
            serial_command = "";
            state = 0;
        }
        break;
    }

    //  serial_command = "";
}
