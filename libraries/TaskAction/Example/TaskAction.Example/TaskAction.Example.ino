#include "TaskAction.h"

static void exampleTaskFn(void)
{
	Serial.println("Task running!");
}
static TaskAction exampleTask(exampleTaskFn, 1000, INFINITE_TICKS);

void setup()
{

}

void loop()
{
	exampleTask.tick();
}