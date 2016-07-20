forward init();
forward prueba();

public main()
{
	print "Main test1.amx\n";
	for(new i = 0; i < 100000; i++)
	{
	}
	init();
	return 123;
}

public init()
{
	print "Init test1.amx\n";
}

public prueba()
{
	print "Test test1.amx\n";
	for(new i = 0; i < 1000000; i++)
	{
	}
	print "Test end test1.amx\n";
}
