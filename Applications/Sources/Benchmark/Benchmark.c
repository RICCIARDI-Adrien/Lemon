/** @file Benchmark.c
 * Execute several benchmarks to determine system speed and test its stability.
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** How many Fibonacci iterations to compute. */
#define BENCHMARK_PROCESSOR_ITERATIONS_COUNT 10000000
/** How many time to create the file. */
#define BENCHMARK_FILE_SYSTEM_ITERATIONS_COUNT 10000

/** Convert the macro identifier to a C string. */
#define BENCHMARK_CONVERT_MACRO_NAME_TO_STRING(X) #X
/** Convert the macro value to a C string. The preprocessor needs two passes to do the conversion, so the BENCHMARK_CONVERT_MACRO_NAME_TO_STRING() is needed. */
#define BENCHMARK_CONVERT_MACRO_VALUE_TO_STRING(X) BENCHMARK_CONVERT_MACRO_NAME_TO_STRING(X)

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

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All available benchmarks. */
static TBenchmarkTest Benchmark_Tests[] =
{
	{
		"processor speed",
		"Compute Fibonacci suite " BENCHMARK_CONVERT_MACRO_VALUE_TO_STRING(BENCHMARK_PROCESSOR_ITERATIONS_COUNT) " times with 32768 bits integer precision",
		BenchmarkProcessor
	},
	{
		"file system speed",
		"Open, write to and close a file " BENCHMARK_CONVERT_MACRO_VALUE_TO_STRING(BENCHMARK_FILE_SYSTEM_ITERATIONS_COUNT) " times",
		BenchmarkFileSystem
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
	
	*Pointer_Start_Time = SystemGetTimerValue();
	
	for (Iterations = 0; Iterations < BENCHMARK_PROCESSOR_ITERATIONS_COUNT; Iterations++)
	{
		MathIntegerAdd(&Penultimate_Number, &Last_Number, &Result);
		MathIntegerCopy(&Last_Number, &Penultimate_Number);
		MathIntegerCopy(&Result, &Last_Number);
	}
	
	*Pointer_End_Time = SystemGetTimerValue();
}

static void BenchmarkFileSystem(unsigned int *Pointer_Start_Time, unsigned int *Pointer_End_Time)
{
	int i, Result;
	unsigned int File_ID;
	char String_Data[] = "Benchmarking...";
	
	// Keep benchmark starting time to compute the elapsed time at benchmark end
	*Pointer_Start_Time = SystemGetTimerValue();
	
	for (i = 0; i < BENCHMARK_FILE_SYSTEM_ITERATIONS_COUNT; i++)
	{
		// Create the file
		Result = FileOpen("_test_", 'w', &File_ID);
		if (Result != ERROR_CODE_NO_ERROR)
		{
			ScreenWriteString("Error while opening the file for writing.\n");
			goto Exit_Error;
		}
		
		// Write some random bytes
		Result = FileWrite(File_ID, String_Data, sizeof(String_Data));
		if (Result != ERROR_CODE_NO_ERROR)
		{
			ScreenWriteString("Error while writing to the file.\n");
			goto Exit_Error;
		}

		FileClose(File_ID);
	}
	
	*Pointer_End_Time = SystemGetTimerValue();
	FileDelete("_test_");
	return;
	
Exit_Error:
	ScreenWriteString("Error on file ");
	ScreenWriteInteger(i);
	ScreenWriteString(".\n");
	FileClose(File_ID);
	FileDelete("_test_");
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
		ScreenWriteString("### Evaluating ");
		ScreenSetFontColor(SCREEN_COLOR_GREEN);
		ScreenWriteString(Benchmark_Tests[i].String_Tested_Subsystem);
		ScreenSetFontColor(SCREEN_COLOR_BLUE);
		ScreenWriteString(" ###\n");
		ScreenWriteString(Benchmark_Tests[i].String_Description);
		ScreenWriteString(".\n");
		
		// Execute it
		Benchmark_Tests[i].Function(&Start_Time, &End_Time);
		
		// Display elapsed time
		ScreenWriteString("Benchmark duration : ");
		ScreenSetFontColor(SCREEN_COLOR_GREEN);
		ScreenWriteUnsignedInteger(End_Time - Start_Time);
		ScreenSetFontColor(SCREEN_COLOR_BLUE);
		ScreenWriteString(" milliseconds.\n\n");
	}
	
	return 0;
}