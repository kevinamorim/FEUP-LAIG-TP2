/* 
* G0_Base: projeto inicial de CGra
*  
*/

#include <iostream>
#include <exception>

#include "CGFapplication.h"
#include "ANFParser.h"
#include "MainScene.h"
#include "MyInterface.h"

using namespace std;

int getKey();

ANFParser* parse(char* filename);

int main(int argc, char* argv[] )
{
	char * filename;

	if(argc > 1)
	{
		filename = argv[1];
	}
	else
	{
		filename = "LAIG_TP1_ANF_T01_G02_v4.anf"; // Default
	}

	CGFapplication app = CGFapplication();

	try {
		app.init(&argc, argv);

		cout << "INIT DONE" << endl;

		ANFParser* parser = parse(filename);

		if(parser->getState() == 0)
		{
			app.setScene(new MainScene(parser->getSceneData()));
			app.setInterface(new MyInterface());

			app.run();
		}
		else 
		{
			getKey();
		}
	}
	catch(GLexception& ex)
	{
		cout << "Error: " << ex.what();
		return -1;
	}
	catch(exception& ex)
	{
		cout << "Unexpected error: " << ex.what();
		return -1;
	}

	return 0;
}

// Prints a message for pressing any key and awaits for key input.
int getKey()
{
	printf("\nPress any key... \n");
	getchar();

	return 0;
}

// Parses a ANF file.
ANFParser* parse(char* filename)
{
	// Creates parser.
	ANFParser* parser = new ANFParser(filename);

	parser->Parse();

	if(parser->getState() != 0)
	{
		printf("> Parsing failed.\n\n");
	}
	else
	{
		printf("> Parsing succeeded.\n\n");
	}

	return parser;
}