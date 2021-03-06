#include <stdexcept>
#include <iostream>
#include <string>

#include <map>

#if defined(DEBUG) || defined(BENCHMARK)
// Benchmark
#include <chrono>

// Variable for timing
std::chrono::time_point<std::chrono::system_clock> start, end;
std::chrono::duration<double> elapsed_seconds;
#endif

#include "ad_database.h"

#ifdef REMOTE
#define FILE_PATH "/tmp2/KDDCup2012/track2/kddcup2012track2.txt"
#elif LOCAL
//#define FILE_PATH "./dat/kddcup2012track2.txt"
#define FILE_PATH "./dat/demotrack.txt"
#else
#define MANUAL_FILE_PATH
#endif

#define PRINT_SEPARATOR std::cout << "********************" << std::endl;

bool get(dsa::Database& database)
{
	#ifdef DEBUG
	std::cout << "get() called." << std::endl;
	#endif

	unsigned int u, a, q;
	unsigned char p, d;
	std::cin >> u >> a >> q >> p >> d;
	#ifdef DEBUG
	std::cout << "Parameters: (u, a, q, p, d) = ("
			  << u << ", "
			  << a << ", "
			  << q << ", "
			  << p << ", "
			  << d << ")" << std::endl;
	#endif 

	PRINT_SEPARATOR
	std::pair<unsigned int, unsigned long> result = dsa::KDD::get(database, u, a, q, p, d);
	std::cout << result.first << " " << result.second << std::endl;
	PRINT_SEPARATOR

	return false;
}

bool clicked(dsa::Database& database)
{
	#ifdef DEBUG
	std::cout << "clicked() called." << std::endl;
	#endif

	unsigned int u;
	std::cin >> u;
	#ifdef DEBUG
	std::cout << "Parameter: (u) = (" << u << ")" << std::endl;
	#endif 

	PRINT_SEPARATOR
	for(const auto& elem : dsa::KDD::clicked(database, u))
		std::cout << elem.first << " " << elem.second << std::endl;
	PRINT_SEPARATOR

	return false;
}

bool impressed(dsa::Database& database)
{
	#ifdef DEBUG
	std::cout << "impressed() called." << std::endl;
	#endif

	unsigned int u1, u2;
	std::cin >> u1 >> u2;
	#ifdef DEBUG
	std::cout << "Parameter: (u1, u2) = (" 
			  << u1 << ", "
			  << u2 << ")" << std::endl;
	#endif

	PRINT_SEPARATOR
	//dsa::KDD::impressed(database, u1, u2);
	
	for(const auto& elem : dsa::KDD::impressed(database, u1, u2))
	{
		std::cout << elem.first;
		#ifdef DEBUG
		std::cout << ", vector size=" << elem.second.size();
		#endif
		std::cout << std::endl;

		for(const auto& prop : elem.second)
		{
			std::cout << '\t'
					  << prop.get_display_url() << ' '
					  << prop.get_advertiser_id() << ' '
					  << prop.get_keyword_id() << ' '
					  << prop.get_title_id() << ' '
					  << prop.get_description_id()
					  << std::endl;
		}
	}
	
	PRINT_SEPARATOR

	return false;
}

bool profit(dsa::Database& database)
{
	#ifdef DEBUG
	std::cout << "profit() called." << std::endl;
	#endif

	unsigned int a;
	double ctr;
	std::cin >> a >> ctr;
	#ifdef DEBUG
	std::cout << "Parameter: (a, ctr) = (" 
			  << a << ", "
			  << ctr << ")" << std::endl;
	#endif

	PRINT_SEPARATOR
	for(const auto& elem : dsa::KDD::profit(database, a, ctr))
		std::cout << elem << std::endl;
	PRINT_SEPARATOR

	return false;
}

bool quit(dsa::Database& database)
{
	//std::cout << "# leave the program" << std::endl;
	return true;
}

typedef bool (*FuncPtr)(dsa::Database&);
typedef std::map<std::string, FuncPtr> InstrMap;

void setup_function_lut(std::map<std::string, FuncPtr>& map)
{
	map["get"] = get;
	map["clicked"] = clicked;
	map["impressed"] = impressed;
	map["profit"] = profit;
	map["quit"] = quit;
}

int main(int argc, char* argv[])
{	
	#if defined(DEBUG) || defined(BENCHMARK)
	start = std::chrono::system_clock::now();
	#endif
	try
	{
		#ifndef MANUAL_FILE_PATH
		dsa::Database database(FILE_PATH);
		#else
		if(argc < 2)
			throw std::runtime_error("main(): Too few argument.");
		else if(argc > 2)
			throw std::runtime_error("main(): Program only take one file path as argument.");
		
		dsa::Database database(argv[1]);
		#endif
	
		#if defined(DEBUG) || defined(BENCHMARK)
		// End timer
		end = std::chrono::system_clock::now();
		elapsed_seconds = end - start;
		std::cout << "database, elapsed time: " << elapsed_seconds.count() << std::endl;
		#endif

		// Setup instruction look-up table
		InstrMap instruction_map;
		setup_function_lut(instruction_map);

		std::string instruction;
		bool quit = false;
		do
		{
			std::cin >> instruction;
			auto elem = instruction_map.find(instruction);
			if (elem == instruction_map.end())
			    quit = true;
			else
			{
				try
				{
					#if defined(DEBUG) || defined(BENCHMARK)
					// Start timer
					start = std::chrono::system_clock::now();
					#endif
					quit = (elem->second)(database);
					#if defined(DEBUG) || defined(BENCHMARK)
					// End timer
					end = std::chrono::system_clock::now();
					elapsed_seconds = end - start;
					std::cout << instruction << ", elapsed time: " << elapsed_seconds.count() << std::endl;
					#endif
				}
				catch(std::runtime_error &e)
				{
					// Stop the program if run time error is caught.
					quit = true;
					std::cerr << "Caught a runtime_error exception: " << e.what () << std::endl;
				}
			}
		}while(!quit);
	}
	catch(std::runtime_error &e)
	{
		std::cerr << "Caught a runtime_error exception: " << e.what () << std::endl;
	}

	return 0;
}