/* This is the entry point for mocked Arduino applications.
 * It accesses setup and loop via extern
 */

extern void setup(void);
extern void loop(void);

int main(void)
{
	setup();

	while (1)
	{
		loop();
	}
}