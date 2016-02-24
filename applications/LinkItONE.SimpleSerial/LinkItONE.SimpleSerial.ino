String serial_in;

static void handle_serial(String& str)
{
  Serial1.println(str);
}

void setup() {
  Serial1.begin(115200);
}

void loop() {
  while(Serial1.available())
  {
    char c  = (char)Serial1.read();
    if (c != '\n')
    {
      serial_in += c;
    }
    else
    {
      handle_serial(serial_in);
      serial_in = "";
    }
  }
}
