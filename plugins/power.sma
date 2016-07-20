#pragma library power

native power(value, exp);
forward main();

public main()
{
	for(new i=0; i < 10; i++)
	{
		printf "value: %d %d\n", i, power(i, 2);
	}
}
