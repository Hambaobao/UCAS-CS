#define PRINTSTR 0x80011100
#define PRINTCH  0x80011140

int value = 2019;

 __attribute__((section(".entry_function"))) _start(void)
{
	// Call PMON BIOS printstr to print message "Hello OS!"
	char *s = "Hello OS! ";
	char *v = "Version:";
	char strnumb[20];

	void (*print)(char *);

	print = PRINTSTR;
	(*print)(s);
	(*print)(v);


	int power;
	int j = value;

	for(power = 1; j > 10; j /= 10)
		power  *= 10;

	int i;
	for(i = 0;power > 0;power /= 10)
	{
		strnumb[i] = '0' + value/power;
		value %= power;
		i++;
	}

	strnumb[i] = '\0';

	(*print)(strnumb);

	return;
}
