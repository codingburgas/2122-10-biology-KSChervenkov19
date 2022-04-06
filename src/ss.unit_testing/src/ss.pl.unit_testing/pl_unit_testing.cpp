// clang-format off
#include "pch_unit_testing.h"
#include "CppUnitTest.h"
// clang-format on

//https://docs.microsoft.com/en-us/visualstudio/test/microsoft-visualstudio-testtools-cppunittestframework-api-reference?view=vs-2022#general_asserts

/************
If you are going to include any external libraries,
please include them in the pch file. Pity your memory!
************/

/************
Including header files from the other projects
is ready to be done! Everything is set up!
************/

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


//EVERY METHOD FROM A LAYER SHOULD BE TESTED IN A DIFFERENT
//CPP FILE! CREATE THE CPP FILE IN THE CORESPODING LAYER FOLDER!


namespace ss::pl::unit_testing
{
	TEST_CLASS(pl_unit_testing)
	{
	public:

		//BEGIN_TEST_METHOD_ATTRIBUTE(methodName)
		//TEST_OWNER(ownerName=> L"MCBerberov19")
		//TEST_PRIORITY(priority => "Low", "Medium", "High")
		//END_TEST_METHOD_ATTRIBUTE()
		//TEST_METHOD(methodName)
		/*{
			//Arrange
			//Declare variables

			//Act
			//Initialize variables

			//Assert
			//Check
		}*/

		/*********
		An Example Tests
		**********/

		//First
		/*
		BEGIN_TEST_METHOD_ATTRIBUTE(binaryConverter)
		TEST_OWNER(L"MCBerberov19")
		TEST_PRIORITY("Medium")
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(convertToBinary)
		{
			// Arrange
			EventGenerator eventGenerator;
			std::vector<int> binary;
			std::vector<int> expected = { 1, 0 };

			// Act
			binary = eventGenerator.convertToBinary(2);

			// Assert
			for (size_t i = 0; i < binary.size(); i++)
			{
				Assert::AreEqual(expected[i], binary[i], L"Expected {1, 0}");
			}

			*************
			General asserts:
			- Assert::AreEqual(expected, actual, message)
			- Assert::AreNotEqual(notExpected, actual, message)
			- Assert::AreSame(expected, actual, message)
			- Assert::AreNotSame(notExpected, actual, message)
			- Assert::IsNull(actual, message)
			- Assert::IsNotNull(actual, message)
			- Assert::IsTrue(bool condition, message)
			- Assert::IsFalse(bool condition, message)
			- Assert::Fail(message)
			*************
		*/


		//Second

		/*
		TEST_METHOD(firstOptionChooseSizeFunction)
		{
			//Arrange
			MazeGenerationAlgorithm testCase;

			//Act
			int result = testCase.chooseSize(2);

			//Assert
			Assert::AreEqual(result, 8, L"Expected 8!");

			*************
			General asserts:
			- Assert::AreEqual(expected, actual, message)
			- Assert::AreNotEqual(notExpected, actual, message)
			- Assert::AreSame(expected, actual, message)
			- Assert::AreNotSame(notExpected, actual, message)
			- Assert::IsNull(actual, message)
			- Assert::IsNotNull(actual, message)
			- Assert::IsTrue(bool condition, message)
			- Assert::IsFalse(bool condition, message)
			- Assert::Fail(message)
			*************
		}
		*/
	};
}
