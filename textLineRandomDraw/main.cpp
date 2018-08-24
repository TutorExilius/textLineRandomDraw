#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

// ****
// Author:	Tutor Exilius	(http://exilius.de)
// Created:	24.08.2018
// ****

// Forward-Declarations
void printInfoParamData( std::initializer_list<std::string> params );
void drawLines( const std::vector<std::string> &textLinesList, const int &cntDraws );

int main( int argc, char **argv )
{
	if( argc >= 2 && argc <= 3 )
	{
		std::string filePath = argv[1];

		// --- read textLines
		std::ifstream in( filePath );

		if( !in )
		{
			std::cerr << "File could not be opened" << std::endl;
			return -1;
		}

		std::vector<std::string> textLineList;
		std::string textLine;

		const int minStringLength = 1;

		while( getline( in, textLine ) )
		{
			if( textLine.size() > minStringLength )
			{
				textLineList.push_back( textLine );
			}
		}
		// ---

		const size_t cntTextLines = textLineList.size();

		if( cntTextLines > 0 )
		{
			// shuffle textLines
			std::random_device rd;
			std::random_shuffle( textLineList.begin(), textLineList.end(),
				[&]( const int &max ){
					std::uniform_int_distribution<int> dist{ 1, max };
					return dist( rd ) - 1;
				} );

			std::string cntDrawStr;

			int cntDraw = 1;
			bool isCntDrawGreater = false;
			char printGreaterCntDrawAnyway = 'n';

			try
			{
				if( argc == 3 )
				{
					cntDrawStr = argv[2];
					cntDraw = std::stoi( cntDrawStr );
				}

				if( cntDraw < 1 )
				{
					std::cout << "Incorrect second parameter. Expected minimum 1" << std::endl;
					std::cout << "default-value: 1 will be used as number of raffles!" << std::endl;

					cntDraw = 1;
				}
				else if( static_cast<size_t>( cntDraw ) > cntTextLines )
				{
					isCntDrawGreater = true;

					std::cout << "Warning: number of draws is greater than number of text lines! All text lines will be raffled!" << std::endl;
					std::cout << "(y/n): ";

					std::cin >> printGreaterCntDrawAnyway;

					if( !std::cin )
					{
						std::cin.clear();
					}

					std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
					std::cin.sync();
				}
			}
			catch( std::invalid_argument err )
			{
				std::cerr << err.what() << std::endl;
				return -1;
			}
			catch( std::out_of_range err )
			{
				std::cerr << err.what() << std::endl;
				return -1;
			}
			catch( ... )
			{
				std::cerr << "Unknown error occured." << std::endl;
				return -1;
			}

			/*
			std::cout << std::endl;
			printInfoParamData( { argv[0], filePath, cntDrawStr } );
			std::cout << std::endl;
			*/

			if( !isCntDrawGreater )
			{
				drawLines( textLineList, cntDraw );
			}
			else
			{
				if( printGreaterCntDrawAnyway == 'y' || printGreaterCntDrawAnyway == 'Y' )
				{
					std::for_each( textLineList.begin(), textLineList.end(),
						[]( const auto &line ){
							std::cout << line << std::endl;
						} );
				}
			}
		}
		else
		{
			std::cout << "Warning: nothing to raffle. No text lines found!" << std::endl;
		}
	}
	else
	{
		std::cout << "Parameter missing!\nUse: textLineRandomDrawer.exe PATH_OF_FILE.txt [NUMBER OF DRAWS {min: 1}]" << std::endl;
		return -1;
	}

	return 0;
}

// prints accepted parameters
// not used yet
void printInfoParamData( std::initializer_list<std::string> params )
{
	std::cout << "Application: arameters:" << *( params.begin() ) << std::endl;

	std::cout << "Parameters:" << std::endl;
	std::cout << "\tFile: " << *( params.begin() + 1) << std::endl;
	std::cout << "\tNumber of draws: " << *( params.begin() + 2 ) << std::endl;
}

void drawLines( const std::vector<std::string> &textLinesList, const int &cntDraws )
{
	std::vector<std::string> raffledList;

	int raffledCnt = 0;

	while( raffledCnt < cntDraws )
	{
		std::string raffledTextLine = textLinesList.at( raffledCnt );

		if( std::find( raffledList.begin(), raffledList.end(), raffledTextLine ) == raffledList.end() )
		{
			raffledList.push_back( raffledTextLine );
			++raffledCnt;
		}

		// shuffle raffledList after every draw
		std::random_device rd;
		std::random_shuffle( raffledList.begin(), raffledList.end(),
			[&]( const int &max ){
				std::uniform_int_distribution<int> dist{ 1, max };
				return dist( rd ) - 1;
			} );
	}

	// --- print raffled list
	// std::cout << "\nRaffled list: " << std::endl;
	// size_t number = 0;
	std::for_each( raffledList.begin(), raffledList.end(),
		[]( const auto &line ){
			std::cout << line << std::endl;
		} );
	// ---
}