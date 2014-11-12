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
		filename = "LAIG_TP1_ANF_T01_G02.anf"; // Default
	}

	CGFapplication app = CGFapplication();

	try {
		app.init(&argc, argv);

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
		getKey();
		return -1;
	}
	catch(exception& ex)
	{
		cout << "Unexpected error: " << ex.what();
		getKey();
		return -1;
	}

	return 0;
}

// Prints a message for pressing any key and awaits for key input.
int getKey()
{
	cout << endl << "Press any key... " << endl;
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
		cout << endl <<  "> Parsing failed. " << endl;
	}
	else
	{
		cout << endl <<  "> Parsing succeeded. " << endl;
	}

	return parser;
}