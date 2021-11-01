#include "TestRunner.h"
#include <iostream>
#include <Windows.h>
#include <exception>
#include "TestTimer.h"
#include "TestExceptionHandler.h"
#include "BasicCalculator.h"

//#define TEST_RUNNER_TEST

TestRunner::TestRunner(std::string name, bool (*funcPtr)()) : testFunctionName{name}, testFunction{funcPtr} { }

bool TestRunner::runTest(TestLogger logger) {	// TODO: make a way to retrieve the LOGLEVEL from TestLogger object
	TestTimer timer {};
	bool result = false;
	timer.startTimer();
	try {
		result = testFunction();
	} catch (std::exception& e) {
		timer.endTimer();
		testResult = TEST_RESULT::EXCEPTION;
		std::string message = testFunctionName + ": exception raised.";
		TestExceptionHandler handler {};
		// message += " " + handler.getCustomizedString(e, LOGLEVEL::detail, timer);
		logger.writeLogInfoToFile(message, timer);
		return false;
	}

	timer.endTimer();

	if (result) {
		testResult = TEST_RESULT::PASS;
		logger.writeLogInfoToFile(std::string(testFunctionName + ": tests passed."), timer);
		return true;
	}

	testResult = TEST_RESULT::FAIL;
	logger.writeLogInfoToFile(std::string(testFunctionName + ": tests failed."), timer);
	return false;
}

#ifdef TEST_RUNNER_TEST

bool testFunc1() {		// expect FAIL
	BasicCalculator calculator {};
	return calculator.add(3, 5) == 7 || calculator.multiply(10, 2) != 20;
}

bool testFunc2() {		// expect PASS
	BasicCalculator calculator {};
	return calculator.add(3, 5) == 8 && calculator.subtract(5, 3) == 2
		&& calculator.multiply(10, 2) == 20 && calculator.divide(10, 2) == 5;
}

bool testFunc3() {		// expect EXCEPTION
	BasicCalculator calculator {};
	return calculator.subtract(5, 3) > 5 || calculator.divide(4, 0) == 4;
}

int main() {

	TestLogger logger1 {};
	TestRunner runner1 {"testFunc1", *testFunc1 };
	bool result1 = runner1.runTest(logger1);
	std::cout << "result1: " << result1 << std::endl;

	std::cout << "\n";

	TestLogger logger2 {};
	TestRunner runner2 {"testFunc2", *testFunc2};
	bool result2 = runner2.runTest(logger2);
	std::cout << "result2: " << result2 << std::endl;

	/*TestLogger logger3{};
	TestRunner runner3 {"testFunc3", *testFunc3};
	bool result3 = runner3.runTest(logger3);
	std::cout << "result3 = " << result3 << std::endl;*/
}

#endif