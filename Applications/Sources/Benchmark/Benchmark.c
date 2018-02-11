/** @file Benchmark.c
 * Execute several benchmarks to determine system speed and test its stability.
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** How many Fibonacci iterations to compute. */
#define BENCHMARK_PROCESSOR_ITERATIONS_COUNT 10000000
/** How many time to create the file. */
#define BENCHMARK_FILE_SYSTEM_ITERATIONS_COUNT 10000
/** How many system calls to execute. */
#define BENCHMARK_SYSTEM_CALL_ITERATIONS_COUNT 100000000

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** A benchmark callback.
 * @param Pointer_Start_Time On output, contain the benchmark starting time in milliseconds.
 * @param Pointer_End_Time On output, contain the benchmark ending time in milliseconds.
 */
typedef void (*BenchmarkFunction)(unsigned int *Pointer_Start_Time, unsigned int *Pointer_End_Time);

/** A benchmark test. */
typedef struct
{
	char String_Tested_Subsystem[64]; //! What subsystem the benchmark is testing.
	char String_Description[256]; //! What the benchmark does.
	BenchmarkFunction Function; //! The test code.
} TBenchmarkTest;

//-------------------------------------------------------------------------------------------------
// Prototypes
//-------------------------------------------------------------------------------------------------
static void BenchmarkProcessor(unsigned int *Pointer_Start_Time, unsigned int *Pointer_End_Time);
static void BenchmarkFileSystem(unsigned int *Pointer_Start_Time, unsigned int *Pointer_End_Time);
static void BenchmarkSystemCall(unsigned int *Pointer_Start_Time, unsigned int *Pointer_End_Time);

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All available benchmarks. */
static TBenchmarkTest Benchmark_Tests[] =
{
	{
		"processor speed",
		"Compute Fibonacci suite " LIBRARIES_CONVERT_MACRO_VALUE_TO_STRING(BENCHMARK_PROCESSOR_ITERATIONS_COUNT) " times with 32768 bits integer precision",
		BenchmarkProcessor
	},
	{
		"file system speed",
		"Open, write to and close a file " LIBRARIES_CONVERT_MACRO_VALUE_TO_STRING(BENCHMARK_FILE_SYSTEM_ITERATIONS_COUNT) " times",
		BenchmarkFileSystem
	},
	{
		"system call interface speed",
		"Execute a simple system call " LIBRARIES_CONVERT_MACRO_VALUE_TO_STRING(BENCHMARK_SYSTEM_CALL_ITERATIONS_COUNT) " times",
		BenchmarkSystemCall
	}
};

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
static void BenchmarkProcessor(unsigned int *Pointer_Start_Time, unsigned int *Pointer_End_Time)
{
	static TMathInteger Penultimate_Number, Last_Number, Result; // Do not store these variables on the stack as they are a bit heavy
	unsigned long long Iterations;
	
	// Initialize the first numbers
	MathIntegerInitializeFromInt(0, &Penultimate_Number);
	MathIntegerInitializeFromInt(1, &Last_Number);
	
	*Pointer_Start_Time = LibrariesTimerGetValue();
	
	for (Iterations = 0; Iterations < BENCHMARK_PROCESSOR_ITERATIONS_COUNT; Iterations++)
	{
		MathIntegerAdd(&Penultimate_Number, &Last_Number, &Result);
		MathIntegerCopy(&Last_Number, &Penultimate_Number);
		MathIntegerCopy(&Result, &Last_Number);
	}
	
	*Pointer_End_Time = LibrariesTimerGetValue();
}

static void BenchmarkFileSystem(unsigned int *Pointer_Start_Time, unsigned int *Pointer_End_Time)
{
	int i, Result;
	unsigned int File_ID;
	char String_Data[] = "Benchmarking...";
	
	// Keep benchmark starting time to compute the elapsed time at benchmark end
	*Pointer_Start_Time = LibrariesTimerGetValue();
	
	for (i = 0; i < BENCHMARK_FILE_SYSTEM_ITERATIONS_COUNT; i++)
	{
		// Create the file
		Result = LibrariesFileOpen("_test_", 'w', &File_ID);
		if (Result != ERROR_CODE_NO_ERROR)
		{
			LibrariesScreenWriteString("Error while opening the file for writing.\n");
			goto Exit_Error;
		}
		
		// Write some random bytes
		Result = LibrariesFileWrite(File_ID, String_Data, sizeof(String_Data));
		if (Result != ERROR_CODE_NO_ERROR)
		{
			LibrariesScreenWriteString("Error while writing to the file.\n");
			goto Exit_Error;
		}

		LibrariesFileClose(File_ID);
	}
	
	*Pointer_End_Time = LibrariesTimerGetValue();
	LibrariesFileDelete("_test_");
	return;
	
Exit_Error:
	LibrariesScreenWriteString("Error on file ");
	LibrariesScreenWriteInteger(i);
	LibrariesScreenWriteString(".\n");
	LibrariesFileClose(File_ID);
	LibrariesFileDelete("_test_");
}

static void BenchmarkSystemCall(unsigned int *Pointer_Start_Time, unsigned int *Pointer_End_Time)
{
	unsigned int i;
	
	// Keep benchmark starting time to compute the elapsed time at benchmark end
	*Pointer_Start_Time = LibrariesTimerGetValue();
	
	for (i = 0; i < BENCHMARK_SYSTEM_CALL_ITERATIONS_COUNT; i++) LibrariesSystemCall(SYSTEM_CALL_SCREEN_GET_COLOR, 0, 0, NULL, NULL);
	
	*Pointer_End_Time = LibrariesTimerGetValue();
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(void)
{
	unsigned int Start_Time, End_Time, i;
	
	// Execute all benchmarks
	for (i = 0; i < sizeof(Benchmark_Tests) / sizeof(TBenchmarkTest); i++)
	{
		// Display benchmark information
		LibrariesScreenWriteString("### Evaluating ");
		LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_GREEN);
		LibrariesScreenWriteString(Benchmark_Tests[i].String_Tested_Subsystem);
		LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
		LibrariesScreenWriteString(" ###\n");
		LibrariesScreenWriteString(Benchmark_Tests[i].String_Description);
		LibrariesScreenWriteString(".\n");
		
		// Execute it
		Benchmark_Tests[i].Function(&Start_Time, &End_Time);
		
		// Display elapsed time
		LibrariesScreenWriteString("Benchmark duration : ");
		LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_GREEN);
		LibrariesScreenWriteUnsignedInteger(End_Time - Start_Time);
		LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
		LibrariesScreenWriteString(" milliseconds.\n\n");
	}
	
	return 0;
}
